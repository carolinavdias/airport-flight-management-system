#include "gestor_entidades/gestor_passengers.h"
#include "entidades/passengers.h"
#include "entidades/flights.h"
#include <stdlib.h>
#include <glib.h>
#include <stdio.h>

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

    int id = passenger_get_id(p);

    int *key = g_new(int, 1);
    *key = id;
    
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

        //obtém ID do passageiro (int)
        int id_passageiro = passenger_get_id(p);

        //converte para string
        char id_str[32];
        snprintf(id_str, sizeof(id_str), "%d", id_passageiro);

        //compara com flight_id
        if (strcmp(id_str, flight_id) == 0) {
            count++;
        }
    }

    return count;
}

bool gestor_passengers_existe(GestorPassengers *g, int id_passageiro) {
    if (!g) return false;
    return g_hash_table_contains(g->tabela_passageiros, &id_passageiro);
}