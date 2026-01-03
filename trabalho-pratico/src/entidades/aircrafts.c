#include "entidades/aircrafts.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

/**
 * Este ficheiro contém a definição da estrutura interna Aeronave e
 * a implementação das funções de acesso (getters), modificação 
 * (setters), validação, criação e libertação de aeronaves.
 *
 * As aeronaves são construídas a partir de linhas do ficheiro
 * aircrafts.csv, sendo garantido o encapsulamento da estrutura
 * através da devolução de cópias nos getters.
 */

/**
 * Estrutura interna que representa uma aeronave 
 */

struct aeronave {
    char *identifier;      /**< Identificador único da aeronave */
    char *manufacturer;    /**< Fabricante */
    char *model;           /**< Modelo */
    int year;              /**< Ano de fabrico */
};

/* ============================================
 * GETTERS
 * ============================================ */

/*
 * Devolvem cópias (g_strdup) dos campos internos para garantir
 * encapsulamento. A memória devolvida deve ser libertada pelo
 * utilizador.
*/

char *aircraft_get_identifier(const Aeronave *a) {
    return a ? g_strdup(a->identifier) : NULL;
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

/*
 * Atualizam os campos da aeronave, libertando previamente
 * a memória existente para evitar fugas de memória.
*/

void aircraft_set_id(Aeronave *a, char *s) {
    if (!a || !s) return;
    g_free(a->identifier);
    a->identifier = g_strdup(s);
}
void aircraft_set_id2(Aeronave *a, char *s) {
    if (!a) return;
    g_free(a->identifier);
    a->identifier = s;
}

void aircraft_set_manuf(Aeronave *a, char *s) {
    if (!a || !s) return;
    g_free(a->manufacturer);
    a->manufacturer = g_strdup(s);
}

void aircraft_set_model(Aeronave *a, char *model_) {
    if (!a || !model_) return;
    g_free(a->model);
    a->model = g_strdup(model_);
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
    if (a->identifier) g_free(a->identifier);
    if (a->manufacturer) g_free(a->manufacturer);
    if (a->model) g_free(a->model);
    g_free(a);
}

