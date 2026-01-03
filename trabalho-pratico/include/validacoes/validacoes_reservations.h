#ifndef VALIDACOES_RESERVATIONS_H
#define VALIDACOES_RESERVATIONS_H

#include "entidades/reservations.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_passengers.h"

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

/* ============================================
 * RESERVAS -> VALIDAÇÕES SINTÁTICAS
 * ============================================ */

/**
 * @brief Valida o identificador de uma reserva.
 *
 * O ID deve ter o formato:
 *   - 'R' seguido de 9 dígitos numéricos.
 *
 * @param s String contendo o ID da reserva.
 * @return 1 se válido, 0 caso contrário.
 */


/**
 * @brief Valida a lista de voos reservados e atribui-a à reserva.
 *
 * A lista deve estar no formato:
 *     ['AA12345','BB54321']
 *
 * A função:
 *  - valida a sintaxe dos parênteses retos;
 *  - separa os voos por vírgulas;
 *  - remove espaços e aspas;
 *  - valida cada ID de voo;
 *  - cria a estrutura interna Voos_reservados;
 *  - atribui a lista à reserva.
 *
 * @param s String contendo a lista de voos.
 * @param r Estrutura da reserva onde a lista será armazenada.
 * @return 1 se válido, 0 caso contrário.
 */


<<<<<<< HEAD
=======
/**
 * @brief Valida campos booleanos de uma reserva.
 *
 * Aceita apenas:
 *   - "true"
 *   - "false"
 *
 * Usado para:
 *   - bagagem extra
 *   - prioridade
 *
 * @param s String contendo o valor booleano.
 * @return 1 se válido, 0 caso contrário.
 */


>>>>>>> 840f49e (limpeza e leaks)
/* ============================================
 * RESERVAS -> VALIDAÇÃO LÓGICA
 * ============================================ */

/**
 * @brief Valida a coerência lógica de uma reserva.
 *
 * Verifica:
 *  - número de voos (1 ou 2);
 *  - existência dos voos no gestor de voos;
 *  - existência do passageiro no gestor de passageiros;
 *  - coerência de escala (se houver 2 voos):
 *        destino do voo 1 = origem do voo 2.
 *
 * @param reserva Ponteiro para a reserva.
 * @param gestor_voos Gestor de voos para validação cruzada.
 * @param gestor_passageiros Gestor de passageiros para validação cruzada.
 * @return 1 se válido, 0 caso contrário.
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

 Reservas *validacoes_campos_reservations(char **campos, GestorFlights *V, GestorPassengers *P);

#endif
