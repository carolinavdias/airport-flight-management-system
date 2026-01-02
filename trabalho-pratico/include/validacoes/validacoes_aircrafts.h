#ifndef VALIDACOES_AIRCRAFTS_H
#define VALIDACOES_AIRCRAFTS_H

#include "entidades/aircrafts.h"

int valida_year (const char *s);

//validação total para parsing
Aeronave *validacoes_campos_aircrafts(char **campos);

#endif
