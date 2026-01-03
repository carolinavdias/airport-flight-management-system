#include "gestor_entidades/gestor.h"
#include <stdlib.h>

/* ============================================
 * ESTRUTURA 
 * ============================================ */

/** 
 * Estrutura interna do gestor geral.
 * Contém ponteiros para todos os gestores de entidades do sistema.
 */

struct gestor_geral {
    GestorAircrafts *aircrafts;
    GestorAirports *airports;
    GestorFlights *flights;
    GestorPassengers *passengers;
    GestorReservations *reservations;
};

/** 
 * Cria o gestor geral e inicializa todos os gestores internos.
 * Cada gestor é responsável por uma entidade específica.
 */

GestorGeral *gestor_geral_cria(void) {
    GestorGeral *g = malloc(sizeof(GestorGeral));
    if (!g) return NULL;

    g->aircrafts    = gestor_aircrafts_cria();
    g->airports     = gestor_airports_cria();
    g->flights      = gestor_flights_novo();
    g->passengers   = gestor_passengers_novo();
    g->reservations = gestor_reservations_cria();

    return g;
}

/**
 * Liberta o gestor geral.
 * Primeiro liberta todos os gestores internos
 * e depois liberta a própria estrutura.
 */

void gestor_geral_liberta(GestorGeral *g) {
    if (!g) return;

    gestor_aircrafts_liberta(g->aircrafts);
    gestor_airports_liberta(g->airports);
    gestor_flights_destroy(g->flights);
    gestor_passengers_destroy(g->passengers);
    gestor_reservations_liberta(g->reservations);

    free(g);
}

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * Devolve o gestor de aeronaves associado ao gestor geral.
 */

GestorAircrafts *gestor_geral_aircrafts(GestorGeral *g) {
    return g ? g->aircrafts : NULL;
}

/**
 * Devolve o gestor de aeroportos associado ao gestor geral.
 */

GestorAirports *gestor_geral_airports(GestorGeral *g) {
    return g ? g->airports : NULL;
}

/**
 * Devolve o gestor de voos associado ao gestor geral.
 */

GestorFlights *gestor_geral_flights(GestorGeral *g) {
    return g ? g->flights : NULL;
}

/**
 * Devolve o gestor de passageiros associado ao gestor geral.
 */

GestorPassengers *gestor_geral_passengers(GestorGeral *g) {
    return g ? g->passengers : NULL;
}

/**
 * Devolve o gestor de reservas associado ao gestor geral.
 */

GestorReservations *gestor_geral_reservations(GestorGeral *g) {
    return g ? g->reservations : NULL;
}
