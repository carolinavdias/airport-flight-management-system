#ifndef FLIGHTS_H
#define FLIGHTS_H

#include <stdint.h>

typedef uint8_t Estado;

#define ESTADO_ON_TIME 0
#define ESTADO_DELAYED 1
#define ESTADO_CANCELLED 2

typedef struct voo Voo;

char *voo_get_flight_id (const Voo *v);
const char *voo_get_code_origin (const Voo *v);
const char *voo_get_code_destination (const Voo *v);
long long voo_get_arrival (const Voo *v);
long long voo_get_actual_arrival (const Voo *v);
const char *voo_get_id_aircraft (const Voo *v);
Estado voo_get_status (const Voo *v);
long long voo_get_departure (const Voo *v);
long long voo_get_actual_departure (const Voo *v);

void voo_set_status (Voo *v, char *status);
void voo_set_code (Voo *v, char *code, char versao);
void voo_set_flight_id (Voo *v, char *id);
//void voo_set_gate (Voo *v, char *campo_gate);
void voo_set_airline (Voo *v, char *airl);
//void voo_set_tracking_url (Voo *v, char *url);
void voo_set_id_aircraft (Voo *v, char *id_airc);

long long converte_dataH (const char *s);
void voo_set_dataH (Voo *v, const char *s, int campo);

Voo *criaVoo ();
void libertaVoo(void *data);

#endif
