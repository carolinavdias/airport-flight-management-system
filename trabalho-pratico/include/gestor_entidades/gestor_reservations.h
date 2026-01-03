#ifndef GESTOR_RESERVATIONS_H
#define GESTOR_RESERVATIONS_H

#include "entidades/reservations.h"

/**
 * @file gestor_reservations.h
 * @brief Interface do gestor de reservas.
 *
 * Este módulo define o gestor responsável por armazenar e gerir todas as
 * reservas do sistema.  
 *
 * O gestor utiliza uma hash table para indexar reservas pelo seu ID,
 * permitindo inserção, pesquisa, verificação de existência e iteração.
 *
 * Inclui ainda uma função auxiliar para obter todas as reservas associadas
 * a um passageiro.
 */

// ===================================================
// ESTRUTURA 
// ===================================================

/**
 * @struct gestor_reservations
 * @brief Estrutura do gestor de reservas.
 *
 * Armazena todas as reservas numa hash table indexada pelo seu ID.
 * A tabela liberta automaticamente as chaves e as reservas armazenadas.
 */

typedef struct gestor_reservations GestorReservations;

// ============================================
// CRIA GESTOR DE RESERVAS
// ============================================ */

/**
 * @brief Cria um novo gestor de reservas.
 *
 * Inicializa a hash table interna responsável por armazenar as reservas.
 *
 * @return Ponteiro para o gestor criado, ou NULL em caso de erro.
 */

GestorReservations *gestor_reservations_cria(void);

// =================================================== 
// OPERAÇÕES BÁSICAS 
// =================================================== 
 
/**
 * @brief Insere uma reserva no gestor.
 *
 * A reserva é indexada pelo seu ID.  
 * A chave é duplicada internamente para garantir encapsulamento.
 *
 * @param g Ponteiro para o gestor.
 * @param r Ponteiro para a reserva a inserir.
 */

void gestor_reservations_insere(GestorReservations *g, Reservas *r);

/**
 * @brief Procura uma reserva pelo seu identificador.
 *
 * @param g Ponteiro para o gestor.
 * @param id Identificador da reserva.
 * @return Ponteiro para a reserva ou NULL se não existir.
 */

Reservas *gestor_reservations_procura(GestorReservations *g, const char *id);

/** 
 * @brief Verifica se uma reserva existe no gestor. 
 * 
 * @param g Ponteiro para o gestor. 
 * @param id Identificador da reserva. 
 * @return 1 se existir, 0 caso contrário. 
 */

int gestor_reservations_existe(GestorReservations *g, const char *id);

/**
 * @brief Obtém todas as reservas associadas a um passageiro.
 *
 * @warning Esta função viola encapsulamento ao devolver GSList*,
 * mas é mantida por compatibilidade com código existente.
 *
 * @param g Ponteiro para o gestor.
 * @param doc_number Identificador do passageiro.
 * @return Lista GSList* contendo Reservas* (não libertar elementos).
 */

typedef struct _GSList GSList;
GSList *gestor_reservations_get_by_passenger(GestorReservations *g, const char *doc_number);

/* ============================================
 * DESTRÓI GESTOR DE RESERVAS
 * ============================================ */

/**
 * @brief Liberta toda a memória associada ao gestor de reservas.
 *
 * Liberta todas as reservas armazenadas e a própria estrutura do gestor.
 *
 * @param g Ponteiro para o gestor.
 */

void gestor_reservations_liberta(GestorReservations *g);

/* ============================================
 * NOVA FUNÇÃO PARA FASE 2 (encapsulamento)
 * ============================================ */

/** 
 * @brief Tipo de função callback usada na iteração sobre reservas. 
 * 
 * @param r Ponteiro para a reserva atual. 
 * @param user_data Dados fornecidos pelo utilizador. 
 */

typedef void (*ReservationIterFunc)(Reservas *r, void *user_data);

/**
 * @brief Itera sobre todas as reservas do gestor.
 *
 * Esta função permite percorrer todas as reservas sem expor a estrutura
 * interna (GHashTable), respeitando o encapsulamento.
 *
 * @param g Ponteiro para o gestor.
 * @param f Função callback a aplicar a cada reserva.
 * @param user_data Dados adicionais fornecidos ao callback.
 */

void gestor_reservations_foreach(GestorReservations *g, ReservationIterFunc f, void *user_data);

/**
 * @brief Conta quantas reservas incluem um determinado voo.
 *
 * Percorre todas as reservas e verifica se o voo aparece na lista de voos
 * reservados.
 *
 * @param g Ponteiro para o gestor.
 * @param flight_id Identificador do voo.
 * @return Número de reservas que incluem o voo.
 */

int gestor_reservations_conta_por_voo(GestorReservations *g, const char *flight_id);

/* ============================================
 * FUNÇÕES PARA CACHE Q4 (OTIMIZAÇÃO)
 * ============================================ */

/**
 * @brief Inicializa o cache para a Q4.
 *
 * Cria a estrutura de dados interna para armazenar gastos semanais
 * por passageiro, permitindo resposta rápida à Q4.
 *
 * @param g Ponteiro para o gestor de reservas.
 */
void gestor_reservations_init_cache_q4(GestorReservations *g);

/**
 * @brief Adiciona um gasto ao cache Q4.
 *
 * @param g Ponteiro para o gestor.
 * @param id_semana Identificador da semana (dias desde época).
 * @param doc_number Documento do passageiro.
 * @param preco Preço a adicionar.
 */
void gestor_reservations_add_gasto_q4(GestorReservations *g, long id_semana, const char *doc_number, double preco);

/**
 * @brief Itera sobre os dados do cache Q4.
 *
 * @param g Ponteiro para o gestor.
 * @param func Função callback chamada para cada (semana, passageiro, gasto).
 * @param user_data Dados passados ao callback.
 */
typedef void (*CacheQ4IterFunc)(long id_semana, const char *doc_number, double gasto, void *user_data);
void gestor_reservations_foreach_cache_q4(GestorReservations *g, CacheQ4IterFunc func, void *user_data);

/**
 * @brief Finaliza o cache Q4 calculando top 10 de cada semana.
 * Deve ser chamado após carregar todas as reservas.
 *
 * @param g Ponteiro para o gestor.
 */
void gestor_reservations_finaliza_cache_q4(GestorReservations *g);

/**
 * @brief Itera sobre os top 10 pré-calculados.
 *
 * @param g Ponteiro para o gestor.
 * @param func Função callback chamada para cada (semana, doc_number no top10).
 * @param user_data Dados passados ao callback.
 */
typedef void (*Top10IterFunc)(long id_semana, const char *doc_number, void *user_data);
void gestor_reservations_foreach_top10(GestorReservations *g, Top10IterFunc func, void *user_data);

#endif

