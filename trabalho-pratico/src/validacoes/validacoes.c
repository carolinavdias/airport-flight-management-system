#define _GNU_SOURCE
#define _XOPEN_SOURCE 700

#include "validacoes.h"
#include "flights.h"
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>

//retorna o número de dias do mês (considerando ano bissexto)
//ano e mes devem ser válidos
int dias_no_mes(int ano, int mes) {
    if (mes < 1 || mes > 12) return 0;

    // Meses com 31 dias
    if (mes == 1 || mes == 3 || mes == 5 || mes == 7 ||
        mes == 8 || mes == 10 || mes == 12)
        return 31;

    // Meses com 30 dias
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11)
        return 30;

    // Fevereiro
    if (ano % 4 == 0) return 29;
    return 28;
}

int valida_converte_data(const char *s, time_t *out)
{
    if (!s || !out) return 0;

    const char *formatos[] = {
        "%Y-%m-%d",
        "%Y-%m-%d %H:%M",
        "%Y-%m-%d %H:%M:%S"
    };

    struct tm tm_ok;
    int encontrou = 0;

    for (int i = 0; i < 3; i++) {
        struct tm tmp = {0};
        char *end = strptime(s, formatos[i], &tmp);

        if (end && *end == '\0') {
            tm_ok = tmp;   
            encontrou = 1;
            break;
        }
    }

    if (!encontrou) return 0;

    time_t t = mktime(&tm_ok);
    if (t == (time_t)-1) return 0;

    struct tm check = *localtime(&t);

    if (tm_ok.tm_year != check.tm_year ||
        tm_ok.tm_mon  != check.tm_mon  ||
        tm_ok.tm_mday != check.tm_mday ||
        tm_ok.tm_hour != check.tm_hour ||
        tm_ok.tm_min  != check.tm_min  ||
        tm_ok.tm_sec  != check.tm_sec)
        return 0;

    *out = t;
    return 1;
}

//converte DataH em time_t
time_t fast_convert(const DataH *d) {
    struct tm tm = {0};
    tm.tm_year = d->data.ano - 1900;
    tm.tm_mon  = d->data.mes - 1;
    tm.tm_mday = d->data.dia;
    tm.tm_hour = d->horas.hora;
    tm.tm_min  = d->horas.mins;
    tm.tm_sec  = 0;

    return mktime(&tm);
}