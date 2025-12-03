#ifndef VALIDACOES_RESERVATIONS_H
#define VALIDACOES_RESERVATIONS_H

#include "reservations.h"
#include "flights.h"
#include "gestor_flights.h"
#include "gestor_passengers.h"
#include <stdbool.h>
#include <glib.h>

//validacao sintatica
int valida_id_reserva(char* string, char **id_reserva);
int valida_voos_reservados(char *string, Voos_reservados *lista);
int valida_bool(char *string, bool *b);
bool v_is_reservation_id(const char *s);

//validacao logica
int valida_RESERVA(Reservas reserva, 
                   GestorFlights *gestor_voos, 
                   GestorPassengers *gestor_passageiros);

#endif