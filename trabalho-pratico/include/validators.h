#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <stdbool.h>
#include <time.h>

//datas: aceita "YYYY-MM-DD" e "YYYY-MM-DD hh:mm" (opcionalmente ":ss")
bool v_parse_date(const char *s, struct tm *out);

//IATA e tipos de aeroporto
bool v_is_iata3(const char *s);                // [A-Z]{3}
bool v_is_airport_type(const char *s);         // small_airport, medium_airport, large_airport, heliport, seaplane_base

//coordenadas
bool v_is_lat(const char *s);                  // [-90, 90]
bool v_is_lng(const char *s);                  // [-180, 180]

//identificadores
bool v_is_flight_id(const char *s);            // [A-Z]{2}[0-9]{5}
bool v_is_reservation_id(const char *s);       // R[0-9]{9}
bool v_is_document_number(const char *s);      // 9 dígitos

//email e género
bool v_is_email(const char *s);
bool v_is_gender(const char *s);               // M / F / O

//estado de voo
bool v_is_status_valid(const char *s);         // Scheduled, Delayed, Cancelled

#endif
