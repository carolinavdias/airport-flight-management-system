#ifndef PASSENGERS_H
#define PASSENGERS_H

/** Forward declaration */

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
    * Todos os getters que retornam strings devolvem ponteiros constantes
    * para campos internos geridos pelo StringPool.
    * O utilizador não deve libertar estas strings.
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
 * Retorna um ponteiro constante para o identificador interno.
 * A memória é gerida pelo StringPool, pelo que o utilizador não deve
 * libertar esta string.
 *
 * @param p Ponteiro para o passageiro.
 * @return Ponteiro para o identificador ou NULL se inválido.
 */

const char *passenger_get_id (const Passageiros *p);

/**
 * @brief Obtém o primeiro nome do passageiro.
 *
 * Retorna um ponteiro constante para o primeiro nome interno.
 * A memória é gerida pelo StringPool, pelo que o utilizador não deve
 * libertar esta string.
 *
 * @param p Ponteiro para o passageiro.
 * @return Ponteiro para o primeiro nome ou NULL se inválido.
 */

const char *passenger_get_primeiro (const Passageiros *p);

/**
 * @brief Obtém o último nome do passageiro.
 *
 * Retorna um ponteiro constante para o último nome interno.
 * A memória é gerida pelo StringPool, pelo que o utilizador não deve
 * libertar esta string.
 *
 * @param p Ponteiro para o passageiro.
 * @return Ponteiro para o último nome ou NULL se inválido.
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
 * Retorna um ponteiro constante para a nacionalidade interna.
 * A memória é gerida pelo StringPool, pelo que o utilizador não deve
 * libertar esta string.
 *
 * @param p Ponteiro para o passageiro.
 * @return Ponteiro para a nacionalidade ou NULL se inválido.
 */

const char *passenger_get_nacionalidade (const Passageiros *p);

/* ============================================
 * SETTERS
 * ============================================ */

/**
 * @brief Define o identificador do passageiro.
 *
 * @param p Ponteiro para o passageiro.
 * @param id Novo identificador.
 */

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
 * Esta função é compatível com gestores de memória().
 *
 * @param data Ponteiro genérico para Passageiros.
 */

void libertaPassageiro(void *data);

#endif
