#ifndef Q2_H
#define Q2_H

#include <glib.h>

//contagem
typedef struct contagem Contagem;

//static void trim(char *s);

//inicializa
void query2_init(GHashTable *tabelaVoos);

//limpeza
void query2_cleanup(void);

//query 2 (top N aeronaves com mais voos realizados, filtradas por fabricante opcional)
char *query2(const char *linhaComando, GHashTable *tabelaAeronaves, GHashTable *tabelaVoos);

//função auxiliar (ordenação por número de voos e depois por identificador)
int comparaContagens(const Contagem *a, const Contagem *b);

#endif