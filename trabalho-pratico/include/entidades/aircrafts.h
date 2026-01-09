#ifndef AIRCRAFTS_H
#define AIRCRAFTS_H

/** Forward declaration */

typedef struct _StringPool StringPool;

/**
 * @file aircrafts.h
 * @brief Interface da entidade Aeronave.
 *
 * Este módulo define a estrutura Aeronave e disponibiliza
 * funções para criação, acesso, modificação e libertação
 * de aeronaves lidas a partir do ficheiro aircrafts.csv.
 */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @struct aeronave 
 * @brief Estrutura da entidade aeronave.
 */

typedef struct aeronave Aeronave;

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * @brief Obtém o identificador da aeronave.
 *
 * Retorna um ponteiro constante para o identificador interno.
 * O utilizador não deve libertar esta memória.
 *
 * @param a Ponteiro para a aeronave.
 * @return Ponteiro para o identificador ou NULL se a aeronave for inválida.
 */

const char *aircraft_get_identifier (const Aeronave *a);

/**
 * @brief Obtém o fabricante da aeronave.
 *
 * Retorna um ponteiro constante para o fabricante interno.
 * O utilizador não deve libertar esta memória.
 *
 * @param a Ponteiro para a aeronave.
 * @return Ponteiro para o fabricante ou NULL se a aeronave for inválida.
 */

const char *aircraft_get_manufacturer (const Aeronave *a);

/**
 * @brief Obtém o modelo da aeronave.
 *
 * Retorna um ponteiro constante para o modelo interno.
 * O utilizador não deve libertar esta memória.
 *
 * @param a Ponteiro para a aeronave.
 * @return Ponteiro para o modelo ou NULL se a aeronave for inválida.
 */

const char *aircraft_get_model (const Aeronave *a);

/**
 * @brief Obtém o ano de fabrico da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @return Ano de fabrico ou 0 se a aeronave for inválida.
 */

int aircraft_get_year(const Aeronave *a);

/* ============================================
 * SETTERS
 * ============================================ */

/**
 * @brief Define o identificador da aeronave.
 *
 * Substitui o identificador atual por uma cópia da string fornecida.
 *
 * @param a Ponteiro para a aeronave.
 * @param id Novo identificador.
 */

void aircraft_set_id (Aeronave *a, char *id, StringPool *pool);

/**
 * @brief Define o modelo da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @param model_ Modelo da aeronave.
 */

void aircraft_set_model (Aeronave *a, char *model_, StringPool *pool);

/**
 * @brief Define o fabricante da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @param manuf Fabricante da aeronave.
 */

void aircraft_set_manuf(Aeronave *a, char *s, StringPool *pool);
/**
 * @brief Define o ano de fabrico da aeronave.
 *
 * @param a Ponteiro para a aeronave.
 * @param year Ano de fabrico.
 */

void aircraft_set_year(Aeronave *a, char *year);

/* ============================================
 * CRIA AERONAVE
 * ============================================ */

/**
 * @brief Cria uma nova aeronave vazia.
 *
 * @return Ponteiro para a aeronave criada.
 */

Aeronave *criaAeronave();

/* ============================================
 * DESTRÓI AERONAVE
 * ============================================ */

/**
 * @brief Liberta a memória associada a uma aeronave.
 *
 * Liberta todos os campos internos e a própria estrutura.
 *
 * @param data Ponteiro genérico para a aeronave.
 */

void libertaAeronave(void *data);

#endif
