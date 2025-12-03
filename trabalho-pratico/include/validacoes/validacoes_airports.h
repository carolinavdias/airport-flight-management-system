#ifndef VALIDACOES_AIRPORTS_H
#define VALIDACOES_AIRPORTS_H

#include "airports.h"
#include <stdbool.h>
#include <glib.h>

int valida_codigoIATA(char* string, char **codigo_IATA);
int valida_coordenadas(const char* string, int versao, double *coordenada);
int valida_tipo_aer(const char *string, Tipo_aeroporto *t);

bool v_is_iata3(const char *s);
bool v_is_airport_type(const char *s);
bool v_is_lat(const char *s);
bool v_is_lng(const char *s);

#endif