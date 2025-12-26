#ifndef Q1_H
#define Q1_H

#include <glib.h>
#include "entidades/airports.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"

//passa para o formato de string o Tipo_aeroporto
const char* tipoToString(Tipo_aeroporto t);

//query 1 (dado um código, imprime as informações do aeroporto)
char *query1(const char *code, GestorAirports *gestorAeroportos, GestorFlights *gestorVoos);

#endif