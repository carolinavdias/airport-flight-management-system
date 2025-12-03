#ifndef VALIDACOES_PASSENGERS_H
#define VALIDACOES_PASSENGERS_H

#include "passengers.h"
#include <stdbool.h>
#include <glib.h>

//validacao documento
int valida_id_passageiro(char* string, int *id_passageiro);
bool v_is_document_number(const char *s);

//validacao e-mail
int valida_email(char *string, char **email);
bool v_is_email(const char *s);

//validacao genero
int valida_genero(const char *string, Genero *g);
bool v_is_gender(const char *s);

#endif