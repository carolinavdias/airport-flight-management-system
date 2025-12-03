#include "aircrafts.h"
#include <glib.h>

void libertaAeronave(void *data) {
    Aeronave *a = data;
    if (!a) return;
    g_free(a->identifier);
    g_free(a->manufacturer);
    g_free(a->model);
    g_free(a);
}