#define _POSIX_C_SOURCE 200809L
#include "csv.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// divide uma linha CSV em campos individuais
// suporta campos com aspas e vírgulas dentro dos campos
int csv_split(const char *line, char ***fields, size_t *count) {
    if (!line || !fields || !count) return -1;
    
    *fields = NULL;
    *count = 0;
    
    // começa com capacidade para 16 campos
    size_t capacity = 16;
    char **result = malloc(capacity * sizeof(char*));
    if (!result) return -1;
    
    const char *p = line;
    
    while (*p) {
        // pular espaços no inicio
        while (*p && isspace((unsigned char)*p)) p++;
        if (!*p) break;
        
        const char *start;
        const char *end;
        
        // verificar se o campo tem aspas
        if (*p == '"') {
            p++; // passar a aspa inicial
            start = p;
            while (*p && *p != '"') p++;
            end = p;
            if (*p == '"') p++; // passar a aspa final
        } else {
            // campo sem aspas
            start = p;
            while (*p && *p != ',') p++;
            end = p;
        }
        
        // alocar espaço e copiar o campo
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
        
        // passar a vírgula se houver
        if (*p == ',') p++;
    }
    
    *fields = result;
    return 0;
}

// liberta a memória dos campos
void csv_free_fields(char **fields, size_t count) {
    if (!fields) return;
    for (size_t i = 0; i < count; i++) {
        free(fields[i]);
    }
    free(fields);
}