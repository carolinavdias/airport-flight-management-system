#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queries/q6.h"
#include "entidades/passengers.h"
#include "entidades/flights.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"

typedef struct {
    const char *nacionalidade;
    GHashTable *contagens_destinos; 
    GestorFlights *gestorVoos;
} DadosQ6;

// Callback para iterar passageiros
static void processa_passageiro_q6(Passageiros *p, void *user_data) {
    DadosQ6 *dados = (DadosQ6 *)user_data;
    
    //filtra por nacionalidade
    const char *p_nat = passenger_get_nacionalidade(p);
    if (!p_nat || strcmp(p_nat, dados->nacionalidade) != 0) {
        return;
    }
    
    //obtem flight_id do passageiro
    const char *flight_id = passenger_get_id(p);
    if (!flight_id) return;
    
    //obtem voo usando o gestor
    Voo *v = gestor_flights_procura(dados->gestorVoos, flight_id);
    if (!v) return;
    
    //ignora voos cancelados
    if (voo_get_status(v) == ESTADO_CANCELLED) return;
    
    //obtem destino
    const char *destino = voo_get_code_destination(v);
    if (!destino) return;
    
    //incrementa contagem para este destino
    gpointer count_ptr = g_hash_table_lookup(dados->contagens_destinos, destino);
    int count = count_ptr ? GPOINTER_TO_INT(count_ptr) : 0;
    count++;
    
    //atualiza (a chave já existe na memória do voo, não duplicar)
    g_hash_table_insert(dados->contagens_destinos, (gpointer)destino, GINT_TO_POINTER(count));
}

//Query 6
char *query6(const char *nationality, 
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos) {
    
    if (!nationality || !gestorPassageiros || !gestorVoos) {
        return strdup("\n");
    }
    
    //cria hash table para contagens
    GHashTable *contagens = g_hash_table_new(g_str_hash, g_str_equal);
    
    //prepara dados para callback
    DadosQ6 dados = {
        .nacionalidade = nationality,
        .contagens_destinos = contagens,
        .gestorVoos = gestorVoos
    };
    
    //itera todos os passageiros
    gestor_passengers_foreach(gestorPassageiros, processa_passageiro_q6, &dados);
    
    //encontra aeroporto mais comum
    const char *melhor_aeroporto = NULL;
    int max_count = 0;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char *aeroporto = (const char *)key;
        int count = GPOINTER_TO_INT(value);
        
        //critério: maior count, ou empate -> menor código lexicográfico
        if (count > max_count || 
            (count == max_count && melhor_aeroporto && strcmp(aeroporto, melhor_aeroporto) < 0)) {
            max_count = count;
            melhor_aeroporto = aeroporto;
        }
    }
    
    g_hash_table_destroy(contagens);
    
    //constroi resultado
    if (!melhor_aeroporto || max_count == 0) {
        return strdup("\n");
    }
    
    char *resultado = NULL;
    int len = asprintf(&resultado, "%s;%d\n", melhor_aeroporto, max_count);
    
    if (len == -1) {
        return strdup("\n");
    }
    
    return resultado;
}