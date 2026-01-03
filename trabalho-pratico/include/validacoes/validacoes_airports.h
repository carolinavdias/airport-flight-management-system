#ifndef VALIDACOES_AIRPORTS_H
#define VALIDACOES_AIRPORTS_H

#include "entidades/airports.h"

/**
 * @file validacoes_airports.h
 * @brief Validações sintáticas e semânticas para aeroportos.
 *
 * Este módulo contém funções responsáveis por validar campos individuais
 * e conjuntos de campos associados a aeroportos durante o parsing.
 *
 * Inclui:
 *  - validação do código IATA;
 *  - validação de coordenadas (latitude e longitude);
 *  - validação do tipo de aeroporto;
 *  - validação completa dos campos e criação da estrutura Aeroporto.
 */


 /** 
  * @brief Valida todos os campos de um aeroporto e cria a estrutura correspondente.
  * 
  * Esta função recebe um array de strings contendo os campos do aeroporto 
  * e verifica se os valores essenciais são válidos: 
  * - código IATA 
  * - latitude 
  * - longitude 
  * - tipo de aeroporto 
  * 
  * Se forem válidos, cria e preenche uma estrutura Aeroporto. 
  * Caso contrário, devolve NULL. 
  * 
  * @param campos Array de strings contendo os campos do aeroporto. 
  * @return Ponteiro para Aeroporto válido, ou NULL se algum campo for inválido. 
  */

Aeroporto *validacoes_campos_airports(char **campos);

#endif
