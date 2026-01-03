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

// Estrutura interna para ordenação
typedef struct {
    char *airline;
    long total_delay;
    int delay_count;
    double delay_avg;
} AirlineData;

// Comparador: maior média primeiro, empate por ordem alfabética
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

// Callback para processar cache Q5
static void processa_cache_q5(const char *airline, long total_delay, int count, void *user_data) {
    GHashTable *airlines = user_data;
    
    AirlineData *ad = g_new(AirlineData, 1);
    ad->airline = g_strdup(airline);
    ad->total_delay = total_delay;
    ad->delay_count = count;
    ad->delay_avg = (double)total_delay / count;
    
    g_hash_table_insert(airlines, g_strdup(airline), ad);
}

static void free_airline_data(gpointer data) {
    AirlineData *ad = data;
    if (ad) {
        g_free(ad->airline);
        g_free(ad);
    }
}

// Query5 - versão otimizada com cache
char *query5(const char *linhaComando, GestorFlights *gestorVoos) {
    int N;
    if (sscanf(linhaComando, "%d", &N) < 1 || N <= 0) {
        return strdup("\n");
    }
    
    // Criar hash table para airlines
    GHashTable *airlines = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, free_airline_data
    );
    
    // Usar cache pre-computado (muito mais rápido!)
    gestor_flights_foreach_q5(gestorVoos, processa_cache_q5, airlines);
    
    guint S = g_hash_table_size(airlines);
    if (S == 0) {
        g_hash_table_destroy(airlines);
        return strdup("\n");
    }
    
    // Converter para array
    AirlineData **lista = g_new(AirlineData*, S);
    GHashTableIter iter;
    gpointer key, value;
    guint idx = 0;
    
    g_hash_table_iter_init(&iter, airlines);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        lista[idx++] = value;
    }
    
    // Ordenar
    qsort(lista, S, sizeof(AirlineData*), compara_delay_dec);
    
    // Output
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
    
    for (guint i = 0; i < S && printed < N; i++, printed++) {
        AirlineData *c = lista[i];
        char linha[512];
        int len = snprintf(linha, sizeof(linha), "%s;%d;%.3f\n",
                       c->airline, c->delay_count, c->delay_avg);
        if (current_pos + len + 1 > buffer_size) {
            buffer_size *= 2;
            char *new_output = realloc(output, buffer_size);
            if (!new_output) {
                free(output);
                g_free(lista);
                g_hash_table_destroy(airlines);
                return strdup("\n");
            }
            output = new_output;
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
    g_free(lista);
    g_hash_table_destroy(airlines);
    
    return output;
}
