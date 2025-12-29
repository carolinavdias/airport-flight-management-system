#ifndef GESTOR_PASSENGERS_H
#define GESTOR_PASSENGERS_H

#include <glib.h>
#include <stdbool.h>
#include "entidades/passengers.h"

typedef struct gestor_passengers GestorPassengers;

//criação / destruição
GestorPassengers *gestor_passengers_novo();
void gestor_passengers_destroy(GestorPassengers *g);

//inserção
void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p);

//consultas
bool gestor_passengers_existe(GestorPassengers *g, char *id_passageiro);

int gestor_passengers_conta_por_voo(GestorPassengers *g, const char *flight_id);

//iteração
typedef void (*PassengerIterFunc)(Passageiros *p, void *user_data);
void gestor_passengers_foreach(GestorPassengers *gp, PassengerIterFunc f, void *user_data);

#endif