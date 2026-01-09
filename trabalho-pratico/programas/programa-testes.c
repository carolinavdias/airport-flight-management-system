#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

/**
 * Este programa:
 *  - executa o programa-principal com um dataset e ficheiro de comandos
 *  - compara os outputs gerados com os resultados esperados
 *  - mede tempo e memória usados
 *  - apresenta um resumo final
 */

/**
 * Compara dois ficheiros linha a linha.
 */

static int compare_files(const char *expected, const char *output, const char t) {
    FILE *f1 = fopen(expected, "r");
    FILE *f2 = fopen(output, "r");
    if (!f1 || !f2) {
        printf("✘ Erro a abrir ficheiros: '%s' ou '%s'\n", expected, output);
        if (f1) fclose(f1);
        if (f2) fclose(f2);
        return -1;
    }

    char line1[4096], line2[4096];
    int line = 1;
    while (1) {
        char *r1 = fgets(line1, sizeof(line1), f1);
        char *r2 = fgets(line2, sizeof(line2), f2);
        if (!r1 || !r2) {
            if (r1 == r2) {
                fclose(f1); fclose(f2);
                return 0;
            } else {
                if (t == 'l') printf("✘ Diferenca no tamanho (linha %d)\n", line);
                fclose(f1); fclose(f2);
                return 1;
            }
        }
        if (strcmp(line1, line2) != 0) {
            if (t == 'l') printf("✘ Diferenca na linha %d:\nEsperado: %sObtido  : %s\n", line, line1, line2);
            fclose(f1); fclose(f2);
            return 1;
        }
        line++;
    }
}

/**
 * Programa principal de testes
 */

int main(int argc, char *argv[]) {
    if (argc != 5) {
        printf("Uso: %s <dataset_dir> <input.txt> <resultados-esperados/> <tipo_de_teste> \n", argv[0]);
        return 1;
    }

    const char *dataset = argv[1];
    const char *input = argv[2];
    const char *expected_dir = argv[3];
    const char *type = argv[4];

    if (type[1] != '\0' || (type[0] != 's' && type[0] != 'l')) {
        printf ("Tipo de teste inválido.\n");
        return 1;
    }
    const char t = type[0];


    printf("====================================\n");
    printf("✔ Programa de Testes LI3\n");
    printf("====================================\n");

    // garante que a pasta resultados está limpa
    if (system("rm -rf resultados && mkdir -p resultados") != 0) {
	fprintf(stderr, "Erro ao criar diretório de resultados.\n");
    }

    // executa o programa principal e mede tempo total
    struct timespec start, end;
    struct rusage usage;
    clock_gettime(CLOCK_REALTIME, &start);

    char command[512];
    snprintf(command, sizeof(command), "./programa-principal %s %s", dataset, input);
    int ret = system(command);

    clock_gettime(CLOCK_REALTIME, &end);
    getrusage(RUSAGE_SELF, &usage);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    if (ret != 0) {
        printf("✘ Erro: programa-principal terminou com código %d\n", ret);
        return 1;
    }

    // compara todos os ficheiros de saída
    char expected_path[512], output_path[512];
    int errors = 0, all = 1;
    for (int i = 1; i <= 120; i++) {
        snprintf(expected_path, sizeof(expected_path), "%s/command%d_output.txt", expected_dir, i);
        snprintf(output_path, sizeof(output_path), "resultados/command%d_output.txt", i);
        printf("\n▶ Comparando comando %d...\n", i);
        int status = compare_files(expected_path, output_path,t);
        if (status != 0) {
	    all = 0;
	    errors++;
	    if (t == 'l') {
            	printf("✘ Diferença encontrada no comando %d\n", i);
	    }
        }
    }

    // resumo final ;))
    printf("\n====================================\n");
    printf("           ✔ Resumo dos Testes\n");
    printf("====================================\n");
    if (all) printf("Todos os ficheiros coincidem com os resultados esperados.\n");
    else printf("%d ficheiros de 120 não coincidem com os resultados esperados.\n", errors);
    printf("Tempo total: %.4f s\n", elapsed);
    printf("Memória usada: %ld KB\n", usage.ru_maxrss);
    printf("====================================\n");

    return 0;
}
