#ifndef AIRPORTS_H
#define AIRPORTS_H

#include <stdint.h>

/**
 * @file airports.h
 * @brief Interface da entidade Aeroporto.
 *
 * Este módulo define a estrutura Aeroporto e as funções
 * associadas para criação, acesso, modificação e destruição de
 * aeroportos, conforme o dataset airports.csv.
 */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @brief Tipo de aeroporto.
 *
 * Representa o tipo do aeroporto segundo o dataset.
 */

typedef uint8_t Tipo_aeroporto;

// * @brief Aeroporto de pequena dimensão
#define TIPO_SMALL_AIRPORT 0

// * @brief Aeroporto de média dimensão
#define TIPO_MEDIUM_AIRPORT 1

// * @brief Aeroporto de grande dimensão
#define TIPO_LARGE_AIRPORT 2

// * @brief Heliporto 
#define TIPO_HELIPORT 3

// * @brief Base de hidroaviões
#define TIPO_SEAPLANE_BASE 4

/**
 * @brief Estrutura abstrata Aeroporto.
 *
 * A definição interna encontra-se no ficheiro airports.c,
 * garantindo encapsulamento.
 */

typedef struct aeroporto Aeroporto;

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * @brief Obtém o código IATA do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return Cópia do código IATA (3 letras).
 */

char *airport_get_code_IATA (const Aeroporto *a);

/**
 * @brief Obtém o nome do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return Cópia do nome do aeroporto.
 */

const char *airport_get_name (const Aeroporto *a);

/**
 * @brief Obtém a cidade do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return Cópia da cidade.
 */

const char *airport_get_city (const Aeroporto *a);

/**
 * @brief Obtém o país do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return Cópia do país.
 */

const char *airport_get_country (const Aeroporto *a);

/**
 * @brief Obtém o tipo do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @return Tipo do aeroporto.
 */

Tipo_aeroporto airport_get_type (Aeroporto *a);

/* ============================================
 * SETTERS 
 * ============================================ */

/**
 * @brief Define o nome do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @param name_ Nome do aeroporto.
 */

void airport_set_name (Aeroporto *a, char *name_);

/**
 * @brief Define a cidade do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @param city_ Cidade do aeroporto.
 */

void airport_set_city (Aeroporto *a, char *city_);

/**
 * @brief Define o país do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @param country_ País do aeroporto.
 */

void airport_set_country (Aeroporto *a, char *country_);

/**
 * @brief Define o código IATA do aeroporto.
 *
 * @param a Ponteiro para o aeroporto.
 * @param code_ Código IATA (3 letras).
 */

void airport_set_code_IATA (Aeroporto *a, char *code_);

/**
 * @brief Define o tipo do aeroporto.
 *
 * Converte a string do dataset para o tipo interno.
 *
 * @param a Ponteiro para o aeroporto.
 * @param tipo String que representa o tipo.
 */

void airport_set_type (Aeroporto *a, char *tipo);

/* ============================================
 * CRIA AEROPORTO
 * ============================================ */

/**
 * @brief Cria um novo aeroporto vazio.
 *
 * @return Ponteiro para o aeroporto criado.
 */

Aeroporto *criaAeroporto ();

/* ============================================
 * DESTRÓI AEROPORTO
 * ============================================ */

/**
 * @brief Liberta a memória associada a um aeroporto.
 *
 * @param data Ponteiro genérico para o aeroporto.
 */

void libertaAeroporto(void *data);

#endif

