#include "gestor_entidades/gestor_reservations.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct gestor_reservations {
    GHashTable *tabela;
};

GestorReservations *gestor_reservations_cria(void) {
    GestorReservations *g = malloc(sizeof(struct gestor_reservations));
    if (!g) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaReserva);
    return g;
}

void gestor_reservations_insere(GestorReservations *g, Reservas *reserva) {
    if (!g || !reserva) return;
    
    // r_get_id_reserva agora retorna cópia com g_strdup()
    char *id = r_get_id_reserva(reserva);
    if (!id) return;
    
    g_hash_table_insert(g->tabela, id, reserva);  // chave será libertada por g_free
}

int gestor_reservations_existe(GestorReservations *g, const char *id_reserva) {
    return g && id_reserva && g_hash_table_contains(g->tabela, id_reserva);
}

Reservas *gestor_reservations_procura(GestorReservations *g, const char *id_reserva) {
    if (!g || !id_reserva) return NULL;
    return g_hash_table_lookup(g->tabela, id_reserva);
}

void gestor_reservations_liberta(GestorReservations *g) {
    if (!g) return;
    if (g->tabela) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}

// Função auxiliar para Q6 - procurar reservas por passageiro
GSList *gestor_reservations_get_by_passenger(GestorReservations *gr, const char *doc_number) {
    if (!gr || !doc_number) return NULL;

    GSList *lista = NULL;
    int id_pessoa = atoi(doc_number);  // converter string para int

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, gr->tabela);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Reservas *r = (Reservas *)value;
        
        int id_reserva = r_get_id_pessoa_reservou(r);  // retorna int
        
        if (id_reserva == id_pessoa) {
            lista = g_slist_prepend(lista, r);
        }
    }

    return lista;
}