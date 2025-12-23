#ifndef Q3_H
#define Q3_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

typedef struct contagens2 Contagens2;

void query3_init(GHashTable *tabelaVoos);
void query3_cleanup(void);

//query 3 (aeroporto com mais partidas reais entre 2 datas, ignora cancelados)
char *query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, GHashTable *tabelaAeroportos);

#endif