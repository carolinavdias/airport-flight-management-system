#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t Estado;

#define ESTADO_ON_TIME 0
#define ESTADO_DELAYED 1
#define ESTADO_CANCELLED 2

typedef struct voo {
    char *flight_id; //voo_id
    long long departure; //partida_prevista
    long long actual_departure; //partida_efetiva
    long long arrival; //chegada prevista
    long long actual_arrival; //chegada efetiva
    char *gate; //porta_embarque
    Estado status; //++ Estado
    char *code_origin; //codigo IATA origem
    char *code_destination; //codigo IATA destino
    char *id_aircraft; //id_aeronave
    char *airline; //companhia aerea
    char *tracking_url;
} Voo;

void libertaVoo(void *data);

#endif