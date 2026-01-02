#include "gestor_entidades/gestor_flights.h"
#include "entidades/flights.h"
#include "queries/q5.h"
#include "utils/utils.h"
#include <glib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

typedef struct gestor_flights {
    GHashTable *tabela_voos;
    Voo **array_ordenado;      // Array ordenado por data (para Q3)
    int num_voos;
    GHashTable *contagens_aircraft;  // aircraft_id -> count (para Q2)
//    Estrt_aux_q5 *listaQ5;
} GestorFlights;

GestorFlights *gestor_flights_novo() {
    GestorFlights *g = malloc(sizeof(GestorFlights));
    if (g == NULL) return NULL;
    g->tabela_voos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaVoo);
    g->array_ordenado = NULL;
    g->num_voos = 0;
    g->contagens_aircraft = NULL;  // será criado no parsing
//    g->listaQ5 = init_lista();
    return g;
}

void gestor_flights_destroy(GestorFlights *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_voos);
    if (g->array_ordenado) free(g->array_ordenado);
    if (g->contagens_aircraft) g_hash_table_destroy(g->contagens_aircraft);  
    free(g);
}

void gestor_flights_inserir(GestorFlights *g, Voo *voo) {
    if (!g || !voo) return;
    char *flight_id = voo_get_flight_id(voo);
    if (!flight_id) return;
    g_hash_table_insert(g->tabela_voos, flight_id, voo);
}

bool gestor_flights_existe(GestorFlights *g, const char *flight_id) {
    return g && flight_id && g_hash_table_contains(g->tabela_voos, flight_id);
}

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

//REMOVIDO: gestor_flights_table() - VIOLA ENCAPSULAMENTO

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

// ===== ARRAY ORDENADO (para Q3) =====

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

// ===== CONTAGENS DE AIRCRAFT (para Q2) ===== 

void gestor_flights_set_contagens_aircraft(GestorFlights *g, GHashTable *contagens) {
    if (!g) return;
    g->contagens_aircraft = contagens;
}

GHashTable *gestor_flights_get_contagens_aircraft(GestorFlights *g) {
    return g ? g->contagens_aircraft : NULL;
}

// ===== PROCURA =====

Voo *gestor_flights_procura(GestorFlights *g, const char *flight_id) {
    if (!g || !flight_id) return NULL;
    return g_hash_table_lookup(g->tabela_voos, flight_id);
}
