#define _POSIX_C_SOURCE 200809L

#include "entidades/passengers.h"
#include "parsers/parser_passengers.h"
#include "validacoes/validacoes_passengers.h"
#include "csv.h"

#include <string.h>
#include <glib.h>

int le_tabela_Passageiros(Contexto *ctx, GestorPassengers *P) {

    int MAX_LINHA = 2000;
    gchar buffer[MAX_LINHA];

    FILE *ficheiro = abrir_ficheiro(ctx, "passengers.csv", "r");
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
        Passageiros *passageiro_atual = criaPassageiro();

        int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

	//ID_PASSAGEIRO
        if (linha_valida && valida_id_passageiro(campos[0])) passenger_set_id(passageiro_atual,campos[0]);
	else linha_valida = 0;

	//PRIMEIRO_NOME
        if (linha_valida) passenger_set_pn(passageiro_atual,campos[1]);

	//ULTIMO_NOME
        if (linha_valida) passenger_set_un(passageiro_atual,campos[2]);

	//DATA NASCIMENTO
        if (linha_valida && valida_Data(campos[3])) passenger_set_dn(passageiro_atual,campos[3]);
	else linha_valida = 0;

	//NACIONALIDADE
        if (linha_valida) passenger_set_nc(passageiro_atual,campos[4]);

	//GENERO
        if (linha_valida && valida_genero(campos[5])) passanger_set_ge(passageiro_atual,campos[5]);
	else linha_valida = 0;

	//EMAIL
        if (linha_valida && valida_email(campos[6])) passenger_set_em(passageiro_atual,campos[6]);
	else linha_valida = 0;

	//TELEFONE
        if (linha_valida) passenger_set_tl(passageiro_atual,campos[7]);

	//MORADA
        if (linha_valida) passenger_set_md(passageiro_atual,campos[8]);

	//FOTOGRAFIA
        if (linha_valida) passenger_set_ft(passageiro_atual,campos[9]);

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
            libertaPassageiro(passageiro_atual);
        } else {
	    gestor_passengers_inserir(P, passageiro_atual);
        }
        if (campos) csv_free_fields(campos, n_campos);
    }

    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}