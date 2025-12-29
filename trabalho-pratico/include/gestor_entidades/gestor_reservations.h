#ifndef GESTOR_RESERVATIONS_H
#define GESTOR_RESERVATIONS_H

#include "entidades/reservations.h"

typedef struct gestor_reservations GestorReservations;

//cria o gestor
GestorReservations *gestor_reservations_cria(void);

//insere uma reserva
void gestor_reservations_insere(GestorReservations *g, Reservas *r);

//procura uma reserva
Reservas *gestor_reservations_procura(GestorReservations *g, const char *id);

//verifica se existe
int gestor_reservations_existe(GestorReservations *g, const char *id);

//REMOVIDO: GHashTable *gestor_reservations_table() - VIOLA ENCAPSULAMENTO

//procurar reservas por passageiro (retorna GSList* que deve ser libertada com g_slist_free)
typedef struct _GSList GSList;
GSList *gestor_reservations_get_by_passenger(GestorReservations *g, const char *doc_number);

//liberta tudo
void gestor_reservations_liberta(GestorReservations *g);

#endif