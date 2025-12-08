#include "gestor_entidades/gestor_reservations.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

GestorReservations *gestor_reservations_cria(void) {
    GestorReservations *g = malloc(sizeof(GestorReservations));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    return g;
}

void gestor_reservations_insere(GestorReservations *g, Reservas *reserva) {
    if (!g || !reserva) return;
    g_hash_table_insert(g->tabela, strdup(reserva->id_reserva), reserva);
}

int gestor_reservations_existe(GestorReservations *g, const char *id_reserva) {
    if (!g || !id_reserva) return 0;
    return g_hash_table_contains(g->tabela, id_reserva);
}

Reservas *gestor_reservations_procura(GestorReservations *g, const char *id_reserva) {
    if (g == NULL || id_reserva == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, id_reserva);
}

GHashTable *gestor_reservations_table(GestorReservations *g) {
    return (g != NULL) ? g->tabela : NULL;
}

void gestor_reservations_liberta(GestorReservations *g) {
    if (g == NULL) return;
    if (g->tabela != NULL) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}