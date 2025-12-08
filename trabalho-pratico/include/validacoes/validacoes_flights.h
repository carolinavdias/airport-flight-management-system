#ifndef VALIDACOES_FLIGHTS_H
#define VALIDACOES_FLIGHTS_H

#include "../entidades/flights.h"
#include "../gestor_entidades/gestor_aircrafts.h"
#include <stdbool.h>
#include <glib.h>

//validações sintáticas
int valida_id_voo (const char* s, char **voo_id);
int qual_mes (int ano, int mes); //auxiliar -> para mudar de lugar
int valida_DataH(const char *s, long long *out);
int valida_Estado(const char *s, Estado *e);

//validação lógica
int valida_VOO(Voo voo, GestorAircrafts *gestor_aeronaves);


#endif
