#ifndef VALIDACOES_PASSENGERS_H
#define VALIDACOES_PASSENGERS_H

#include "entidades/passengers.h"

//validacao documento
int valida_id_passageiro(const char *s);

//validacao genero
int valida_genero(const char *s);

//validacao e-mail
int valida_email(const char *s);

//validacao data de nascimento
int valida_Data (const char *s);

//validação total para parsing
Passageiros *validacoes_campos_passengers(char **campos);

#endif
