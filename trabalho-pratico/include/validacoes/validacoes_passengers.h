#ifndef VALIDACOES_PASSENGERS_H
#define VALIDACOES_PASSENGERS_H

#include "../entidades/passengers.h"
#include <stdbool.h>
#include <glib.h>

//validacao documento
int valida_id_passageiro(const char* s, int *id_passageiro);

//validacao genero
int valida_genero(const char *s, Genero *g);

//validacao e-mail
int valida_email(const char *s, char **email);

//validacao data de nascimento
int valida_Data (const char *s, int *data);


#endif
