#include "entidades/airports.h"
#include <glib.h>

typedef struct aeroporto {
    char *code_IATA; //codigo_IATA_aer
    char *name; //name_aeroporto
    char *city; //cidade_aeroporto
    char *country; //pais_aeroporto
    double latitude; //latitude ++
    double longitude; //longitude ++
    char *code_ICAO; //codigo_ICAO_aeroporto ++
    Tipo_aeroporto type; // ++
    int arrival_count;
    int departure_count;
} Aeroporto;

//GETTERS

char *airport_get_code_IATA (Aeroporto *a) {
    return a->code_IATA;
}

char *airport_get_name (Aeroporto *a) {
    return a->name;
}

char *airport_get_city (Aeroporto *a) {
    return a->city;
}

char *airport_get_country (Aeroporto *a) {
    return a->country;
}

Tipo_aeroporto airport_get_type (Aeroporto *a) {
    return a->type;
}

//SETTERS
void airport_set_code_IATA (Aeroporto *a, char *code_) {
   g_free(a->code_IATA);
   a->code_IATA = g_strdup(code_);
}

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

void airport_set_coordenada (Aeroporto *a, char *coordenada, int versao) {
   switch (versao) {
	case 1 : a->latitude = atof(coordenada);
		 break;
	case 2 : a->longitude = atof(coordenada);
		 break;
   }
}

void airport_set_code_ICAO (Aeroporto *a, char *code_i) {
   g_free(a->code_ICAO);
   a->code_ICAO = g_strdup(code_i);
}

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


//CRIA E DESTROI

Aeroporto *criaAeroporto () {
    Aeroporto *ap = calloc (1, sizeof *ap);
    return ap;
}

void libertaAeroporto(void *data) {
    Aeroporto *a = data;
    if (!a) return;
//    g_free(a->code_IATA);
    g_free(a->name);
    g_free(a->city);
    g_free(a->country);
    g_free(a->code_ICAO);
    g_free(a);
}
