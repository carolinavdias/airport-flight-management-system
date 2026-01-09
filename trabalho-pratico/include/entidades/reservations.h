#ifndef RESERVATIONS_H
#define RESERVATIONS_H

/** Forward declaration */

typedef struct _StringPool StringPool;

/** 
    * @file reservations.h 
    * @brief Interface da entidade Reservas. 
    * 
    * Este módulo define as estruturas abstratas `Reservas` e `Voos_reservados`, 
    * bem como as funções de criação, acesso, modificação e libertação de reservas 
    * lidas do ficheiro `reservations.csv`. 
    */

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/**
 * @struct voos_reservados
 * @brief Estrutura que representa a lista de voos reservados.
 */

typedef struct voos_reservados Voos_reservados;

/**
 * @struct reservas
 * @brief Estrutura que representa uma reserva.
 */

typedef struct reservas Reservas;

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * @brief Obtém o ID da reserva.
 *
 * Retorna um ponteiro constante para o ID interno da reserva.
 * A memória é gerida pelo StringPool, pelo que o utilizador não deve
 * libertar esta string.
 *
 * @param r Ponteiro para a reserva.
 * @return Ponteiro para o ID ou NULL se inválido.
 */

const char *r_get_id_reserva (const Reservas *r);

/**
 * @brief Obtém o ID numérico da pessoa que fez a reserva.
 *
 * @param r Ponteiro para a reserva.
 * @return ID da pessoa (int).
 */

int r_get_id_pessoa_reservou (const Reservas *r);  

/**
 * @brief Obtém o número de voos associados à reserva.
 *
 * @param r Ponteiro para a reserva.
 * @return Número de voos (>=0).
 */

int r_get_lista_n_voos (const Reservas *r);

/**
 * @brief Obtém a lista interna de IDs de voos.
 *
 * Retorna um ponteiro constante para o array interno de strings.
 * As strings são geridas pelo StringPool e não devem ser libertadas.
 *
 * @param r Ponteiro para a reserva.
 * @return Array interno de strings ou NULL se inválido.
 */

const char **r_get_lista_voos_reserv (const Reservas *r);

/**
 * @brief Obtém o preço total da reserva.
 *
 * @param r Ponteiro para a reserva.
 * @return Preço da reserva (double).
 */

double r_get_preco(const Reservas *r);

/**
 * @brief Obtém o ID de um voo da reserva por índice.
 *
 * Retorna uma **cópia** do ID do voo, garantindo encapsulamento.
 * O utilizador deve libertar a string retornada com `g_free()`.
 *
 * @param r Ponteiro para a reserva.
 * @param indice Índice do voo (0 <= indice < número de voos).
 * @return Cópia do ID do voo ou NULL se o índice for inválido.
 */

char *r_get_voo_por_indice(const Reservas *r, int indice);

/* ============================================
 * SETTERS 
 * ============================================ */

/* ============================================
 * USADO PARA SET
 * ============================================ */

/**
 * @brief Cria uma estrutura de lista de voos com espaço para n voos.
 * @param n Número de voos.
 * @return Estrutura alocada dinamicamente.
 */

Voos_reservados *cria0_lista_reserva (int n);

/**
 * @brief Liberta toda a memória associada a uma lista de voos reservados.
 * @param data Ponteiro genérico para Voos_reservados.
 */

void liberta_lista_reserva(void *data);

/**
 * @brief Define o ID de um voo na lista interna.
 * @param vr Estrutura de voos reservados.
 * @param i Índice.
 * @param s ID do voo.
 */

void set_lista_voos (Voos_reservados *vr, int i, char *s, StringPool *pool);

/**
 * @brief Substitui a lista de voos da reserva.
 * @param r Ponteiro para a reserva.
 * @param novo Nova estrutura de voos reservados.
 */

void r_set_lista (Reservas *r, Voos_reservados *novo);

/**
 * @brief Define o ID da reserva.
 * @param r Ponteiro para a reserva.
 * @param s Novo ID.
 */

void r_set_id_reserva (Reservas *r, char *s, StringPool *pool);

 /** 
    * @brief Define o ID da pessoa que fez a reserva. 
    * @param r Ponteiro para a reserva. 
    * @param s String numérica. 
    */

void r_set_id_pessoa_reservou (Reservas *r, char *s);

 /** 
    * @brief Define o preço da reserva. 
    * @param r Ponteiro para a reserva.
    * @param s String numérica. 
    */

void r_set_preco (Reservas *r, char *s);

/* ============================================
 * CRIA RESERVA
 * ============================================ */

/**
 * @brief Cria uma nova reserva vazia.
 * @return Estrutura alocada dinamicamente.
 */

Reservas *criaReserva ();

/* ============================================
 * DESTRÓI RESERVA
 * ============================================ */

/**
 * @brief Liberta toda a memória associada a uma reserva.
 *
 * Compatível com gestores de memória().
 *
 * @param data Ponteiro genérico para Reservas.
 */

void libertaReserva(void *data);

#endif
