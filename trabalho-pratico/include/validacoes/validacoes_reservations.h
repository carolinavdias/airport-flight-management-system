#ifndef VALIDACOES_RESERVATIONS_H
#define VALIDACOES_RESERVATIONS_H

#include "entidades/reservations.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_passengers.h"

#include <glib.h>

/**
 * @file validacoes_reservations.h
 * @brief Validações sintáticas e lógicas para reservas.
 *
 * Este módulo contém funções responsáveis por validar campos individuais
 * e a coerência global de uma reserva durante o parsing.
 *
 * Inclui:
 *  - validação sintática do ID da reserva;
 *  - validação da lista de voos reservados;
 *  - validação de campos booleanos;
 *  - validação lógica completa (existência de voos, passageiro e escala);
 *  - criação da estrutura Reservas apenas quando os dados são válidos.
 */


/* ============================================================ 
 * RESERVA -> VALIDAÇÃO COMPLETA DO PARSING 
 * ============================================================ */ 

/** 
 * @brief Valida todos os campos de uma reserva e cria a estrutura correspondente. 
 * 
 * Esta função valida: 
 * - ID da reserva; 
 * - lista de voos reservados; 
 * - ID do passageiro; 
 * - bagagem extra (bool); 
 * - prioridade (bool). 
 * 
 * Se todos os campos forem válidos, cria e preenche a estrutura Reservas. 
 * Caso contrário, devolve NULL. 
 * 
 * @param campos Array de strings contendo os campos da reserva. 
 * @param V Gestor de voos para validação lógica. 
 * @param P Gestor de passageiros para validação lógica. 
 * @return Ponteiro para Reservas válido, ou NULL se algum campo for inválido. 
 */

 Reservas *validacoes_campos_reservations(char **campos, GestorFlights *V, GestorPassengers *P, StringPool *pool);

#endif
