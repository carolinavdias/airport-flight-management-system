#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include "queries/q4.h"
#include "entidades/passengers.h"
#include "entidades/flights.h"
#include "entidades/reservations.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * Q4: Passageiro que mais vezes apareceu no top 10 de gastos semanais
 * 
 * Conforme enunciado:
 * - Semana: domingo a sábado
 * - Gasto: soma dos preços das reservas do passageiro
 * - Data: departure (partida ESTIMADA) do voo
 * - Filtro: se captar semana parcialmente, essa semana também conta
 * - Empate: menor document_number
 * 
 * NOTA: O enunciado da Q4 NÃO diz para excluir voos cancelados!
 *       (Q1, Q3 e Q6 dizem explicitamente, Q4 não diz)
 */

/* =====================================================
 * FUNÇÕES PARA CÁLCULO DE DATAS E SEMANAS
 * ===================================================== */

/**
 * @brief Verifica se ano é bissexto
 */
static int eh_bissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

/**
 * @brief Converte data para número de dias desde 1 Jan 0001
 */
static long data_para_dias(int ano, int mes, int dia) {
    // Fórmula O(1) em vez de loop
    int a = ano - 1;
    long dias_anos = a * 365L + a/4 - a/100 + a/400;
    int dias_mes_acum[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    long dias_meses = dias_mes_acum[mes - 1];
    if (mes > 2 && eh_bissexto(ano)) dias_meses++;
    return dias_anos + dias_meses + dia;
}

/**
 * @brief Converte string "YYYY-MM-DD" para dias desde época
 * @return -1 se formato inválido
 */
static long parse_data_str(const char *str) {
    int ano, mes, dia;
    if (sscanf(str, "%d-%d-%d", &ano, &mes, &dia) != 3) {
        return -1;
    }
    return data_para_dias(ano, mes, dia);
}

/* =====================================================
 * ESTRUTURAS AUXILIARES
 * ===================================================== */

typedef struct {
    char *doc_number;
    double total_gasto;
} GastoPassageiro;


/* =====================================================
 * DADOS PARA ITERAÇÃO
 * ===================================================== */

typedef struct {
    GHashTable *semanas;
    GestorFlights *gestorVoos;
    long inicio_dias;
    long fim_dias;
} DadosRecolha;

/* =====================================================
 * QUERY 4 - IMPLEMENTAÇÃO PRINCIPAL
 * ===================================================== */

/* =====================================================
 * NOVA VERSÃO OTIMIZADA - USA CACHE TOP10
 * ===================================================== */

typedef struct {
    GHashTable *contagem;  // doc_number -> count
    long inicio_dias;
    long fim_dias;
} DadosTop10;

static int semana_no_intervalo_v2(long id_semana, long inicio, long fim) {
    if (inicio < 0 || fim < 0) return 1;
    long dom = id_semana, sab = id_semana + 6;
    return (dom <= fim && sab >= inicio);
}

static void conta_top10(long id_semana, const char *doc_number, void *user_data) {
    DadosTop10 *d = user_data;
    if (!semana_no_intervalo_v2(id_semana, d->inicio_dias, d->fim_dias)) return;
    int *c = g_hash_table_lookup(d->contagem, doc_number);
    if (!c) { c = g_new(int, 1); *c = 0; g_hash_table_insert(d->contagem, g_strdup(doc_number), c); }
    (*c)++;
}

char *query4(const char *linhaComando,
             GestorPassengers *gestorPassageiros,
             //GestorFlights *gestorVoos,
             GestorReservations *gestorReservas) {
    
    // =========================================================
    // FASE 0: Parse do filtro opcional [begin_date end_date]
    // =========================================================
    char data_ini_str[32] = "";
    char data_fim_str[32] = "";
    long inicio_dias = -1, fim_dias = -1;
    
    if (linhaComando && linhaComando[0] != '\0') {
        if (sscanf(linhaComando, "%31s %31s", data_ini_str, data_fim_str) == 2) {
            inicio_dias = parse_data_str(data_ini_str);
            fim_dias = parse_data_str(data_fim_str);
        }
    }
    
    // =========================================================
    // FASE 1+2: Usar cache pre-calculado de top 10
    // =========================================================
    GHashTable *contagem_top10 = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, g_free
    );
    
    DadosTop10 dados_top = {
        .contagem = contagem_top10,
        .inicio_dias = inicio_dias,
        .fim_dias = fim_dias
    };
    
    gestor_reservations_foreach_top10(gestorReservas, conta_top10, &dados_top);

    // FASE 3: Encontrar passageiro com mais aparições
    // =========================================================
    if (g_hash_table_size(contagem_top10) == 0) {
        g_hash_table_destroy(contagem_top10);
        
        return strdup("\n");
    }
    
    const char *melhor_doc = NULL;
    int max_count = 0;
    
    GHashTableIter iter_cnt;
    gpointer key_doc, val_cnt;
    g_hash_table_iter_init(&iter_cnt, contagem_top10);
    
    while (g_hash_table_iter_next(&iter_cnt, &key_doc, &val_cnt)) {
        int count = *((int *)val_cnt);
        const char *doc = key_doc;
        
        // Mais aparições OU empate com menor doc_number
        if (count > max_count || 
            (count == max_count && melhor_doc && strcmp(doc, melhor_doc) < 0)) {
            max_count = count;
            melhor_doc = doc;
        }
    }
    
    // =========================================================
    // FASE 4: Formatar resultado
    // =========================================================
    char *resultado = NULL;
    
    if (melhor_doc && max_count > 0) {
        // Procurar passageiro usando a string com 9 dígitos
        Passageiros *p = gestor_passengers_procura(gestorPassageiros, melhor_doc);
        
        if (p) {
            // Obter dados (alguns getters retornam cópias!)
            char *primeiro = passenger_get_primeiro(p);   // cópia - libertar!
            char *ultimo = passenger_get_ultimo(p);       // cópia - libertar!
            char *nacionalidade = passenger_get_nacionalidade(p);  // cópia - libertar!
            int dob_int = passenger_get_data(p);          // int - não libertar
            
            // Formatar dob: YYYYMMDD -> YYYY-MM-DD
            char dob_str[16];
            snprintf(dob_str, sizeof(dob_str), "%04d-%02d-%02d",
                    dob_int / 10000, 
                    (dob_int / 100) % 100, 
                    dob_int % 100);
            
            // Output com VÍRGULAS (o interpreter converte para ; ou =)
            // Formato: document_number,first_name,last_name,dob,nationality,count_top_10
            if (asprintf(&resultado, "%s;%s;%s;%s;%s;%d\n",
                        melhor_doc, 
                        primeiro, 
                        ultimo, 
                        dob_str, 
                        nacionalidade, 
                        max_count) == -1) {
                resultado = strdup("\n");
            }
            
            // Libertar cópias!
            g_free(primeiro);
            g_free(ultimo);
            g_free(nacionalidade);
        } else {
            resultado = strdup("\n");
        }
    } else {
        resultado = strdup("\n");
    }
    
    // Limpeza
    g_hash_table_destroy(contagem_top10);
    
    
    return resultado;
}
