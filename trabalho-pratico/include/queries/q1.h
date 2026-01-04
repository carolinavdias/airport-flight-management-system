#ifndef Q1_H
#define Q1_H

#include "entidades/airports.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * @file q1.h
 * @brief Interface da Query 1.
 *
 * Este módulo define a Query 1, responsável por produzir um resumo
 * de um aeroporto identificado pelo seu código IATA.
 *
 * A informação devolvida inclui:
 * - Código IATA
 * - Nome do aeroporto
 * - Cidade
 * - País
 * - Tipo de aeroporto
 * - Número de chegadas
 * - Número de partidas
 */

/**
 * @brief Executa a Query 1.
 *
 * Dado um código IATA válido, esta função procura o aeroporto
 * correspondente e devolve uma string formatada com a informação
 * pedida pela especificação do enunciado.
 *
 * A string devolvida segue o formato:
 * IATA;nome;cidade;país;tipo;chegadas;partidas
 *
 * Caso o código seja inválido ou o aeroporto não exista,
 * é devolvida uma linha vazia (`"\n"`).
 *
 * @param code Código IATA do aeroporto a consultar.
 * @param gestorAeroportos Gestor de aeroportos.
 * @param gestorVoos Gestor de voos (não utilizado na versão otimizada).
 * @param gestorReservas Gestor de reservas (não utilizado na versão otimizada).
 *
 * @return String dinamicamente alocada com o resultado da query,
 *         devendo ser libertada com `free`, ou uma linha vazia em caso de erro.
 */

char *query1(const char *code, GestorAirports *gestorAeroportos, 
             GestorFlights *gestorVoos, GestorReservations *gestorReservas);
#endif
