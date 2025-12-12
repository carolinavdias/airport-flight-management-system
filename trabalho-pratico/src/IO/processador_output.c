#include "IO/processador_output.h"
#include <stdlib.h>
#include <string.h>

struct ErrorWriter {
    FILE *file;
    int header_escrito;
};


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
    
    // if (ficheiro_erros && !header_escrito) {
    //     fputs(header, ficheiro_erros);
    //     fputc('\n', ficheiro_erros);
    //     header_escrito = 1;
    // }
    if (header && !writer->header_escrito) {
        fputs(header, writer->file);
        fputc('\n', writer->file);
        writer->header_escrito = 1;
    }
    
    return writer;
}

void processador_output_write_error(ErrorWriter *writer, const char *line) {
    if (!writer || !writer->file || !line) return;
    
    // if (ficheiro_erros) {
    //     fputs(buffer, ficheiro_erros);
    //     fputc('\n', ficheiro_erros);
    // }
    fputs(line, writer->file);
    fputc('\n', writer->file);
}

void processador_output_close_error_file(ErrorWriter *writer) {
    if (!writer) return;
    
    if (writer->file) {
        fclose(writer->file);
    }
    
    free(writer);
}
