#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <stdbool.h>

#include "queries/interpreter.h"
#include "queries/q1.h"
#include "queries/q2.h"
#include "queries/q3.h"
#include "queries/q4.h"
#include "queries/q5.h"
#include "queries/q6.h"

#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * =====================================================
 * INTERPRETER
 * =====================================================
 */

 /**
 * Este módulo funciona como um dispatcher de queries.
 * Recebe comandos do programa principal e encaminha
 * a execução para a query correta.
 *
 * Também gere:
 *  - Formato normal (';') ou alternativo ('=')
 *  - Parsing específico de parâmetros
 *  - Escrita consistente do output
 */

/**
 * =====================================================
 * FORMATAÇÃO DE OUTPUT
 * =====================================================
 */

/**
 * Aplica o formato correto ao resultado da query.
 *
 * Se o formato alternativo estiver ativo (query terminada em 'S'),
 * substitui todos os separadores ';' por '='.
 */

static void aplica_formato(char *str, bool formato_alternativo) {
    if (!str) return;

    char separador = formato_alternativo ? '=' : ';';

    //troca TODAS os ´;´ pelo separador correto
    for (int i = 0; str[i]; i++) {
        if (str[i] == ';') {
            str[i] = separador;
        }
    }
}

/* =====================================================
 * EXTRAÇÃO DO NÚMERO DA QUERY
 * =====================================================
 */

/**
 * Extrai o número da query e o formato de output.
 *
 * Exemplo:
 *  - "3"  → query 3, formato normal
 *  - "5S" → query 5, formato alternativo
 */

static int extrai_numero_query(const char *comando, bool *formato_alternativo) {
    if (!comando || !comando[0]) return 0;

    *formato_alternativo = false;

    //conta dígitos iniciais
    int i = 0;
    while (comando[i] && comando[i] >= '0' && comando[i] <= '9') {
        i++;
    }

    //se o próximo caractere é 'S' e é o fim da string
    if (comando[i] == 'S' && (comando[i+1] == ' ' || comando[i+1] == '\0')) {
        *formato_alternativo = true;
    }

    //converte apenas a parte numérica
    char num_str[16];
    int copy_len = i;
    if (copy_len >= (int)sizeof(num_str)) {
        copy_len = sizeof(num_str) - 1;
    }

    for (int j = 0; j < copy_len; j++) {
        num_str[j] = comando[j];
    }
    num_str[copy_len] = '\0';

    return atoi(num_str);
}

/**
 * =====================================================
 * FUNÇÃO PRINCIPAL DO INTERPRETER
 * =====================================================
 */

void interpreta_comando(const char *comando,
                        const char *param,
                        FILE *out,
                        GestorAirports *gestorAeroportos,
                        GestorFlights *gestorVoos,
                        GestorAircrafts *gestorAeronaves,
                        GestorPassengers *gestorPassageiros,
                        GestorReservations *gestorReservas) {

    if (!comando || !out) {
        fprintf(out, "\n");
        return;
    }

    bool formato_alternativo = false;
    int query_num = extrai_numero_query(comando, &formato_alternativo);

    /* =================================================
     * Q4: caso especial (pode não ter parâmetros)
     * =================================================
     */
    if (query_num == 4) {
        char *resultado = query4(param ? param : "",
                                 gestorPassageiros,
                                 gestorReservas);

        if (resultado && resultado[0] != '\n') {
            aplica_formato(resultado, formato_alternativo);
            fprintf(out, "%s", resultado);
        } else {
            fprintf(out, "\n");
        }

        if (resultado) free(resultado);
        return;
    }

    /* =================================================
     * RESTANTES QUERIES
     * =================================================
     */
    if (param && param[0]) {
        char *resultado = NULL;

        switch (query_num) {
            case 1:
                resultado = query1(param, gestorAeroportos, gestorVoos, gestorReservas);
                break;

            case 2:
                resultado = query2(param, gestorAeronaves, gestorVoos);
                break;

            case 3: {
                char d1[32], d2[32];
                char data_inicio[64], data_fim[64];

                if (sscanf(param, "%31s %31s", d1, d2) == 2) {
                    snprintf(data_inicio, sizeof(data_inicio), "%s 00:00", d1);
                    snprintf(data_fim, sizeof(data_fim), "%s 23:59", d2);

                    resultado = query3(data_inicio, data_fim,
                                       gestorVoos, gestorAeroportos);
                }
                break;
            }

            case 5:
                resultado = query5(param, gestorVoos);
                break;

            case 6:
                resultado = query6(param, gestorPassageiros,
                                   gestorVoos, gestorReservas);
                break;
        }

        if (resultado && resultado[0] != '\n') {
            aplica_formato(resultado, formato_alternativo);
            fprintf(out, "%s", resultado);
        } else {
            fprintf(out, "\n");
        }

        if (resultado) free(resultado);
    } else {
        fprintf(out, "\n");
    }
}

