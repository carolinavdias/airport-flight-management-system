#ifndef VALIDACOES_AIRPORTS_H
#define VALIDACOES_AIRPORTS_H

#include "entidades/airports.h"

int valida_codigoIATA (const char *s);
int valida_coordenadas (const char* s, int versao);
int valida_tipo_aer(const char *s);

Aeroporto *validacoes_campos_airports(char **campos);

#endif
