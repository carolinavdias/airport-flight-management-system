#ifndef Q3_H
#define Q3_H

#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"

//query 3 (aeroporto com mais partidas reais entre 2 datas, ignora cancelados)
char *query3(const char *data_inicio, const char *data_fim, 
             GestorFlights *gestorVoos, GestorAirports *gestorAeroportos);

#endif