#define _POSIX_C_SOURCE 200809L

#include "queries/q1.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "entidades/airports.h"
#include "validacoes/validacoes_airports.h"

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

//query 1 (dado um código de aeroporto, procura-o na tabela e imprime as suas informações)
char *query1(const char *code, GHashTable *tabelaAeroportos) {
    if (!valida_codigoIATA(code)) {
        return strdup("\n");
    }
    
    Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, code);
    if (a == NULL) {
        return strdup("\n");
    }
    
    char *resultado = NULL;
    int len = asprintf(&resultado, "%s,%s,%s,%s,%s\n",
                       airport_get_code_IATA(a),
                       airport_get_name(a),
                       airport_get_city(a),
                       airport_get_country(a),
                       tipoToString(airport_get_type(a)));
    
    if (len == -1) {
        return strdup("\n");
    }
    
    return resultado;
}