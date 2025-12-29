#ifndef GESTOR_PASSENGERS_H
#define GESTOR_PASSENGERS_H

#include <stdbool.h>
#include "entidades/passengers.h"

typedef struct gestor_passengers GestorPassengers;

//criação / destruição
GestorPassengers *gestor_passengers_novo();
void gestor_passengers_destroy(GestorPassengers *g);

//inserção
void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p);

//consultas
bool gestor_passengers_existe(GestorPassengers *g, const char *id_passageiro);

int gestor_passengers_conta_por_voo(GestorPassengers *g, const char *flight_id);

//procurar passageiro por id
Passageiros *gestor_passengers_procura(GestorPassengers *g, const char *doc_number);

//iteração
typedef void (*PassengerIterFunc)(Passageiros *p, void *user_data);
void gestor_passengers_foreach(GestorPassengers *gp, PassengerIterFunc f, void *user_data);

#endif