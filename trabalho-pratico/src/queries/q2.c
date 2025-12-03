#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <glib.h>
#include "q2.h"
#include "q3.h"
#include "read.h"
#include "errors.h"

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

int identificadorValido(const char *id) {
    if (!id || strlen(id) != 7) return 0;
    if (!isupper((unsigned char)id[0]) || !isupper((unsigned char)id[1])) return 0;
    if (id[2] != '-') return 0;
    for (int i = 3; i < 7; i++)
        if (!isdigit((unsigned char)id[i])) return 0;
    return 1;
}

int comparaContagens(const Contagem *a, const Contagem *b) {
    if (a->count != b->count)
        return b->count - a->count;
    return g_strcmp0(a->identifier, b->identifier);
}

// =====================================================
// QUERY 2 OTIMIZADA
// =====================================================

void query2(const char *linhaComando,
            GHashTable *tabelaAeronaves,
            GHashTable *tabelaVoos,
            FILE *out)
{
    int N;
    char fabricante_raw[200] = "";

    int arg = sscanf(linhaComando, "%d %[^\n]", &N, fabricante_raw);
    if (arg < 1 || N <= 0) {
        fprintf(out, "\n");
        return;
    }

    if (arg == 2) trim(fabricante_raw);
    else fabricante_raw[0] = '\0';

    int usar_filtro = (fabricante_raw[0] != '\0');
    gchar *fabricante_lower = usar_filtro ? g_ascii_strdown(fabricante_raw, -1) : NULL;

    // =====================================================
    // OTIMIZAÇÃO 1: Usar hash table SIMPLES sem alocações
    // =====================================================
    GHashTable *contagens = g_hash_table_new(g_str_hash, g_str_equal);

    // OTIMIZAÇÃO 2: Iterar com iterator (mais rápido que get_values)
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, tabelaVoos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;

        // Ignorar cancelados (usar constante do header q3.h)
        if (v->status == ESTADO_CANCELLED) continue;
        
        // Ignorar voos sem aeronave
        if (!v->id_aircraft || v->id_aircraft[0] == '\0') continue;

        // OTIMIZAÇÃO 3: Incrementar DIRETO sem malloc
        gpointer old_count = g_hash_table_lookup(contagens, v->id_aircraft);
        int new_count = old_count ? GPOINTER_TO_INT(old_count) + 1 : 1;
        
        // Reutilizar a chave original (não duplicar)
        g_hash_table_insert(contagens, v->id_aircraft, GINT_TO_POINTER(new_count));
    }

    // =====================================================
    // OTIMIZAÇÃO 4: Construir lista só com aeronaves relevantes
    // =====================================================
    GList *resultado = NULL;

    g_hash_table_iter_init(&iter, tabelaAeronaves);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Aeronave *a = value;

        // Aplicar filtro de fabricante (case-insensitive inline)
        if (usar_filtro) {
            const char *s1 = a->manufacturer;
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

        // Obter contagem
        gpointer count_ptr = g_hash_table_lookup(contagens, a->identifier);
        int count = count_ptr ? GPOINTER_TO_INT(count_ptr) : 0;

        // Criar estrutura Contagem
        Contagem *c = g_new(Contagem, 1);
        c->identifier   = g_strdup(a->identifier);
        c->manufacturer = g_strdup(a->manufacturer);
        c->model        = g_strdup(a->model);
        c->count        = count;

        resultado = g_list_prepend(resultado, c);
    }

    if (fabricante_lower) g_free(fabricante_lower);

    // =====================================================
    // Ordenar e imprimir
    // =====================================================
    resultado = g_list_sort(resultado, (GCompareFunc) comparaContagens);

    int printed = 0;
    for (GList *l = resultado; l != NULL && printed < N; l = l->next, printed++) {
        Contagem *c = l->data;
        fprintf(out, "%s,%s,%s,%d\n",
                c->identifier, c->manufacturer, c->model, c->count);
    }

    if (printed == 0) {
        fprintf(out, "\n");
    }

    // Libertar memória
    for (GList *l = resultado; l != NULL; l = l->next) {
        Contagem *c = l->data;
        g_free(c->identifier);
        g_free(c->manufacturer);
        g_free(c->model);
        g_free(c);
    }
    g_list_free(resultado);
    g_hash_table_destroy(contagens);
}