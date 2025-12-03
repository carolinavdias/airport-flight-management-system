#include "airports.h"
#include <glib.h>

void libertaAeroporto(void *data) {
    Aeroporto *a = data;
    if (!a) return;
    g_free(a->code_IATA);
    g_free(a->name);
    g_free(a->city);
    g_free(a->country);
    g_free(a->code_ICAO);
    g_free(a);
}