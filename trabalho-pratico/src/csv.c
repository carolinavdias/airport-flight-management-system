#define _POSIX_C_SOURCE 200809L
#include "csv.h"
#include <stdlib.h>
#include <string.h>
#include <glib.h>

//Funciona mas é lenta
gchar **parse_csv_line(const gchar *line) {
    GPtrArray *arr = g_ptr_array_new_with_free_func(g_free);
    const gchar *p = line;

    while (*p != '\0') {
        /* must start with quote */
        if (*p != '"') {
            g_ptr_array_free(arr, TRUE);
            return NULL;
        }
        p++; /* skip opening quote */

        const gchar *start = p;
        while (*p != '\0' && *p != '"') p++;
        if (*p != '"') { g_ptr_array_free(arr, TRUE); return NULL; }

        size_t len = (size_t)(p - start);
        gchar *field = g_malloc(len + 1);
        memcpy(field, start, len);
        field[len] = '\0';
        g_ptr_array_add(arr, field);

        p++; /* skip closing quote */
        if (*p == ',') { p++; continue; }
        while (*p == '\r') p++;
        if (*p == '\n') p++;
        break;
    }

    g_ptr_array_add(arr, NULL);
    return (gchar **)g_ptr_array_free(arr, FALSE);
}

void liberta_ifcampos (char **campos) {
    if (!campos) return;
    g_strfreev(campos);
}
