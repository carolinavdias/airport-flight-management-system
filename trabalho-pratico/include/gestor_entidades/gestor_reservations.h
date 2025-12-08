#ifndef GESTOR_RESERVATIONS_H
#define GESTOR_RESERVATIONS_H

#include <glib.h>
#include "../entidades/reservations.h"

typedef struct {
    GHashTable *tabela;  //chave: id_reserva, valor: Reservas*
} GestorReservations;

//cria o gestor
GestorReservations *gestor_reservations_cria(void);

//insere uma reserva
void gestor_reservations_insere(GestorReservations *g, Reservas *r);

//procura uma reserva
Reservas *gestor_reservations_procura(GestorReservations *g, const char *id);

//verifica se existe
int gestor_reservations_existe(GestorReservations *g, const char *id);

//devolve a hash table
GHashTable *gestor_reservations_table(GestorReservations *g);

//liberta tudo
void gestor_reservations_liberta(GestorReservations *g);

#endif