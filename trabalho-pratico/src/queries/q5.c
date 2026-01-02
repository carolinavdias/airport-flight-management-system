#include "entidades/flights.h"
#include "utils/utils.h"
#include "queries/q5.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
//Função auxiliar adiciona um voo ao array a ser usado na Q5
int adiciona_voo_para_Q5 (ContagemQ5 **arr,int n, char *hora_efetiva, char *hora_prevista, char *airline) {
    int i = 0;
    for (; i < n; i++) {
        if (strcmp((*arr)[i].airline,airline) == 0) {
            float d = (*arr)[i].delay_avg;
            int current_count = (*arr)[i].delay_count;
            int new_delay = get_time_datah(hora_efetiva) - get_time_datah(hora_prevista); // to float
            (*arr)[i].delay_avg = (d * current_count + new_delay) / (current_count + 1);
            (*arr)[i].delay_count++;
            return n;
        }
    }
    ContagemQ5 *tmp = realloc(*arr,(n+1) * sizeof(ContagemQ5));
    if (!tmp) {
        return -1;
    }
    *arr = tmp;

    int new_delay = get_time_datah(hora_efetiva) - get_time_datah(hora_prevista);
    (*arr)->airline = strdup(airline);
    (*arr)->delay_avg = new_delay;
    (*arr)->delay_count = 1;

    return n + 1;
}

int compara_delay_dec(const void *a, const void *b) {
    const ContagemQ5 *x = (const ContagemQ5 *)a;
    const ContagemQ5 *y = (const ContagemQ5 *)b;

    if (x->delay_avg < y->delay_avg) return 1;
    if (x->delay_avg > y->delay_avg) return -1;

    //Caso 0: ordenar por ordem alfabetica
    if (strcmp(x->airline,y->airline) < 0) return 1;
    else return -1; //Não há duas linhas com airlines iguais
}


char *query5(const char *linhaComando, GestorFlights *gestorVoos) {

    int N;
    int arg = sscanf(linhaComando, "%d", &N);
    if (arg < 1 || N <= 0) {
        return strdup("\n");
    }

    ComtagemQ5 *lista = get_listaQ5(gestorVoos);

    qsort (lista, S, sizeof(ContagemQ5 *),compara_delay_dec);

    size_t buffer_size = 4096;
    char *output = malloc(buffer_size);
    if (!output) {
        return strdup("\n");
    }

    output[0] = '\0';
    size_t current_pos = 0;

    int printed = 0;

    for (int i = 0; i < S && printed < N; i++, printed++) {
        ContagemQ5 *c = lista[i];

        char linha[512];
        int len = snprintf(linha, sizeof(linha), "%s=%.3f=%d\n",
                       c->airline, c->delay_avg, c->delay_count);

        if (current_pos + len + 1 > buffer_size) {
            buffer_size *= 2;
            char *new_output = realloc(output, buffer_size);
            if (!new_output) {
                free(output);

                return strdup("\n");
            }
            output = new_output;
        }

        memcpy(output + current_pos, linha, len);
        current_pos += len;
   }

    if (printed == 0) {
        free(output);
        return strdup("\n");
    }

    output[current_pos] = '\0';
    return output;

}
*/
