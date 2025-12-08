#ifndef CSV_H
#define CSV_H

#include <stddef.h>
#include <glib.h>

gchar **parse_csv_line(const gchar *line);
void liberta_ifcampos (char **campos);

#endif
