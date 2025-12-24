#define _POSIX_C_SOURCE 200809L

#include "parsers/parser_aircrafts.h"
#include "entidades/aircrafts.h"
#include "validacoes/validacoes_aircrafts.h"
#include "utils/utils.h"
#include <string.h>
#include <stdlib.h>
#include <glib.h>

int le_tabela_Aeronaves(Contexto *ctx, GestorAircrafts *AC) {

    int MAX_LINHA = 2000;
    gchar buffer[MAX_LINHA];


    FILE *ficheiro = abrir_ficheiro(ctx, "aircrafts.csv", "r");
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
    	Aeronave *aeronave_atual = criaAeronave();

       	int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;

        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

        if (linha_valida && n_campos < 4) linha_valida = 0; //VERIFICAR
        
        if (linha_valida && (!campos[0] || campos[0][0] == '\0')) linha_valida = 0;
        if (linha_valida && (!campos[1] || campos[1][0] == '\0')) linha_valida = 0;
        if (linha_valida && (!campos[2] || campos[2][0] == '\0')) linha_valida = 0;
        if (linha_valida && (!campos[3] || campos[3][0] == '\0')) linha_valida = 0;
        
        if (linha_valida && !valida_year(campos[3])) linha_valida = 0;

        if (linha_valida) {
            aircraft_set_id(aeronave_atual, campos[0]);
            aircraft_set_manuf(aeronave_atual, campos[1]);
            aircraft_set_model(aeronave_atual, campos[2]); 
            aircraft_set_year(aeronave_atual, atoi(campos[3])); 
        }

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