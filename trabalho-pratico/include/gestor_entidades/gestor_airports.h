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

//liberta toda a memória (aeroportos + gestor)
void gestor_airports_liberta(GestorAirports *g);

// === CONTAGENS DE PASSAGEIROS (para Q1 otimizada) ===
void gestor_airports_incrementa_chegadas(GestorAirports *g, const char *code);
void gestor_airports_incrementa_partidas(GestorAirports *g, const char *code);
int gestor_airports_get_chegadas(GestorAirports *g, const char *code);
int gestor_airports_get_partidas(GestorAirports *g, const char *code);
void gestor_airports_init_contagens(GestorAirports *g);

#endif
