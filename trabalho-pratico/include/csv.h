#ifndef CSV_H
#define CSV_H

#include <stddef.h>
#include <glib.h>

// divide linha CSV em campos
int csv_split(const char *line, char ***fields, size_t *count);

// liberta campos alocados
void csv_free_fields(char **fields, size_t count);



//gchar **parse_csv_line(const gchar *line);
//void liberta_ifcampos (char **campos);


#endif
