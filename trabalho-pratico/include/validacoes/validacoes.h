#ifndef VALIDACOES_H
#define VALIDACOES_H

#include "flights.h"
#include <stdbool.h>
#include <time.h>

int dias_no_mes(int ano, int mes);
int valida_converte_data(const char *s, time_t *out);
time_t fast_convert(const DataH *d);

#endif