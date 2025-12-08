#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include "parser_reservations.h"
#include "reseravtions.h"
#include "gestor_reservations."
#include "gestor_passangers.h"
#include "gestor_flights.h"
#include "validacoes/validacoes_passangers.h"
#include "csv.h"


int le_tabela_Reservas(Contexto ctx, GestorVoos *V, GestorPassengers *P, GestorReservas *R) {


    FILE *ficheiro = abrir_ficheiro(&ctx, "reservations.csv", "r");
    if (ficheiro == NULL) return 0;
    char header[MAX_LINHA];

    no_header = 1;
    if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
    else {
        buffer[strcspn(buffer,"\n")] = '\0';
        strcpy(header, buffer);
    }

    FILE *ficheiro_erros = NULL;
    int header_escrito = 0;

    while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
        Reservas *reserva_atual = calloc(1, sizeof(Reservas));  // ← CALLOC!

        int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = parse_csv_line(buffer);

        if (linha_valida && !valida_id_reserva(campos[0], &reserva_atual->id_reserva)) linha_valida = 0;
        if (linha_valida && !valida_voos_reservados(campos[1], &reserva_atual->reserva_lista)) linha_valida = 0;
        if (linha_valida) reserva_atual->id_pessoa_reservou = atoi(campos[2]);
        if (linha_valida) reserva_atual->lugar_reservado = g_strdup(campos[3]);
        if (linha_valida) reserva_atual->preco_reserva = atof(campos[4]);
        if (linha_valida && !valida_bool(campos[5], &reserva_atual->bagagem_extra)) linha_valida = 0;
        if (linha_valida && !valida_bool(campos[6], &reserva_atual->prioridade)) linha_valida = 0;
        if (linha_valida) reserva_atual->qr_code = g_strdup(campos[7]);

        if (linha_valida && !valida_RESERVA(*reserva_atual, tabela1, tabela4)) linha_valida = 0;

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
        libertaReserva(reserva_atual);  // ← USA FUNÇÃO DE LIBERTAÇÃO!
        } else {
            //g_hash_table_insert(tabela5, g_strdup(reserva_atual->id_reserva), reserva_atual);
        }
        liberta_ifcampos(campos); //if (campos) csv_free_fields(campos, n_campos);
    }
    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}

