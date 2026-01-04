#include "IO/processador_output.h"
#include <stdlib.h>
#include <string.h>

/**
 * Estrutura interna do Writer.
 */

typedef struct ErrorWriter { 
    FILE *file;           /**< Apontador para o ficheiro onde os erros serão escritos. */ 
    int header_escrito;   /**< Flag que indica se o cabeçalho já foi escrito (1) ou não (0). */ 
} ErrorWriter;

/**
 * Cria ficheiro de erros.
 */

ErrorWriter* processador_output_create_error_file(const char *entity_name, 
                                                  const char *header) {
    if (!entity_name) return NULL;
    
    ErrorWriter *writer = malloc(sizeof(ErrorWriter));
    if (!writer) return NULL;
    
    writer->header_escrito = 0;
    
    char path[512];
    snprintf(path, sizeof(path), "resultados/%s_errors.csv", entity_name);
    writer->file = fopen(path, "w");
    
    if (!writer->file) {
        free(writer);
        return NULL;
    }
    
    if (header && !writer->header_escrito) {
        fputs(header, writer->file);
        fputc('\n', writer->file);
        writer->header_escrito = 1;
    }
    
    return writer;
}

/**
 * Escreve uma linha de erro no ficheiro.
 */

void processador_output_write_error(ErrorWriter *writer, const char *line) {
    if (!writer || !writer->file || !line) return;
    
    fputs(line, writer->file);
    fputc('\n', writer->file);
}

/**
 * Fecha o ficheiro e liberta a estrutura.
 */

void processador_output_close_error_file(ErrorWriter *writer) {
    if (!writer) return;
    
    if (writer->file) {
        fclose(writer->file);
    }
    
    free(writer);
}