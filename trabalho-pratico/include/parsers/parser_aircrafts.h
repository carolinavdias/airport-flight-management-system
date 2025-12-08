#ifndef PARSER_AIRCRAFTS_H
#define PARSER_AIRCRAFTS_H

#include "../entidades/aircrafts.h"
#include "../gestor_entidades/gestor_aircrafts.h"
#include "../utils/utils.h"
#include <stdbool.h>
#include <glib.h>

int le_tabela_Aeronaves(Contexto ctx, GestorAircrafts *AC);

#endif