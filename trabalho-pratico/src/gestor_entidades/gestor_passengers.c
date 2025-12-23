#include "gestor_entidades/gestor_passengers.h"
#include <stdlib.h>
#include <glib.h>

typedef struct gestor_passengers {
    GHashTable *tabela_passageiros;
} GestorPassengers;

static gpointer key_int_new(int x) {
    int *p = malloc(sizeof(int));
    *p = x;
    return p;
}

GestorPassengers *gestor_passengers_novo() {
    GestorPassengers *g = malloc(sizeof(GestorPassengers));
    if (g == NULL) return NULL;
    g->tabela_passageiros = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, libertaPassageiro);
    return g;
}

void gestor_passengers_destroy(GestorPassengers *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_passageiros);
    free(g);
}

void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p) {
    if (!g || !p) return;
    g_hash_table_insert(g->tabela_passageiros, key_int_new(passenger_get_id(p)), p);
}

bool gestor_passengers_existe(GestorPassengers *g, int id_passageiro) {
    if (!g) return false;
    return g && id_passageiro && g_hash_table_contains(g->tabela_passageiros, &id_passageiro);
}