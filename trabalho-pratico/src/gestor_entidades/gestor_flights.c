#include "gestor_entidades/gestor_flights.h"
#include <stdlib.h>
#include <string.h>

GestorFlights *gestor_flights_novo() {
    GestorFlights *g = malloc(sizeof(GestorFlights));
    if (g == NULL) return NULL;
    g->tabela_voos = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    return g;
}

void gestor_flights_destroy(GestorFlights *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_voos);
    free(g);
}

void gestor_flights_inserir(GestorFlights *g, Voo *voo) {
    if (!g || !voo) return;
    g_hash_table_insert(g->tabela_voos, strdup(voo->flight_id), voo);
}

bool gestor_flights_existe(GestorFlights *g, const char *id_voo) {
    if (!g || !id_voo) return false;
    return g_hash_table_contains(g->tabela_voos, id_voo);
}

const char *gestor_flights_obter_origem(GestorFlights *g, const char *id_voo) {
    if (!g) return NULL;
    Voo *v = g_hash_table_lookup(g->tabela_voos, id_voo);
    return v ? v->code_origin : NULL;
}

const char *gestor_flights_obter_destino(GestorFlights *g, const char *id_voo) {
    if (!g) return NULL;
    Voo *v = g_hash_table_lookup(g->tabela_voos, id_voo);
    return v ? v->code_destination : NULL;
}

GHashTable *gestor_flights_table(GestorFlights *g) {
    return (g != NULL) ? g->tabela_voos : NULL;
}