#include "gestor_entidades/gestor_reservations.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===================================================
// ESTRUTURA 
// ===================================================

/**
 * Estrutura interna do gestor de reservas.
 *
 * Armazena todas as reservas numa hash table indexada pelo seu ID.
 * A tabela liberta automaticamente as chaves e as reservas armazenadas.
 */

struct gestor_reservations {
    GHashTable *tabela;   // Hash table: id_reserva → Reservas*
};

// ============================================
// CRIA GESTOR DE RESERVAS
// ============================================ */

GestorReservations *gestor_reservations_cria(void) {
    GestorReservations *g = malloc(sizeof(struct gestor_reservations));
    if (!g) return NULL;
    g->tabela = g_hash_table_new_full(
        g_str_hash, 
        g_str_equal, 
        g_free,          // liberta chave
        libertaReserva   // liberta Reservas*
    );
    return g;
}

// =================================================== 
// OPERAÇÕES BÁSICAS 
// =================================================== 

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

// ============================================
// DESTRÓI GESTOR DE RESERVAS
// ============================================ */

void gestor_reservations_liberta(GestorReservations *g) {
    if (!g) return;
    if (g->tabela) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}

/**
 * Função auxiliar usada na Q6.
 *
 * Percorre todas as reservas e devolve uma GSList* contendo as reservas
 * cujo id_pessoa_reservou coincide com o doc_number fornecido.
 */

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

// ============================================
// NOVA FUNÇÃO PARA FASE 2 (encapsulamento)
// ============================================ 

void gestor_reservations_foreach(GestorReservations *g, ReservationIterFunc f, void *user_data) {
    if (!g || !g->tabela || !f) return;
    
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, g->tabela);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        f((Reservas *)value, user_data);
    }
}
/**
 * Conta quantas reservas incluem um determinado voo.
 *
 * Percorre todas as reservas e verifica se o flight_id aparece
 * na lista de voos reservados.
 */

int gestor_reservations_conta_por_voo(GestorReservations *g, const char *flight_id) {
    if (!g || !g->tabela || !flight_id) return 0;
    
    int count = 0;
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, g->tabela);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Reservas *r = (Reservas *)value;
        int n_voos = r_get_lista_n_voos(r);
        
        for (int i = 0; i < n_voos; i++) {
            char *voo_id = r_get_voo_por_indice(r, i);
            if (voo_id && strcmp(voo_id, flight_id) == 0) {
                count++;
                g_free(voo_id);
                break;
            }
            g_free(voo_id);
        }
    }
    
    return count;
}
