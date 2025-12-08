#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include "parser_passangers.h"
#include "passangers.h"
#include "gestor_passangers.h"
#include "validacoes/validacoes_passangers.h"
#include "csv.h"


int le_tabela_Passageiros(Contexto ctx, GestorPassengers *P) {

    FILE *ficheiro = abrir_ficheiro(&ctx, "passengers.csv", "r");
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
        Passageiros *passageiro_atual = calloc(1, sizeof(Passageiros));  // ← CALLOC!

        int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = parse_csv_line(buffer);

        if (linha_valida && !valida_id_passageiro(campos[0], &passageiro_atual->id_passageiro)) linha_valida = 0;
        if (linha_valida) passageiro_atual->primeiro_nome = g_strdup(campos[1]);
        if (linha_valida) passageiro_atual->ultimo_nome = g_strdup(campos[2]);
        if (linha_valida && !valida_Data(campos[3], &passageiro_atual->data_nascimento)) linha_valida = 0;
        if (linha_valida) passageiro_atual->nacionalidade = g_strdup(campos[4]);
        if (linha_valida && !valida_genero(campos[5], &passageiro_atual->genero_passageiro)) linha_valida = 0;
        if (linha_valida && !valida_email(campos[6], &passageiro_atual->email_passageiro)) linha_valida = 0;
        if (linha_valida) passageiro_atual->telefone_passageiro = g_strdup(campos[7]);
        if (linha_valida) passageiro_atual->morada_passageiro = g_strdup(campos[8]);
        if (linha_valida) passageiro_atual->fotografia_passageiro = g_strdup(campos[9]);

        if (!linha_valida) {
            if (!ficheiro_erros) {
                ficheiro_erros = fopen("resultados/passengers_errors.csv", "w");
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
            libertaPassageiro(passageiro_atual);  // ← USA FUNÇÃO DE LIBERTAÇÃO!
        } else {
                //g_hash_table_insert(tabela4, GINT_TO_POINTER(passageiro_atual->id_passageiro), passageiro_atual);
		gestor_passengers_inserir(P, passageiro_atual);
        }
        liberta_ifcampos(campos); //if (campos) csv_free_fields(campos, n_campos);
    }
    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;

