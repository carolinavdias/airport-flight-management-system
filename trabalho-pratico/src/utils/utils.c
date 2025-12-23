#include "utils/utils.h"
#include <string.h>
#include <stdlib.h>

typedef struct contexto {
    char dataset_dir[256];
} Contexto;

Contexto *cria_contexto () {
    Contexto *ctx = calloc(1, sizeof *ctx);
    return ctx;
}

char *get_contexto (Contexto *ctx) {
    return ctx->dataset_dir;
}

void set_contexto (Contexto *ctx, char *d) {
    strncpy(ctx->dataset_dir, d , sizeof(ctx->dataset_dir)-1);
}

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

//dia com mes e ano válidos ->auxiliar
int qual_mes (int ano, int mes) {
    if (mes == 2) {
        if (ano % 4 == 0) return 29;
        else return 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    return 31;
}