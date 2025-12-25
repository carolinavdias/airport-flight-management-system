#include "gestor_entidades/gestor_airports.h"
#include <stdlib.h>

//tabela guarda todos os aeroportos
typedef struct gestor_airports {
    GHashTable *tabela;
} GestorAirports;

GestorAirports *gestor_airports_cria(void) {
    GestorAirports *g = malloc(sizeof(GestorAirports));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeroporto);
    return g;
}

void gestor_airports_insere(GestorAirports *g, Aeroporto *a) {
    if (g == NULL || a == NULL || airport_get_code_IATA(a) == NULL) return;
    g_hash_table_insert(g->tabela, airport_get_code_IATA(a), a);
}

Aeroporto *gestor_airports_procura(GestorAirports *g, const char *code_IATA) {
    if (g == NULL || code_IATA == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, code_IATA);
}

int gestor_airports_existe(GestorAirports *g, const char *code_IATA) {
    return g && code_IATA && g_hash_table_contains(g->tabela, code_IATA);
}

//ALTERAR
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