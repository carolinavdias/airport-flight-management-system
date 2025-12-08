#include "gestor_airports.h"
#include <stdlib.h>
#include <string.h>

GestorAirports *gestor_airports_cria(void) {
    GestorAirports *g = malloc(sizeof(GestorAirports));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, free, libertaAeroporto);
    return g;
}

void gestor_airports_insere(GestorAirports *g, Aeroporto *a) {
    if (g == NULL || a == NULL || a->code_IATA == NULL) return;
    g_hash_table_insert(g->tabela, strdup(a->code_IATA), a);
}

Aeroporto *gestor_airports_procura(GestorAirports *g, const char *code_IATA) {
    if (g == NULL || code_IATA == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, code_IATA);
}

int gestor_airports_existe(GestorFlights *g, const char *codigo_IATA) {
    return g && codigo_IATA && g_hash_table_contains(g->tabela, codigo_IATA);
}

GHashTable *gestor_airports_table(GestorAirports *g) {
    return (g != NULL) ? g->tabela : NULL;
}

void gestor_airports_liberta(GestorAirports *g) {
    if (g == NULL) return;
    if (g->tabela != NULL) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}
