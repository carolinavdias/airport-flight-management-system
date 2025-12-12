#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>

typedef struct contexto Contexto;
Contexto *cria_contexto ();
char *get_contexto (Contexto *ctx);
void set_contexto (Contexto *ctx, char *d);

FILE* abrir_ficheiro(Contexto *ctx, const char *nome_ficheiro, const char *modo);
int qual_mes (int ano, int mes);

#endif
