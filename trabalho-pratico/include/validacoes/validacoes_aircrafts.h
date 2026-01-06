#ifndef VALIDACOES_AIRCRAFTS_H
#define VALIDACOES_AIRCRAFTS_H

#include "entidades/aircrafts.h"
#include <glib.h>

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

Aeronave *validacoes_campos_aircrafts(char **campos, StringPool *pool);

#endif
