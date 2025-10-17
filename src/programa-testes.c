#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/resource.h>

static int compare_files(const char *expected, const char *output) {
    FILE *f1 = fopen(expected, "r");
    FILE *f2 = fopen(output, "r");
    if (!f1 || !f2) {
        printf("Erro a abrir ficheiros: '%s' ou '%s'\n", expected, output);
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
                printf("Ficheiros iguais (linhas comparadas: %d)\n", line-1);
                fclose(f1); fclose(f2);
                return 0;
            } else {
                printf("Diferenca no tamanho: um ficheiro terminou antes do outro (linha %d)\n", line);
                fclose(f1); fclose(f2);
                return 1;
            }
        }
        if (strcmp(line1, line2) != 0) {
            printf("Diferenca na linha %d:\nEsperado: %sObtido  : %s\n", line, line1, line2);
            fclose(f1); fclose(f2);
            return 1;
        }
        line++;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 4) {
        printf("Uso: %s <dataset_dir> <input.txt> <resultados-esperados/>\n", argv[0]);
        return 1;
    }
    const char *dataset = argv[1];
    const char *input = argv[2];
    const char *expected_dir = argv[3];
    (void)dataset; (void)input; (void)expected_dir;

    struct timespec start, end;
    struct rusage usage;
    clock_gettime(CLOCK_REALTIME, &start);

    int status = compare_files("resultados-esperados/command1_output.txt",
                               "resultados/command1_output.txt");

    clock_gettime(CLOCK_REALTIME, &end);
    getrusage(RUSAGE_SELF, &usage);
    double elapsed = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("\nResumo testes:\n");
    printf("Status comparacao: %s\n", status==0 ? "OK" : "FALHOU");
    printf("Tempo total: %.4f s\n", elapsed);
    printf("Memoria usada: %ld KB\n", usage.ru_maxrss);
    return status != 0;
}
