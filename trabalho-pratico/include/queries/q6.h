#ifndef Q6_H
#define Q6_H

#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"

// Query 6: Aeroporto de destino mais comum para passageiros de uma nacionalidade
char *query6(const char *nationality, 
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos);

#endif