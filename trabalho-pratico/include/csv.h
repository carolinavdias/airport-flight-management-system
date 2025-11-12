#ifndef CSV_H
#define CSV_H

#include <stddef.h>

// Parser CSV (RFC-4180 simplificado):
// - separador vírgula ','
// - campos podem vir entre aspas; aspas duplas escapadas "" => "
// - preserva campos vazios
// Retorna 0 em sucesso; out_fields deve ser libertado com csv_free_fields
int csv_split(const char *line, char ***out_fields, size_t *out_n);
void csv_free_fields(char **fields, size_t n);
gchar **parse_csv_line(const gchar *line);

#endif // CSV_H
