#ifndef INTERPRETER_H
#define INTERPRETER_H

#include <stdio.h>

#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

#include "queries/q5.h"

/**
 * @file interpreter.h
 * @brief Interpretador de comandos das queries.
 *
 * Este módulo é responsável por interpretar uma linha de comando
 * (query + parâmetros), identificar qual a query a executar
 * e chamar a função correspondente.
 *
 * Também gere:
 * - O formato de saída (normal ';' ou alternativo '=')
 * - A validação básica dos parâmetros
 * - A escrita do resultado no ficheiro de output
 */

/**
 * @brief Interpreta e executa uma query.
 *
 * Recebe o comando da query (ex: "1", "3S", "5"),
 * os parâmetros associados à query e os gestores globais.
 *
 * A função:
 * - Determina o número da query
 * - Identifica se deve usar formato alternativo ('S')
 * - Executa a query correspondente
 * - Aplica o formato correto ao resultado
 * - Escreve o output no ficheiro fornecido
 *
 * Caso ocorra qualquer erro ou a query não produza resultado,
 * é escrita uma linha vazia.
 *
 * @param comando String com o identificador da query (ex: "1", "3S")
 * @param param String com os parâmetros da query (pode ser NULL)
 * @param out Ficheiro de saída onde o resultado será escrito
 * @param gestorAeroportos Gestor de aeroportos
 * @param gestorVoos Gestor de voos
 * @param gestorAeronaves Gestor de aeronaves
 * @param gestorPassageiros Gestor de passageiros
 * @param gestorReservas Gestor de reservas
 */

void interpreta_comando(const char *comando,
                        const char *param,
                        FILE *out,
                        GestorAirports *gestorAeroportos,
                        GestorFlights *gestorVoos,
                        GestorAircrafts *gestorAeronaves,
                        GestorPassengers *gestorPassageiros,
                        GestorReservations *gestorReservas);

#endif
