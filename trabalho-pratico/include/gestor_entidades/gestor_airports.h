#ifndef GESTOR_AIRPORTS_H
#define GESTOR_AIRPORTS_H

#include <glib.h>
#include "airports.h"

// tabela guarda todas as aeronaves
typedef struct gestor_airports {
    GHashTable *tabela;
} GestorAirports;

/* Cria o gestor (inicia a hash table) */
GestorAirports *gestor_airports_cria(void);

/* Insere uma aeronave no gestor */
void gestor_airports_insere(GestorAirports *g, Aeroporto *a);

/* Obtém uma aeronave pelo identifier */
Aeroporto *gestor_airports_procura(GestorAirports *g, const char *code_IATA);

/* Verifica se uma aeronave existe */
int gestor_airports_existe(GestorAirports *g, const char *code_IATA);

/* Devolve a hash table (para queries que a precisam) */
GHashTable *gestor_airports_table(GestorAirports *g);

/* Liberta toda a memória (aeronaves + gestor) */
void gestor_airports_liberta(GestorAirports *g);

#endif
