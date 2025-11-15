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

// Adicione estes protótipos se não existirem
void libertaAeronave(void *data);
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

    // ⚠️ CORREÇÃO: Tabelas com funções de destruição
    GHashTable *tabelaAeronaves = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeronave);
    GHashTable *tabelaVoos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaVoo);
    GHashTable *tabelaAeroportos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeroporto);
    GHashTable *tabelaPassageiros = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, libertaPassageiro);
    GHashTable *tabelaReservas = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaReserva);

    // Carregar dados
    le_tabela(3, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(1, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(2, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(4, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);
    le_tabela(5, ctx, tabelaVoos, tabelaAeroportos, tabelaAeronaves, tabelaPassageiros, tabelaReservas);

    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);
    g_free(caminhoVoos);

    if (!tabelaAeroportos || !tabelaAeronaves || !tabelaVoos) {
        // ⚠️ CORREÇÃO: Libertar tabelas mesmo em caso de erro
        g_hash_table_destroy(tabelaAeroportos);
        g_hash_table_destroy(tabelaAeronaves);
        g_hash_table_destroy(tabelaVoos);
        g_hash_table_destroy(tabelaPassageiros);
        g_hash_table_destroy(tabelaReservas);
        
        errors_write_csv("resultados/errors.csv");
        errors_end();
        return EXIT_FAILURE;
    }

    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        
        // ⚠️ CORREÇÃO: Libertar tabelas
        g_hash_table_destroy(tabelaAeroportos);
        g_hash_table_destroy(tabelaAeronaves);
        g_hash_table_destroy(tabelaVoos);
        g_hash_table_destroy(tabelaPassageiros);
        g_hash_table_destroy(tabelaReservas);
        
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
                if (param)
                    query1(param, tabelaAeroportos, out);
                else
                    fprintf(out, "\n");
                break;

            case 2:
                if (param)
                    query2(param, tabelaAeronaves, tabelaVoos, out);
                else
                    fprintf(out, "\n");
                break;

            case 3: {
                char data_inicio[32], data_fim[32];
                if (param && sscanf(param, "%31s %31s", data_inicio, data_fim) == 2)
                    query3(data_inicio, data_fim, tabelaVoos, tabelaAeroportos, out); // ⚠️ CORREÇÃO AQUI
                else
                    fprintf(out, "\n");
                break;
            }

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

    // ⚠️ CORREÇÃO CRÍTICA: LIBERTAR TODAS AS TABELAS
    g_hash_table_destroy(tabelaAeroportos);
    g_hash_table_destroy(tabelaAeronaves);
    g_hash_table_destroy(tabelaVoos);
    g_hash_table_destroy(tabelaPassageiros);
    g_hash_table_destroy(tabelaReservas);

    errors_write_csv("resultados/errors.csv");
    errors_end();

    return EXIT_SUCCESS;
}