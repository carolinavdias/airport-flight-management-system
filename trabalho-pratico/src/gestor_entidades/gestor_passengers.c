#include "gestor_entidades/gestor_passengers.h"
#include "entidades/passengers.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

/* ============================================
 * ESTRUTURA
 * ============================================ */

/**
 * Estrutura interna do gestor de passageiros.
 *
 * Armazena todos os passageiros numa hash table indexada pelo seu ID.
 * A tabela liberta automaticamente as chaves e os passageiros armazenados.
 */

struct gestor_passengers {
    GHashTable *tabela_passageiros;  /**< Tabela principal de passageiros */
    GHashTable *cache_q6;            /**< nacionalidade -> (destino -> count) para Q6 */
};

/* ============================================
 * CRIA GESTOR DE PASSAGEIROS
 * ============================================ */

GestorPassengers *gestor_passengers_novo(void) {
    GestorPassengers *g = malloc(sizeof(struct gestor_passengers));
    if (!g) return NULL;

    g->tabela_passageiros = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,                            // liberta chave
        (GDestroyNotify)libertaPassageiro  // liberta Passageiros*
    );

    g->cache_q6 = NULL;
    return g;
}

/* ============================================
 * DESTRÓI GESTOR DE PASSAGEIROS
 * ============================================ */

void gestor_passengers_destroy(GestorPassengers *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_passageiros);
    if (g->cache_q6) g_hash_table_destroy(g->cache_q6);
    free(g);
}

/* ============================================
 * INSERÇÃO
 * ============================================ */

void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p) {
    if (!g || !p) return;

    const char *id = passenger_get_id(p);
    if (!id) return;

    char *key = g_strdup(id);
    g_hash_table_insert(g->tabela_passageiros, key, p);
}

/* ============================================
 * CONSULTAS
 * ============================================ */

int gestor_passengers_conta_por_voo(GestorPassengers *g, const char *flight_id) {
    if (!g || !flight_id) return 0;

    int count = 0;
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, g->tabela_passageiros);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Passageiros *p = (Passageiros *)value;

        // NOTA: passenger_get_id devolve ponteiro interno (não libertar)
        const char *p_flight = passenger_get_id(p);
        if (p_flight && strcmp(p_flight, flight_id) == 0) {
            count++;
        }
    }

    return count;
}

bool gestor_passengers_existe(GestorPassengers *g, const char *id_passageiro) {
    if (!g || !id_passageiro) return false;

    return g_hash_table_contains(g->tabela_passageiros, id_passageiro);
}

Passageiros *gestor_passengers_procura(GestorPassengers *g, const char *doc_number) {
    if (!g || !doc_number) return NULL;
    return g_hash_table_lookup(g->tabela_passageiros, doc_number);
}

/* ============================================
 * ITERAÇÃO
 * ============================================ */

void gestor_passengers_foreach(GestorPassengers *g, PassengerIterFunc f, void *user_data) {
    if (!g || !g->tabela_passageiros || !f) return;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->tabela_passageiros);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        f((Passageiros *)value, user_data);
    }
}
/* ============================================
 * FUNÇÕES PARA CACHE Q6 (OTIMIZAÇÃO)
 * ============================================ */

static void liberta_destinos_q6(gpointer data) {
    g_hash_table_destroy((GHashTable *)data);
}

void gestor_passengers_init_cache_q6(GestorPassengers *gp) {
    if (!gp) return;
    gp->cache_q6 = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free, liberta_destinos_q6
    );
}

void gestor_passengers_add_destino_q6(GestorPassengers *gp, const char *nacionalidade, const char *destino) {
    if (!gp || !gp->cache_q6 || !nacionalidade || !destino) return;

    GHashTable *destinos = g_hash_table_lookup(gp->cache_q6, nacionalidade);
    if (!destinos) {
        destinos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
        g_hash_table_insert(gp->cache_q6, g_strdup(nacionalidade), destinos);
    }

    int count = GPOINTER_TO_INT(g_hash_table_lookup(destinos, destino));
    g_hash_table_insert(destinos, g_strdup(destino), GINT_TO_POINTER(count + 1));
}

void gestor_passengers_foreach_destinos_q6(GestorPassengers *gp, const char *nacionalidade, DestinoIterFunc func, void *user_data) {
    if (!gp || !gp->cache_q6 || !nacionalidade || !func) return;

    GHashTable *destinos = g_hash_table_lookup(gp->cache_q6, nacionalidade);
    if (!destinos) return;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, destinos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        func((const char *)key, GPOINTER_TO_INT(value), user_data);
    }
}
