#include "gestor_reservations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GestorReservations *gestor_reservations_novo() {
    GestorReservations *g = malloc(sizeof(GestorReservations));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    return g;
}

void gestor_reservations_destroy(GestorReservations *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela);
    free(g);
}

void gestor_reservations_inserir(GestorReservations *g, Reserva *reserva) {
    if (!g || !reserva) return;
    g_hash_table_insert(g->tabela, strdup(reserva->id_reserva), reserva);
}

int gestor_reservations_existe(GestorReservations *g, const char *id_reserva) {
    return g && id_reserva && g_hash_table_contains(g->tabela, id_reserva);
}

Reserva *gestor_reservations_procura(GestorReservations *g, const char *id_reserva) {
    if (g == NULL || id_reserva == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, id_reserva);
}

