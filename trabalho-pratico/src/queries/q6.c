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

/**
 * Q6: Aeroporto de destino mais comum para passageiros de uma nacionalidade
 * 
 * Conforme enunciado:
 * - Considera apenas voos com estado DIFERENTE de cancelado
 * - Em caso de empate: menor código IATA (ordem lexicográfica)
 * - Se não existem passageiros da nacionalidade: linha vazia
 * 
 * Output: code;arrival_count (usa vírgulas, interpreter converte)
 */

typedef struct {
    const char *nacionalidade;
    GHashTable *contagens_destinos;  // airport_code -> int count
    GestorFlights *gestorVoos;
    GestorReservations *gestorReservas;
} DadosQ6;

/**
 * @brief Processa um passageiro - conta destinos das suas reservas
 */
static void processa_passageiro_q6(Passageiros *p, void *user_data) {
    DadosQ6 *dados = (DadosQ6 *)user_data;

    // Obter nacionalidade (retorna cópia!)
    char *p_nat = passenger_get_nacionalidade(p);
    if (!p_nat || strcmp(p_nat, dados->nacionalidade) != 0) {
        g_free(p_nat);
        return;
    }
    g_free(p_nat);

    // Obter ID do passageiro (retorna const char*, não precisa free)
    const char *id = passenger_get_id(p);
    if (!id) return;
    
    // Obter reservas deste passageiro
    GSList *reservas = gestor_reservations_get_by_passenger(dados->gestorReservas, id);

    for (GSList *it = reservas; it; it = it->next) {
        Reservas *r = it->data;
        
        // Usar função ENCAPSULADA para obter voos
        int n_voos = r_get_lista_n_voos(r);
        
        for (int i = 0; i < n_voos; i++) {
            // Usar r_get_voo_por_indice() em vez de r_get_lista_voos_reserv()
            char *voo_id = r_get_voo_por_indice(r, i);
            if (!voo_id) continue;
            
            Voo *voo = gestor_flights_procura(dados->gestorVoos, voo_id);
            g_free(voo_id);  // Libertar cópia!
            
            if (!voo) continue;
            
            // Q6 diz: "voos com estado diferente de cancelado"
            Estado status = voo_get_status(voo);
            if (status == ESTADO_CANCELLED) continue;

            // Obter destino
            const char *destino = voo_get_code_destination(voo);
            if (!destino || !destino[0]) continue;

            // Incrementar contagem
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

/**
 * @brief Query 6 - Destino mais comum por nacionalidade
 */
char *query6(const char *param,
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas) {

    if (!param || !param[0]) return strdup("\n");

    // Criar tabela de contagens
    GHashTable *contagens = g_hash_table_new(g_str_hash, g_str_equal);

    DadosQ6 dados = {
        .nacionalidade = param,
        .contagens_destinos = contagens,
        .gestorVoos = gestorVoos,
        .gestorReservas = gestorReservas
    };

    // Iterar sobre todos os passageiros
    gestor_passengers_foreach(gestorPassageiros, processa_passageiro_q6, &dados);

    // Verificar se há resultados
    if (g_hash_table_size(contagens) == 0) {
        g_hash_table_destroy(contagens);
        return strdup("\n");
    }

    // Encontrar aeroporto com mais passageiros
    // Em caso de empate: menor código IATA (ordem lexicográfica)
    const char *melhor_aeroporto = NULL;
    int max_count = 0;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char *aeroporto = key;
        int count = GPOINTER_TO_INT(value);
        
        if (count > max_count || 
            (count == max_count && melhor_aeroporto && strcmp(aeroporto, melhor_aeroporto) < 0)) {
            max_count = count;
            melhor_aeroporto = aeroporto;
        }
    }

    // Formatar resultado com VÍRGULAS (interpreter converte para ; ou =)
    char *resultado = NULL;
    if (melhor_aeroporto && max_count > 0) {
        if (asprintf(&resultado, "%s;%d\n", melhor_aeroporto, max_count) == -1) {
            resultado = strdup("\n");
        }
    } else {
        resultado = strdup("\n");
    }

    g_hash_table_destroy(contagens);

    return resultado;
}