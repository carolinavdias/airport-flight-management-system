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
 * =====================================================
 * QUERY 6
 * =====================================================
 */

/**
 * Aeroporto de destino mais comum para passageiros
 * de uma determinada nacionalidade.
 *
 * Esta versão utiliza um cache pré-calculado durante
 * o parsing dos dados, evitando percorrer reservas
 * e voos em cada execução da query.
 */

/**
 * =====================================================
 * ESTRUTURA AUXILIAR
 * =====================================================
 */

/*
 * Estrutura usada para armazenar o melhor destino
 * encontrado durante a iteração do cache.
 */

typedef struct {
    const char *melhor_destino;  /**< código do aeroporto */
    int max_count;               /**< número máximo de chegadas */
} DadosQ6Cache;

/**
 * =====================================================
 * CALLBACK DE PROCESSAMENTO DO CACHE
 * =====================================================
 */

/**
 * Callback chamada para cada aeroporto de destino
 * associado à nacionalidade pedida.
 *
 * Atualiza o melhor destino de acordo com:
 *  - maior número de ocorrências
 *  - em caso de empate, menor código lexicográfico
 */

static void encontra_melhor_destino(const char *destino, int count, void *user_data) {
    DadosQ6Cache *dados = user_data;

    if (count > dados->max_count ||
        (count == dados->max_count && dados->melhor_destino && strcmp(destino, dados->melhor_destino) < 0)) {
        dados->melhor_destino = destino;
        dados->max_count = count;
    }
}

/**
 * =====================================================
 * QUERY 6 — IMPLEMENTAÇÃO PRINCIPAL
 * =====================================================
 */

char *query6(const char *linhaComando,
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas) {

    // Estes gestores não são usados diretamente nesta versão
    (void)gestorVoos;
    (void)gestorReservas;

    /**
     * =================================================
     * FASE 0: Validação da nacionalidade
     * =================================================
     */
    if (!linhaComando || linhaComando[0] == '\0') {
        return strdup("\n");
    }

    // Copiar nacionalidade (pode conter espaços)
    char nacionalidade[128];
    strncpy(nacionalidade, linhaComando, 127);
    nacionalidade[127] = '\0';

    // Remover newline final, se existir
    char *nl = strchr(nacionalidade, '\n');
    if (nl) *nl = '\0';

    /**
     * =================================================
     * FASE 1: Percorrer cache pré-computado
     * =================================================
     */
    DadosQ6Cache dados = {
        .melhor_destino = NULL,
        .max_count = 0
    };

    gestor_passengers_foreach_destinos_q6(
        gestorPassageiros,
        nacionalidade,
        encontra_melhor_destino,
        &dados
    );

    if (!dados.melhor_destino || dados.max_count == 0) {
	return strdup("\n");
    }

    /**
     * =================================================
     * FASE 2: Formatação do resultado
     * =================================================
     */
    char *resultado = NULL;

    // Formato: airport_code;arrival_count
    if (asprintf(&resultado, "%s;%d\n",
                 dados.melhor_destino,
                 dados.max_count) == -1) {
        return strdup("\n");
    }

    return resultado;
}
