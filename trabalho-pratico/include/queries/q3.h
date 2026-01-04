#ifndef Q3_H
#define Q3_H

#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"

/**
 * @file q3.h
 * @brief Interface da Query 3.
 *
 * Este módulo define a Query 3, responsável por determinar o aeroporto
 * com maior número de partidas reais num intervalo de datas.
 *
 * Voos cancelados são ignorados, conforme especificado no enunciado.
 */

/**
 * @brief Executa a Query 3.
 *
 * Determina o aeroporto com maior número de partidas reais
 * entre duas datas (inclusive).
 *
 * Apenas são considerados voos:
 * - Com data de partida real dentro do intervalo
 * - Que não estejam cancelados
 *
 * Em caso de empate no número de partidas, o aeroporto com
 * código IATA lexicograficamente menor é escolhido.
 *
 * O resultado é devolvido no formato:
 * IATA;nome;cidade;país;número_de_partidas
 *
 * Caso as datas sejam inválidas, não existam voos no intervalo
 * ou não seja possível determinar um aeroporto válido,
 * é devolvida uma linha vazia (`"\n"`).
 *
 * @param data_inicio Data/hora inicial do intervalo (formato válido).
 * @param data_fim Data/hora final do intervalo (formato válido).
 * @param gestorVoos Gestor de voos (com estrutura ordenada).
 * @param gestorAeroportos Gestor de aeroportos.
 *
 * @return String dinamicamente alocada com o resultado da query,
 *         devendo ser libertada com `free`, ou uma linha vazia em caso de erro.
 */

char *query3(const char *data_inicio, const char *data_fim, 
             GestorFlights *gestorVoos, GestorAirports *gestorAeroportos);

#endif