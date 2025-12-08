#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include "queries/q3.h"
#include "validacoes/validacoes_flights.h"
#include "entidades/flights.h"     
#include "entidades/airports.h" 
// q1 removido
// validators removido

// =====================================================
// VALIDAÇÕES (já declarada no header)
// =====================================================

int idVooValido(const char *id) {
    if (!id) return 0;
    int len = strlen(id);
    if (len < 5 || len > 8) return 0;

    int i = 0;
    if (!isupper((unsigned char)id[i++]) || !isupper((unsigned char)id[i++])) return 0;
    if (id[i] == '-') i++;
    for (; i < len; i++) {
        if (!isdigit((unsigned char)id[i])) return 0;
    }
    return 1;
}

// =====================================================
// QUERY 3 OTIMIZADA
// =====================================================

void query3(const char *data_inicio, const char *data_fim,
            GHashTable *tabelaVoos,
            GHashTable *tabelaAeroportos,
            FILE *out)
{
    if (!data_inicio || !data_fim || !tabelaVoos || !tabelaAeroportos) {
        fprintf(out, "\n");
        return;
    }

    // =====================================================
    // OTIMIZAÇÃO 1: Converter datas UMA ÚNICA VEZ
    // =====================================================
    long long t_inicio, t_fim;
    
    // Usar a função valida_DataH do validators.h
    if (!valida_DataH(data_inicio, &t_inicio) || 
        !valida_DataH(data_fim, &t_fim)) {
        fprintf(out, "\n");
        return;
    }

    // =====================================================
    // OTIMIZAÇÃO 2: Hash table SEM alocações desnecessárias
    // =====================================================
    GHashTable *contagens = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        NULL,       // NÃO liberta keys (são referências)
        g_free      // liberta int*
    );

    // =====================================================
    // OTIMIZAÇÃO 3: Iterar COM ITERATOR (mais rápido)
    // =====================================================
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, tabelaVoos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;

        // Filtros rápidos
        if (v->status == ESTADO_CANCELLED) continue;  // Cancelado (constante do q3.h)
        if (!v->code_origin) continue;                 // Sem origem

        int t_partida = v->actual_departure;

        // Verificar intervalo (comparação de inteiros é MUITO rápida)
        if (t_partida < t_inicio || t_partida > t_fim)
            continue;

        // =====================================================
        // OTIMIZAÇÃO 4: Incrementar DIRETO na hash table
        // =====================================================
        int *ptr = g_hash_table_lookup(contagens, v->code_origin);

        if (!ptr) {
            int *novo = g_new(int, 1);
            *novo = 1;
            g_hash_table_insert(contagens, v->code_origin, novo);
        } else {
            (*ptr)++;
        }
    }

    // =====================================================
    // OTIMIZAÇÃO 5: Encontrar máximo em UMA PASSAGEM
    // =====================================================
    const char *melhor = NULL;
    int max = 0;

    g_hash_table_iter_init(&iter, contagens);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *code = key;
        int count = *((int *)value);

        // Critério: maior count, ou empate → menor código
        if (count > max || (count == max && melhor && strcmp(code, melhor) < 0)) {
            max = count;
            melhor = code;
        }
    }

    // =====================================================
    // Output
    // =====================================================
    if (!melhor || max == 0) {
        fprintf(out, "\n");
    } else {
        Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, melhor);
        if (a) {
            fprintf(out, "%s,%s,%s,%s,%d\n",
                a->code_IATA, a->name, a->city, a->country, max);
        } else {
            fprintf(out, "\n");
        }
    }

    g_hash_table_destroy(contagens);
}