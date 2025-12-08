#ifndef PARSER_AIRPORTS_H
#define PARSER_AIRPORTS_H

#include "../entidades/airports.h"
#include "../gestor_entidades/gestor_airports.h"
#include "../utils/utils.h"
#include <stdbool.h>
#include <glib.h>

int le_tabela_Aeroportos(Contexto ctx, GestorAirports *AP);

#endif