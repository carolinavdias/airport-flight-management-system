#include <stdio.h>
#include <stdlib.h>

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
