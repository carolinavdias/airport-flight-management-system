#ifndef GESTOR_AIRCRAFTS_H
#define GESTOR_AIRCRAFTS_H

#include "entidades/aircrafts.h"

/**
 * @file gestor_aircrafts.h
 * @brief Gestor de aeronaves.
 *
 * Este módulo implementa um gestor responsável por armazenar,
 * aceder e percorrer todas as aeronaves lidas do ficheiro
 * aircrafts.csv, recorrendo a uma tabela de dispersão.
 */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @struct gestor_aircrafts
 * @brief Estrutura do gestor de aeronaves.
 *
 * Internamente, o gestor utiliza uma estrutura de dados onde
 * as aeronaves são indexadas pelo seu identificador.
 */

typedef struct gestor_aircrafts GestorAircrafts;

/* ============================================
 * GESTÃO DE AERONAVES
 * ============================================ */

/**
 * @brief Cria e inicializa um gestor de aeronaves.
 *
 * Aloca a estrutura do gestor e cria a tabela de dispersão
 * interna para armazenar as aeronaves.
 *
 * @return Ponteiro para o gestor criado ou NULL em caso de erro.
 */

GestorAircrafts *gestor_aircrafts_cria(void);

/**
 * @brief Insere uma aeronave no gestor.
 *
 * A aeronave é armazenada na tabela interna, indexada
 * pelo seu identificador.
 *
 * @param g Ponteiro para o gestor.
 * @param a Ponteiro para a aeronave a inserir.
 */

void gestor_aircrafts_insere(GestorAircrafts *g, Aeronave *a);

/**
 * @brief Procura uma aeronave pelo seu identificador.
 *
 * @param g Ponteiro para o gestor.
 * @param identifier Identificador da aeronave.
 * @return Ponteiro para a aeronave encontrada ou NULL se não existir.
 */

Aeronave *gestor_aircrafts_procura(GestorAircrafts *g, const char *identifier);

/**
 * @brief Verifica se uma aeronave existe no gestor.
 *
 * @param g Ponteiro para o gestor.
 * @param identifier Identificador da aeronave.
 * @return 1 se a aeronave existir, 0 caso contrário.
 */

int gestor_aircrafts_existe(GestorAircrafts *g, const char *identifier);

/**
 * @brief Função de iteração sobre aeronaves.
 *
 * Esta função é aplicada a cada aeronave armazenada
 * no gestor, permitindo realizar operações genéricas
 * como agregações ou estatísticas.
 *
 * @param a Ponteiro para a aeronave.
 * @param user_data Dados adicionais fornecidos à função.
 */

typedef void (*AircraftIterFunc)(Aeronave *a, void *user_data);

/**
 * @brief Aplica uma função a todas as aeronaves do gestor.
 *
 * Percorre todas as entradas da tabela interna e aplica
 * a função fornecida a cada aeronave.
 *
 * @param g Ponteiro para o gestor.
 * @param f Função a aplicar.
 * @param user_data Dados adicionais a passar à função.
 */

void gestor_aircrafts_foreach(GestorAircrafts *g, AircraftIterFunc f, void *user_data);

/* ============================================
 * DESTRÓI GESTOR DE AERONAVES 
 * ============================================ */

/**
 * @brief Liberta toda a memória associada ao gestor.
 *
 * Liberta a tabela interna, todas as aeronaves armazenadas
 * e a própria estrutura do gestor.
 *
 * @param g Ponteiro para o gestor.
 */

void gestor_aircrafts_liberta(GestorAircrafts *g);

#endif
