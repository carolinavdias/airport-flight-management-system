#include "gestor_entidades/gestor_aircrafts.h"
#include <stdlib.h>
#include <glib.h>
//#include <string.h>

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/** 
 * Estrutura interna do gestor aeronaves.
 */
typedef struct gestor_aircrafts {
    GHashTable *tabela;   /**< Hash table: id_aeronave -> Aeronave* */
} GestorAircrafts; 

/* ============================================
 * GESTÃO DE AERONAVES
 * ============================================ */

/**
 * Cria e inicializa o gestor de aeronaves.
 * A tabela de dispersão utiliza strings como chave
 * e liberta automaticamente as aeronaves armazenadas.
 */

GestorAircrafts *gestor_aircrafts_cria(void) {
    GestorAircrafts *g = malloc(sizeof(GestorAircrafts));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeronave);
    return g;
}

/**
 * Insere uma aeronave na tabela do gestor.
 * O identificador da aeronave é duplicado
 * e usado como chave.
 */

void gestor_aircrafts_insere(GestorAircrafts *g, Aeronave *a) { 
    if (!g || !a) return; 
    
    const char *id_int = aircraft_get_identifier(a); 
    if (!id_int) return; 
    
    char *id = g_strdup(id_int); 
    g_hash_table_insert(g->tabela, id, a); 
}

/**
 * Procura uma aeronave pelo seu identificador.
 */

Aeronave *gestor_aircrafts_procura(GestorAircrafts *g, const char *identifier) {
    if (g == NULL || identifier == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, identifier);
}

/**
 * Verifica se uma aeronave existe na tabela.
 */

int gestor_aircrafts_existe(GestorAircrafts *g, const char *identifier) {
    return g && g->tabela && identifier &&
           g_hash_table_contains(g->tabela, identifier);
}

/**
 * Percorre todas as aeronaves do gestor e aplica
 * a função fornecida a cada uma.
 */

void gestor_aircrafts_foreach(GestorAircrafts *g, AircraftIterFunc f, void *user_data) {
    if (!g || !g->tabela || !f) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, g->tabela);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        f((Aeronave *)value, user_data);
    }
}

/* ============================================
 * DESTRÓI GESTOR DE RESERVAS
 * ============================================ */

/**
 * Liberta o gestor e todos os recursos associados.
 */

void gestor_aircrafts_liberta(GestorAircrafts *g) {
    if (g == NULL) return;
    if (g->tabela != NULL) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}