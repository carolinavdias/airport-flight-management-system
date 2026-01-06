#ifndef VALIDACOES_FLIGHTS_H
#define VALIDACOES_FLIGHTS_H

#include "entidades/flights.h"
#include "gestor_entidades/gestor_aircrafts.h"

/**
 * @file validacoes_flights.h
 * @brief Validações sintáticas e lógicas para voos.
 *
 * Este módulo contém funções responsáveis por validar campos individuais
 * e conjuntos de campos associados a voos durante o parsing.
 *
 * Inclui:
 *  - validação sintática do ID do voo;
 *  - validação de datas e horas no formato YYYY-MM-DD HH:MM;
 *  - validação do estado do voo;
 *  - validação lógica completa (coerência entre horários, estado e aeronave);
 *  - criação da estrutura Voo apenas quando os dados são válidos.
 */


/**
 * @brief Valida todos os campos de um voo e cria a estrutura correspondente.
 *
 * Esta função valida:
 *  - ID do voo
 *  - datas de partida e chegada
 *  - estado
 *  - códigos IATA de origem e destino
 *  - horários actualizados (dependendo do estado)
 *
 * Se todos os campos forem válidos, cria e preenche a estrutura Voo.
 * Caso contrário, devolve NULL.
 *
 * @param campos Array de strings contendo os campos do voo.
 * @param AC Gestor de aeronaves para validação lógica.
 * @return Ponteiro para Voo válido, ou NULL se algum campo for inválido.
 */

Voo *validacoes_campos_flights(char **campos, GestorAircrafts *AC, StringPool *pool);

#endif
