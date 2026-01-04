#ifndef Q4_H
#define Q4_H

#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * @file q4.h
 * @brief Interface da Query 4.
 *
 * Este módulo define a Query 4, responsável por determinar o passageiro
 * que mais vezes apareceu no Top 10 semanal de gastos em reservas.
 */

/**
 * @brief Executa a Query 4.
 *
 * Para cada semana (domingo a sábado), é calculado o Top 10 de passageiros
 * com maior gasto total em reservas.
 *
 * O gasto semanal de um passageiro corresponde à soma dos preços das
 * reservas associadas a voos cuja data considerada é a "departure estimada".
 *
 * O resultado da query é o passageiro que apareceu mais vezes no Top 10
 * ao longo das semanas analisadas.
 *
 * Regras do enunciado:
 * - A semana é definida de domingo a sábado
 * - O filtro de datas pode captar semanas parciais (essas contam)
 * - Em caso de empate, vence o passageiro com menor `document_number`
 * - O enunciado não exclui voos cancelados
 *
 * O resultado é devolvido no formato:
 * document_number;first_name;last_name;dob;nationality;count_top_10
 *
 * Caso não existam dados válidos ou o filtro resulte em vazio,
 * é devolvida uma linha vazia (`"\n"`).
 *
 * @param linhaComando String com argumentos opcionais:
 *        `begin_date end_date` no formato YYYY-MM-DD.
 * @param gestorPassageiros Gestor de passageiros.
 * @param gestorReservas Gestor de reservas (com cache de Top 10 semanal).
 *
 * @return String dinamicamente alocada com o resultado da query,
 *         que deve ser libertada com `free`, ou uma linha vazia em caso de erro.
 */

char *query4(const char *linhaComando,
             GestorPassengers *gestorPassageiros,
             GestorReservations *gestorReservas);

#endif
