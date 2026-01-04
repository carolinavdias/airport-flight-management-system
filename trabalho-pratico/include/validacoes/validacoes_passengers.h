#ifndef VALIDACOES_PASSENGERS_H
#define VALIDACOES_PASSENGERS_H

#include "entidades/passengers.h"

/** 
 * @file validacoes_passengers.h 
 * @brief Validações sintáticas e semânticas para passageiros. 
 * 
 * Este módulo contém funções responsáveis por validar campos individuais 
 * e conjuntos de campos associados a passageiros durante o parsing. 
 *
 * Inclui:
 * - validação do ID do passageiro; 
 * - validação do género; 
 * - validação do email; 
 * - validação da data de nascimento; 
 * - criação da estrutura Passageiros apenas quando os dados são válidos. 
 */


/**
 * @brief Valida todos os campos de um passageiro e cria a estrutura correspondente.
 *
 * Esta função valida:
 *  - ID
 *  - data de nascimento
 *  - género
 *  - email
 *
 * Se todos os campos forem válidos, cria e preenche a estrutura Passageiros.
 * Caso contrário, devolve NULL.
 *
 * @param campos Array de strings contendo os campos do passageiro.
 * @return Ponteiro para Passageiros válido, ou NULL se algum campo for inválido.
 */

Passageiros *validacoes_campos_passengers(char **campos, GHashTable *lista_strings);

#endif
