#ifndef Q3_H
#define Q3_H

#include <stdio.h>
#include <glib.h>

typedef struct contagens2 Contagens2;

//query 3 (aeroporto com mais partidas reais entre 2 datas, ignora cancelados)
void query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, GHashTable *tabelaAeroportos, FILE *out);

#endif