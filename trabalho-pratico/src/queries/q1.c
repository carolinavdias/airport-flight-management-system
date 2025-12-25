#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries/q1.h"
#include "entidades/airports.h"
#include "entidades/flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "validacoes/validacoes_airports.h"

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
} DadosQ1;

static void conta_movimentos(Voo *v, void *user_data) { 
    DadosQ1 *d = user_data; 
    
    // ignorar voos cancelados 
    if (voo_get_status(v) == ESTADO_CANCELLED) 
        return; 
    
    const char *orig = voo_get_code_origin(v); 
    const char *dest = voo_get_code_destination(v); 
        
    if (orig && strcmp(orig, d->codigo) == 0) 
        d->partidas++; 
        
    if (dest && strcmp(dest, d->codigo) == 0) 
        d->chegadas++; 
}

// ---------------------------- 
// QUERY 1 (Fase 2) 
// ---------------------------- 
char *query1(const char *code, GestorAirports *gestorAeroportos, GestorFlights *gestorVoos) { 
    //valida código IATA 
    if (!valida_codigoIATA(code)) 
        return strdup("\n"); 
        
    //procura aeroporto 
    Aeroporto *a = gestor_airports_procura(gestorAeroportos, code); 
    if (!a) return strdup("\n"); 
    
    //prepara contadores 
    DadosQ1 dados = { 
        .codigo = code, 
        .chegadas = 0, 
        .partidas = 0 
    }; 
    
    //itera sobre todos os voos (ENCAPSULADO) 
    gestor_flights_foreach(gestorVoos, conta_movimentos, &dados); 
    
    //constrói resposta 
    char *resultado = NULL; 
    int len = asprintf(&resultado, "%s,%s,%s,%s,%s,%d,%d\n", 
        airport_get_code_IATA(a), 
        airport_get_name(a), 
        airport_get_city(a), 
        airport_get_country(a), 
        tipoToString(airport_get_type(a)), 
        dados.chegadas, 
        dados.partidas); 
    
    if (len == -1) return strdup("\n"); 

    return resultado; 
}