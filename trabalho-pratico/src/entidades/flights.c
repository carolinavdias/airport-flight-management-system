#include "entidades/flights.h"
#include <glib.h>

void libertaVoo(void *data) {
    Voo *v = data;
    if (!v) return;
    g_free(v->flight_id);
    g_free(v->gate);
    g_free(v->code_origin);
    g_free(v->code_destination);
    g_free(v->id_aircraft);
    g_free(v->airline);
    g_free(v->tracking_url);
    g_free(v);
}