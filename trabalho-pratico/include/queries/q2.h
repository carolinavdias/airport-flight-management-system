#ifndef Q2_H
#define Q2_H

#include "gestor_entidades/gestor_aircrafts.h"  
#include "gestor_entidades/gestor_flights.h"

/**
 * @file q2.h
 * @brief Interface da Query 2.
 *
 * Este módulo define a Query 2, responsável por listar as aeronaves
 * mais utilizadas, ordenadas pelo número de voos associados.
 *
 * A query suporta opcionalmente um filtro por fabricante e devolve
 * os N primeiros resultados, de acordo com a especificação do enunciado.
 */

/**
 * @brief Executa a Query 2.
 *
 * A linha de comando deve conter:
 * - Um inteiro N (número máximo de aeronaves a listar)
 * - Opcionalmente, o nome de um fabricante
 *
 * O resultado é uma string formatada, com uma linha por aeronave,
 * seguindo o formato:
 * identificador;fabricante;modelo;numero_de_voos
 *
 * Caso não existam resultados válidos ou a linha de comando seja inválida,
 * é devolvida uma linha vazia (`"\n"`).
 *
 * @param linhaComando String com os argumentos da query.
 * @param gestorAeronaves Gestor de aeronaves.
 * @param gestorVoos Gestor de voos (usado para obter contagens pré-computadas).
 *
 * @return String dinamicamente alocada com o resultado da query,
 *         devendo ser libertada com `free`, ou uma linha vazia em caso de erro.
 */

char *query2(const char *linhaComando, GestorAircrafts *gestorAeronaves, GestorFlights *gestorVoos);

#endif