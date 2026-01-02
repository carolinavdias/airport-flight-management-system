#define _POSIX_C_SOURCE 200809L
#include "entidades/flights.h"
#include "gestor_entidades/gestor_flights.h"
#include "utils/utils.h"
#include "queries/q5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

// Dados para iteração
typedef struct {
    GHashTable *airlines;
} DadosQ5;

// Estrutura interna para acumular dados
typedef struct {
    char *airline;
    int total_delay;
    int delay_count;
    double delay_avg;
} AirlineData;

// Callback para processar voos
static void processa_voo_q5(Voo *v, void *user_data) {
    DadosQ5 *dados = user_data;
    
    // Apenas voos Delayed (conforme enunciado)
    if (voo_get_status(v) != ESTADO_DELAYED) return;
    
    char *airline = voo_get_airline(v);
    if (!airline) return;
    
    // Calcular atraso em minutos (actual_departure - departure)
    long long departure = voo_get_departure(v);
    long long actual = voo_get_actual_departure(v);
    
    // Extrair componentes de tempo
    int dep_dia = (departure / 10000) % 100;
    int dep_hora = (departure % 10000) / 100;
    int dep_min = departure % 100;
    
    int act_dia = (actual / 10000) % 100;
    int act_hora = (actual % 10000) / 100;
    int act_min = actual % 100;
    
    int dep_total = dep_dia * 24 * 60 + dep_hora * 60 + dep_min;
    int act_total = act_dia * 24 * 60 + act_hora * 60 + act_min;
    int delay = act_total - dep_total;
    
    if (delay < 0) delay = 0;
    
    // Procurar ou criar entrada
    AirlineData *ad = g_hash_table_lookup(dados->airlines, airline);
    if (!ad) {
        ad = g_new0(AirlineData, 1);
        ad->airline = g_strdup(airline);
        ad->delay_count = 0;
        ad->total_delay = 0;
        g_hash_table_insert(dados->airlines, g_strdup(airline), ad);
    }
    
    ad->total_delay += delay;
    ad->delay_count++;
    ad->delay_avg = (double)ad->total_delay / ad->delay_count;
    
    g_free(airline);
}

// Comparador: maior média primeiro, empate por ordem alfabética
int compara_delay_dec(const void *a, const void *b) {
    const AirlineData *x = *(const AirlineData **)a;
    const AirlineData *y = *(const AirlineData **)b;
    
    if (x->delay_avg < y->delay_avg) return 1;
    if (x->delay_avg > y->delay_avg) return -1;
    
    // Empate: ordem alfabética
    return strcmp(x->airline, y->airline);
}

static void free_airline_data(gpointer data) {
    AirlineData *ad = data;
    if (ad) {
        g_free(ad->airline);
        g_free(ad);
    }
}

// Query5 
char *query5(const char *linhaComando, GestorFlights *gestorVoos) {
    int N;
    int arg = sscanf(linhaComando, "%d", &N);
    if (arg < 1 || N <= 0) {
        return strdup("\n");
    }
    
    // Criar hash table para airlines
    GHashTable *airlines = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, free_airline_data
    );
    
    DadosQ5 dados = { .airlines = airlines };
    
    // Iterar voos
    gestor_flights_foreach(gestorVoos, processa_voo_q5, &dados);
    
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
        // Formato: airline;delayed_flights_count;average_delay
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
