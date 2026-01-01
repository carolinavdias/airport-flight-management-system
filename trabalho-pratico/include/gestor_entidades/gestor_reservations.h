#ifndef GESTOR_RESERVATIONS_H
#define GESTOR_RESERVATIONS_H

#include "entidades/reservations.h"

typedef struct gestor_reservations GestorReservations;

/* ============================================
 * FUNÇÕES EXISTENTES (sem alterações)
 * ============================================ */

//cria o gestor
GestorReservations *gestor_reservations_cria(void);

//insere uma reserva
void gestor_reservations_insere(GestorReservations *g, Reservas *r);

//procura uma reserva
Reservas *gestor_reservations_procura(GestorReservations *g, const char *id);

//verifica se existe
int gestor_reservations_existe(GestorReservations *g, const char *id);

//procurar reservas por passageiro (retorna GSList* que deve ser libertada com g_slist_free)
//NOTA: Esta função viola encapsulamento mas mantém-se para compatibilidade
typedef struct _GSList GSList;
GSList *gestor_reservations_get_by_passenger(GestorReservations *g, const char *doc_number);

//liberta tudo
void gestor_reservations_liberta(GestorReservations *g);

/* ============================================
 * NOVA FUNÇÃO PARA FASE 2 (encapsulamento)
 * ============================================ */

/**
 * @brief Tipo de função callback para iteração sobre reservas
 * @param r Ponteiro para a reserva atual
 * @param user_data Dados do utilizador passados ao foreach
 */
typedef void (*ReservationIterFunc)(Reservas *r, void *user_data);

/**
 * @brief Itera sobre todas as reservas, chamando a função callback para cada uma
 * 
 * Esta função permite iterar sobre todas as reservas sem expor a estrutura
 * interna (GHashTable), respeitando o encapsulamento.
 * 
 * @param g Ponteiro para o gestor de reservas
 * @param f Função callback a chamar para cada reserva
 * @param user_data Dados a passar para a função callback
 */
void gestor_reservations_foreach(GestorReservations *g, ReservationIterFunc f, void *user_data);

#endif
// Conta quantas reservas têm este voo
int gestor_reservations_conta_por_voo(GestorReservations *g, const char *flight_id);
