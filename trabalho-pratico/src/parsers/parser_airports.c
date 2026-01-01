#define _POSIX_C_SOURCE 200809L

#include "parsers/parser_airports.h"
#include "validacoes/validacoes_airports.h"
#include "entidades/airports.h"
#include "utils/utils.h"

#include <glib.h>

int le_tabela_Aeroportos(Contexto *ctx, GestorAirports *AP) {

    int MAX_LINHA = 2000;
    gchar buffer[MAX_LINHA];

    FILE *ficheiro = abrir_ficheiro(ctx, "airports.csv", "r");
    if (ficheiro == NULL) return 0;

    int  no_header = 1;
    char header[MAX_LINHA];

    if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
    else {
        buffer[strcspn(buffer,"\n")] = '\0';
        strcpy(header, buffer);
    }

    FILE *ficheiro_erros = NULL;
    int header_escrito = 0;

    while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
        Aeroporto *aeroporto_atual = criaAeroporto();

        int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

	//NAME
        if (linha_valida) airport_set_name(aeroporto_atual,campos[1]);

	//CITY
        if (linha_valida) airport_set_city(aeroporto_atual,campos[2]);

	//COUNTRY
        if (linha_valida) airport_set_country(aeroporto_atual,campos[3]);

	//CODE_IATA
        if (linha_valida) airport_set_code_IATA(aeroporto_atual,campos[0]); //CONFIRMAR CAMPO

	//TIPO_AEROPORTO
        if (linha_valida && valida_tipo_aer(campos[7])) airport_set_type(aeroporto_atual,campos[7]);
	else linha_valida = 0;

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