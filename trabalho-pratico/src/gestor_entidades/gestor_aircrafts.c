#include "gestor_aircrafts.h"
#include <stdlib.h>
#include <string.h>

GestorAircrafts *gestor_aircrafts_cria(void) {
    GestorAircrafts *g = malloc(sizeof(GestorAircrafts));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, free, libertaAeronave);
    return g;
}

void gestor_aircrafts_insere(GestorAircrafts *g, Aeronave *a) {
    if (g == NULL || a == NULL || a->identifier == NULL) return;
    g_hash_table_insert(g->tabela, strdup(a->identifier), a);
}

Aeronave *gestor_aircrafts_procura(GestorAircrafts *g, const char *identifier) {
    if (g == NULL || identifier == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, identifier);
}

GHashTable *gestor_aircrafts_table(GestorAircrafts *g) {
    return (g != NULL) ? g->tabela : NULL;
}

void gestor_aircrafts_liberta(GestorAircrafts *g) {
    if (g == NULL) return;
    if (g->tabela != NULL) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}