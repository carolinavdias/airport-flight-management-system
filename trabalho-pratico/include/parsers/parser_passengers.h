#ifndef PARSER_PASSENGERS_H
#define PARSER_PASSENGERS_H

#include "../entidades/passengers.h"
#include "../gestor_entidades/gestor_passengers.h"
#include "../utils/utils.h"
#include <stdbool.h>
#include <glib.h>

int le_tabela_Passageiros(Contexto ctx, GestorPassengers *P);

#endif