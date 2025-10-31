#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "q1.h"

#define MAX_PATH_LEN 256
#define MAX_LINE_LEN 100
#define MAX_CODE_LEN 4  // 3 letras + '\0'
#define MAX_FILENAME_LEN 128

//ALTERAR <ficheiro_comandos>
int main(int argc, char **argv) {
    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    //caminho completo para airports.csv
    char caminhoAeroportos[MAX_PATH_LEN];
    snprintf(caminhoAeroportos, MAX_PATH_LEN,
            "%s/airports.csv", argv[1]); //limita numero de caracteres (MAX_PATH_LEN) -> máximo)

    //carrega aeroportos
    GHashTable *tabelaAeroportos = carregarAeroportos(caminhoAeroportos);

    //abre ficheiro de comandos
    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        g_hash_table_destroy(tabelaAeroportos);
        return EXIT_FAILURE;
    }

    char linha[MAX_LINE_LEN];
    //fgets(linha, MAX_LINE_LEN, ficheiroComandos);

    int numeroComando = 1;

    //garante que a pasta "resultados" existe antes de criar ficheiros
    g_mkdir_with_parents("resultados", 0755); //0755 -> representa permissões de ficheiros/pastas

    while (fgets(linha, sizeof(linha), ficheiroComandos)) {
        int idQuery;
        char codigo[MAX_CODE_LEN];

        if (sscanf(linha, "%d %3s", &idQuery, codigo) == 2 && idQuery == 1) { //lê dados a partir de uma string
            char nomeOutput[MAX_FILENAME_LEN];
            snprintf(nomeOutput, MAX_FILENAME_LEN,
                    "resultados/comandos%d_output.txt", numeroComando);

            FILE *out = fopen(nomeOutput, "w");
            if (!out) {
                perror("Erro ao criar ficheiro de resultado");
                continue;
            }

            query1(codigo, tabelaAeroportos, out);
            fclose(out);
        }

        numeroComando++;
    }

    fclose(ficheiroComandos);
    g_hash_table_destroy(tabelaAeroportos);

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