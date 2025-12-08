#define _POSIX_C_SOURCE 200809L
#include "parsers/parser_airports.h"
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

int le_tabela_Aeroportos(Contexto ctx, GestorAirports *AP) {

    FILE *ficheiro = abrir_ficheiro(&ctx, "airports.csv", "r");
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
        Aeroporto *aeroporto_atual = calloc(1, sizeof(Aeroporto));

        int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

        if (linha_valida && !valida_codigoIATA(campos[0], &aeroporto_atual->code_IATA)) linha_valida = 0;
        if (linha_valida) aeroporto_atual->name = g_strdup(campos[1]);
        if (linha_valida) aeroporto_atual->city = g_strdup(campos[2]);
        if (linha_valida) aeroporto_atual->country = g_strdup(campos[3]);
        if (linha_valida && !valida_coordenadas(campos[4],1,&aeroporto_atual->latitude)) linha_valida = 0;
        if (linha_valida && !valida_coordenadas(campos[5],2,&aeroporto_atual->longitude)) linha_valida = 0;
        if (linha_valida) aeroporto_atual->code_ICAO = g_strdup(campos[6]);
        if (linha_valida && !valida_tipo_aer(campos[7], &aeroporto_atual->type)) linha_valida = 0;

        if (!linha_valida) {
            if (!ficheiro_erros) {
                ficheiro_erros = fopen("resultados/airports_errors.csv", "w");
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
            libertaAeroporto(aeroporto_atual);
        } else {
           gestor_airports_insere(AP, aeroporto_atual);
        }
        if (campos) csv_free_fields(campos, n_campos);
    }
    
    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}