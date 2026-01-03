#ifndef VALIDACOES_AIRCRAFTS_H
#define VALIDACOES_AIRCRAFTS_H

#include "entidades/aircrafts.h"

 /** 
  * @file validacoes_aircrafts.h 
  * @brief Validações sintáticas e semânticas para aeronaves. 
  * 
  * Este módulo contém funções responsáveis por validar campos individuais 
  * e conjuntos de campos associados a aeronaves durante o parsing. 
  * 
  * Inclui: 
  * - validação do ano de fabrico;
  * - validação completa dos campos de uma aeronave; 
  * - criação da estrutura Aeronave apenas quando os dados são válidos. 
  */

/**
 * @brief Valida um ano de fabrico.
 *
 * O ano deve:
 *  - ter exatamente 4 dígitos;
 *  - ser composto apenas por caracteres numéricos;
 *  - ser menor ou igual ao ano atual (2025).
 *
 * @param s String contendo o ano.
 * @return 1 se o ano for válido, 0 caso contrário.
 */

int valida_year (const char *s);

/**
 * @brief Valida todos os campos de uma aeronave e cria a estrutura correspondente.
 *
 * Esta função recebe um array de strings contendo os campos da aeronave
 * (ID, fabricante, modelo, ano) e verifica se os valores são válidos.
 *
 * Se forem válidos, cria e preenche uma estrutura Aeronave.
 * Caso contrário, devolve NULL.
 *
 * @param campos Array de strings contendo os campos da aeronave.
 * @return Ponteiro para Aeronave válida, ou NULL se algum campo for inválido.
 */

Aeronave *validacoes_campos_aircrafts(char **campos);

#endif
