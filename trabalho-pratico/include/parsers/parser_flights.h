#ifndef PARSER_FLIGHTS_H
#define PARSER_FLIGHTS_H

#include "../entidades/flights.h"
#include "../gestor_entidades/gestor_flights.h"
#include "../gestor_entidades/gestor_aircrafts.h"
#include "../utils/utils.h"
#include <stdbool.h>
#include <glib.h>

int le_tabela_Voos(Contexto ctx, GestorFlights *V, GestorAircrafts *AC);

#endif