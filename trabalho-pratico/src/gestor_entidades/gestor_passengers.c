#include "gestor_entidades/gestor_passengers.h"
#include "entidades/passengers.h"
#include "entidades/flights.h"
#include <stdlib.h>
#include <glib.h>

typedef struct gestor_passengers {
    GHashTable *tabela_passageiros;
} GestorPassengers;

/*
static gpointer key_int_new(int x) {
    int *p = malloc(sizeof(int));
    *p = x;
    return p;
}
*/

GestorPassengers *gestor_passengers_novo() {
    GestorPassengers *g = malloc(sizeof(GestorPassengers));
    if (!g) return NULL;

    g->tabela_passageiros = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,        //liberta chave (string duplicada)
        libertaPassageiro  //função que liberta Passageiro
    );

    return g;
}

void gestor_passengers_destroy(GestorPassengers *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_passageiros);
    free(g);
}

void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p) {
    if (!g || !p) return;

    int id = passenger_get_id(p);

    g_hash_table_insert(
        g->tabela_passageiros,
        GINT_TO_POINTER(id),
        p
    );
}

int gestor_passengers_conta_por_voo(GestorPassengers *g, const char *flight_id) {
    if (!g || !flight_id) return 0;

    int *count = g_hash_table_lookup(g->tabela_passageiros, flight_id);
    return count ? *count : 0;
}

bool gestor_passengers_existe(GestorPassengers *g, int id_passageiro) {
    if (!g) return false;
    return g && id_passageiro && g_hash_table_contains(g->tabela_passageiros, &id_passageiro);
}