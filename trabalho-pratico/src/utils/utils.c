#define _POSIX_C_SOURCE 200809L
#include "utils/utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

/* ============================================================
 * CONTEXTO
 * ============================================================ */

/**
 * Estrutura interna do contexto global.
 */

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

/**
 * Abre um ficheiro no diretório do dataset
 */

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

/**
 * Dia com mes e ano válidos -> auxiliar
 */

int qual_mes (int ano, int mes) {
    if (mes == 2) {
        if (ano % 4 == 0) return 29;
        else return 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    return 31;
}
/* ============================================================
 * STRING POOL (tipo opaco - implementação)
 * ============================================================ */

struct _StringPool {
    GHashTable *table;
};

StringPool *cria_string_pool(void) {
    StringPool *pool = g_new(StringPool, 1);
    pool->table = g_hash_table_new_full(
        g_str_hash,
        g_str_equal,
        g_free,
        NULL
    );
    return pool;
}

const char *string_pool_get(StringPool *pool, const char *s) {
    if (!pool || !s) return NULL;
    char *existente = g_hash_table_lookup(pool->table, s);
    if (existente) return existente;
    char *nova = g_strdup(s);
    g_hash_table_insert(pool->table, nova, nova);
    return nova;
}

void string_pool_clear(StringPool *pool) {
    if (pool) g_hash_table_remove_all(pool->table);
}

void string_pool_destroy(StringPool *pool) {
    if (pool) {
        g_hash_table_destroy(pool->table);
        g_free(pool);
    }
}

/* ============================================================
 * CSV
 * ============================================================ */

/**
 * Divide uma linha CSV em campos individuais.
 * 
 * Suporta campos com aspas e vírgulas dentro dos campos.
 */

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

/**
 * Liberta a memória dos campos
 */

void csv_free_fields(char **fields, size_t count) {
    if (!fields) return;
    for (size_t i = 0; i < count; i++) {
        free(fields[i]);
    }
    free(fields);
}
