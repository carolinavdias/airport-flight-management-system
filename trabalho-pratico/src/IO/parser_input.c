#define _POSIX_C_SOURCE 200809L

#include "IO/parser_input.h"
#include "utils/utils.h"
#include <stdio.h>
#include <string.h>

/** Tamanho máximo de uma linha lida do CSV. */

#define MAX_LINHA 2000

/**
 * Lê o header do CSV.
 */

int parser_input_read_header(const char *dataset_dir, const char *filename, char *header_out, size_t size) {
    
    if (!dataset_dir || !filename || !header_out) return 0;

    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dataset_dir, filename);
    FILE *ficheiro = fopen(path, "r");
    if (ficheiro == NULL) return 0;
    
    char buffer[MAX_LINHA];
    
    if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) {
        fclose(ficheiro);
        return 0;
    }
    
    buffer[strcspn(buffer, "\n")] = '\0';
    strncpy(header_out, buffer, size - 1);
    header_out[size - 1] = '\0';
    
    fclose(ficheiro);
    return 1;
}

/**
 * Lê o CSV linha a linha e aplica um callback.
 */

int parser_input_read_csv(const char *dataset_dir, const char *filename, LineCallback callback, void *user_data) {
    
    if (!dataset_dir || !filename || !callback) return 0;
    
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", dataset_dir, filename);
    FILE *ficheiro = fopen(path, "r");
    if (ficheiro == NULL) return 0;
    
    char buffer[MAX_LINHA];
    
    //descarta header
    if (!fgets(buffer, sizeof(buffer), ficheiro)) {
        fclose(ficheiro);
        return 0;
    }
    
    while (fgets(buffer, sizeof(buffer), ficheiro)) { 
        
        buffer[strcspn(buffer, "\n")] = '\0';
        
        if (buffer[0] == '\0') continue;

        char **campos = NULL;
        size_t n_campos = 0;
        
        if (csv_split(buffer, &campos, &n_campos) == 0) {
            
            callback(campos, n_campos, user_data);

            csv_free_fields(campos, n_campos);
        }
    }
    
    fclose(ficheiro);
    return 1;
}