#ifndef Q5_H
#define Q5_H

#include "gestor_entidades/gestor_flights.h"

/**
 * @file q5.h
 * @brief Interface da Query 5.
 *
 * Este módulo define a Query 5, responsável por calcular o Top N
 * companhias aéreas com maior atraso médio.
 */

/**
 * @brief Executa a Query 5.
 *
 * A Query 5 calcula, para cada companhia aérea, o atraso médio
 * considerando apenas voos com atraso registado.
 *
 * O atraso médio é calculado como:
 * atraso_médio = soma_dos_atrasos / número_de_voos_com_atraso
 *
 * As companhias aéreas são ordenadas por:
 * 1. Maior atraso médio (decrescente)
 * 2. Em caso de empate, ordem alfabética do código da companhia
 *
 * O resultado é limitado aos N primeiros elementos.
 *
 * O formato do resultado é:
 * airline;delay_count;delay_avg
 *
 * Caso o valor de N seja inválido ou não existam dados,
 * é devolvida uma linha vazia (`"\n"`).
 *
 * @param linhaComando String contendo o valor de N.
 * @param gestorVoos Gestor de voos, que contém o cache pré-calculado
 *        de atrasos por companhia aérea.
 *
 * @return String dinamicamente alocada com o resultado da query,
 *         que deve ser libertada com `free`, ou uma linha vazia em caso de erro.
 */

char *query5(const char *linhaComando, GestorFlights *gestorVoos);

#endif
