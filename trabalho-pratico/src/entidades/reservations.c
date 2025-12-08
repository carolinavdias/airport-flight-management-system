#include "entidades/reservations.h"
#include <glib.h>
#include <stdlib.h>

void libertaReserva(void *data) {
    Reservas *a = data;
    if (!a) return;
    g_free(a->id_reserva);
    if (a->reserva_lista.lista_voos_reservados) {
        for (int i = 0; i < a->reserva_lista.n_voos; i++) {
            g_free(a->reserva_lista.lista_voos_reservados[i]);
        }
        free(a->reserva_lista.lista_voos_reservados);
    }
    g_free(a->lugar_reservado);
    g_free(a->qr_code);
    g_free(a);
}