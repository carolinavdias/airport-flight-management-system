#include "gestor_entidades/gestor_airports.h"
#include <stdlib.h>
#include <glib.h>

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * Implementação interna do gestor de aeroportos.
 *
 * O gestor utiliza:
 *  - uma hash table principal para armazenar os aeroportos,
 *    indexados pelo código IATA;
 *  - duas tabelas auxiliares para contagem de chegadas e partidas,
 *    usadas para otimização de consultas estatísticas.
 */

typedef struct gestor_airports {
    GHashTable *tabela;               /**< Aeroportos por código IATA */
    GHashTable *contagens_chegadas;   /**< Contagem de chegadas por aeroporto */
    GHashTable *contagens_partidas;   /**< Contagem de partidas por aeroporto */
} GestorAirports;

/* ============================================
 * GESTÃO DE AEROPORTOS
 * ============================================ */

/** 
 * Cria e inicializa o gestor de aeroportos.
 *
 * Aloca memória para o gestor e cria a hash table principal,
 * responsável por armazenar os aeroportos.
 *
 * As tabelas de contagens são inicializadas a NULL e só são criadas
 * quando explicitamente pedidas.
 */

GestorAirports *gestor_airports_cria(void) {
    GestorAirports *g = malloc(sizeof(GestorAirports));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeroporto);
    g->contagens_chegadas = NULL;
    g->contagens_partidas = NULL;
    return g;
}

/* ============================================
 * CONTAGENS DE PASSAGEIROS (Q1 OTIMIZADA)
 * ============================================ */

/** 
 * Inicializa as tabelas auxiliares de contagem de chegadas e partidas.
 *
 * Cada tabela associa um código IATA a um inteiro que representa
 * o número acumulado de ocorrências.
 *
 * Deve ser chamada antes de usar as funções de incremento ou consulta.
 */

void gestor_airports_init_contagens(GestorAirports *g) {
    if (!g) return;
    g->contagens_chegadas = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
    g->contagens_partidas = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);
}

/** 
 * Insere um aeroporto no gestor.
 *
 * O aeroporto é armazenado na tabela principal e indexado pelo
 * seu código IATA.
 *
 * A chave é duplicada para garantir encapsulamento.
 */

void gestor_airports_insere(GestorAirports *g, Aeroporto *a) {
    if (g == NULL || a == NULL) return;
    const char *code = airport_get_code_IATA(a);
    if (code == NULL) return;
    char *key = strdup(code);
    g_hash_table_insert(g->tabela, key, a);
}

/** 
 * Procura um aeroporto no gestor através do código IATA.
 *
 * Devolve o ponteiro para a estrutura Aeroporto armazenada,
 * ou NULL caso o aeroporto não exista.
 */

Aeroporto *gestor_airports_procura(GestorAirports *g, const char *code_IATA) {
    if (g == NULL || code_IATA == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, code_IATA);
}

/**
 * Verifica se um aeroporto existe no gestor.
 *
 * Retorna verdadeiro (1) se existir uma entrada com o código IATA
 * indicado, ou falso (0) caso contrário.
 */

int gestor_airports_existe(GestorAirports *g, const char *code_IATA) {
    return g && code_IATA && g_hash_table_contains(g->tabela, code_IATA);
}

/** 
 * Incrementa o contador de chegadas de um aeroporto.
 *
 * Se o aeroporto ainda não tiver registos, o contador é iniciado a 1.
 */

void gestor_airports_incrementa_chegadas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_chegadas) return;
    gpointer valor = g_hash_table_lookup(g->contagens_chegadas, code);
    int count = GPOINTER_TO_INT(valor) + 1;
    g_hash_table_insert(g->contagens_chegadas, g_strdup(code), GINT_TO_POINTER(count));
}

/** 
 * Incrementa o contador de partidas de um aeroporto.
 *
 * Funciona de forma idêntica ao contador de chegadas, mas utilizando
 * a tabela de partidas.
 */

void gestor_airports_incrementa_partidas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_partidas) return;
    gpointer valor = g_hash_table_lookup(g->contagens_partidas, code);
    int count = GPOINTER_TO_INT(valor) + 1;
    g_hash_table_insert(g->contagens_partidas, g_strdup(code), GINT_TO_POINTER(count));
}

/** 
 * Obtém o número total de chegadas registadas para um aeroporto.
 *
 * Se o aeroporto não tiver registos ou as contagens não estiverem
 * inicializadas, devolve 0.
 */

int gestor_airports_get_chegadas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_chegadas) return 0;
    return GPOINTER_TO_INT(g_hash_table_lookup(g->contagens_chegadas, code));
}

/** 
 * Obtém o número total de partidas registadas para um aeroporto.
 *
 * Se o aeroporto não tiver registos ou as contagens não estiverem
 * inicializadas, devolve 0.
 */

int gestor_airports_get_partidas(GestorAirports *g, const char *code) {
    if (!g || !code || !g->contagens_partidas) return 0;
    return GPOINTER_TO_INT(g_hash_table_lookup(g->contagens_partidas, code));
}

/* ============================================
 * DESTRÓI GESTOR DE AEROPORTOS
 * ============================================ */

/** 
 * Liberta toda a memória associada ao gestor de aeroportos.
 *
 * Destrói a tabela principal, as tabelas de contagens (se existirem)
 * e, por fim, liberta a estrutura do gestor.
 */

void gestor_airports_liberta(GestorAirports *g) {
    if (g == NULL) return;
    if (g->tabela) g_hash_table_destroy(g->tabela);
    if (g->contagens_chegadas) g_hash_table_destroy(g->contagens_chegadas);
    if (g->contagens_partidas) g_hash_table_destroy(g->contagens_partidas);
    free(g);
}
