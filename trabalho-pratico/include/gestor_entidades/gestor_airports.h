#ifndef GESTOR_AIRPORTS_H
#define GESTOR_AIRPORTS_H

#include "entidades/airports.h"

//tabela guarda todos os aeroportos
typedef struct gestor_airports GestorAirports;

//cria o gestor (inicia a hash table)
GestorAirports *gestor_airports_cria(void);

//insere um aeroporto no gestor
void gestor_airports_insere(GestorAirports *g, Aeroporto *a);

//obtém um aeroporto pelo code_IATA
Aeroporto *gestor_airports_procura(GestorAirports *g, const char *code_IATA);

//verifica se um aeroporto existe
int gestor_airports_existe(GestorAirports *g, const char *code_IATA);

//REMOVIDO: GHashTable *gestor_airports_table() - VIOLA ENCAPSULAMENTO

//liberta toda a memória (aeroportos + gestor)
void gestor_airports_liberta(GestorAirports *g);

#endif