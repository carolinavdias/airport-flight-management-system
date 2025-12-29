#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>
#include <glib.h>

#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_aircrafts.h"

void interpreta_comando(const char *comando,
                        const char *param,
                        FILE *out,
                        GestorAirports *gestorAeroportos,
                        GestorFlights *gestorVoos,
                        GestorAircrafts *gestorAeronaves,
                        GestorPassengers *gestorPassageiros);
    
#endif