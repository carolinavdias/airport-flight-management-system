#ifndef Q3_H
#define Q3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"

typedef struct contagens2 Contagens2;

//query 3 (aeroporto com mais partidas reais entre 2 datas, ignora cancelados)
char *query3(const char *data_inicio, const char *data_fim, GestorFlights *gestorVoos, GestorAirports *gestorAeroportos);

#endif