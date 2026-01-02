#define _POSIX_C_SOURCE 200809L
#include "utils/utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <glib.h>

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

void set_contexto (Contexto *ctx, const char *d) {
    strncpy(ctx->dataset_dir, d , sizeof(ctx->dataset_dir)-1);
}

//abre um ficheiro no diretório do dataset
FILE *abrir_ficheiro(Contexto *ctx, const char *nome_ficheiro, const char *modo) {
    char path[1024];
    if (ctx->dataset_dir[0] == '\0') {
	fprintf(stderr, "Diretoria do dataset não encontrada.\n");
	return NULL;
    }
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
/*
//struct auxiliares para Q5
typedef struct contagemQ5 {
    char *airline;
    float delay_avg;
    int delay_count;
} ContagemQ5;

typedef struct estrt_aux_q5 {
    ContagemQ5 *listaQ5;
    int num_voos;
} Estrt_aux_q5;

Estrt_aux_q5 *init_lista() {
    Estrt_aux_q5 *nova_lista = malloc(sizeof *nova_lista);
    if (!nova_lista) return NULL;

    nova_lista->listaQ5 = calloc(1, sizeof(ContagemQ5));
    nova_lista->num_voos = 0;
    return nova_lista;
}

//Função auxiliar para Q5
int get_time_datah (char *s) {

    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    return hora * 60 +
           min;
}
*/
//===================================
//ERRORS
//===================================

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

//===================================
//CSV
//===================================

//divide uma linha CSV em campos individuais
//suporta campos com aspas e vírgulas dentro dos campos
int csv_split(const char *line, char ***fields, size_t *count) {
    if (!line || !fields || !count) return -1;
    
    *fields = NULL;
    *count = 0;
    
    //começa com capacidade para 16 campos
    size_t capacity = 16;
    char **result = malloc(capacity * sizeof(char*));
    if (!result) return -1;
    
    const char *p = line;
    
    while (*p) {
        //salta espaços no inicio
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        
        const char *start;
        const char *end;
        
        //verifica se o campo tem aspas
        if (*p == '"') {
            p++; //passa a aspa inicial
            start = p;
            while (*p && *p != '"') p++;
            end = p;
            if (*p == '"') p++; //passa a aspa final
        } else {
            //campo sem aspas
            start = p;
            while (*p && *p != ',') p++;
            end = p;
        }
        
        //aloca espaço e copia o campo
        size_t len = end - start;
        char *field = malloc(len + 1);
        if (!field) {
            csv_free_fields(result, *count);
            return -1;
        }
        memcpy(field, start, len);
        field[len] = '\0';
        
        //expande array se necessário
        if (*count >= capacity) {
            capacity *= 2;
            char **new_result = realloc(result, capacity * sizeof(char*));
            if (!new_result) {
                free(field);
                csv_free_fields(result, *count);
                return -1;
            }
            result = new_result;
        }
        
        result[*count] = field;
        (*count)++;
        
        //passa a vírgula se houver
        if (*p == ',') p++;
    }
    
    *fields = result;
    return 0;
}

//liberta a memória dos campos
void csv_free_fields(char **fields, size_t count) {
    if (!fields) return;
    for (size_t i = 0; i < count; i++) {
        free(fields[i]);
    }
    free(fields);
}
