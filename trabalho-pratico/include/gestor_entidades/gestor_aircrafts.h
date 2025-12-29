#ifndef GESTOR_AIRCRAFTS_H
#define GESTOR_AIRCRAFTS_H

#include "entidades/aircrafts.h"

//tabela guarda todas as aeronaves
typedef struct gestor_aircrafts GestorAircrafts;

//cria o gestor (inicia a hash table)
GestorAircrafts *gestor_aircrafts_cria(void);

//insere uma aeronave no gestor
void gestor_aircrafts_insere(GestorAircrafts *g, Aeronave *a);

//obtém uma aeronave pelo identifier
Aeronave *gestor_aircrafts_procura(GestorAircrafts *g, const char *identifier);

//verifica se uma aeronave existe
int gestor_aircrafts_existe(GestorAircrafts *g, const char *identifier);

typedef void (*AircraftIterFunc)(Aeronave *a, void *user_data);

void gestor_aircrafts_foreach(GestorAircrafts *g, AircraftIterFunc f, void *user_data);

//liberta toda a memória (aeronaves + gestor)
void gestor_aircrafts_liberta(GestorAircrafts *g);

#endif