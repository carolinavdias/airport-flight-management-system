#include "utils.h"
#include <string.h>

//abre um ficheiro no diretório do dataset
FILE *abrir_ficheiro(Contexto *ctx, const char *nome_ficheiro, const char *modo) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", ctx->dataset_dir, nome_ficheiro);
    FILE *ficheiro = fopen(path, modo);
    if (ficheiro == NULL) {
        perror("Erro ao abrir o ficheiro.\n");
        return NULL;
    }
    return ficheiro;
}