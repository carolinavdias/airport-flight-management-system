#ifndef GESTOR_PASSENGERS_H
#define GESTOR_PASSENGERS_H

#include <stdbool.h>
#include "entidades/passengers.h"

/**
 * @file gestor_passengers.h
 * @brief Interface do gestor de passageiros.
 *
 * Este módulo define o gestor responsável por armazenar e gerir todos os
 * passageiros do sistema.  
 *
 * O gestor utiliza uma hash table para indexar passageiros pelo seu
 * identificador, permitindo inserção, pesquisa, verificação de existência,
 * contagem de passageiros por voo e iteração sobre todos os passageiros.
 */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @struct gestor_passengers
 * @brief Estrutura interna do gestor de passageiros.
 * Estrutura que representa o gestor de passageiros.
 * A implementação interna utiliza hash tables.
 */

typedef struct gestor_passengers GestorPassengers;

/* ============================================
 * CRIA GESTOR DE PASSAGEIRO
 * ============================================ */

/** 
 * @brief Cria um novo gestor de passageiros. 
 * 
 * Inicializa a hash table interna responsável por armazenar os passageiros. 
 * 
 * @return Ponteiro para o gestor criado, ou NULL em caso de erro. 
 */

GestorPassengers *gestor_passengers_novo();

/* ============================================
 * DESTRÓI GESTOR DE PASSAGEIRO
 * ============================================ */

/**
 * @brief Liberta toda a memória associada ao gestor de passageiros.
 *
 * Liberta todos os passageiros armazenados e a própria estrutura do gestor.
 *
 * @param g Ponteiro para o gestor.
 */

void gestor_passengers_destroy(GestorPassengers *g);

/* ============================================
 * INSERÇÃO 
 * ============================================ */

/** 
 * @brief Insere um passageiro no gestor. 
 * 
 * O passageiro é indexado pelo seu identificador único. 
 * 
 * @param g Ponteiro para o gestor. 
 * @param p Ponteiro para o passageiro a inserir. 
 */

void gestor_passengers_inserir(GestorPassengers *g, Passageiros *p);

/* ============================================
 * CONSULTAS
 * ============================================ */

/**
 * @brief Verifica se um passageiro existe no gestor.
 *
 * @param g Ponteiro para o gestor.
 * @param id_passageiro Identificador do passageiro.
 * @return true se existir, false caso contrário.
 */

bool gestor_passengers_existe(GestorPassengers *g, const char *id_passageiro);

/**
 * @brief Conta quantos passageiros estão associados a um determinado voo.
 *
 * Esta função percorre todos os passageiros e compara o ID do voo associado.
 *
 * @param g Ponteiro para o gestor.
 * @param flight_id Identificador do voo.
 * @return Número de passageiros associados ao voo.
 */

int gestor_passengers_conta_por_voo(GestorPassengers *g, const char *flight_id);

/**
 * @brief Procura um passageiro pelo seu identificador.
 *
 * @param g Ponteiro para o gestor.
 * @param doc_number Identificador do passageiro.
 * @return Ponteiro para o passageiro ou NULL se não existir.
 */

Passageiros *gestor_passengers_procura(GestorPassengers *g, const char *doc_number);

/* ============================================
 * INTERAÇÃO
 * ============================================ */

/**
 * @brief Função callback usada na iteração de passageiros.
 *
 * @param p Ponteiro para o passageiro.
 * @param user_data Dados adicionais fornecidos pelo utilizador.
 */

typedef void (*PassengerIterFunc)(Passageiros *p, void *user_data);

/**
 * @brief Aplica uma função a todos os passageiros do gestor.
 *
 * @param gp Ponteiro para o gestor.
 * @param f Função callback a aplicar.
 * @param user_data Dados adicionais fornecidos ao callback.
 */

void gestor_passengers_foreach(GestorPassengers *gp, PassengerIterFunc f, void *user_data);

/**
 * @brief Inicializa o cache Q6 para contagem de destinos por nacionalidade.
 * @param gp Ponteiro para o gestor.
 */
void gestor_passengers_init_cache_q6(GestorPassengers *gp);

/**
 * @brief Adiciona contagem ao cache Q6.
 * @param gp Ponteiro para o gestor.
 * @param nacionalidade Nacionalidade do passageiro.
 * @param destino Codigo do aeroporto de destino.
 */
void gestor_passengers_add_destino_q6(GestorPassengers *gp, const char *nacionalidade, const char *destino);

/**
 * @brief Itera sobre destinos de uma nacionalidade.
 * @param gp Ponteiro para o gestor.
 * @param nacionalidade Nacionalidade a consultar.
 * @param func Callback para cada destino.
 * @param user_data Dados do utilizador.
 */
typedef void (*DestinoIterFunc)(const char *destino, int count, void *user_data);
void gestor_passengers_foreach_destinos_q6(GestorPassengers *gp, const char *nacionalidade, DestinoIterFunc func, void *user_data);

#endif

