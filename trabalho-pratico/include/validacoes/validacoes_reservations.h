#ifndef VALIDACOES_RESERVATIONS_H
#define VALIDACOES_RESERVATIONS_H

#include "entidades/reservations.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_passengers.h"

//validacao sintatica
int valida_id_reserva (const char *s);
int valida_set_voos_reservados(const char *s, Reservas *r);
int valida_bool (const char *s);

//validacao logica
int valida_RESERVA(Reservas *reserva, GestorFlights *gestor_voos, GestorPassengers *gestor_passageiros);

#endif
