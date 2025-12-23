#define _XOPEN_SOURCE 700

#include "queries/q3.h"
#include "validacoes/validacoes_flights.h"
#include "entidades/flights.h"
#include "entidades/airports.h"

typedef struct contagens2 {
    char *code;
    int count;
} Contagens2;

// =====================================================
// QUERY 3
// =====================================================

void query3(const char *data_inicio, const char *data_fim,GHashTable *tabelaVoos,GHashTable *tabelaAeroportos,FILE *out)
{
    if (!data_inicio || !data_fim || !tabelaVoos || !tabelaAeroportos) {
        fprintf(out, "\n");
        return;
    }

    // =====================================================
    // OTIMIZAÇÃO 1: Converter datas UMA ÚNICA VEZ
    // =====================================================
    
    // Usar a função valida_DataH do validators.h
    if (!valida_DataH(data_inicio) ||
        !valida_DataH(data_fim)) {
        fprintf(out, "\n");
        return;
    }

    long long t_inicio = converte_dataH(data_inicio);
    long long t_fim = converte_dataH(data_fim);

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
        if (voo_get_status(v) == ESTADO_CANCELLED) continue;  // Cancelado (constante do q3.h)
        if (!voo_get_code_origin(v)) continue;                 // Sem origem

        long long t_partida = voo_get_actual_departure(v);

        // Verificar intervalo (comparação de inteiros é MUITO rápida)
        if (t_partida < t_inicio || t_partida > t_fim)
            continue;

        // =====================================================
        // OTIMIZAÇÃO 4: Incrementar DIRETO na hash table
        // =====================================================
        int *ptr = g_hash_table_lookup(contagens, (gpointer)voo_get_code_origin(v));

        if (!ptr) {
            int *novo = g_new(int, 1);
            *novo = 1;
            g_hash_table_insert(contagens, (gpointer)voo_get_code_origin(v), novo);
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
                airport_get_code_IATA(a), airport_get_name(a), airport_get_city(a), airport_get_country(a), max);
        } else {
            fprintf(out, "\n");
        }
    }

    g_hash_table_destroy(contagens);
}