#ifndef GESTOR_FLIGHTS_H
#define GESTOR_FLIGHTS_H

#include <stdbool.h>
#include "entidades/flights.h"

/**
 * @file gestor_flights.h
 * @brief Interface do gestor de voos.
 *
 * Este módulo define o gestor responsável por armazenar, consultar e
 * manipular todos os voos do sistema.  
 *
 * O gestor utiliza uma hash table para indexar voos pelo seu flight_id,
 * permitindo inserção, pesquisa, verificação de existência e iteração.
 *
 * Inclui ainda:
 *  - um array ordenado de voos,
 *  - uma tabela auxiliar de contagens por aircraft_id (usada na Q2).
 */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @struct gestor_flights
 * @brief Estrutura interna do gestor de voos.
 * Estrutura que representa o gestor de voos.
 * A implementação interna utiliza hash tables.
 */

typedef struct gestor_flights GestorFlights;

/* ============================================
 * CRIA GESTOR DE VOO
 * ============================================ */

/**
 * @brief Cria um novo gestor de voos.
 *
 * Inicializa a hash table interna e define os campos auxiliares.
 *
 * @return Ponteiro para o gestor criado, ou NULL em caso de erro.
 */

GestorFlights *gestor_flights_novo();

/* ============================================
 * DESTRÓI GESTOR DE VOO 
 * ============================================ */

 /** 
  * @brief Liberta toda a memória associada ao gestor de voos. 
  * 
  * Liberta todos os voos armazenados, o array ordenado (se existir) 
  * e a tabela auxiliar de contagens. 
  * 
  * @param g Ponteiro para o gestor. 
  */

void gestor_flights_destroy(GestorFlights *g);

/* ============================================
 * OPERAÇÕES BÁSICAS
 * ============================================ */

 /** 
  * @brief Insere um voo no gestor. 
  * 
  * O voo é indexado pelo seu flight_id. 
  * 
  * @param g Ponteiro para o gestor. 
  * @param voo Ponteiro para o voo a inserir. 
  */

void gestor_flights_inserir(GestorFlights *g, Voo *voo);

/**
 * @brief Verifica se um voo existe no gestor.
 *
 * @param g Ponteiro para o gestor.
 * @param flight_id Identificador do voo.
 * @return true se existir, false caso contrário.
 */

bool gestor_flights_existe(GestorFlights *g, const char *flight_id); 

/**
 * @brief Imprime todos os voos (debug).
 *
 * @param g Ponteiro para o gestor.
 */

void gestor_flights_print(GestorFlights *g);

/**
 * @brief Aplica uma função a todos os voos do gestor.
 *
 * @param g Ponteiro para o gestor.
 * @param func Função a aplicar.
 * @param user_data Dados adicionais fornecidos ao callback.
 */

void gestor_flights_foreach(GestorFlights *g, void (*func)(Voo *, void *), void *user_data);

/* ============================================
 * CONSULTAS 
 * ============================================ */

/** 
 * @brief Obtém o código IATA de origem de um voo. 
 * 
 * @param g Ponteiro para o gestor. 
 * @param flight_id Identificador do voo. 
 * @return Código IATA de origem ou NULL se não existir. 
 */

const char *gestor_flights_obter_origem(GestorFlights *g, const char *flight_id);

/**
 * @brief Obtém o código IATA de destino de um voo.
 *
 * @param g Ponteiro para o gestor.
 * @param flight_id Identificador do voo.
 * @return Código IATA de destino ou NULL se não existir.
 */

const char *gestor_flights_obter_destino(GestorFlights *g, const char *flight_id);

/* ============================================
 * ARRAY ORDENADO (Q3)
 * ============================================ */

/** 
 * @brief Define o array ordenado de voos. 
 * 
 * @param g Ponteiro para o gestor. 
 * @param array Array de voos ordenado. 
 * @param num_voos Número de voos no array. 
 */

void gestor_flights_set_array_ordenado(GestorFlights *g, Voo **array, int num_voos);

/**
 * @brief Obtém o array ordenado de voos.
 *
 * @param g Ponteiro para o gestor.
 * @param num_voos Ponteiro para receber o número de voos.
 * @return Array ordenado ou NULL.
 */

Voo **gestor_flights_get_array_ordenado(GestorFlights *g, int *num_voos);

/* ============================================
 * CONTAGENS DE AIRCRAFT (Q2) 
 * ============================================ */

typedef struct _GHashTable GHashTable;

/**
 * @brief Define a tabela de contagens por aircraft_id.
 *
 * @param g Ponteiro para o gestor.
 * @param contagens Hash table com contagens.
 */

void gestor_flights_set_contagens_aircraft(GestorFlights *g, GHashTable *contagens);

/**
 * @brief Obtém a tabela de contagens por aircraft_id.
 *
 * @param g Ponteiro para o gestor.
 * @return Hash table ou NULL.
 */

GHashTable *gestor_flights_get_contagens_aircraft(GestorFlights *g);

/* ============================================
 * PROCURA
 * ============================================ */

/** 
 * @brief Procura um voo pelo seu identificador. 
 * 
 * @param gf Ponteiro para o gestor. 
 * @param flight_id Identificador do voo. 
 * @return Ponteiro para o voo ou NULL se não existir. 
 */

Voo *gestor_flights_procura(GestorFlights *gf, const char *flight_id);

#endif
