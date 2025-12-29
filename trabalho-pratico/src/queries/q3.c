#define _XOPEN_SOURCE 700

#include "queries/q3.h"
#include "validacoes/validacoes_flights.h"
#include "entidades/flights.h"
#include "entidades/airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <glib.h>

typedef struct contagens2 {
    char *code;
    int count;
} Contagens2;

// =====================================================
// INÍCIO DO INTERVALO - encontra primeiro voo >= data
// =====================================================
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

// =====================================================
// QUERY 3 - USA ARRAY DO GESTOR (JÁ ORDENADO)
// =====================================================
char *query3(const char *data_inicio, const char *data_fim, 
             GestorFlights *gestorVoos, GestorAirports *gestorAeroportos) {

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

    GHashTable *contagens = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, g_free);

    //itera apenas voos no intervalo
    for (int i = inicio; i < num_voos; i++) {
        Voo *v = voos_ordenados[i];
        long long t = voo_get_actual_departure(v);

        if (t > t_fim) break;

        const char *origem = voo_get_code_origin(v);
        if (!origem) continue;

        int *c = g_hash_table_lookup(contagens, origem);
        if (!c) {
            int *novo = g_new(int, 1);
            *novo = 1;
            g_hash_table_insert(contagens, (gpointer)origem, novo);
        } else {
            (*c)++;
        }
    }

    //encontra aeroporto com mais partidas
    const char *melhor = NULL;
    int max = 0;

    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        int count = *((int *)value);
        char *code = key;

        if (count > max || (count == max && melhor && strcmp(code, melhor) < 0)) {
            max = count;
            melhor = code;
        }
    }

    char *resultado = NULL;

    if (melhor && max > 0) {
        //usa função do gestor (encapsulamento)
        Aeroporto *a = gestor_airports_procura(gestorAeroportos, melhor);

        if (a) {
            //guardar getters para libertar
            char *iata = airport_get_code_IATA(a);
            char *name = airport_get_name(a);
            char *city = airport_get_city(a);
            char *country = airport_get_country(a);
            
            if (asprintf(&resultado, "%s;%s;%s;%s;%d\n",
                        iata,
                        name,
                        city,
                        country,
                        max) == -1) {
                resultado = strdup("\n");
            }
            
            //libertar strings
            g_free(iata);
            g_free(name);
            g_free(city);
            g_free(country);
        } else {
            resultado = strdup("\n");
        }
    } else {
        resultado = strdup("\n");
    }

    g_hash_table_destroy(contagens);
    return resultado ? resultado : strdup("\n");
}