#ifndef VALIDACOES_RESERVATIONS_H
#define VALIDACOES_RESERVATIONS_H

#include "../entidades/reservations.h"
#include "../entidades/flights.h"
#include "../gestor_entidades/gestor_flights.h"
#include "../gestor_entidades/gestor_passengers.h"
#include <stdbool.h>
#include <glib.h>

//validacao sintatica
int valida_id_reserva (const char *s, char **id_reserva);
int valida_voos_reservados(const char *s, Voos_reservados *lista);
int valida_bool (const char *s, bool *b);

//validacao logica
int valida_RESERVA(Reservas reserva, GestorFlights *gestor_voos, GestorPassengers *gestor_passageiros);

#endif