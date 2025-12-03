#ifndef GESTOR_PASSENGERS_H
#define GESTOR_PASSENGERS_H

#include <glib.h>
#include <stdbool.h>
#include "passengers.h"

typedef struct {
    GHashTable *tabela_passageiros;   // chave = int*, valor = Passageiro*
} GestorPassengers;

// Criação / destruição
GestorPassengers *gestor_passengers_novo();
void gestor_passengers_destroy(GestorPassengers *g);

// Inserção
void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p);

// Consultas
bool gestor_passengers_existe(GestorPassengers *g, int id_passageiro);

#endif
