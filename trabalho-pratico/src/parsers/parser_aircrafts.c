#define _POSIX_C_SOURCE 200809L
#include "parsers/parser_aircrafts.h"
#include "validacoes/validacoes_aircrafts.h"
#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>

#define MAX_LINHA 20000
static char buffer[MAX_LINHA];

int le_tabela_Aeronaves(Contexto ctx, GestorAircrafts *AC) {

    FILE *ficheiro = abrir_ficheiro(&ctx, "aircrafts.csv", "r");
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
    	Aeronave *aeronave_atual = calloc(1, sizeof(Aeronave));

       	int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

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
            libertaAeronave(aeronave_atual);
        } else {
            gestor_aircrafts_insere(AC, aeronave_atual);
        }
        if (campos) csv_free_fields(campos, n_campos);
    }

    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}