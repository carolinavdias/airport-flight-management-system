#ifndef CSV_H
#define CSV_H

#include <stddef.h>
#include <glib.h>

// função para dividir linha CSV em campos
int csv_split(const char *line, char ***fields, size_t *count);

// função para libertar campos
void csv_free_fields(char **fields, size_t count);

#endif