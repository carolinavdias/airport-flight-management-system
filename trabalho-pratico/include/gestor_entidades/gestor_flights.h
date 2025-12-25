#ifndef GESTOR_FLIGHTS_H
#define GESTOR_FLIGHTS_H

#include <glib.h>
#include <stdbool.h>
#include "entidades/flights.h"

typedef struct gestor_flights GestorFlights;

//criação / destruição
GestorFlights *gestor_flights_novo();
void gestor_flights_destroy(GestorFlights *g);

//operações básicas
void gestor_flights_inserir(GestorFlights *g, Voo *voo);
bool gestor_flights_existe(GestorFlights *g, const char *flight_id); //VERIFICAR SE É USADO

void gestor_flights_print(GestorFlights *g);

void gestor_flights_foreach(GestorFlights *g, void (*func)(Voo *, void *), void *user_data);

//consultas
const char *gestor_flights_obter_origem(GestorFlights *g, const char *flight_id);
const char *gestor_flights_obter_destino(GestorFlights *g, const char *flight_id);

//array ordenado
void gestor_flights_set_array_ordenado(GestorFlights *g, Voo **array, int num_voos);
Voo **gestor_flights_get_array_ordenado(GestorFlights *g, int *num_voos);

#endif