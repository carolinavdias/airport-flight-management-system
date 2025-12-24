#ifndef GESTOR_FLIGHTS_H
#define GESTOR_FLIGHTS_H

#include <glib.h>
#include <stdbool.h>
#include "entidades/flights.h"

typedef struct gestor_flights GestorFlights;

//criação / destruição
GestorFlights *gestor_flights_novo();
void gestor_flights_destroy(GestorFlights *g);

//inserção
void gestor_flights_inserir(GestorFlights *g, Voo *voo);

//consultas
bool gestor_flights_existe(GestorFlights *g, const char *flight_id); //VERIFICAR SE É USADO
//const char *gestor_flights_obter_destino(GestorFlights *g, const char *flight_id);

//para queries que precisam iterar
GHashTable *gestor_flights_table(GestorFlights *g);

#endif