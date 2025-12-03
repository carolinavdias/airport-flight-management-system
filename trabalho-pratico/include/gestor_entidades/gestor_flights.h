#ifndef GESTOR_FLIGHTS_H
#define GESTOR_FLIGHTS_H

#include <glib.h>
#include "flights.h"

typedef struct {
    GHashTable *tabela_voos;   // chave: id do voo (char*), valor: Voo*
} GestorFlights;

// Criação / destruição
GestorFlights *gestor_flights_novo();
void gestor_flights_destroy(GestorFlights *g);

// Inserção
void gestor_flights_inserir(GestorFlights *g, Voo *voo);

// Consultas
bool gestor_flights_existe(GestorFlights *g, const char *id_voo);
const char *gestor_flights_obter_origem(GestorFlights *g, const char *id_voo);
const char *gestor_flights_obter_destino(GestorFlights *g, const char *id_voo);

#endif
