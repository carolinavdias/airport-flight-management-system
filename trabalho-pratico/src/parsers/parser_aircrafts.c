#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include "parser_aircrafts.h"
#include "aircrafts.h"
#include "gestor_aircrafts.h"
#include "validacoes/validacoes_aircrafts.h"
#include "csv.h"


int le_tabela_Aeronaves(int opcao_inserida, Contexto ctx, GestorAircrafts *AC) {

    FILE *ficheiro = abrir_ficheiro(&ctx, "aircrafts.csv", "r");
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
    	Aeronave *aeronave_atual = calloc(1, sizeof(Aeronave));  // ← CALLOC!

       	int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = parse_csv_line(buffer);

        if (linha_valida) aeronave_atual->identifier = g_strdup(campos[0]);
        if (linha_valida) aeronave_atual->manufacturer = g_strdup(campos[1]);
        if (linha_valida) aeronave_atual->model = g_strdup(campos[2]);
        if (linha_valida && !valida_year(campos[3], &aeronave_atual->year)) linha_valida = 0;
        if (linha_valida) aeronave_atual->capacity = atoi(campos[4]);
        if (linha_valida) aeronave_atual->range = atoi(campos[5]);
        if (!linha_valida) {
            if (!ficheiro_erros) {
                 ficheiro_erros = fopen("resultados/aircrafts_errors.csv", "w");
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
            libertaAeronave(aeronave_atual);  // ← USA FUNÇÃO DE LIBERTAÇÃO!
        } else {
            gestor_aircrafts_insere(AC, aeronave_atual);
	    //g_hash_table_insert(tabela3, g_strdup(aeronave_atual->identifier), aeronave_atual);
        }
        liberta_ifcampos(campos); //if (campos) csv_free_fields(campos, n_campos);
    }

    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}
