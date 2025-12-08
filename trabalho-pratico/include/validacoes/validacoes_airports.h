#ifndef VALIDACOES_AIRPORTS_H
#define VALIDACOES_AIRPORTS_H

#include "../entidades/airports.h"
#include <stdbool.h>
#include <glib.h>

int valida_codigoIATA (const char* s, char **codigo_IATA);
int valida_coordenadas (const char* s, int versao, double *coordenada);
int valida_tipo_aer(const char *string, Tipo_aeroporto *t);

#endif
