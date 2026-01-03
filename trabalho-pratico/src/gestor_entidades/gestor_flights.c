#include "gestor_entidades/gestor_flights.h"
#include "entidades/flights.h"
#include "queries/q5.h"
#include "utils/utils.h"
#include <glib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * Estrutura interna do gestor de voos.
 *
 * Contém:
 *  - uma hash table principal (flight_id → Voo),
 *  - um array ordenado de voos (para Q3),
 *  - uma tabela auxiliar de contagens por aircraft_id (para Q2).
 */

typedef struct gestor_flights { 
    GHashTable *tabela_voos;          /**< Tabela principal de voos */
    Voo **array_ordenado;             /**< Array ordenado por data (Q3) */ 
    int num_voos;                     /**< Número de voos no array ordenado */
    GHashTable *contagens_aircraft;   /**< Contagens por aircraft_id (Q2) */
    GHashTable *cache_q5;             /**< airline -> {total_delay, count} para Q5 */
} GestorFlights;

/**
 * @brief Estrutura para dados de atraso por airline.
 */

typedef struct dadosAtrasoQ5{
    long total_delay;
    int count;
} DadosAtrasoQ5;

/* ============================================
 * CRIA GESTOR DE VOO
 * ============================================ */

GestorFlights *gestor_flights_novo() {
    GestorFlights *g = malloc(sizeof(GestorFlights));
    if (g == NULL) return NULL;
    g->tabela_voos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaVoo);
    g->array_ordenado = NULL;
    g->num_voos = 0;
    g->contagens_aircraft = NULL;  // será criado no parsing
    g->cache_q5 = NULL;  // será criado no parsing
//    g->listaQ5 = init_lista();
    return g;
}

/* ============================================
 * DESTRÓI GESTOR DE VOO
 * ============================================ */

void gestor_flights_destroy(GestorFlights *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_voos);
    if (g->array_ordenado) free(g->array_ordenado);
    if (g->contagens_aircraft) g_hash_table_destroy(g->contagens_aircraft);  
    if (g->cache_q5) g_hash_table_destroy(g->cache_q5);
    free(g);
}

/* ============================================
 * OPERAÇÕES BÁSICAS 
 * ============================================ */

void gestor_flights_inserir(GestorFlights *g, Voo *voo) {
    if (!g || !voo) return;
    char *flight_id = voo_get_flight_id(voo);
    if (!flight_id) return;
    g_hash_table_insert(g->tabela_voos, flight_id, voo);
}

bool gestor_flights_existe(GestorFlights *g, const char *flight_id) {
    return g && flight_id && g_hash_table_contains(g->tabela_voos, flight_id);
}

void gestor_flights_foreach(GestorFlights *g, void (*func)(Voo *, void *), void *user_data) {
    if (!g || !g->tabela_voos || !func) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, g->tabela_voos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        func((Voo *)value, user_data);
    }
}

void gestor_flights_print(GestorFlights *g) {
    if (!g || !g->tabela_voos) return;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->tabela_voos);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;
        char *fid = voo_get_flight_id(v);
        printf("%s %s %s\n", fid, voo_get_code_origin(v), voo_get_code_destination(v));
        g_free(fid);
    }
}

/* ============================================
 * CONSULTAS 
 * ============================================ */

const char *gestor_flights_obter_origem(GestorFlights *g, const char *flight_id) {
    if (!g) return NULL;
    Voo *v = g_hash_table_lookup(g->tabela_voos, flight_id);
    return v ? voo_get_code_origin(v) : NULL;
}

const char *gestor_flights_obter_destino(GestorFlights *g, const char *flight_id) {
    if (!g) return NULL;
    Voo *v = g_hash_table_lookup(g->tabela_voos, flight_id);
    return v ? voo_get_code_destination(v) : NULL;
}

/* ============================================
 * ARRAY ORDENADO (Q3) 
 * ============================================ */

void gestor_flights_set_array_ordenado(GestorFlights *g, Voo **array, int num_voos) {
    if (!g) return;
    g->array_ordenado = array;
    g->num_voos = num_voos;
}

Voo **gestor_flights_get_array_ordenado(GestorFlights *g, int *num_voos) {
    if (!g) return NULL;
    if (num_voos) *num_voos = g->num_voos;
    return g->array_ordenado;
}

/* ============================================
 * CONTAGENS DE AIRCRAFT (Q2) 
 * ============================================ */

void gestor_flights_set_contagens_aircraft(GestorFlights *g, GHashTable *contagens) {
    if (!g) return;
    g->contagens_aircraft = contagens;
}

int gestor_flights_get_contagem_aircraft(GestorFlights *g, const char *aircraft_id) {
    if (!g || !g->contagens_aircraft || !aircraft_id) return 0;
    gpointer val = g_hash_table_lookup(g->contagens_aircraft, aircraft_id);
    return val ? GPOINTER_TO_INT(val) : 0;
}


/* ============================================
 * PROCURA 
 * ============================================ */

Voo *gestor_flights_procura(GestorFlights *g, const char *flight_id) {
    if (!g || !flight_id) return NULL;
    return g_hash_table_lookup(g->tabela_voos, flight_id);
}

/* ============================================
 * FUNÇÕES PARA CACHE Q5 (OTIMIZAÇÃO)
 * ============================================ */

static void liberta_dados_q5(gpointer data) {
    g_free(data);
}

void gestor_flights_init_cache_q5(GestorFlights *g) {
    if (!g) return;
    g->cache_q5 = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, liberta_dados_q5);
}

void gestor_flights_add_atraso_q5(GestorFlights *g, const char *airline, int delay) {
    if (!g || !g->cache_q5 || !airline) return;
    
    DadosAtrasoQ5 *dados = g_hash_table_lookup(g->cache_q5, airline);
    if (!dados) {
        dados = g_new(DadosAtrasoQ5, 1);
        dados->total_delay = 0;
        dados->count = 0;
        g_hash_table_insert(g->cache_q5, g_strdup(airline), dados);
    }
    dados->total_delay += delay;
    dados->count++;
}

DadosAtrasoQ5 *gestor_flights_get_atraso_q5(GestorFlights *g, const char *airline) {
    if (!g || !g->cache_q5 || !airline) return NULL;
    return g_hash_table_lookup(g->cache_q5, airline);
}

void gestor_flights_foreach_q5(GestorFlights *g, AirlineIterFunc func, void *user_data) {
    if (!g || !g->cache_q5 || !func) return;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->cache_q5);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char *airline = (const char *)key;
        DadosAtrasoQ5 *dados = (DadosAtrasoQ5 *)value;
        func(airline, dados->total_delay, dados->count, user_data);
    }
}
