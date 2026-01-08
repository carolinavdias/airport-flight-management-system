#include "entidades/airports.h"
#include "utils/utils.h"
#include <glib.h>

/**
 * Este ficheiro contém a definição da estrutura interna Aeroporto e
 * a implementação das funções de acesso (getters), modificação (setters),
 * criação e libertação de memória associadas a aeroportos.
 */

/**
 * Estrutura interna que representa um aeroporto
 */

typedef struct aeroporto {
    const char *code_IATA;          /**< Código IATA do aeroporto */
    const char *name;               /**< Nome do aeroporto */
    const char *city;               /**< Cidade onde o aeroporto se localiza */
    const char *country;            /**< País do aeroporto */
    Tipo_aeroporto type;            /**< Tipo de aeroporto */
} Aeroporto;

/* ============================================
 * GETTERS
 * ============================================ */

/** 
 * Devolvem ponteiros constantes para os campos internos, geridos pelo StringPool.
 * O utilizador não deve libertar esta memória.
 */

const char *airport_get_code_IATA (const Aeroporto *a) {
    return a->code_IATA;
}

const char *airport_get_name (const Aeroporto *a) {
    return a->name;
}

const char *airport_get_city (const Aeroporto *a) {
    return a->city;
}

const char *airport_get_country (const Aeroporto *a) {
    return a->country;
}

Tipo_aeroporto airport_get_type (const Aeroporto *a) {
    return a->type;
}

/* ============================================
 * SETTERS
 * ============================================ */

/** 
 * Atualizam os campos usando o StringPool.
 * Não é necessário libertar as strings individualmente.
 */

void airport_set_name (Aeroporto *a, char *name_, StringPool *pool) {
   a->name = string_pool_get(pool,name_); 
}

void airport_set_city (Aeroporto *a, char *city_, StringPool *pool) {
   a->city = string_pool_get(pool,city_);
}

void airport_set_country (Aeroporto *a, char *country_, StringPool *pool) {
   a->country = string_pool_get(pool,country_); 
}

void airport_set_code_IATA (Aeroporto *a, char *code_, StringPool *pool) {
   a->code_IATA = string_pool_get(pool,code_); 
}

/**
 * Conversão do tipo textual do dataset para o tipo interno.
 * A decisão é feita com base no(s) primeiro(s) caractere(s).
 */

void airport_set_type (Aeroporto *a, char *tipo) {
   switch (tipo[0]) {
	case 's' : if (tipo[1] == 'm') a->type = TIPO_SMALL_AIRPORT;
	      else a->type = TIPO_SEAPLANE_BASE;
	      break;
	case 'm' : a->type = TIPO_MEDIUM_AIRPORT;
	      break;
	case 'l' : a->type = TIPO_LARGE_AIRPORT;
	      break;
	case 'h' : a->type = TIPO_HELIPORT;
	      break;
   }
}

/* ============================================
 * CRIA AEROPORTO
 * ============================================ */

/**
 * A estrutura é inicializada a zero e deve ser posteriormente
 * preenchida com os setters.
 */

Aeroporto *criaAeroporto () {
    Aeroporto *ap = calloc (1, sizeof *ap);
    return ap;
}

/* ============================================
 * DESTRÓI AEROPORTO
 * ============================================ */

/**
 * Liberta todos os campos internos e a própria estrutura.
 */

void libertaAeroporto(void *data) {
    Aeroporto *a = data;
    if (!a) return;
    g_free(a);
}

