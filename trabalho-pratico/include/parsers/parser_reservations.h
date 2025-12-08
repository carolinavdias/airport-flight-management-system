#ifndef PARSER_RESERVATIONS_H
#define PARSER_RESERVATIONS_H

#include "../entidades/reservations.h"
#include "../gestor_entidades/gestor_reservations.h"
#include "../gestor_entidades/gestor_flights.h"
#include "../gestor_entidades/gestor_passengers.h"
#include "../utils/utils.h"
#include <stdbool.h>
#include <glib.h>

int le_tabela_Reservas(Contexto ctx, GestorFlights *V, GestorPassengers *P, GestorReservations *R);

#endif