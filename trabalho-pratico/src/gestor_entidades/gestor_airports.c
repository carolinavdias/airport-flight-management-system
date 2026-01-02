#include "gestor_entidades/gestor_airports.h"
#include <stdlib.h>
#include <glib.h>

typedef struct gestor_airports {
    GHashTable *tabela;
    GHashTable *contagens_chegadas;
    GHashTable *contagens_partidas;
} GestorAirports;

GestorAirports *gestor_airports_cria(void) {
    GestorAirports *g = malloc(sizeof(GestorAirports));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeroporto);
    g->contagens_chegadas = NULL;
    g->contagens_partidas = NULL;
    return g;
}

void gestor_airports_init_contagens(GestorAirports *g) {
    if (!g) return;
    g->contagens_chegadas = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    g->contagens_partidas = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}

void gestor_airports_insere(GestorAirports *g, Aeroporto *a) {
    if (g == NULL || a == NULL) return;
    char *code = airport_get_code_IATA(a);
    if (code == NULL) return;
    g_hash_table_insert(g->tabela, code, a);
}

Aeroporto *gestor_airports_procura(GestorAirports *g, const char *code_IATA) {
    if (g == NULL || code_IATA == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, code_IATA);
}

int gestor_airports_existe(GestorAirports *g, const char *code_IATA) {
    return g && code_IATA && g_hash_table_contains(g->tabela, code_IATA);
}

void gestor_airports_liberta(GestorAirports *g) {
    if (g == NULL) return;
    if (g->tabela) g_hash_table_destroy(g->tabela);
    if (g->contagens_chegadas) g_hash_table_destroy(g->contagens_chegadas);
    if (g->contagens_partidas) g_hash_table_destroy(g->contagens_partidas);
    free(g);
}

void gestor_airports_incrementa_chegadas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_chegadas) return;
    gpointer valor = g_hash_table_lookup(g->contagens_chegadas, code);
    int count = GPOINTER_TO_INT(valor) + 1;
    g_hash_table_insert(g->contagens_chegadas, g_strdup(code), GINT_TO_POINTER(count));
}

void gestor_airports_incrementa_partidas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_partidas) return;
    gpointer valor = g_hash_table_lookup(g->contagens_partidas, code);
    int count = GPOINTER_TO_INT(valor) + 1;
    g_hash_table_insert(g->contagens_partidas, g_strdup(code), GINT_TO_POINTER(count));
}

int gestor_airports_get_chegadas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_chegadas) return 0;
    return GPOINTER_TO_INT(g_hash_table_lookup(g->contagens_chegadas, code));
}

int gestor_airports_get_partidas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_partidas) return 0;
    return GPOINTER_TO_INT(g_hash_table_lookup(g->contagens_partidas, code));
}
