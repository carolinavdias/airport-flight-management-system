#ifndef PROCESSADOR_OUTPUT_H
#define PROCESSADOR_OUTPUT_H

#include <stdio.h>

typedef struct ErrorWriter ErrorWriter;

// Struct para gerir erros
struct ErrorWriter;

// Funções para gerir ficheiros de erro CSV
ErrorWriter* processador_output_create_error_file(const char *entity_name, 
                                                  const char *header);

void processador_output_write_error(ErrorWriter *writer, const char *line);

void processador_output_close_error_file(ErrorWriter *writer);

#endif
