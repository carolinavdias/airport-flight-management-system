#ifndef CSV_H
#define CSV_H

#include <stddef.h>

// divide linha CSV em campos
int csv_split(const char *line, char ***fields, size_t *count);

// liberta campos alocados
void csv_free_fields(char **fields, size_t count);

#endif