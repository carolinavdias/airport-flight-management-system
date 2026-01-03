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

/* ============================================
 * PASSAGEIROS -> VALIDAÇÃO SINTÁTICA
 * ============================================ */

/**
 * @brief Valida o identificador do passageiro.
 *
 * O ID deve ter exatamente 9 dígitos numéricos.
 *
 * @param s String contendo o ID.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_id_passageiro(const char *s);

/**
 * @brief Valida o género do passageiro.
 *
 * Aceita apenas:
 *  - 'M' (Masculino)
 *  - 'F' (Feminino)
 *  - 'O' (Outro)
 *
 * @param s String contendo o género.
 * @return 1 se válido, 0 caso contrário.
 */


/**
 * @brief Valida um email.
 *
 * O email deve seguir o formato:
 *   username@dominio.ext
 *
 * Regras:
 *  - username: letras minúsculas, dígitos ou '.'
 *  - domínio: apenas letras minúsculas
 *  - extensão: 2 a 3 letras minúsculas
 *
 * @param s String contendo o email.
 * @return 1 se válido, 0 caso contrário.
 */


/**
<<<<<<< HEAD
=======
 * @brief Valida uma data no formato YYYY-MM-DD.
 *
 * Verifica:
 *  - tamanho correto (10 caracteres)
 *  - posições fixas de '-'
 *  - todos os restantes caracteres são dígitos
 *  - limites válidos para ano, mês e dia
 *
 * @param s String contendo a data.
 * @return 1 se válido, 0 caso contrário.
 */


/**
>>>>>>> 840f49e (limpeza e leaks)
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

Passageiros *validacoes_campos_passengers(char **campos);

#endif
