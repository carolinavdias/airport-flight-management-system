#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "queries/q2.h"
#include "entidades/flights.h"
#include "entidades/aircrafts.h"

//contagem
typedef struct contagem {
    char *identifier;
    char *manufacturer;
    char *model;
    int count;
} Contagem;

// =====================================================
// HASH TABLE GLOBAL 
// =====================================================
static GHashTable *contagem_global = NULL;

// =====================================================
// FUNÇÕES AUXILIARES
// =====================================================

static void trim(char *s) {
    if (!s) return;
    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);

    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

int comparaContagens(const Contagem *a, const Contagem *b) {
    if (a->count != b->count)
        return b->count - a->count;
    return g_strcmp0(a->identifier, b->identifier);
}

// =====================================================
// INICIALIZAÇÃO (chamar uma vez no início do programa)
// =====================================================

void query2_init(GHashTable *tabelaVoos) {
    if (contagem_global != NULL) return; // Já inicializada
    
    //cria hash table com destrutor para libertar chaves duplicadas
    contagem_global = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, tabelaVoos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;
        
        //ignora cancelados
        if (voo_get_status(v) == ESTADO_CANCELLED) continue;
        
        //ignora voos sem aeronave
        const char *aircraft_id = voo_get_id_aircraft(v);
        if (!aircraft_id || aircraft_id[0] == '\0') continue;
        
        //verifica se já existe na hash table
        gpointer old_count = g_hash_table_lookup(contagem_global, aircraft_id);
        
        if (old_count) {
            //já existe - incrementa contagem
            int new_count = GPOINTER_TO_INT(old_count) + 1;
            //não precisa duplicar chave, pois ela já existe
            g_hash_table_replace(contagem_global, 
                                 g_hash_table_lookup_extended(contagem_global, aircraft_id, (gpointer*)&key, NULL) ? key : NULL,
                                 GINT_TO_POINTER(new_count));
        } else {
            //nova entrada - duplica chave
            g_hash_table_insert(contagem_global, 
                               g_strdup(aircraft_id), 
                               GINT_TO_POINTER(1));
        }
    }
}

// =====================================================
// LIMPEZA (chamar no final do programa)
// =====================================================
void query2_cleanup(void) {
    if (contagem_global != NULL) {
        g_hash_table_destroy(contagem_global);
        contagem_global = NULL;
    }
}

// =====================================================
// QUERY 2 OTIMIZADA (só consulta)
// =====================================================

char *query2(const char *linhaComando, GHashTable *tabelaAeronaves, GHashTable *tabelaVoos) {
    //garante que a hash table está inicializada
    if (contagem_global == NULL) {
        query2_init(tabelaVoos);
    }
    
    int N;
    char fabricante_raw[200] = "";

    int arg = sscanf(linhaComando, "%d %[^\n]", &N, fabricante_raw);
    if (arg < 1 || N <= 0) {
        return strdup("\n");
    }

    if (arg == 2) trim(fabricante_raw);
    else fabricante_raw[0] = '\0';

    int usar_filtro = (fabricante_raw[0] != '\0');
    gchar *fabricante_lower = usar_filtro ? g_ascii_strdown(fabricante_raw, -1) : NULL;

    //constrói lista apenas com aeronaves relevantes
    GList *resultado = NULL;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, tabelaAeronaves);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Aeronave *a = value;

        //aplica filtro de fabricante (case-insensitive inline)
        if (usar_filtro) {
            const char *s1 = aircraft_get_manuf(a);
            const char *s2 = fabricante_lower;
            int match = 1;

            for (; *s1 && *s2; s1++, s2++) {
                if (g_ascii_tolower(*s1) != *s2) {
                    match = 0;
                    break;
                }
            }
            if (!(*s1 == '\0' && *s2 == '\0')) match = 0;

            if (!match) continue;
        }

        //obtém contagem da hash table global (já calculada)
        gpointer count_ptr = g_hash_table_lookup(contagem_global, aircraft_get_identifier(a));
        int count = count_ptr ? GPOINTER_TO_INT(count_ptr) : 0;

        //cria estrutura Contagem
        Contagem *c = g_new(Contagem, 1);
        c->identifier   = aircraft_get_identifier(a);
        c->manufacturer = aircraft_get_manuf(a);
        c->model        = aircraft_get_model(a);
        c->count        = count;

        resultado = g_list_prepend(resultado, c);
    }

    if (fabricante_lower) g_free(fabricante_lower);

    //ordena
    resultado = g_list_sort(resultado, (GCompareFunc) comparaContagens);

    //constrói string de resultado
    if (resultado == NULL) {
        return strdup("\n");
    }

    //calcula tamanho necessário
    size_t buffer_size = 4096;
    char *output = malloc(buffer_size);
    if (!output) {
        g_list_free_full(resultado, g_free);
        return strdup("\n");
    }
    
    output[0] = '\0';
    size_t current_pos = 0;

    int printed = 0;
    for (GList *l = resultado; l != NULL && printed < N; l = l->next, printed++) {
        Contagem *c = l->data;
        
        //constrói linha 
        char linha[512];
        int len = snprintf(linha, sizeof(linha), "%s, %s, %s, %d\n",
                          c->identifier, c->manufacturer, c->model, c->count);
        
        //verifica se precisa expandir buffer
        if (current_pos + len + 1 > buffer_size) {
            buffer_size *= 2;
            char *new_output = realloc(output, buffer_size);
            if (!new_output) {
                free(output);
                g_list_free_full(resultado, g_free);
                return strdup("\n");
            }
            output = new_output;
        }
        
        //concatena linha
        strcpy(output + current_pos, linha);
        current_pos += len;
    }

    //se não imprimiu nada, retorna linha vazia
    if (printed == 0) {
        free(output);
        g_list_free_full(resultado, g_free);
        return strdup("\n");
    }

    //limpa lista
    g_list_free_full(resultado, g_free);
    
    return output;
}