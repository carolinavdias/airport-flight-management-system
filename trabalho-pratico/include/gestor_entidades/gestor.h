#ifndef GESTOR_H
#define GESTOR_H

#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * @file gestor.h
 * @brief Gestor geral que agrega todos os gestores de entidades.
 *
 * Este módulo define um gestor central responsável por agregar
 * todos os gestores de entidades do sistema, nomeadamente:
 * aeronaves, aeroportos, voos, passageiros e reservas.
 *
 * O gestor geral permite que as queries e outros módulos
 * acedam a todas as entidades através de um único ponto de acesso.
 */

/**
 * @struct gestor_geral
 * @brief Estrutura que agrega todos os gestores de entidades.
 *
 * Esta estrutura contém ponteiros para todos os gestores
 * individuais utilizados no sistema.
 */

typedef struct gestor_geral GestorGeral;

/**
 * @brief Cria e inicializa o gestor geral.
 *
 * Aloca memória para o gestor geral e cria todos os gestores
 * internos associados às diferentes entidades.
 *
 * @return Ponteiro para o gestor geral criado ou NULL em caso de erro.
 */

GestorGeral *gestor_geral_cria(void);

/**
 * @brief Liberta toda a memória associada ao gestor geral.
 *
 * Liberta todos os gestores internos agregados e,
 * por fim, liberta a própria estrutura do gestor geral.
 *
 * @param g Ponteiro para o gestor geral.
 */

void gestor_geral_liberta(GestorGeral *g);

/* ============================================
 * GETTERS DOS GESTORES INTERNOS
 * ============================================ */

/**
 * @brief Obtém o gestor de aeronaves.
 *
 * @param g Ponteiro para o gestor geral.
 * @return Ponteiro para o gestor de aeronaves.
 */

GestorAircrafts *gestor_geral_aircrafts(GestorGeral *g);

/**
 * @brief Obtém o gestor de aeroportos.
 *
 * @param g Ponteiro para o gestor geral.
 * @return Ponteiro para o gestor de aeroportos.
 */

GestorAirports *gestor_geral_airports(GestorGeral *g);

/**
 * @brief Obtém o gestor de voos.
 *
 * @param g Ponteiro para o gestor geral.
 * @return Ponteiro para o gestor de voos.
 */

GestorFlights *gestor_geral_flights(GestorGeral *g);

/**
 * @brief Obtém o gestor de passageiros.
 *
 * @param g Ponteiro para o gestor geral.
 * @return Ponteiro para o gestor de passageiros.
 */

GestorPassengers *gestor_geral_passengers(GestorGeral *g);

/**
 * @brief Obtém o gestor de reservas.
 *
 * @param g Ponteiro para o gestor geral.
 * @return Ponteiro para o gestor de reservas.
 */

GestorReservations *gestor_geral_reservations(GestorGeral *g);

#endif
