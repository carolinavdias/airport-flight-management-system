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
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]); //<dataser-fase-1>, agrv[0] só
        return EXIT_FAILURE;
    }
    Contexto ctx;
    strncpy(ctx.dataset_dir, argv[1], sizeof(ctx.dataset_dir)-1);

    //construção dinâmica dos caminhos
    gchar *caminhoAeroportos = g_build_filename(argv[1], "airports.csv", NULL);
    gchar *caminhoAeronaves  = g_build_filename(argv[1], "aircrafts.csv", NULL);
    gchar *caminhoVoos       = g_build_filename(argv[1], "flights.csv", NULL);

    //inicia sistema de erros
    errors_begin();

    //carrega tabelas (fase 1)
//    GHashTable *tabelaAeroportos = carregarAeroportos(caminhoAeroportos);
//    GHashTable *tabelaAeronaves  = carregarAeronaves(caminhoAeronaves);
//    GHashTable *tabelaVoos       = carregarVoos(caminhoVoos);
    GHashTable *tabelaAeronaves = g_hash_table_new(g_str_hash, g_str_equal); //, g_free, libertaAeronave);     //tabela3
    GHashTable *tabelaVoos = g_hash_table_new (g_str_hash, g_str_equal); //, g_free, libertaVoo);        //tabela1
    GHashTable *tabelaAeroportos = g_hash_table_new(g_str_hash, g_str_equal); // g_free, libertaAeroporto);  //tabela2
    GHashTable *tabelaPassageiros = g_hash_table_new (g_direct_hash, g_direct_equal); // NULL, NULL);         //tabela4
    GHashTable *tabelaReservas = g_hash_table_new(g_str_hash, g_str_equal); // g_free, libertaReserva);            //tabela5


    le_tabela(3,ctx,tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(1,ctx,tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(2,ctx,tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(4,ctx,tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(5,ctx,tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    printf ("Acabou de ler as tabelas.\n");

    //liberta strings de caminho
    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);
    g_free(caminhoVoos);
    printf ("Acabou de libertar os caminhos.\n");

    //verifica se os ficheiros principais foram carregados
    if (!tabelaAeroportos || !tabelaAeronaves || !tabelaVoos) {
        errors_write_csv("resultados/errors.csv");
        errors_end();
        return EXIT_FAILURE;
    }
    printf ("Acabou de verificar se os ficheiros foram carregados.\n");

    //abre o ficheiro de comandos
    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        //g_hash_table_destroy(tabelaAeroportos);
        //g_hash_table_destroy(tabelaAeronaves);
        //g_hash_table_destroy(tabelaVoos);
        //g_hash_table_destroy(tabelaReservas);
        errors_write_csv("resultados/errors.csv");
        errors_end();
        return EXIT_FAILURE;
    }
    printf ("Abriu o ficheiro de comandos.\n");

    //garante que a pasta de resultados existe
    gchar *dirResultados = g_build_filename("trabalho-pratico", "resultados", NULL);
    printf("D\n");
    g_mkdir_with_parents(dirResultados, 0755);
    printf("E\n");
    g_free(dirResultados);
    printf ("Garantiu que a pasta de resultados existe.\n");

    char *linha = NULL;
    printf("A\n");
    size_t tamanho = 0;
    printf("B\n");
    int numeroComando = 1;
    printf("C\n");

    printf("Aqui");

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

    printf("Aqui2");

    free(linha);
    fclose(ficheiroComandos);

    //liberta tabelas e escreve erros
    //g_hash_table_destroy(tabelaAeroportos);
    //g_hash_table_destroy(tabelaAeronaves);
    //g_hash_table_destroy(tabelaVoos);
    //g_hash_table_destroy(tabelaReservas);

    errors_write_csv("resultados/errors.csv");
    errors_end();

    return EXIT_SUCCESS;
}
