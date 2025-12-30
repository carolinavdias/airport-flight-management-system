#include "entidades/aircrafts.h"
#include "utils/utils.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

struct aeronave {
    char *identifier;
    char *manufacturer;
    char *model;
    int year;
};

//GETTERS com g_strdup() para encapsulamento
char *aircraft_get_identifier(Aeronave *a) {
    return a ? (a->identifier) : NULL;
}

char *aircraft_get_manufacturer(Aeronave *a) {
    return a ? g_strdup(a->manufacturer) : NULL;
}

char *aircraft_get_model(Aeronave *a) {
    return a ? g_strdup(a->model) : NULL;
}

int aircraft_get_year(Aeronave *a) {
    return a ? a->year : 0;
}

//SETTERS - compatíveis com o .h original
void aircraft_set_identifier(Aeronave *a, char *s) {
    if (!a || !s) return;
    g_free(a->identifier);
    a->identifier = g_strdup(s);
}

void aircraft_set_manufacturer(Aeronave *a, char *s) {
    if (!a || !s) return;
    g_free(a->manufacturer);
    a->manufacturer = g_strdup(s);
}

void aircraft_set_model(Aeronave *a, char *model_) {
    if (!a || !model_) return;
    g_free(a->model);
    a->model = g_strdup(model_);
}

void aircraft_set_year(Aeronave *a, int year) {
    if (!a) return;
    a->year = year;
}

//VALIDAÇÃO
static int valida_year(const char *s) {
    if (!s || s[0] == '\0') return 0;
    for (int i = 0; s[i]; i++) {
        if (!isdigit(s[i])) return 0;
    }
    int year = atoi(s);
    return year >= 1900 && year <= 2100;
}

// Função auxiliar para remover aspas
static void remove_quotes(char *str) {
    if (!str) return;
    size_t len = strlen(str);
    if (len >= 2 && str[0] == '"' && str[len-1] == '"') {
        memmove(str, str + 1, len - 2);
        str[len - 2] = '\0';
    }
}

// Libertar campos do csv_split
static void free_csv_fields(char **campos, size_t n) {
    if (!campos) return;
    for (size_t i = 0; i < n; i++) {
        free(campos[i]);
    }
    free(campos);
}

int valida_aeronave(const char *linha) {
    if (!linha || linha[0] == '\0')
        return 0;

    char **campos = NULL;
    size_t n_campos = 0;

    // Usar csv_split que processa CSV com aspas corretamente
    if (csv_split(linha, &campos, &n_campos) != 0) {
        return 0;
    }

    // Dataset fase-1 tem 6 campos: identifier, manuf, model, year, capacity, range
    // Mas só usamos os primeiros 4
    if (n_campos < 4) {
        free_csv_fields(campos, n_campos);
        return 0;
    }

    // Remover aspas dos campos
    for (size_t i = 0; i < n_campos; i++) {
        remove_quotes(campos[i]);
    }

    // Campos não podem estar vazios (os 4 que usamos)
    if (campos[0][0] == '\0' ||
        campos[1][0] == '\0' ||
        campos[2][0] == '\0' ||
        campos[3][0] == '\0') {
        free_csv_fields(campos, n_campos);
        return 0;
    }

    // Validar ano
    if (!valida_year(campos[3])) {
        free_csv_fields(campos, n_campos);
        return 0;
    }

    free_csv_fields(campos, n_campos);
    return 1;
}

//CRIA E DESTROI
Aeronave *criaAeronave(const char *linha) {

    if (!valida_aeronave(linha)) {
        errors_add("aircrafts.csv", -1, linha);
        return NULL;
    }

    Aeronave *a = calloc(1, sizeof *a);
    if (!a) return NULL;

    char **campos = NULL;
    size_t n_campos = 0;

    if (csv_split(linha, &campos, &n_campos) != 0) {
        free(a);
        return NULL;
    }

    // Remover aspas
    for (size_t i = 0; i < n_campos && i < 4; i++) {
        remove_quotes(campos[i]);
    }

    // Copiar campos
    a->identifier = g_strdup(campos[0]);
    a->manufacturer = g_strdup(campos[1]);
    a->model = g_strdup(campos[2]);
    a->year = atoi(campos[3]);

    free_csv_fields(campos, n_campos);
    return a;
}

void libertaAeronave(void *data) {
    Aeronave *a = data;
    if (!a) return;
    g_free(a->identifier);
    g_free(a->manufacturer);
    g_free(a->model);
    g_free(a);
}