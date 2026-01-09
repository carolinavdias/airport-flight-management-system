#ifndef GESTOR_AIRPORTS_H
#define GESTOR_AIRPORTS_H

#include "entidades/airports.h"

/**
 * @file gestor_airports.h
 * @brief Interface do gestor de aeroportos.
 *
 * Este módulo define o gestor responsável por armazenar e gerir todos os
 * aeroportos do sistema, permitindo inserção, pesquisa e verificação de
 * existência.
 *
 * Inclui ainda estruturas auxiliares para contagem de chegadas e partidas
 * por aeroporto, usadas em consultas estatísticas otimizadas (ex.: Q1).
 */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @struct gestor_airports
 * @brief Estrutura interna do gestor de aeroportos.
 *
 * O gestor utiliza uma hash table para armazenar aeroportos
 * indexados pelo código IATA, bem como tabelas auxiliares para
 * contagem de chegadas e partidas.
 */

typedef struct gestor_airports GestorAirports;

/* ============================================
 * CONTAGENS DE PASSAGEIROS 
 * ============================================ */

/**
 * @brief Cria um novo gestor de aeroportos.
 *
 * Inicializa a estrutura de dados responsável por armazenar os aeroportos.
 *
 * @return Ponteiro para o gestor criado, ou NULL em caso de erro.
 */

GestorAirports *gestor_airports_cria(void);

/**
 * @brief Insere um aeroporto no gestor.
 *
 * O aeroporto é indexado pelo seu código IATA.
 *
 * @param g Ponteiro para o gestor.
 * @param a Ponteiro para o aeroporto a inserir.
 */

void gestor_airports_insere(GestorAirports *g, Aeroporto *a);

/**
 * @brief Procura um aeroporto pelo código IATA.
 *
 * @param g Ponteiro para o gestor.
 * @param code_IATA Código IATA do aeroporto.
 * @return Ponteiro para o aeroporto, ou NULL se não existir.
 */

Aeroporto *gestor_airports_procura(GestorAirports *g, const char *code_IATA);

/**
 * @brief Verifica se um aeroporto existe no gestor.
 *
 * @param g Ponteiro para o gestor.
 * @param code_IATA Código IATA do aeroporto.
 * @return 1 se existir, 0 caso contrário.
 */

int gestor_airports_existe(GestorAirports *g, const char *code_IATA);

/* ============================================
 * GESTÃO DE AEROPORTOS
 * ============================================ */

/**
 * @brief Inicializa as estruturas de contagem de chegadas e partidas.
 *
 * Deve ser chamada antes de usar as funções de incremento e consulta.
 *
 * @param g Ponteiro para o gestor.
 */

void gestor_airports_init_contagens(GestorAirports *g);

/* ============================================
 * CONTAGENS DE CHEGADAS E PARTIDAS
 * ============================================ */

/**
 * @brief Incrementa o número de chegadas associadas a um aeroporto.
 *
 * @param g Ponteiro para o gestor.
 * @param code Código IATA do aeroporto.
 */

void gestor_airports_incrementa_chegadas(GestorAirports *g, const char *code);

/**
 * @brief Incrementa o número de partidas associadas a um aeroporto.
 *
 * @param g Ponteiro para o gestor.
 * @param code Código IATA do aeroporto.
 */

void gestor_airports_incrementa_partidas(GestorAirports *g, const char *code);

/**
 * @brief Obtém o número total de chegadas registadas num aeroporto.
 *
 * @param g Ponteiro para o gestor.
 * @param code Código IATA do aeroporto.
 * @return Número de chegadas.
 */

int gestor_airports_get_chegadas(GestorAirports *g, const char *code);

/**
 * @brief Obtém o número total de partidas registadas num aeroporto.
 *
 * @param g Ponteiro para o gestor.
 * @param code Código IATA do aeroporto.
 * @return Número de partidas.
 */

int gestor_airports_get_partidas(GestorAirports *g, const char *code);

/* ============================================
 * DESTRÓI GESTOR DE AEROPORTOS
 * ============================================ */

/**
 * @brief Liberta toda a memória associada ao gestor de aeroportos.
 *
 * Liberta os aeroportos armazenados, as tabelas auxiliares e o próprio gestor.
 *
 * @param g Ponteiro para o gestor.
 */

void gestor_airports_liberta(GestorAirports *g);

#endif
