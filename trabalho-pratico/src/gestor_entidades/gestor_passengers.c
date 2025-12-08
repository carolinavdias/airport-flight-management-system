#include "gestor_entidades/gestor_passengers.h"
#include <stdlib.h>

static gpointer key_int_new(int x) {
    int *p = malloc(sizeof(int));
    *p = x;
    return p;
}

GestorPassengers *gestor_passengers_novo() {
    GestorPassengers *g = malloc(sizeof(GestorPassengers));
    g->tabela_passageiros = g_hash_table_new_full(g_int_hash, g_int_equal, free, NULL);
    return g;
}

void gestor_passengers_destroy(GestorPassengers *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_passageiros);
    free(g);
}

void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p) {
    if (!g || !p) return;
    g_hash_table_insert(g->tabela_passageiros, key_int_new(p->id_passageiro), p);
}

bool gestor_passengers_existe(GestorPassengers *g, int id_passageiro) {
    if (!g) return false;
    return g_hash_table_contains(g->tabela_passageiros, &id_passageiro);
}