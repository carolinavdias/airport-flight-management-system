#ifndef AIRPORTS_H
#define AIRPORTS_H

#include <stdint.h>

typedef uint8_t Tipo_aeroporto;

#define TIPO_SMALL_AIRPORT 0
#define TIPO_MEDIUM_AIRPORT 1
#define TIPO_LARGE_AIRPORT 2
#define TIPO_HELIPORT 3
#define TIPO_SEAPLANE_BASE 4

typedef struct aeroporto Aeroporto;

char *airport_get_code_IATA (Aeroporto *a);
char *airport_get_name (Aeroporto *a);
char *airport_get_city (Aeroporto *a);
char *airport_get_country (Aeroporto *a);
Tipo_aeroporto airport_get_type (Aeroporto *a);

void airport_set_name (Aeroporto *a, char *name_);
void airport_set_city (Aeroporto *a, char *city_);
void airport_set_country (Aeroporto *a, char *country_);
void airport_set_code_IATA (Aeroporto *a, char *code_);
void airport_set_type (Aeroporto *a, char *tipo);

Aeroporto *criaAeroporto ();
void libertaAeroporto(void *data);

#endif