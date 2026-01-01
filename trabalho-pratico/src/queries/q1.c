#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries/q1.h"
#include "entidades/airports.h"
#include "entidades/flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"
#include "validacoes/validacoes_airports.h"
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

//estrutura auxiliar para contagem
typedef struct { 
    const char *codigo; 
    int chegadas; 
    int partidas; 
    GestorReservations *gestorReservas;
} DadosQ1;

static void conta_movimentos(Voo *v, void *user_data) { 
    DadosQ1 *d = user_data; 
    
    // ignorar voos cancelados 
    if (voo_get_status(v) == ESTADO_CANCELLED) 
        return; 
    
    const char *orig = voo_get_code_origin(v); 
    const char *dest = voo_get_code_destination(v); 
    char *flight_id = voo_get_flight_id(v);
        
    if (orig && strcmp(orig, d->codigo) == 0) { 
        int num_pass = gestor_reservations_conta_por_voo(d->gestorReservas, flight_id); 
        d->partidas += num_pass; 
    } 
    
    if (dest && strcmp(dest, d->codigo) == 0) { 
        int num_pass = gestor_reservations_conta_por_voo(d->gestorReservas, flight_id); 
        d->chegadas += num_pass; 
    }
    
    g_free(flight_id);
}

// ---------------------------- 
// QUERY 1 (Fase 2) 
// ---------------------------- 
char *query1(const char *code, GestorAirports *gestorAeroportos, 
             GestorFlights *gestorVoos, GestorReservations *gestorReservas) { 
    
    if (!valida_codigoIATA(code)) 
        return strdup("\n"); 
    
    Aeroporto *a = gestor_airports_procura(gestorAeroportos, code); 
    if (!a) return strdup("\n"); 
    
    DadosQ1 dados = { 
        .codigo = code, 
        .chegadas = 0, 
        .partidas = 0,
        .gestorReservas = gestorReservas
    }; 
    
    gestor_flights_foreach(gestorVoos, conta_movimentos, &dados); 
    
    char *iata = airport_get_code_IATA(a);
    char *name = airport_get_name(a);
    char *city = airport_get_city(a);
    char *country = airport_get_country(a);
    
    char *resultado = NULL; 
    int len = asprintf(&resultado, "%s;%s;%s;%s;%s;%d;%d\n", 
                       iata, 
                       name, 
                       city, 
                       country, 
                       tipoToString(airport_get_type(a)), 
                       dados.chegadas, 
                       dados.partidas); 
    
    g_free(iata);
    g_free(name);
    g_free(city);
    g_free(country);
    
    if (len == -1) return strdup("\n"); 
    
    return resultado; 
}
