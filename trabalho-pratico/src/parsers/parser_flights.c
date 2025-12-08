#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include "parser_flights.h"
#include "flights.h"
#include "gestor_flights.h"
#include "validacoes/validacoes_flights.h"
#include "csv.h"



int le_tabela_Voos(int opcao_inserida, Contexto ctx, GestorFlights *V, GestorAircrafts *AC) {

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
        Voo *voo_atual = calloc(1, sizeof(Voo));  // ← CALLOC INICIALIZA TUDO A 0/NULL!
        int linha_valida = 1;
        int e_maybe = -1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = parse_csv_line(buffer);

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

        // Validação lógica
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
            limpar_voo_parcial(voo_atual);  // ← LIBERTA STRINGS PARCIAIS!
        } else {
            //g_hash_table_insert(tabela1, g_strdup(voo_atual->flight_id), voo_atual);
	    gestor_flights_inserir(V, voo_atual);
        }
        liberta_ifcampos(campos);//if (campos) csv_free_fields(campos, n_campos);
    }

    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}
