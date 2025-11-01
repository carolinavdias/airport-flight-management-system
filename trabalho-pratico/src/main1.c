#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "q1.h"
#include "q2.h" 

//VERIFICAR ficheiro_comandos
int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //construção dinâmica dos caminhos
    gchar *caminhoAeroportos = g_build_filename(argv[1], "airports.csv", NULL);
    gchar *caminhoAeronaves  = g_build_filename(argv[1], "aircrafts.csv", NULL);
    gchar *caminhoVoos       = g_build_filename(argv[1], "flights.csv", NULL);

    //carrega tabelas
    GHashTable *tabelaAeroportos = carregarAeroportos(caminhoAeroportos);
    GHashTable *tabelaAeronaves  = carregarAeronaves(caminhoAeronaves);

    //liberta strings de caminho
    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);

    //abre o ficheiro de comandos
    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        g_hash_table_destroy(tabelaAeroportos);
        g_hash_table_destroy(tabelaAeronaves);
        g_free(caminhoVoos);
        return EXIT_FAILURE;
    }

    //garante que a pasta de resultados existe
    g_mkdir_with_parents("resultados", 0755);

    char *linha = NULL;
    size_t tamanho = 0;
    int numeroComando = 1;

    while (getline(&linha, &tamanho, ficheiroComandos) != -1) {
        //remove \n
        linha[strcspn(linha, "\n")] = '\0';

        int idQuery = 0;
        char *param = NULL;

        //divide a linha em 2 componentes: número da query e parâmetro
        if (sscanf(linha, "%d", &idQuery) == 1) {
            //encontra o espaço (se houver)
            char *espaco = strchr(linha, ' ');
            if (espaco != NULL)
                param = espaco + 1; //representa o que vem depois do espaço
        }

        //prepara nome do ficheiro de output
        gchar *nomeOutput = g_strdup_printf("resultados/comando%d_output.txt", numeroComando);
        FILE *out = fopen(nomeOutput, "w");
        if (!out) {
            perror("Erro ao criar ficheiro de resultado");
            g_free(nomeOutput);
            numeroComando++;
            continue;
        }

        //executa a query correspondente
        switch (idQuery) {
            case 1: //executa se der 1
                if (param)
                    query1(param, tabelaAeroportos, out);
                else
                    fprintf(out, "\n"); // sem parâmetro válido
                break;

            case 2: //executa se der 2
                if (param)
                    query2(param, tabelaAeronaves, caminhoVoos, out);
                else
                    fprintf(out, "\n");
                break;

            default: //executa se der outro valor
                fprintf(stderr, "Query desconhecida: %d\n", idQuery);
                fprintf(out, "\n");
                break;
        }

        fclose(out);
        g_free(nomeOutput);
        numeroComando++;
    }

    free(linha);
    fclose(ficheiroComandos);
    g_hash_table_destroy(tabelaAeroportos);
    g_hash_table_destroy(tabelaAeronaves);
    g_free(caminhoVoos);

    return EXIT_SUCCESS;
}


//CAROLINA
/*
int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <pasta_dataset> <input.txt>\n", argv[0]);
        return 1;
    }
    const char *dataset = argv[1];
    const char *input = argv[2];
    (void)dataset; (void)input;

    // gera um output de exemplo para o test runner
    FILE *f = fopen("resultados/command1_output.txt", "w");
    if (!f) {
        perror("resultados/command1_output.txt");
        return 2;
    }
    fprintf(f, "OPO,Francisco de Sá Carneiro Airport,Porto,PT,large_airport\n");
    fclose(f);
    printf("OK: resultados/command1_output.txt gerado (dummy)\n");
    return 0;
}
*/