#ifndef GESTOR_H
#define GESTOR_H

/**
 * @file gestor.h
 * @brief Gestor geral que agrega todos os gestores de entidades.
 *
 * Este módulo fornece uma estrutura centralizada que contém todos os
 * gestores individuais (aeronaves, aeroportos, voos, passageiros e reservas).
 *
 * O objetivo é permitir que as queries e restantes componentes do sistema
 * acedam a todas as entidades através de um único objeto.
 */

#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * @struct gestor_geral
 * @brief Estrutura que agrega todos os gestores de entidades.
 */
typedef struct gestor_geral GestorGeral;

/**
 * @brief Cria e inicializa o gestor geral.
 *
 * Aloca memória e cria todos os gestores internos.
 *
 * @return Ponteiro para GestorGeral ou NULL em caso de erro.
 */
GestorGeral *gestor_geral_cria(void);

/**
 * @brief Liberta toda a memória associada ao gestor geral.
 *
 * Liberta todos os gestores internos e, por fim, a própria estrutura.
 *
 * @param g Ponteiro para o gestor geral.
 */
void gestor_geral_liberta(GestorGeral *g);

/* ============================
 * GETTERS DOS GESTORES INTERNOS
 * ============================ */

/**
 * @brief Obtém o gestor de aeronaves.
 */
GestorAircrafts *gestor_geral_aircrafts(GestorGeral *g);

/**
 * @brief Obtém o gestor de aeroportos.
 */
GestorAirports *gestor_geral_airports(GestorGeral *g);

/**
 * @brief Obtém o gestor de voos.
 */
GestorFlights *gestor_geral_flights(GestorGeral *g);

/**
 * @brief Obtém o gestor de passageiros.
 */
GestorPassengers *gestor_geral_passengers(GestorGeral *g);

/**
 * @brief Obtém o gestor de reservas.
 */
GestorReservations *gestor_geral_reservations(GestorGeral *g);

#endif
