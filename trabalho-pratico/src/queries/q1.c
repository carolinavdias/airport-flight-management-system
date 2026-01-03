#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries/q1.h"
#include "entidades/airports.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"
#include "validacoes/validacoes_airports.h"
#include "validacoes/validacoes.h"
#include <glib.h>

//conversão do tipo de aeroporto
const char* tipoToString(Tipo_aeroporto t) {
    switch (t) {
        case TIPO_SMALL_AIRPORT: return "small_airport";
        case TIPO_MEDIUM_AIRPORT: return "medium_airport";
        case TIPO_LARGE_AIRPORT: return "large_airport";
        case TIPO_HELIPORT: return "heliport";
        case TIPO_SEAPLANE_BASE: return "seaplane_base";
        default: return "error";
    }
}

// ---------------------------- 
// QUERY 1 (Fase 2) - OTIMIZADA
// Usa contagens pré-computadas durante o parsing
// ---------------------------- 
char *query1(const char *code, GestorAirports *gestorAeroportos, 
             GestorFlights *gestorVoos, GestorReservations *gestorReservas) { 
    
    (void)gestorVoos;      // não usado na versão otimizada
    (void)gestorReservas;  // não usado na versão otimizada
    
    if (!valida_codigoIATA(code)) 
        return strdup("\n"); 
    
    Aeroporto *a = gestor_airports_procura(gestorAeroportos, code); 
    if (!a) return strdup("\n"); 
    
    // Obter contagens pré-computadas (O(1) em vez de O(n*m))
    int chegadas = gestor_airports_get_chegadas(gestorAeroportos, code);
    int partidas = gestor_airports_get_partidas(gestorAeroportos, code);
    
    const char *iata = airport_get_code_IATA(a);
    const char *name = airport_get_name(a);
    const char *city = airport_get_city(a);
    const char *country = airport_get_country(a);
    
    char *resultado = NULL; 
    int len = asprintf(&resultado, "%s;%s;%s;%s;%s;%d;%d\n", 
                       iata, 
                       name, 
                       city, 
                       country, 
                       tipoToString(airport_get_type(a)), 
                       chegadas, 
                       partidas); 
    
    g_free((char*)iata);
    //g_free(name);
    //g_free(city);
    //g_free(country);
    
    if (len == -1) return strdup("\n"); 
    
    return resultado; 
}
