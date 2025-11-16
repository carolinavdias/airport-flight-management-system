#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "q1.h"
#include "q2.h"
#include "q3.h"
#include "errors.h"
#include "read.h"

void libertaAeronave(void *data);
void libertaAeroporto(void *data);
void libertaVoo(void *data);
void libertaPassageiro(void *data);
void libertaReserva(void *data);


int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Contexto ctx;
    strncpy(ctx.dataset_dir, argv[1], sizeof(ctx.dataset_dir)-1);

    gchar *caminhoAeroportos = g_build_filename(ctx.dataset_dir, "airports.csv", NULL);
    gchar *caminhoAeronaves  = g_build_filename(ctx.dataset_dir, "aircrafts.csv", NULL);
    gchar *caminhoVoos       = g_build_filename(ctx.dataset_dir, "flights.csv", NULL);

    errors_begin();

    g_mkdir_with_parents("resultados", 0755);

    // libertações automáticas das estruturas
    GHashTable *tabelaAeronaves =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeronave);

    GHashTable *tabelaVoos =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaVoo);

    GHashTable *tabelaAeroportos =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeroporto);

    GHashTable *tabelaPassageiros =
        g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, libertaPassageiro);

    GHashTable *tabelaReservas =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaReserva);


    le_tabela(3, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(1, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(2, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(4, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(5, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);

    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);
    g_free(caminhoVoos);

    if (!tabelaAeroportos || !tabelaAeronaves || !tabelaVoos) {
        errors_write_csv("resultados/errors.csv");
        errors_end();
        return EXIT_FAILURE;
    }

    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        errors_write_csv("resultados/errors.csv");
        errors_end();
        return EXIT_FAILURE;
    }

    char *linha = NULL;
    size_t tamanho = 0;
    int numeroComando = 1;

    while (getline(&linha, &tamanho, ficheiroComandos) != -1) {
        linha[strcspn(linha, "\n")] = '\0';

        int idQuery = 0;
        char *param = NULL;

        if (sscanf(linha, "%d", &idQuery) == 1) {
            char *espaco = strchr(linha, ' ');
            if (espaco != NULL)
                param = espaco + 1;
        }

        gchar *nomeOutput = g_strdup_printf("resultados/command%d_output.txt", numeroComando);
        FILE *out = fopen(nomeOutput, "w");
        if (!out) {
            perror("Erro ao criar ficheiro de resultado");
            g_free(nomeOutput);
            numeroComando++;
            continue;
        }

        switch (idQuery) {
            case 1:
//printf("Entrou na q1.\n");
                if (param)
                    query1(param, tabelaAeroportos, out);
                else
                    fprintf(out, "\n");
                break;

            case 2:
//printf("Entrou na q2.\n");
                if (param)
                    query2(param, tabelaAeronaves, tabelaVoos, out);
                else
                    fprintf(out, "\n");
                break;

            case 3: 
//printf("Entrou na q3.\n");
//printf("%s\n", linha);
                char d1[16], d2[16], data_inicio[32], data_fim[32];
                if (param && sscanf(param, "%31s %31s", d1, d2) == 2) {
		sprintf(data_inicio, "%s 00:00", d1);
		sprintf(data_fim, "%s 23:59",d2);
//		printf ("%s \\ %s\n", data_inicio, data_fim);
                //if (param && sscanf(param, "%16[^ ] %31[^\n]", data_inicio, data_fim) == 2) {
	//	printf("%s\n", linha);
                    query3(data_inicio, data_fim, tabelaVoos, tabelaAeroportos, out);}
                else
                    fprintf(out, "\n");
                break;

            default:
                fprintf(out, "\n");
                break;
        }

        fclose(out);
        g_free(nomeOutput);
        numeroComando++;
    }

    free(linha);
    fclose(ficheiroComandos);

    errors_write_csv("resultados/errors.csv");
    errors_end();

    return EXIT_SUCCESS;
}
