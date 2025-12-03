#ifndef GESTOR_AIRCRAFTS_H
#define GESTOR_AIRCRAFTS_H

#include <glib.h>
#include "aircrafts.h"

// tabela guarda todas as aeronaves
typedef struct gestor_aircrafts {
    GHashTable *tabela;
} GestorAircrafts;

/* Cria o gestor (inicia a hash table) */
GestorAircrafts *gestor_aircrafts_cria(void);

/* Insere uma aeronave no gestor */
void gestor_aircrafts_insere(GestorAircrafts *g, Aeronave *a);

/* Obtém uma aeronave pelo identifier */
Aeronave *gestor_aircrafts_procura(GestorAircrafts *g, const char *identifier);

/* Verifica se uma aeronave existe */
int gestor_aircrafts_existe(GestorAircrafts *g, const char *identifier);

/* Devolve a hash table (para queries que a precisam) */
GHashTable *gestor_aircrafts_table(GestorAircrafts *g);

/* Liberta toda a memória (aeronaves + gestor) */
void gestor_aircrafts_liberta(GestorAircrafts *g);

#endif