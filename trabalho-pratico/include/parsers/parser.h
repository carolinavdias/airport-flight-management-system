#ifndef PARSER_H
#define PARSER_H

#include "utils/utils.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

int* read (Contexto *ctx, GestorFlights *V, GestorAirports *AP, GestorAircrafts *AC, GestorPassengers *P, GestorReservations *R);

#endif