#define _POSIX_C_SOURCE 200809L
#include "parsers/parser_flights.h"
#include "validacoes/validacoes_flights.h"
#include "validacoes/validacoes_airports.h"
#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>

#define MAX_LINHA 20000
static char buffer[MAX_LINHA];

//função auxiliar para libertar strings de voo parcialmente preenchido
static void limpar_voo_parcial(Voo *v) {
    if (!v) return;
    if (v->flight_id) g_free(v->flight_id);
    if (v->gate) g_free(v->gate);
    if (v->code_origin) g_free(v->code_origin);
    if (v->code_destination) g_free(v->code_destination);
    if (v->id_aircraft) g_free(v->id_aircraft);
    if (v->airline) g_free(v->airline);
    if (v->tracking_url) g_free(v->tracking_url);
    free(v);
}

int le_tabela_Voos(Contexto ctx, GestorFlights *V, GestorAircrafts *AC) {

    FILE *ficheiro = abrir_ficheiro(&ctx, "flights.csv", "r");
    if (ficheiro == NULL) return 0;

    int no_header = 1;
    char header[MAX_LINHA];

    if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
    else {
        buffer[strcspn(buffer,"\n")] = '\0';
        strcpy(header, buffer);
    }

    FILE *ficheiro_erros = NULL;
    int header_escrito = 0;

    while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
        Voo *voo_atual = calloc(1, sizeof(Voo));
        int linha_valida = 1;
        int e_maybe = -1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

        if (linha_valida && !valida_id_voo(campos[0], &voo_atual->flight_id)) linha_valida = 0;
        if (linha_valida && !valida_DataH(campos[1], &voo_atual->departure)) linha_valida = 0;
        if (linha_valida) {
	    if (strcmp(campos[2], "N/A") == 0) {
                 e_maybe = 2;
                 voo_atual->actual_departure = -2;
            } else if (!valida_DataH(campos[2], &voo_atual->actual_departure)) linha_valida = 0;
        }
        if (linha_valida && !valida_DataH(campos[3], &voo_atual->arrival)) linha_valida = 0;
        if (linha_valida) {
            if (e_maybe == 2) {
                 if (strcmp(campos[4], "N/A") != 0) linha_valida = 0;
                 else voo_atual->actual_arrival = -2;
            } else if (!valida_DataH(campos[4], &voo_atual->actual_arrival)) linha_valida = 0;
        }
        if (linha_valida) voo_atual->gate = g_strdup(campos[5]);
        if (linha_valida) {
           if (!valida_Estado(campos[6], &voo_atual->status)) linha_valida = 0;
           if (voo_atual->status == 2 && e_maybe != 2) linha_valida = 0;
        }
        if (linha_valida && !valida_codigoIATA(campos[7], &voo_atual->code_origin)) linha_valida = 0;
        if (linha_valida && !valida_codigoIATA(campos[8], &voo_atual->code_destination)) linha_valida = 0;
        if (linha_valida) voo_atual->id_aircraft = g_strdup(campos[9]);
        if (linha_valida) voo_atual->airline = g_strdup(campos[10]);
        if (linha_valida) voo_atual->tracking_url = g_strdup(campos[11]);

        //validação lógica
        if (linha_valida && !valida_VOO(*voo_atual, AC)) linha_valida = 0;

        if (!linha_valida) {
            if (!ficheiro_erros) {
                ficheiro_erros = fopen("resultados/flights_errors.csv", "w");
                if (ficheiro_erros && !header_escrito) {
                    fputs(header, ficheiro_erros);
                    fputc('\n', ficheiro_erros);
                    header_escrito = 1;
                }
            }
            if (ficheiro_erros) {
                fputs(buffer, ficheiro_erros);
                fputc('\n', ficheiro_erros);
            }
            limpar_voo_parcial(voo_atual);
        } else {
	    gestor_flights_inserir(V, voo_atual);
        }
        if (campos) csv_free_fields(campos, n_campos);
    }

    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}