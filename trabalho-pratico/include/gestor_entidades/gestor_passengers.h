#ifndef GESTOR_PASSENGERS_H
#define GESTOR_PASSENGERS_H

//#include <glib.h>
#include <stdbool.h>
#include "entidades/passengers.h"

typedef struct gestor_passengers GestorPassengers;

//criação / destruição
GestorPassengers *gestor_passengers_novo();
void gestor_passengers_destroy(GestorPassengers *g);

//inserção
void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p);

//consultas
bool gestor_passengers_existe(GestorPassengers *g, int id_passageiro);

#endif