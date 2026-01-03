#include "gestor_entidades/gestor.h"
#include <stdlib.h>

struct gestor_geral {
    GestorAircrafts *aircrafts;
    GestorAirports *airports;
    GestorFlights *flights;
    GestorPassengers *passengers;
    GestorReservations *reservations;
};

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

void gestor_geral_liberta(GestorGeral *g) {
    if (!g) return;

    gestor_aircrafts_liberta(g->aircrafts);
    gestor_airports_liberta(g->airports);
    gestor_flights_destroy(g->flights);
    gestor_passengers_destroy(g->passengers);
    gestor_reservations_liberta(g->reservations);

    free(g);
}

// ===================================================
// GETTERS
// ===================================================

GestorAircrafts *gestor_geral_aircrafts(GestorGeral *g) {
    return g ? g->aircrafts : NULL;
}

GestorAirports *gestor_geral_airports(GestorGeral *g) {
    return g ? g->airports : NULL;
}

GestorFlights *gestor_geral_flights(GestorGeral *g) {
    return g ? g->flights : NULL;
}

GestorPassengers *gestor_geral_passengers(GestorGeral *g) {
    return g ? g->passengers : NULL;
}

GestorReservations *gestor_geral_reservations(GestorGeral *g) {
    return g ? g->reservations : NULL;
}
