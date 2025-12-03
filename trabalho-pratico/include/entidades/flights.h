#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <stdbool.h>

//typedef uint8_t Estado;
/*
#define ESTADO_ON_TIME 0
#define ESTADO_DELAYED 1
#define ESTADO_CANCELLED 2
*/

typedef struct data_def {
    int ano, mes, dia;
} Data;

typedef struct hora_def {
    int hora, mins;
} Hora;

typedef struct dataH {
    Data data;
    Hora horas;
} DataH;

typedef enum {
    ESTADO_SCHEDULED,
    ESTADO_DELAYED,
    ESTADO_CANCELLED,
    ESTADO_ERROR
} Estado;

typedef struct voo {
    char *flight_id; //voo_id
    int departure; //partida_prevista
    int actual_departure; //partida_efetiva
    int arrival; //chegada prevista
    int actual_arrival; //chegada efetiva
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