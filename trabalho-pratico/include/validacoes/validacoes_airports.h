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
 * @brief Valida um código IATA.
 *
 * O código IATA deve:
 *  - ter exatamente 3 caracteres;
 *  - ser composto apenas por letras maiúsculas (A–Z).
 *
 * @param s String contendo o código IATA.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_codigoIATA (const char *s);

/**
 * @brief Valida uma coordenada geográfica (latitude ou longitude).
 *
 * A função verifica:
 *  - formato numérico válido (incluindo sinal e ponto decimal);
 *  - apenas um ponto decimal permitido;
 *  - limites válidos conforme o tipo:
 *      - versao = 1 → latitude (−90 a 90)
 *      - versao = 2 → longitude (−180 a 180)
 *
 * @param s String contendo a coordenada.
 * @param versao 1 para latitude, 2 para longitude.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_coordenadas (const char* s, int versao);

/**
 * @brief Valida o tipo de aeroporto.
 *
 * Aceita apenas os tipos definidos na especificação:
 *  - small_airport
 *  - medium_airport
 *  - large_airport
 *  - heliport
 *  - seaplane_base
 *
 * @param s String contendo o tipo.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_tipo_aer(const char *s);

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
