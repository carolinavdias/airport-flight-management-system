#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "errors.h"

//estrutura interna de um erro
typedef struct {
    char *ficheiro;
    int   linha;
    char *mensagem;
} Erro;

//lista global interna
static GList *ERROS = NULL;

void errors_begin(void) {
    //limpa caso já exista algo
    if (ERROS) {
        for (GList *l = ERROS; l; l = l->next) {
            Erro *e = l->data;
            g_free(e->ficheiro);
            g_free(e->mensagem);
            g_free(e);
        }
        g_list_free(ERROS);
        ERROS = NULL;
    }
}

void errors_add(const char *ficheiro, int linha, const char *mensagem) {
    if (!ficheiro || !mensagem) return;
    Erro *e = g_new(Erro, 1);
    e->ficheiro = g_strdup(ficheiro);
    e->linha = linha;
    e->mensagem = g_strdup(mensagem);
    ERROS = g_list_append(ERROS, e);
}

void errors_write_csv(const char *caminho) {
    FILE *f = fopen(caminho, "w");
    if (!f) {
        perror("errors_write_csv");
        return;
    }
    fprintf(f, "file;line;message\n");
    for (GList *l = ERROS; l; l = l->next) {
        Erro *e = l->data;
        fprintf(f, "%s;%d;%s\n", e->ficheiro, e->linha, e->mensagem);
    }
    fclose(f);
}

void errors_end(void) {
    for (GList *l = ERROS; l; l = l->next) {
        Erro *e = l->data;
        g_free(e->ficheiro);
        g_free(e->mensagem);
        g_free(e);
    }
    g_list_free(ERROS);
    ERROS = NULL;
}