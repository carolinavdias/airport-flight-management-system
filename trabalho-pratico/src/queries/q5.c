#define _POSIX_C_SOURCE 200809L

#include <math.h>
#include "entidades/flights.h"
#include "gestor_entidades/gestor_flights.h"
#include "utils/utils.h"
#include "queries/q5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

/*
 * =====================================================
 * QUERY 5
 * =====================================================
 */

/** 
 * Top N companhias aéreas com maior atraso médio.
 *
 * Esta implementação utiliza um cache pré-computado
 * no parsing dos voos, evitando percorrer todos os voos
 * para cada execução da query.
 */

/* =====================================================
 * ESTRUTURA AUXILIAR PARA ORDENAR RESULTADOS
 * =====================================================
 */

/*
 * Estrutura que armazena os dados agregados de uma
 * companhia aérea.
 */

typedef struct {
    char *airline;        /**< código da companhia aérea */
    long total_delay;     /**< soma total dos atrasos */
    int  delay_count;     /**< número de voos com atraso */
    double delay_avg;     /**<  atraso médio */
} AirlineData;

/* =====================================================
 * FUNÇÃO DE COMPARAÇÃO PARA ORDENAÇÃO
 * =====================================================
 */

/*
 * Compara duas companhias aéreas segundo as regras da Query 5:
 *  - maior atraso médio primeiro
 *  - em caso de empate, ordem alfabética do código
 *
 * O atraso médio é arredondado a 3 casas decimais antes
 * da comparação.
 */

int compara_delay_dec(const void *a, const void *b) {
    const AirlineData *x = *(const AirlineData **)a;
    const AirlineData *y = *(const AirlineData **)b;
    
    // Arredondar para 3 casas decimais antes de comparar
    double avg_x = round(x->delay_avg * 1000) / 1000;
    double avg_y = round(y->delay_avg * 1000) / 1000;
    
    if (avg_x < avg_y) return 1;
    if (avg_x > avg_y) return -1;
    return strcmp(x->airline, y->airline);
}

/** 
 * =====================================================
 * CALLBACK PARA PROCESSAR CACHE PRÉ-COMPUTADO
 * =====================================================
 */

/*
 * Callback chamada pelo gestor de voos para cada
 * companhia aérea presente no cache da Query 5.
 *
 * Constrói uma estrutura AirlineData e insere-a
 * numa hash table.
 */

static void processa_cache_q5(const char *airline, long total_delay, int count, void *user_data) {
    GHashTable *airlines = user_data;
    
    AirlineData *ad = g_new(AirlineData, 1);
    ad->airline = g_strdup(airline);
    ad->total_delay = total_delay;
    ad->delay_count = count;
    ad->delay_avg = (double)total_delay / count;
    
    g_hash_table_insert(airlines, g_strdup(airline), ad);
}

/** 
 * Função auxiliar para libertar AirlineData.
 */

static void free_airline_data(gpointer data) {
    AirlineData *ad = data;
    if (ad) {
        g_free(ad->airline);
        g_free(ad);
    }
}

/** 
 * =====================================================
 * QUERY 5 — IMPLEMENTAÇÃO PRINCIPAL
 * =====================================================
 */

char *query5(const char *linhaComando, GestorFlights *gestorVoos) {

    /** 
     * =================================================
     * FASE 0: Validação de argumentos
     * =================================================
     */

    int N;
    if (sscanf(linhaComando, "%d", &N) < 1 || N <= 0) {
        return strdup("\n");
    }

    /** 
     * =================================================
     * FASE 1: Construção da tabela de companhias
     * =================================================
     */

    GHashTable *airlines = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free, free_airline_data
    );

    // Percorre cache pré-calculado no gestor
    gestor_flights_foreach_q5(
        gestorVoos, processa_cache_q5, airlines
    );

    guint total = g_hash_table_size(airlines);
    if (total == 0) {
        g_hash_table_destroy(airlines);
        return strdup("\n");
    }

    /** 
     * =================================================
     * FASE 2: Converter hash table para array
     * =================================================
     */

    AirlineData **lista = g_new(AirlineData*, total);

    GHashTableIter iter;
    gpointer key, value;
    guint idx = 0;

    g_hash_table_iter_init(&iter, airlines);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        lista[idx++] = value;
    }

    /** 
     * =================================================
     * FASE 3: Ordenação
     * =================================================
     */

    qsort(lista, total, sizeof(AirlineData*), compara_delay_dec);

    /** 
     * =================================================
     * FASE 4: Construção da string de output
     * =================================================
     */

    size_t buffer_size = 4096;
    char *output = malloc(buffer_size);
    if (!output) {
        g_free(lista);
        g_hash_table_destroy(airlines);
        return strdup("\n");
    }

    output[0] = '\0';
    size_t current_pos = 0;
    int printed = 0;

    for (guint i = 0; i < total && printed < N; i++, printed++) {
        AirlineData *c = lista[i];

        char linha[512];
        int len = snprintf(linha, sizeof(linha),
                           "%s;%d;%.3f\n",
                           c->airline,
                           c->delay_count,
                           c->delay_avg);

        if (current_pos + len + 1 > buffer_size) {
            buffer_size *= 2;
            char *novo = realloc(output, buffer_size);
            if (!novo) {
                free(output);
                g_free(lista);
                g_hash_table_destroy(airlines);
                return strdup("\n");
            }
            output = novo;
        }

        memcpy(output + current_pos, linha, len);
        current_pos += len;
    }

    if (printed == 0) {
        free(output);
        g_free(lista);
        g_hash_table_destroy(airlines);
        return strdup("\n");
    }

    output[current_pos] = '\0';

    /** 
     * =================================================
     * FASE 5: Limpeza
     * =================================================
     */

    g_free(lista);
    g_hash_table_destroy(airlines);

    return output;
}