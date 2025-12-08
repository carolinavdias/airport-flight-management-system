#ifndef PARSER_FLIGHTS_H
#define PARSER_FLIGHTS_H

#include "flights.h"
#include "gestor_aircrafts.h"
#include <stdbool.h>
#include <glib.h>

int le_tabela_Voos(int opcao_inserida, Contexto ctx, GestorFlights *V, GestorAircrafts *AC);

#endif
