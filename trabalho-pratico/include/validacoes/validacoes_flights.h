#ifndef VALIDACOES_FLIGHTS_H
#define VALIDACOES_FLIGHTS_H

#include "flights.h"
#include "gestor_aircrafts.h" 
#include <stdbool.h>
#include <glib.h>

//validações sintáticas 
int valida_id_voo(const char *string, char **flight_id);
int valida_Estado(const char *s, Estado *e);
int compara_dataH(char *datah1, char *datah2);

bool v_is_flight_id(const char *s);
bool v_is_status_valid(const char *s);

//validação lógica
int valida_VOO(Voo voo, GestorAircrafts *gestor_aeronaves); 

#endif