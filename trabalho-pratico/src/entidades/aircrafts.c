#include "entidades/aircrafts.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

/**
 * Este ficheiro contém a definição da estrutura interna Aeronave e
 * a implementação das funções de acesso (getters), modificação 
 * (setters), criação e libertação de aeronaves.
 *
 * As aeronaves são construídas a partir de linhas do ficheiro
 * aircrafts.csv, através do encapsulamento da estrutura e acesso 
 * controlado via getters.
 */

/**
 * Estrutura interna que representa uma aeronave 
 */

struct aeronave {
    const char *identifier;      /**< Identificador único da aeronave */
    const char *manufacturer;    /**< Fabricante */
    const char *model;           /**< Modelo */
    int year;                    /**< Ano de fabrico */
};

/* ============================================
 * GETTERS
 * ============================================ */

/** Devolvem ponteiros constantes para os campos internos. */

const char *aircraft_get_identifier(const Aeronave *a) {
    return a->identifier;
}

const char *aircraft_get_manufacturer(const Aeronave *a) {
    return a->manufacturer;
}

const char *aircraft_get_model(const Aeronave *a) {
    return a->model;
}

int aircraft_get_year(const Aeronave *a) {
    return a->year;
}

/* ============================================
 * SETTERS
 * ============================================ */

/** Atualizam os campos usando o StringPool (não é necessário libertar memória). */

void aircraft_set_id(Aeronave *a, char *s, StringPool *pool) {
    if (!a || !s) return;
    a->identifier = string_pool_get(pool,s); 
}

void aircraft_set_manuf(Aeronave *a, char *s, StringPool *pool) {
    if (!a || !s) return;
    a->manufacturer = string_pool_get(pool,s); 
}

void aircraft_set_model(Aeronave *a, char *model_, StringPool *pool) {
    if (!a || !model_) return;
    a->model = string_pool_get(pool,model_); 
}

void aircraft_set_year(Aeronave *a, char *year) {
    if (!a || !year) return;
    a->year = atoi(year);
}

/* ============================================
 * CRIA AERONAVE
 * ============================================ */

Aeronave *criaAeronave() {
   Aeronave *a = calloc (1, sizeof *a);
   return a;
}

/* ============================================
 * DESTRÓI AERONAVE
 * ============================================ */

void libertaAeronave(void *data) {
    Aeronave *a = data;
    if (!a) return;
    g_free(a);
}

