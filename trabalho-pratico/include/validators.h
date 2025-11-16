#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <stdbool.h>
#include <time.h>
#include "q1.h"
#include "q3.h"
#include "read.h"

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





bool v_parse_date2(const char *s, time_t *out);

int valida_id_voo(char *s, char **voo_id);

int valida_id_voo2 (char* string, char **voo_id);
int qual_mes (int ano, int mes);
int qual_mes2(int mes);
time_t parseDate_(const char *dateStr) ;
time_t fast_convert(const DataH *d);

int valida_DataH(const char *s, int *out);

int valida_DataH9(const char *s, time_t *out);
int valida_DataH3 (char *string, time_t *datah); // com validação incluida
int valida_Data (char *string, Data *data);
int valida_Estado (const char *string, Estado *e);

int valida_codigoIATA (char* string, char **codigo_IATA); // funcao traducao e validacao token para codigo_IATA_aeroporto

int valida_coordenadas (const char* string, int versao, double *coordenada);
int valida_tipo_aer(const char *string, Tipo_aeroporto *t);
int valida_year (char *string, int *year);
int valida_id_passageiro(char* string, int *id_passageiro);
int valida_genero (const char* string, Genero *g);
int valida_email(char *string, char **email);
int valida_id_reserva (char* string, char **id_reserva);
int valida_voos_reservados(char *string, Voos_reservados *lista); //char ***lista
int valida_bool (char *string, bool *b);

int compara_dataH (char *datah1, char *datah2); //se 1 for não anterior a 2, entao retorna 1 else retorna 0
int valida_VOO (Voo voo, GHashTable *tabela);
int valida_RESERVA (Reservas reserva, GHashTable *tabela_v, GHashTable *tabela_p);















#endif
