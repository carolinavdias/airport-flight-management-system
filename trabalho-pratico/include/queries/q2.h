#ifndef Q2_H
#define Q2_H


#include <stdio.h> //tipo FILE*
#include <glib.h>

//contagem
typedef struct contagem Contagem;

//static void trim(char *s);

//query 2 (top N aeronaves com mais voos realizados, filtradas por fabricante opcional)
void query2(const char *linhaComando, GHashTable *tabelaAeronaves, GHashTable *tabelaVoos, FILE *out);

//função auxiliar (ordenação por número de voos e depois por identificador)
int comparaContagens(const Contagem *a, const Contagem *b);

#endif
