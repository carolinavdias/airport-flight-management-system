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

//programa principal (executa as queries a partir do ficheiro de comandos)
int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //construção dinâmica dos caminhos
    gchar *caminhoAeroportos = g_build_filename(argv[1], "airports.csv", NULL);
    gchar *caminhoAeronaves  = g_build_filename(argv[1], "aircrafts.csv", NULL);
    gchar *caminhoVoos       = g_build_filename(argv[1], "flights.csv", NULL);

    //inicia sistema de erros
    errors_begin();

    //carrega tabelas (fase 1)
    GHashTable *tabelaAeroportos = carregarAeroportos(caminhoAeroportos);
    GHashTable *tabelaAeronaves  = carregarAeronaves(caminhoAeronaves);
    GHashTable *tabelaVoos       = carregarVoos(caminhoVoos);

    //liberta strings de caminho
    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);
    g_free(caminhoVoos);

    //verifica se os ficheiros principais foram carregados
    if (!tabelaAeroportos || !tabelaAeronaves || !tabelaVoos) {
        errors_write_csv("resultados/errors.csv");
        errors_end();
        return EXIT_FAILURE;
    }

    //abre o ficheiro de comandos
    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        g_hash_table_destroy(tabelaAeroportos);
        g_hash_table_destroy(tabelaAeronaves);
        g_hash_table_destroy(tabelaVoos);
        errors_write_csv("resultados/errors.csv");
        errors_end();
        return EXIT_FAILURE;
    }

    //garante que a pasta de resultados existe
    g_mkdir_with_parents("resultados", 0755);

    char *linha = NULL;
    size_t tamanho = 0;
    int numeroComando = 1;

    //lê e executa cada comando do ficheiro
    while (getline(&linha, &tamanho, ficheiroComandos) != -1) {
        linha[strcspn(linha, "\n")] = '\0';

        int idQuery = 0;
        char *param = NULL;

        if (sscanf(linha, "%d", &idQuery) == 1) {
            char *espaco = strchr(linha, ' ');
            if (espaco != NULL)
                param = espaco + 1;
        }

        //nome do ficheiro de output (seguindo o formato do enunciado)
        gchar *nomeOutput = g_strdup_printf("resultados/command%d_output.txt", numeroComando);
        FILE *out = fopen(nomeOutput, "w");
        if (!out) {
            perror("Erro ao criar ficheiro de resultado");
            g_free(nomeOutput);
            numeroComando++;
            continue;
        }

        //executa a query correspondente
        switch (idQuery) {
            case 1:
                if (param)
                    query1(param, tabelaAeroportos, out);
                else
                    fprintf(out, "\n");
                break;

            case 2:
                if (param)
                    query2(param, tabelaAeronaves, out);
                else
                    fprintf(out, "\n");
                break;

            case 3: {
                char data_inicio[32], data_fim[32];
                if (param && sscanf(param, "%31s %31s", data_inicio, data_fim) == 2)
                    query3(data_inicio, data_fim, tabelaVoos, out);
                else
                    fprintf(out, "\n");
                break;
            }

            default:
                fprintf(out, "\n"); //query desconhecida
                break;
        }

        fclose(out);
        g_free(nomeOutput);
        numeroComando++;
    }

    free(linha);
    fclose(ficheiroComandos);

    //liberta tabelas e escreve erros
    g_hash_table_destroy(tabelaAeroportos);
    g_hash_table_destroy(tabelaAeronaves);
    g_hash_table_destroy(tabelaVoos);

    errors_write_csv("resultados/errors.csv");
    errors_end();

    return EXIT_SUCCESS;
}
