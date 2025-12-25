#ifndef Q2_H
#define Q2_H

#include "gestor_entidades/gestor_aircrafts.h"  
#include "gestor_entidades/gestor_flights.h"

//inicializa
void query2_init(GestorFlights *gestorVoos);

//limpeza
void query2_cleanup(void);

//query 2
char *query2(const char *linhaComando, GestorAircrafts *gestorAeronaves, GestorFlights *gestorVoos);

#endif