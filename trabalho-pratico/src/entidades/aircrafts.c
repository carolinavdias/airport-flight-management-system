#include "entidades/aircrafts.h"
#include "validacoes/validacoes_aircrafts.h"
#include "utils/utils.h"
#include <glib.h>
#include <stdlib.h>  
#include <string.h>
#include <stdio.h>

typedef struct aeronave {
    char *identifier;
    char *manufacturer;
    char *model;
    int year;
} Aeronave;

//GETTERS
char *aircraft_get_identifier (Aeronave *a) {
    return a->identifier;
}

char *aircraft_get_manuf (Aeronave *a) {
    return a->manufacturer;
}

char *aircraft_get_model (Aeronave *a) {
    return a->model;
}

int aircraft_get_year(Aeronave *a) {
    return a->year;
}

//SETTERS
void aircraft_set_id (Aeronave *a, char *id) {
    if (!a) return; 
    g_free(a->identifier);
    a->identifier = g_strdup(id);
}

void aircraft_set_model (Aeronave *a, char *model) {
    if (!a) return;
    g_free(a->model);
    a->model = g_strdup(model);
}

void aircraft_set_manuf (Aeronave *a, char *manuf) {
    if (!a) return;
    g_free(a->manufacturer);
    a->manufacturer = g_strdup(manuf);
}

void aircraft_set_year(Aeronave *a, int year) {
    if (!a) return;
    a->year = year;
}

int valida_aeronave(const char *linha) {
    if (!linha || linha[0] == '\0')
        return 0;

    char *tmp = strdup(linha);
    if (!tmp) return 0;

    char *campos[4];
    int count = 0;

    char *token = strtok(tmp, ";");
    while (token && count < 4) {
        campos[count++] = token;
        token = strtok(NULL, ";");
    }

    // Tem de ter exatamente 4 campos
    if (count != 4) {
        free(tmp);
        return 0;
    }

    // Campos não podem estar vazios
    if (campos[0][0] == '\0' ||
        campos[1][0] == '\0' ||
        campos[2][0] == '\0' ||
        campos[3][0] == '\0') {
        free(tmp);
        return 0;
    }

    // Validar ano
    if (!valida_year(campos[3])) {
        free(tmp);
        return 0;
    }

    free(tmp);
    return 1;
}

//CRIA E DESTROI
Aeronave *criaAeronave(const char *linha) {

    if (!valida_aeronave(linha)) {
        errors_add("aircrafts.csv", -1, linha);   // envia para errors.csv
        return NULL;
    }

    Aeronave *a = calloc(1, sizeof *a);

    char identifier[32], manufacturer[64], model[64], year[8];
    sscanf(linha, "%31[^;];%63[^;];%63[^;];%7s",
           identifier, manufacturer, model, year);

    a->identifier = strdup(identifier);
    a->manufacturer = strdup(manufacturer);
    a->model = strdup(model);
    a->year = atoi(year);

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