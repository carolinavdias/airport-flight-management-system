#include "gestor_entidades/gestor_flights.h"

typedef struct gestor_flights {
    GHashTable *tabela_voos;   //chave: id do voo (char*), valor: Voo*
} GestorFlights;

GestorFlights *gestor_flights_novo() {
    GestorFlights *g = malloc(sizeof(GestorFlights));
    if (g == NULL) return NULL;
    g->tabela_voos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaVoo);
    return g;
}

void gestor_flights_destroy(GestorFlights *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_voos);
    free(g);
}

void gestor_flights_inserir(GestorFlights *g, Voo *voo) {
    if (!g || !voo || !voo_get_flight_id(voo)) return;
    g_hash_table_insert(g->tabela_voos, strdup(voo_get_flight_id(voo)), voo);
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

GHashTable *gestor_flights_table(GestorFlights *g) {
    return (g != NULL) ? g->tabela_voos : NULL;
}