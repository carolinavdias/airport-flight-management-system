#include <stdlib.h>
#include "gestor_aircrafts.h"

GestorAircrafts *gestor_aircrafts_cria(void) {
    GestorAircrafts *g = malloc(sizeof(GestorAircrafts));
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, free, libertaAeronave);
    return g;
}

void gestor_aircrafts_insere(GestorAircrafts *g, Aeronave *a) {
    g_hash_table_insert(g->tabela, strdup(a->identifier), a);
}

Aeronave *gestor_aircrafts_get(GestorAircrafts *g, const char *identifier) {
    return g_hash_table_procura(g->tabela, identifier);
}

GHashTable *gestor_aircrafts_table(GestorAircrafts *g) {
    return g->tabela;
}

void gestor_aircrafts_liberta(GestorAircrafts *g) {
    g_hash_table_destroy(g->tabela);
    free(g);
}
