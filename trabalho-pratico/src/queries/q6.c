#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

// Callback para iterar passageiros
static void processa_passageiro_q6(Passageiros *p, void *user_data) {
    DadosQ6 *dados = (DadosQ6 *)user_data;

    const char *p_nat = passenger_get_nacionalidade(p);
    if (!p_nat || strcmp(p_nat, dados->nacionalidade) != 0) return;

    const char *doc = passenger_get_id(p);
    if (!doc) return;

    GSList *reservas = gestor_reservations_get_by_passenger(dados->gestorReservas, doc);
    for (GSList *it = reservas; it; it = it->next) {
        Reservas *r = it->data;
        char **voos = r_get_lista_voos_reserv(r);
        int n = r_get_lista_n_voos(r);

        for (int i = 0; i < n; i++) {
            const char *flight_id = voos[i];
            Voo *v = gestor_flights_procura(dados->gestorVoos, flight_id);
            if (!v) continue;
            if (voo_get_status(v) == ESTADO_CANCELLED) continue;

            const char *destino = voo_get_code_destination(v);
            if (!destino) continue;

            // Atualiza contagem
            int count = GPOINTER_TO_INT(g_hash_table_lookup(
                        dados->contagens_destinos, destino));
            count++;

            g_hash_table_insert(dados->contagens_destinos,
                                (gpointer)destino,
                                GINT_TO_POINTER(count));
        }
    }
    g_slist_free(reservas);
}

//Query 6
char *query6(const char *nationality, 
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas) {
    if (!nationality || !gestorPassageiros || !gestorVoos || !gestorReservas)
        return strdup("\n");

    GHashTable *contagens = g_hash_table_new(g_str_hash, g_str_equal);

    DadosQ6 dados = {
        .nacionalidade = nationality,
        .contagens_destinos = contagens,
        .gestorVoos = gestorVoos,
        .gestorReservas = gestorReservas  
    };

    gestor_passengers_foreach(gestorPassageiros,
                              processa_passageiro_q6,
                              &dados);

    const char *melhor_aeroporto = NULL;
    int max_count = 0;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char *aeroporto = key;
        int count = GPOINTER_TO_INT(value);

        if (count > max_count ||
            (count == max_count && melhor_aeroporto &&
             strcmp(aeroporto, melhor_aeroporto) < 0)) {
            max_count = count;
            melhor_aeroporto = aeroporto;
        }
    }

    g_hash_table_destroy(contagens);

    if (!melhor_aeroporto)
        return strdup("\n");

    char *resultado = NULL;
    if (asprintf(&resultado, "%s;%d\n", melhor_aeroporto, max_count) == -1) {
        return strdup("\n");
    }
    return resultado;
}