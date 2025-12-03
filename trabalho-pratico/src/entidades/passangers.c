#include "passangers.h"
#include <glib.h>

void libertaPassageiro(void *data) {
    Passageiros *a = data;
    if (!a) return;
    g_free(a->primeiro_nome);
    g_free(a->ultimo_nome);
    g_free(a->nacionalidade);
    g_free(a->email_passageiro);
    g_free(a->telefone_passageiro);
    g_free(a->morada_passageiro);
    g_free(a->fotografia_passageiro);
    g_free(a);
}