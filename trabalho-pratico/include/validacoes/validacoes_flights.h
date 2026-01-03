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

/* ============================================
 * VOO -> VALIDAÇÕES SINTÁTICAS
 * ============================================ */

/**
 * @brief Valida o identificador de um voo.
 *
 * O ID deve ter o formato:
 *  - 2 letras maiúsculas (A–Z)
 *  - seguidas de 5 dígitos (0–9)
 *
 * @param s String contendo o ID do voo.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_id_voo (const char *s);

/**
<<<<<<< HEAD
=======
 * @brief Valida uma data e hora no formato YYYY-MM-DD HH:MM.
 *
 * Verifica:
 *  - tamanho correto (16 caracteres);
 *  - posições fixas de '-', '-', ' ' e ':';
 *  - todos os restantes caracteres são dígitos;
 *  - limites válidos para ano, mês, dia, hora e minuto;
 *  - data não superior à data atual (2025-09-30).
 *
 * @param s String contendo a data e hora.
 * @return 1 se válido, 0 caso contrário.
 */


/**
>>>>>>> 840f49e (limpeza e leaks)
 * @brief Valida o estado textual de um voo.
 *
 * Aceita apenas:
 *  - "On Time"
 *  - "Delayed"
 *  - "Cancelled"
 *
 * @param s String contendo o estado.
 * @return 1 se válido, 0 caso contrário.
 */


/* ============================================
 * VOO -> VALIDAÇÃO LÓGICA
 * ============================================ */

/**
 * @brief Valida a coerência lógica de um voo.
 *
 * Verifica:
 *  - origem diferente do destino;
 *  - aeronave existente no gestor;
 *  - arrival >= departure;
 *  - regras específicas para voos cancelados;
 *  - regras específicas para voos atrasados;
 *  - coerência entre horários scheduled e actual.
 *
 * @param voo Ponteiro para o voo.
 * @param gestor_aeronaves Gestor de aeronaves para validação cruzada.
 * @return 1 se válido, 0 caso contrário.
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

Voo *validacoes_campos_flights(char **campos, GestorAircrafts *AC);

#endif
