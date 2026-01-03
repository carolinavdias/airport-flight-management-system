#include "gestor_entidades/gestor_passengers.h"
#include "entidades/passengers.h"
#include <glib.h>
#include <stdlib.h>
#include <string.h>

// =================================================== 
// ESTRUTURA
// =================================================== 

/**
 * Estrutura interna do gestor de passageiros.
 *
 * Armazena todos os passageiros numa hash table indexada pelo seu ID.
 * A tabela liberta automaticamente as chaves e os passageiros armazenados.
 */

struct gestor_passengers {
    GHashTable *tabela_passageiros;  /**< Hash table: id_passageiro -> Passageiros* */
};

// =================================================== 
// CRIA GESTOR DE PASSAGEIROS
// =================================================== 

GestorPassengers *gestor_passengers_novo(void) {
    GestorPassengers *g = malloc(sizeof(struct gestor_passengers));
    if (!g) return NULL;

    g->tabela_passageiros = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,                            // liberta chave
        (GDestroyNotify)libertaPassageiro  // liberta Passageiros*
    );

    return g;
}

// =================================================== 
// DESTRÓI GESTOR DE PASSAGEIROS
// =================================================== 

void gestor_passengers_destroy(GestorPassengers *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_passageiros);
    free(g);
}

// =================================================== 
// INSERÇÃO
// =================================================== 

void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p) {
    if (!g || !p) return;

    const char *id = passenger_get_id(p);
    if (!id) return;
    
    char *key = g_strdup(id);
    g_hash_table_insert(g->tabela_passageiros, key, p);
}

// =================================================== 
// CONSULTAS
// =================================================== 

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

// =================================================== 
// ITERAÇÃO
// =================================================== 

void gestor_passengers_foreach(GestorPassengers *g, PassengerIterFunc f, void *user_data) {
    if (!g || !g->tabela_passageiros || !f) return;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->tabela_passageiros);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        f((Passageiros *)value, user_data);
    }
}