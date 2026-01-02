#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <stdint.h>

/**
 * @file flights.h
 * @brief Interface da entidade Voo.
 *
 * Este módulo define a estrutura Voo e as funções
 * associadas para criação, acesso, modificação e destruição
 * de voos, conforme o dataset flights.csv.
 */

/**
 * @brief Estado do voo.
 */

typedef uint8_t Estado;

// * @brief Voo no horário previsto
#define ESTADO_ON_TIME 0
// * @brief Voo atrasado
#define ESTADO_DELAYED 1
// @brief Voo cancelado
#define ESTADO_CANCELLED 2

/**
 * @brief Estrutura Voo.
 *
 * A definição concreta encontra-se no ficheiro flights.c,
 * garantindo encapsulamento.
 */

typedef struct voo Voo;

// GETTERS

/**
 * @brief Obtém o identificador do voo.
 *
 * @param v Ponteiro para o voo.
 * @return Cópia do identificador do voo.
 */

char *voo_get_flight_id (const Voo *v);

/**
 * @brief Obtém o código IATA do aeroporto de origem.
 *
 * @param v Ponteiro para o voo.
 * @return Código IATA da origem.
 */

const char *voo_get_code_origin (const Voo *v);

/**
 * @brief Obtém o código IATA do aeroporto de destino.
 *
 * @param v Ponteiro para o voo.
 * @return Código IATA do destino.
 */

const char *voo_get_code_destination (const Voo *v);

/**
 * @brief Obtém a data/hora prevista de chegada.
 *
 * @param v Ponteiro para o voo.
 * @return Data/hora prevista de chegada.
 */

long long voo_get_arrival (const Voo *v);

/**
 * @brief Obtém a data/hora efetiva de chegada.
 *
 * @param v Ponteiro para o voo.
 * @return Data/hora efetiva de chegada.
 */

long long voo_get_actual_arrival (const Voo *v);

/**
 * @brief Obtém o identificador da aeronave.
 *
 * @param v Ponteiro para o voo.
 * @return Identificador da aeronave.
 */

const char *voo_get_id_aircraft (const Voo *v);

/**
 * @brief Obtém o estado do voo.
 *
 * @param v Ponteiro para o voo.
 * @return Estado do voo.
 */

Estado voo_get_status (const Voo *v);

/**
 * @brief Obtém a data/hora prevista de partida.
 *
 * @param v Ponteiro para o voo.
 * @return Data/hora prevista de partida.
 */

long long voo_get_departure (const Voo *v);

/**
 * @brief Obtém a data/hora efetiva de partida.
 *
 * @param v Ponteiro para o voo.
 * @return Data/hora efetiva de partida.
 */

long long voo_get_actual_departure (const Voo *v);

/**
 * @brief Obtém a companhia aérea do voo
 * @param v Ponteiro para o voo
 * @return Cópia da airline (deve ser libertada com g_free)
 */
char *voo_get_airline(const Voo *v);

//SETTERS

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

void voo_set_code (Voo *v, char *code, char versao);

/**
 * @brief Define o identificador do voo.
 *
 * @param v Ponteiro para o voo.
 * @param id Identificador do voo.
 */

void voo_set_flight_id (Voo *v, char *id);

/**
 * @brief Define a companhia aérea.
 *
 * @param v Ponteiro para o voo.
 * @param airl Nome da companhia aérea.
 */

void voo_set_airline (Voo *v, char *airl);

/**
 * @brief Define o identificador da aeronave.
 *
 * @param v Ponteiro para o voo.
 * @param id_airc Identificador da aeronave.
 */


void voo_set_id_aircraft (Voo *v, char *id_airc);

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

//CRIA E DESTRÓI

/**
 * @brief Cria um novo voo vazio.
 *
 * @return Ponteiro para o voo criado.
 */

Voo *criaVoo ();

/**
 * @brief Liberta a memória associada a um voo.
 *
 * @param data Ponteiro genérico para o voo.
 */

void libertaVoo(void *data);

#endif

