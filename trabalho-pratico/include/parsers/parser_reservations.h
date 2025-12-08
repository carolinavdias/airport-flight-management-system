#ifndef PARSER_RESERVATIONS_H
#define PARSER_RESERVATIONS_H

#include "reservations.h"
#include "gestor_reservations.h"
#include <stdbool.h>
#include <glib.h>

int le_tabela_Reservas(Contexto ctx, GestorVoos *V, GestorPassengers *P, GestorReservas *R);

#endif
