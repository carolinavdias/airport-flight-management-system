#ifndef Q6_H
#define Q6_H

#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * @file q6.h
 * @brief Interface da Query 6.
 *
 * A Query 6 determina o aeroporto de destino mais comum
 * para passageiros de uma determinada nacionalidade.
 */

/**
 * @brief Executa a Query 6.
 *
 * Para uma dada nacionalidade, esta query calcula o aeroporto
 * de destino mais frequente, considerando todas as reservas
 * efetuadas por passageiros dessa nacionalidade.
 *
 * Regras:
 * - Apenas são considerados voos não cancelados
 * - Em caso de empate, é escolhido o aeroporto cujo código
 *   seja lexicograficamente menor
 *
 * O resultado tem o seguinte formato:
 * airport_code;arrival_count
 *
 * Caso a nacionalidade seja inválida ou não existam dados,
 * é devolvida uma linha vazia (`"\n"`).
 *
 * Esta implementação utiliza um cache pré-computado no parsing
 * dos dados, garantindo execução eficiente da query.
 *
 * @param nationality String com a nacionalidade a filtrar.
 * @param gestorPassageiros Gestor de passageiros (contém cache da Q6).
 * @param gestorVoos Gestor de voos (não utilizado diretamente).
 * @param gestorReservas Gestor de reservas (não utilizado diretamente).
 *
 * @return String dinamicamente alocada com o resultado da query,
 *         que deve ser libertada com `free`, ou uma linha vazia em caso de erro.
 */

char *query6(const char *nationality, 
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas);

#endif