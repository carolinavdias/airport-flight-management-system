#ifndef Q5_H
#define Q5_H

#include "gestor_entidades/gestor_flights.h"

typedef struct contagemQ5 ContagemQ5;
typedef struct estrt_aux_q5 Estrt_aux_q5;

int compara_delay_dec(const void *a, const void *b);

//adiciona voo para array durante o parser
int adiciona_voo_para_Q5 (ContagemQ5 **arr,int n, char *hora_efetiva, char *hora_prevista, char *airline);

//query 2
char *query5(const char *linhaComando, GestorFlights *gestorVoos);

#endif
