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
    if (g == NULL || a == NULL || a->identifier == NULL) return;
    g_hash_table_insert(g->tabela, strdup(a->identifier), a);
}

Aeroporto *gestor_airports_procura(GestorAirports *g, const char *identifier) {
    if (g == NULL || identifier == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, identifier);
}

GHashTable *gestor_airports_table(GestorAirports *g) {
    return (g != NULL) ? g->tabela : NULL;
}

void gestor_airports_liberta(GestorAircrafts *g) {
    if (g == NULL) return;
    if (g->tabela != NULL) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}
