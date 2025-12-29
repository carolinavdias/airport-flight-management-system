#include "gestor_entidades/gestor_passengers.h"
#include "entidades/passengers.h"
#include "entidades/flights.h"
#include <stdlib.h>
#include <glib.h>
#include <stdio.h>
#include <string.h>

typedef struct gestor_passengers {
    GHashTable *tabela_passageiros;
} GestorPassengers;

GestorPassengers *gestor_passengers_novo() {
    GestorPassengers *g = malloc(sizeof(GestorPassengers));
    if (!g) return NULL;

    g->tabela_passageiros = g_hash_table_new_full(
        g_int_hash,
        g_int_equal,
        g_free, 
        (GDestroyNotify)libertaPassageiro  
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

    char *key = g_strdup(passenger_get_id(p));

    g_hash_table_insert(g->tabela_passageiros, key, p);
}

int gestor_passengers_conta_por_voo(GestorPassengers *g, const char *flight_id) {
    if (!g || !flight_id) return 0;

    int count = 0;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, g->tabela_passageiros);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Passageiros *p = (Passageiros *)value;

        const char *p_flight = passenger_get_id(p); 
        if (strcmp(p_flight, flight_id) == 0) count++;
    }

    return count;
}

bool gestor_passengers_existe(GestorPassengers *g, int id_passageiro) {
    if (!g) return false;
    return g_hash_table_contains(g->tabela_passageiros, &id_passageiro);
}

void gestor_passengers_foreach(GestorPassengers *g, PassengerIterFunc f, void *user_data) {
    if (!g || !g->tabela_passageiros || !f) return;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->tabela_passageiros);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        f((Passageiros *)value, user_data);
    }
}