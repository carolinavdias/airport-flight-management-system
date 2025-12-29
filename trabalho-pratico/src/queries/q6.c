#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>  
#include "queries/q6.h"
#include "entidades/passengers.h"
#include "entidades/flights.h"
#include "entidades/reservations.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"

typedef struct {
    const char *nacionalidade;
    GHashTable *contagens_destinos; 
    GestorFlights *gestorVoos;
    GestorReservations *gestorReservas;
} DadosQ6;

static void processa_passageiro_q6(Passageiros *p, void *user_data) {
    DadosQ6 *dados = (DadosQ6 *)user_data;

    // CORRIGIDO: passenger_get_nacionalidade() retorna cópia, precisa free()
    char *p_nat = passenger_get_nacionalidade(p);
    if (!p_nat || strcmp(p_nat, dados->nacionalidade) != 0) {
        g_free(p_nat);
        return;
    }
    g_free(p_nat);  // Libertar após validação

    const char *id = passenger_get_id(p);
    GSList *reservas = gestor_reservations_get_by_passenger(dados->gestorReservas, id);

    for (GSList *it = reservas; it; it = it->next) {
        Reservas *r = it->data;
        char **lista_voos = r_get_lista_voos_reserv(r);
        int n_voos = r_get_lista_n_voos(r);

        for (int i = 0; i < n_voos; i++) {
            Voo *voo = gestor_flights_procura(dados->gestorVoos, lista_voos[i]);
            if (!voo) continue;

            const char *destino = voo_get_code_destination(voo);
            if (!destino || !destino[0]) continue;

            int count = GPOINTER_TO_INT(g_hash_table_lookup(
                dados->contagens_destinos, destino
            ));
            g_hash_table_insert(dados->contagens_destinos,
                                (gpointer)destino,
                                GINT_TO_POINTER(count + 1));
        }
    }

    g_slist_free(reservas);
}

char *query6(const char *param,
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas) {

    if (!param || !param[0]) return strdup("\n");

    GHashTable *contagens = g_hash_table_new(g_str_hash, g_str_equal);

    DadosQ6 dados = {
        .nacionalidade = param,
        .contagens_destinos = contagens,
        .gestorVoos = gestorVoos,
        .gestorReservas = gestorReservas
    };

    gestor_passengers_foreach(gestorPassageiros, processa_passageiro_q6, &dados);

    if (g_hash_table_size(contagens) == 0) {
        g_hash_table_destroy(contagens);
        return strdup("\n");
    }

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char *aeroporto = key;
        int count = GPOINTER_TO_INT(value);
        printf("%s: %d\n", aeroporto, count);  // DEBUG
    }

    // TODO: Formatar saída correta (top 3, ordenado)
    char *resultado = strdup("TODO: implementar formatação Q6\n");

    g_hash_table_destroy(contagens);

    return resultado;
}