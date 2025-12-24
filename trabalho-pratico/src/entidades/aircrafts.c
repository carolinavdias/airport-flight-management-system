#include "entidades/aircrafts.h"
#include <glib.h>
#include <stdlib.h>  
#include <string.h>

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

//CRIA E DESTROI
Aeronave *criaAeronave () {
    Aeronave *a = calloc (1, sizeof *a);
    a->year = 0;
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