#ifndef Q1_H
#define Q1_H

#include <stdio.h> //tipo FILE*
#include <glib.h>
#include "entidades/airports.h"

//Passa para o formato de string o Tipo_aeroporto
const char* tipoToString(Tipo_aeroporto t);

//query 1 (dado um código, imprime as informações do aeroporto)
void query1(const char *code, GHashTable *tabelaAeroportos, FILE *out);

//void libertaAeroporto(void *data);

#endif
