#define _POSIX_C_SOURCE 200809L
#include "csv.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Função para dividir linha CSV (suporta aspas e vírgulas dentro de campos)
int csv_split(const char *line, char ***fields, size_t *count) {
    if (!line || !fields || !count) return -1;
    
    *fields = NULL;
    *count = 0;
    
    size_t capacity = 16;
    char **result = malloc(capacity * sizeof(char*));
    if (!result) return -1;
    
    const char *p = line;
    
    while (*p) {
        // pular espaços iniciais
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        
        const char *start;
        const char *end;
        
        if (*p == '"') {
            // campo com aspas
            p++; // pular aspa inicial
            start = p;
            while (*p && *p != '"') p++;
            end = p;
            if (*p == '"') p++; // pular aspa final
        } else {
            // campo sem aspas
            start = p;
            while (*p && *p != ',') p++;
            end = p;
        }
        
        // alocar e copiar campo
        size_t len = end - start;
        char *field = malloc(len + 1);
        if (!field) {
            csv_free_fields(result, *count);
            return -1;
        }
        memcpy(field, start, len);
        field[len] = '\0';
        
        // expandir array se necessário
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
        
        // pular vírgula
        if (*p == ',') p++;
    }
    
    *fields = result;
    return 0;
}

// Função para libertar campos alocados
void csv_free_fields(char **fields, size_t count) {
    if (!fields) return;
    for (size_t i = 0; i < count; i++) {
        free(fields[i]);
    }
    free(fields);
}