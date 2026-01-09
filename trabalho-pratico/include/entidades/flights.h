#ifndef FLIGHTS_H
#define FLIGHTS_H

/** Forward declaration */

typedef struct _StringPool StringPool;

#include <stdint.h>

/**
 * @file flights.h
 * @brief Interface da entidade Voo.
 *
 * Este módulo define a estrutura Voo e as funções
 * associadas para criação, acesso, modificação e destruição
 * de voos, conforme o dataset flights.csv.
 */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @brief Estado do voo.
 */

typedef uint8_t Estado;

/** @brief Voo no horário previsto */
#define ESTADO_ON_TIME 0

/** @brief Voo atrasado */
#define ESTADO_DELAYED 1

/** @brief Voo cancelado */
#define ESTADO_CANCELLED 2

/**
 * @brief Estrutura Voo.
 *
 * A definição concreta encontra-se no ficheiro flights.c,
 * garantindo encapsulamento.
 */

typedef struct voo Voo;

/* ============================================
 * GETTERS 
 * ============================================ */

/**
 * @brief Obtém o identificador do voo.
 *
 * Retorna um ponteiro constante para o identificador interno.
 * A memória é gerida pelo StringPool, pelo que o utilizador não deve
 * libertar esta string.
 *
 * @param v Ponteiro para o voo.
 * @return Ponteiro para o identificador ou NULL se o voo for inválido.
 */

const char *voo_get_flight_id (const Voo *v);

/**
 * @brief Obtém o código IATA do aeroporto de origem.
 *
 * Retorna um ponteiro constante para o código interno.
 * O utilizador não deve libertar esta memória.
 *
 * @param v Ponteiro para o voo.
 * @return Ponteiro para o código IATA de origem ou NULL.
 */

const char *voo_get_code_origin (const Voo *v);

/**
 * @brief Obtém o código IATA do aeroporto de destino.
 *
 * Retorna um ponteiro constante para o código interno.
 * O utilizador não deve libertar esta memória.
 *
 * @param v Ponteiro para o voo.
 * @return Ponteiro para o código IATA de destino ou NULL.
 */

const char *voo_get_code_destination (const Voo *v);

 /** 
  * @brief Obtém a data/hora prevista de chegada. 
  * 
  * @param v Ponteiro para o voo. 
  * @return Valor inteiro ordenável representando a data/hora prevista. 
  */

long long voo_get_arrival (const Voo *v);

/**
 * @brief Obtém a data/hora efetiva de chegada.
 *
 * @param v Ponteiro para o voo.
 * @return Valor inteiro ordenável representando a data/hora efetiva.
 */

long long voo_get_actual_arrival (const Voo *v);

/**
 * @brief Obtém o identificador da aeronave associada ao voo.
 *
 * Retorna um ponteiro constante para o identificador interno.
 * O utilizador não deve libertar esta memória.
 *
 * @param v Ponteiro para o voo.
 * @return Ponteiro para o identificador da aeronave ou NULL.
 */

const char *voo_get_id_aircraft (const Voo *v);

/**
 * @brief Obtém o estado do voo.
 *
 * @param v Ponteiro para o voo.
 * @return Estado do voo (enum Estado).
 */

Estado voo_get_status (const Voo *v);

/** 
 * @brief Obtém a data/hora prevista de partida. 
 * 
 * @param v Ponteiro para o voo. 
 * @return Valor inteiro ordenável representando a data/hora prevista.  
 */

long long voo_get_departure (const Voo *v);

/**
 * @brief Obtém a data/hora efetiva de partida.
 *
 * @param v Ponteiro para o voo.
 * @return Valor inteiro ordenável representando a data/hora efetiva.
 */

long long voo_get_actual_departure (const Voo *v);

/**
 * @brief Obtém a companhia aérea do voo.
 *
 * Retorna um ponteiro constante para a companhia aérea interna.
 * A memória é gerida pelo StringPool, pelo que o utilizador não deve
 * libertar esta string.
 *
 * @param v Ponteiro para o voo.
 * @return Ponteiro para a companhia aérea ou NULL.
 */

const char *voo_get_airline(const Voo *v);

/* ============================================
 * SETTERS
 * ============================================ */

/**
 * @brief Define o estado do voo.
 *
 * @param v Ponteiro para o voo.
 * @param status String que representa o estado.
 */

void voo_set_status (Voo *v, char *status);

/**
 * @brief Define o código IATA de origem ou destino.
 *
 * @param v Ponteiro para o voo.
 * @param code Código IATA.
 * @param versao 'o' para origem, 'd' para destino.
 */

void voo_set_code (Voo *v, char *code, char versao, StringPool *pool);

/**
 * @brief Define o identificador do voo.
 *
 * @param v Ponteiro para o voo.
 * @param id Identificador do voo.
 */

void voo_set_flight_id (Voo *v, char *id, StringPool *pool);

/**
 * @brief Define a companhia aérea.
 *
 * @param v Ponteiro para o voo.
 * @param airl Nome da companhia aérea.
 */

void voo_set_airline (Voo *v, char *airl, StringPool *pool);

/**
 * @brief Define o identificador da aeronave.
 *
 * @param v Ponteiro para o voo.
 * @param id_airc Identificador da aeronave.
 */


void voo_set_id_aircraft (Voo *v, char *id_airc, StringPool *pool);

/* ============================================
 * USADO PARA SET
 * ============================================ */

/**
 * @brief Converte uma data/hora em formato textual para inteiro.
 *
 * @param s String no formato YYYY-MM-DD HH:MM.
 * @return Valor inteiro ordenável.
 */

long long converte_dataH (const char *s);

/**
 * @brief Define um campo de data/hora do voo.
 *
 * @param v Ponteiro para o voo.
 * @param s String da data/hora.
 * @param campo Identificador do campo:
 *        1 - partida prevista
 *        2 - partida efetiva
 *        3 - chegada prevista
 *        4 - chegada efetiva
 */

void voo_set_dataH (Voo *v, const char *s, int campo);

/* ============================================
 * CRIA VOO 
 * ============================================ */

/**
 * @brief Cria um novo voo vazio.
 *
 * @return Ponteiro para o voo criado.
 */

Voo *criaVoo ();

/* ============================================
 * DESTRÓI VOO 
 * ============================================ */

/**
 * @brief Liberta a memória associada a um voo.
 *
 * @param data Ponteiro genérico para o voo.
 */

void libertaVoo(void *data);

/**
 * @brief Compara dois voos pela data/hora de partida real.
 *
 * Função de comparação usada com qsort() para ordenar um array
 * de apontadores para Voo por ordem cronológica crescente.
 *
 * A comparação é feita com base no campo:
 *  - actual_departure (data/hora real de partida)
 *
 *  - Retorna valor negativo se o primeiro voo ocorrer antes do segundo
 *  - Retorna valor positivo se o primeiro voo ocorrer depois do segundo
 *  - Retorna 0 se as datas forem iguais
 *
 * Esta função é utilizada no parser para ordenar os voos uma única vez,
 * permitindo que a Query 3 seja executada de forma eficiente.
 *
 * @param a Apontador para um apontador de Voo
 * @param b Apontador para um apontador de Voo
 * @return Valor inteiro conforme convenção do qsort()
 */

int compara_voos_por_data(const void *a, const void *b);

#endif

