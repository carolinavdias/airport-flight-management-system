#include "entidades/aircrafts.h"
#include <glib.h>

typedef struct aeronave {
    char *identifier;
    char *manufacturer;
    char *model;
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

//SETTERS
void aircraft_set_id (Aeronave *a, char *id) {
    g_free(a->identifier);
    a->identifier = g_strdup(id);
}

void aircraft_set_model_ (Aeronave *a, char *model_) {
    g_free(a->model);
    a->model = g_strdup(model_);
}

void aircraft_set_manuf (Aeronave *a, char *manuf) {
    g_free(a->manufacturer);
    a->manufacturer = g_strdup(manuf);
}

//CRIA E DESTROI
Aeronave *criaAeronave () {
    Aeronave *a = calloc (1, sizeof *a);
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