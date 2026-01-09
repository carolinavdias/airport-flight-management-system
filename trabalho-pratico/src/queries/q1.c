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

/**
 * Converte o tipo de aeroporto (enum Tipo_aeroporto)
 * para a respetiva representação textual usada na saída.
 */

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

/**
 * ===========================
 * QUERY 1 — IMPLEMENTAÇÃO
 * =========================== */

/**
 * Versão otimizada:
 * - Utiliza contagens de chegadas e partidas pré-computadas
 *   durante a fase de parsing.
 * - Evita percorrer todos os voos, garantindo complexidade O(1).
 */

char *query1(const char *code,
             GestorAirports *gestorAeroportos,
             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas) {

    /** Estes gestores não são utilizados nesta versão otimizada,
        mas mantêm-se para compatibilidade com versões não otimizadas
        ou com o interpretador */
    (void)gestorVoos;
    (void)gestorReservas;

    /** Validação do código IATA */
    if (!valida_codigoIATA(code))
        return strdup("\n");

    /** Procura do aeroporto no gestor */
    Aeroporto *a = gestor_airports_procura(gestorAeroportos, code);
    if (!a)
        return strdup("\n");

    /** Obtenção das contagens pré-computadas */
    int chegadas = gestor_airports_get_chegadas(gestorAeroportos, code);
    int partidas = gestor_airports_get_partidas(gestorAeroportos, code);

    /** Acesso aos campos do aeroporto */
    const char *iata    = airport_get_code_IATA(a);
    const char *name    = airport_get_name(a);
    const char *city    = airport_get_city(a);
    const char *country = airport_get_country(a);

    /** Construção da string de saída */
    char *resultado = NULL;
    int len = asprintf(&resultado,
                       "%s;%s;%s;%s;%s;%d;%d\n",
                       iata,
                       name,
                       city,
                       country,
                       tipoToString(airport_get_type(a)),
                       chegadas,
                       partidas);

    if (len == -1)
        return strdup("\n");

    return resultado;
}
