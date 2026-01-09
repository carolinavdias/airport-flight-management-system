#define _XOPEN_SOURCE 700

#include "queries/q3.h"
#include "queries/q1.h"
#include "validacoes/validacoes_flights.h"
#include "validacoes/validacoes.h"
#include "entidades/flights.h"
#include "entidades/airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>

/**
 * Estrutura auxiliar para armazenar o número de partidas
 * e chegadas associadas a um aeroporto.
 */

typedef struct contagens_aeroporto {
    int partidas;   /**< Número total de voos que partiram do aeroporto. */
    int chegadas;   /**< Número total de voos que chegaram ao aeroporto. */
} ContagensAeroporto;

/**
 * =====================================================
 * FUNÇÃO AUXILIAR
 * ===================================================== */

/**
 * Encontra, através de pesquisa binária, o índice do primeiro voo
 * cuja data de partida real é maior ou igual a t_inicio.
 *
 * O array de voos deve estar previamente ordenado por data
 * de partida real.
 */

static int intervalo_inicio(Voo **voos_ordenados, int num_voos, long long t_inicio) {
    int esq = 0, dir = num_voos - 1;
    int resultado = num_voos;

    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        long long t = voo_get_actual_departure(voos_ordenados[meio]);

        if (t >= t_inicio) {
            resultado = meio;
            dir = meio - 1;
        } else {
            esq = meio + 1;
        }
    }

    return resultado;
}

/**
 * =====================================================
 * QUERY 3 — IMPLEMENTAÇÃO OTIMIZADA
 * ===================================================== */

 /**
 * Utiliza o array de voos ordenado fornecido pelo gestor,
 * evitando iterações desnecessárias sobre todo o conjunto
 * de voos.
 */

char *query3(const char *data_inicio, const char *data_fim,
             GestorFlights *gestorVoos, GestorAirports *gestorAeroportos) {

    // Validações iniciais
    if (!gestorVoos || !gestorAeroportos) {
        return strdup("\n");
    }

    if (!data_inicio || !data_fim) {
        return strdup("\n");
    }

    if (!valida_DataH(data_inicio) || !valida_DataH(data_fim)) {
        return strdup("\n");
    }

    //obtém array ordenado do gestor (já vem do parser ordenado)
    int num_voos;
    Voo **voos_ordenados = gestor_flights_get_array_ordenado(gestorVoos, &num_voos);

    if (!voos_ordenados || num_voos == 0) {
        return strdup("\n");
    }

    long long t_inicio = converte_dataH(data_inicio);
    long long t_fim = converte_dataH(data_fim);

    //busca binária no array (já ordenado)
    int inicio = intervalo_inicio(voos_ordenados, num_voos, t_inicio);

    GHashTable *contagens = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    //itera apenas voos no intervalo
    for (int i = inicio; i < num_voos; i++) {
        Voo *v = voos_ordenados[i];
        long long t = voo_get_actual_departure(v);

        if (t > t_fim) break;

        // Ignorar voos cancelados (conforme enunciado)
        if (voo_get_status(v) == ESTADO_CANCELLED) continue;

        const char *origem = voo_get_code_origin(v);
        const char *destino = voo_get_code_destination(v);

        //conta partidas
        if (origem) {
            ContagensAeroporto *c = g_hash_table_lookup(contagens, origem);
            if (!c) {
                c = g_new0(ContagensAeroporto, 1);
                g_hash_table_insert(contagens, g_strdup(origem), c);
            }
            c->partidas++;
        }

        //conta chegadas
        if (destino) {
            ContagensAeroporto *c = g_hash_table_lookup(contagens, destino);
            if (!c) {
                c = g_new0(ContagensAeroporto, 1);
                g_hash_table_insert(contagens, g_strdup(destino), c);
            }
            c->chegadas++;
        }
    }

    //encontra aeroporto com mais partidas
    const char *melhor = NULL;
    int max_total = 0;
    int melhor_partidas = 0;


    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        ContagensAeroporto *c = (ContagensAeroporto *)value;
        int total = c->partidas;  // Q3 conta só partidas
        char *code = key;

        if (total > max_total || (total == max_total && melhor && strcmp(code, melhor) < 0)) {
            max_total = total;
            melhor = code;
            melhor_partidas = c->partidas;

        }
    }

    char *resultado = NULL;

    if (melhor && max_total > 0) {

        //usa função do gestor (encapsulamento)
        Aeroporto *a = gestor_airports_procura(gestorAeroportos, melhor);

        if (a) {
            //guardar getters para libertar
            const char *iata = airport_get_code_IATA(a);
            const char *name = airport_get_name(a);
            const char *city = airport_get_city(a);
            const char *country = airport_get_country(a);

            if (asprintf(&resultado, "%s;%s;%s;%s;%d\n",
                        iata,
                        name,
                        city,
                        country,
                        melhor_partidas) == -1) {
                resultado = strdup("\n");
            }

        } else {
            resultado = strdup("\n");
        }
    } else {
        resultado = strdup("\n");
    }

    g_hash_table_destroy(contagens);
    return resultado ? resultado : strdup("\n");
}
