#include "entidades/airports.h"
#include <glib.h>

/**
 * Este ficheiro contém a definição da estrutura interna Aeroporto e
 * a implementação das funções de acesso (getters), modificação (setters),
 * criação e libertação de memória associadas a aeroportos.
 */

// Estrutura interna que representa um aeroporto

typedef struct aeroporto {
    char *code_IATA;          /**< Código IATA do aeroporto */
    char *name;               /**< Nome do aeroporto */
    char *city;               /**< Cidade onde o aeroporto se localiza */
    char *country;            /**< País do aeroporto */
    Tipo_aeroporto type;      /**< Tipo de aeroporto */
} Aeroporto;

/* ============================================
 * GETTERS
 * ============================================ */

 /*
  * Devolvem cópias (g_strdup) dos campos internos de modo a evitar
  * o acesso direto à estrutura e garantir encapsulamento.
  * 
  * A memória devolvida deve ser libertada pelo utilizador.
  */

char *airport_get_code_IATA (const Aeroporto *a) {
    return g_strdup(a->code_IATA);
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

Tipo_aeroporto airport_get_type (Aeroporto *a) {
    return a->type;
}

/* ============================================
 * SETTERS
 * ============================================ */

 /* 
  * Substituem os valores atuais dos campos, libertando previamente
  * a memória associada para evitar fugas de memória.
  */

void airport_set_name (Aeroporto *a, char *name_) {
   g_free(a->name);
   a->name = g_strdup(name_);
}

void airport_set_city (Aeroporto *a, char *city_) {
   g_free(a->city);
   a->city = g_strdup(city_);
}

void airport_set_country (Aeroporto *a, char *country_) {
   g_free(a->country);
   a->country = g_strdup(country_);
}

void airport_set_code_IATA (Aeroporto *a, char *code_) {
    g_free(a->code_IATA);
   a->code_IATA = g_strdup(code_);  //estava a->country (bug)
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

// Liberta todos os campos internos e a própria estrutura

void libertaAeroporto(void *data) {
    Aeroporto *a = data;
    if (!a) return;
    if (a->name) g_free(a->name);
    if (a->city) g_free(a->city);
    if (a->country) g_free(a->country);
    if (a->code_IATA) g_free(a->code_IATA);
    g_free(a);
}

