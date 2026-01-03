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
/**
 * @brief Obtém o número de voos de um aircraft específico.
 *
 * @param g Ponteiro para o gestor.
 * @param aircraft_id Identificador do aircraft.
 * @return Número de voos ou 0 se não encontrado.
 */
int gestor_flights_get_contagem_aircraft(GestorFlights *g, const char *aircraft_id);

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

/**
 * @brief Estrutura para dados de atraso por airline.
 */
typedef struct dadosAtrasoQ5 DadosAtrasoQ5;

/**
 * @brief Inicializa o cache Q5 para atrasos por airline.
 * @param g Ponteiro para o gestor.
 */
void gestor_flights_init_cache_q5(GestorFlights *g);

/**
 * @brief Adiciona atraso ao cache Q5.
 * @param g Ponteiro para o gestor.
 * @param airline Nome da companhia aerea.
 * @param delay Atraso em minutos.
 */
void gestor_flights_add_atraso_q5(GestorFlights *g, const char *airline, int delay);

/**
 * @brief Obtem dados de atraso de uma airline.
 * @param g Ponteiro para o gestor.
 * @param airline Nome da companhia.
 * @return Ponteiro para DadosAtrasoQ5 ou NULL.
 */
DadosAtrasoQ5 *gestor_flights_get_atraso_q5(GestorFlights *g, const char *airline);

/**
 * @brief Itera sobre todas as airlines no cache Q5.
 * @param g Ponteiro para o gestor.
 * @param func Callback para cada airline.
 * @param user_data Dados do utilizador.
 */
typedef void (*AirlineIterFunc)(const char *airline, long total_delay, int count, void *user_data);
void gestor_flights_foreach_q5(GestorFlights *g, AirlineIterFunc func, void *user_data);

#endif
