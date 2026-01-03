#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>  
#include "queries/q6.h"
#include "entidades/passengers.h"
#include "entidades/flights.h"
#include "entidades/reservations.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * Q6: Aeroporto de destino mais comum para passageiros de uma nacionalidade
 * Versão otimizada usando cache pre-computado
 */

typedef struct {
    const char *melhor_destino;
    int max_count;
} DadosQ6Cache;

static void encontra_melhor_destino(const char *destino, int count, void *user_data) {
    DadosQ6Cache *dados = user_data;
    
    if (count > dados->max_count || 
        (count == dados->max_count && dados->melhor_destino && strcmp(destino, dados->melhor_destino) < 0)) {
        dados->melhor_destino = destino;
        dados->max_count = count;
    }
}

char *query6(const char *linhaComando,
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas) {
    
    (void)gestorVoos;
    (void)gestorReservas;
    
    // Parse nacionalidade (pode ter espacos, ex: "South Africa")
    if (!linhaComando || linhaComando[0] == '\0') {
        return strdup("\n");
    }
    
    char nacionalidade[128];
    strncpy(nacionalidade, linhaComando, 127);
    nacionalidade[127] = '\0';
    
    // Remover newline se existir
    char *nl = strchr(nacionalidade, '\n');
    if (nl) *nl = '\0';
    
    // Usar cache pre-computado
    DadosQ6Cache dados = { .melhor_destino = NULL, .max_count = 0 };
    gestor_passengers_foreach_destinos_q6(gestorPassageiros, nacionalidade, encontra_melhor_destino, &dados);
    
    if (!dados.melhor_destino || dados.max_count == 0) {
        return strdup("\n");
    }
    
    // Formato: code;arrival_count
    char *resultado = NULL;
    if (asprintf(&resultado, "%s;%d\n", dados.melhor_destino, dados.max_count) == -1) {
        return strdup("\n");
    }
    
    return resultado;
}
