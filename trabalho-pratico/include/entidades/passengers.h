#ifndef PASSENGERS_H
#define PASSENGERS_H

/* Forward declaration */
typedef struct _StringPool StringPool;

#include <stdint.h>
#include <glib.h>

/** 
    * @file passengers.h 
    * @brief Interface da entidade Passageiros. 
    * 
    * Este módulo define a estrutura Passageiros e disponibiliza 
    * funções para criação, acesso, modificação e libertação de passageiros 
    * lidos do ficheiro passengers.csv. 
    * 
    * Todos os getters que retornam strings devolvem cópias (g_strdup), 
    * cabendo ao utilizador libertar a memória retornada. 
    */ 

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @struct passageiros
 * @brief Estrutura da entidade passageiro.
 */

typedef struct passageiros Passageiros;

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * @brief Obtém o identificador do passageiro.
 *
 * Retorna o identificador único do passageiro.  
 * A string devolvida é propriedade interna e não deve ser libertada.
 *
 * @param p Ponteiro para o passageiro.
 * @return Identificador ou NULL se inválido.
 */

const char *passenger_get_id (const Passageiros *p);

/**
 * @brief Obtém o primeiro nome do passageiro.
 *
 * Retorna uma cópia do primeiro nome.
 *
 * @param p Ponteiro para o passageiro.
 * @return String alocada dinamicamente (libertar com g_free).
 */

const char *passenger_get_primeiro (const Passageiros *p);

/**
 * @brief Obtém o último nome do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @return Cópia do último nome (libertar com g_free).
 */

const char *passenger_get_ultimo (const Passageiros *p);

/**
 * @brief Obtém a data de nascimento do passageiro.
 *
 * A data é armazenada como inteiro no formato YYYYMMDD.
 *
 * @param p Ponteiro para o passageiro.
 * @return Data de nascimento ou 0 se inválido.
 */

int passenger_get_data (Passageiros *p);

/**
 * @brief Obtém a nacionalidade do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @return Cópia da nacionalidade (libertar com g_free).
 */

const char *passenger_get_nacionalidade (const Passageiros *p);

/**
 * @brief Define o identificador do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @param id Novo identificador.
 */

/* ============================================
 * SETTERS
 * ============================================ */

void passenger_set_id (Passageiros *p, char *id, StringPool *pool);

 /** 
    * @brief Define o primeiro nome do passageiro.
    *  
    * @param p Ponteiro para o passageiro. 
    * @param pn Primeiro nome. 
    */

void passenger_set_pn (Passageiros *p, char *pn, StringPool *pool);

/**
 * @brief Define o último nome do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @param un Último nome.
 */

void passenger_set_un (Passageiros *p, char *un, StringPool *pool);

/**
 * @brief Define a data de nascimento do passageiro.
 *
 * Converte uma string no formato YYYY-MM-DD para um inteiro YYYYMMDD.
 *
 * @param p Ponteiro para o passageiro.
 * @param dn Data textual no formato YYYY-MM-DD.
 */

void passenger_set_dn (Passageiros *p, char *dn);

/**
 * @brief Define a nacionalidade do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @param nc Nacionalidade.
 */

void passenger_set_nc (Passageiros *p, char *nc, StringPool *pool);

/* ============================================
 * CRIA PASSAGEIRO
 * ============================================ */

/**
 * @brief Cria um novo passageiro vazio.
 *
 * Todos os campos são inicializados a NULL/0.
 *
 * @return Ponteiro para o passageiro criado.
 */

Passageiros *criaPassageiro();

/* ============================================
 * DESTRÓI PASSAGEIRO
 * ============================================ */

/**
 * @brief Liberta toda a memória associada a um passageiro.
 *
 * Esta função é compatível com g_hash_table_new_full().
 *
 * @param data Ponteiro genérico para Passageiros.
 */

void libertaPassageiro(void *data);

#endif
