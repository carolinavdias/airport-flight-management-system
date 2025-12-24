#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "queries/q2.h"
#include "entidades/flights.h"
#include "entidades/aircrafts.h"
#include "gestor_entidades/gestor_aircrafts.h" 

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
// ESTRUTURA AUXILIAR PARA O FOREACH
// =====================================================
typedef struct {
    const char *fabricante_lower;
    int usar_filtro;
    GList *resultado;
} DadosFiltro;

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
// CALLBACK PARA PROCESSAR CADA AERONAVE
// =====================================================
static void processa_aeronave(Aeronave *a, void *user_data) {
    DadosFiltro *dados = (DadosFiltro *)user_data;
    
    // Aplicar filtro de fabricante (case-insensitive)
    if (dados->usar_filtro) {
        const char *s1 = aircraft_get_manuf(a);
        const char *s2 = dados->fabricante_lower;
        int match = 1;
        
        for (; *s1 && *s2; s1++, s2++) {
            if (g_ascii_tolower(*s1) != *s2) {
                match = 0;
                break;
            }
        }
        if (!(*s1 == '\0' && *s2 == '\0')) match = 0;
        
        if (!match) return;
    }
    
    // Obter contagem da hash table global
    gpointer count_ptr = g_hash_table_lookup(contagem_global, aircraft_get_identifier(a));
    int count = count_ptr ? GPOINTER_TO_INT(count_ptr) : 0;
    
    // Criar estrutura Contagem
    Contagem *c = g_new(Contagem, 1);
    c->identifier   = aircraft_get_identifier(a);
    c->manufacturer = aircraft_get_manuf(a);
    c->model        = aircraft_get_model(a);
    c->count        = count;
    
    dados->resultado = g_list_prepend(dados->resultado, c);
}

// =====================================================
// INICIALIZAÇÃO (chamar uma vez no início do programa)
// =====================================================

void query2_init(GHashTable *tabelaVoos) {
    if (contagem_global != NULL) return;
    
    contagem_global = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, tabelaVoos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;
        
        if (voo_get_status(v) == ESTADO_CANCELLED) continue;
        
        const char *aircraft_id = voo_get_id_aircraft(v);
        if (!aircraft_id || aircraft_id[0] == '\0') continue;
        
        gpointer old_count = g_hash_table_lookup(contagem_global, aircraft_id);
        
        if (old_count) {
            int new_count = GPOINTER_TO_INT(old_count) + 1;
            g_hash_table_replace(contagem_global, 
                                 g_hash_table_lookup_extended(contagem_global, aircraft_id, (gpointer*)&key, NULL) ? key : NULL,
                                 GINT_TO_POINTER(new_count));
        } else {
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
// QUERY 2 OTIMIZADA - USANDO FOREACH
// =====================================================

char *query2(const char *linhaComando, GestorAircrafts *gestorAeronaves, GHashTable *tabelaVoos) {
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

    // ✅ Preparar dados para o foreach
    DadosFiltro dados = {
        .fabricante_lower = fabricante_lower,
        .usar_filtro = usar_filtro,
        .resultado = NULL
    };

    // ✅ USAR FOREACH em vez de iterar diretamente
    gestor_aircrafts_foreach(gestorAeronaves, processa_aeronave, &dados);

    if (fabricante_lower) g_free(fabricante_lower);

    // Ordenar
    dados.resultado = g_list_sort(dados.resultado, (GCompareFunc) comparaContagens);

    // Construir string de resultado
    if (dados.resultado == NULL) {
        return strdup("\n");
    }

    size_t buffer_size = 4096;
    char *output = malloc(buffer_size);
    if (!output) {
        g_list_free_full(dados.resultado, g_free);
        return strdup("\n");
    }
    
    output[0] = '\0';
    size_t current_pos = 0;

    int printed = 0;
    for (GList *l = dados.resultado; l != NULL && printed < N; l = l->next, printed++) {
        Contagem *c = l->data;
        
        char linha[512];
        int len = snprintf(linha, sizeof(linha), "%s, %s, %s, %d\n",
                          c->identifier, c->manufacturer, c->model, c->count);
        
        if (current_pos + len + 1 > buffer_size) {
            buffer_size *= 2;
            char *new_output = realloc(output, buffer_size);
            if (!new_output) {
                free(output);
                g_list_free_full(dados.resultado, g_free);
                return strdup("\n");
            }
            output = new_output;
        }
        
        strcpy(output + current_pos, linha);
        current_pos += len;
    }

    if (printed == 0) {
        free(output);
        g_list_free_full(dados.resultado, g_free);
        return strdup("\n");
    }

    g_list_free_full(dados.resultado, g_free);
    
    return output;
}