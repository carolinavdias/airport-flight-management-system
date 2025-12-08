#define _POSIX_C_SOURCE 200809L
#include "parsers/parser_reservations.h"
#include "validacoes/validacoes_reservations.h"
#include "validacoes/validacoes_passengers.h"
#include "validacoes/validacoes_flights.h"
#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>

#define MAX_LINHA 20000
static char buffer[MAX_LINHA];

int le_tabela_Reservas(Contexto ctx, GestorFlights *V, GestorPassengers *P, GestorReservations *R) {

    FILE *ficheiro = abrir_ficheiro(&ctx, "reservations.csv", "r");
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
        Reservas *reserva_atual = calloc(1, sizeof(Reservas));

        int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

        if (linha_valida && !valida_id_reserva(campos[0], &reserva_atual->id_reserva)) linha_valida = 0;
        if (linha_valida && !valida_voos_reservados(campos[1], &reserva_atual->reserva_lista)) linha_valida = 0;
        if (linha_valida) reserva_atual->id_pessoa_reservou = atoi(campos[2]);
        if (linha_valida) reserva_atual->lugar_reservado = g_strdup(campos[3]);
        if (linha_valida) reserva_atual->preco_reserva = atof(campos[4]);
        if (linha_valida && !valida_bool(campos[5], &reserva_atual->bagagem_extra)) linha_valida = 0;
        if (linha_valida && !valida_bool(campos[6], &reserva_atual->prioridade)) linha_valida = 0;
        if (linha_valida) reserva_atual->qr_code = g_strdup(campos[7]);

        //validação lógica (usa gestores V e P)
        if (linha_valida && !valida_RESERVA(*reserva_atual, V, P)) linha_valida = 0;

        if (!linha_valida) {
             if (!ficheiro_erros) {
                 ficheiro_erros = fopen("resultados/reservations_errors.csv", "w");
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
        libertaReserva(reserva_atual);
        } else {
            gestor_reservations_insere(R, reserva_atual);
        }
        if (campos) csv_free_fields(campos, n_campos);
    }
    
    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}