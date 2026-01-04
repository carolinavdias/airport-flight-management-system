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
 * =====================================================
 * QUERY 4
 * ===================================================== */

 /** 
 * Passageiro que mais vezes apareceu no Top 10 semanal
 * de gastos em reservas.
 *
 * Nota:
 * O enunciado da Query 4 NÃO menciona exclusão de voos
 * cancelados, pelo que estes são considerados.
 */

/** 
 * =====================================================
 * FUNÇÕES AUXILIARES PARA DATAS
 * =====================================================
 */

/** 
 * Verifica se um ano é bissexto.
 */

static int eh_bissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

/** 
 * Converte uma data (ano, mês, dia) para o número total
 * de dias desde 01-01-0001.
 *
 * Implementação em O(1), sem ciclos.
 */

static long data_para_dias(int ano, int mes, int dia) {
    // Fórmula O(1) em vez de loop
    int a = ano - 1;
    long dias_anos = a * 365L + a/4 - a/100 + a/400;

    int dias_mes_acum[] = 
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};

    long dias_meses = dias_mes_acum[mes - 1];
    if (mes > 2 && eh_bissexto(ano)) dias_meses++;
    
    return dias_anos + dias_meses + dia;
}

/**
 * Converte uma string no formato YYYY-MM-DD
 * para o número de dias desde a época.
 *
 * Retorna -1 se o formato for inválido.
 */

static long parse_data_str(const char *str) {
    int ano, mes, dia;
    if (sscanf(str, "%d-%d-%d", &ano, &mes, &dia) != 3) {
        return -1;
    }
    return data_para_dias(ano, mes, dia);
}

/** 
 * =====================================================
 * ESTRUTURAS AUXILIARES
 * ===================================================== 
 */

/**
 * Estrutura usada para contar quantas vezes
 * um passageiro aparece no Top 10 semanal.
 */

typedef struct { 
    GHashTable *contagem;   /**< Tabela de dispersão que mapeia doc_number -> count. */ 
    long inicio_dias;       /**< Data inicial do intervalo, representada em dias desde uma época definida. */ 
    long fim_dias;          /**< Data final do intervalo, representada em dias desde a mesma época. */ 
} DadosTop10;

/**
 * =====================================================
 * FUNÇÕES AUXILIARES DO CACHE TOP 10
 * =====================================================
 */

/*
 * Verifica se uma semana (domingo a sábado) interseta
 * o intervalo de datas fornecido.
 */

static int semana_no_intervalo_v2(long id_semana, long inicio, long fim) {
    if (inicio < 0 || fim < 0) return 1;
    long dom = id_semana, sab = id_semana + 6;
    
    return (dom <= fim && sab >= inicio);
}

/** 
 * Callback chamada para cada entrada do Top 10 semanal
 * pré-calculado no gestor de reservas.
 *
 * Incrementa a contagem do passageiro se a semana
 * estiver dentro do intervalo considerado.
 */

static void conta_top10(long id_semana,
                        const char *doc_number,
                        void *user_data) {

    DadosTop10 *d = user_data;

    if (!semana_no_intervalo(id_semana, d->inicio_dias, d->fim_dias))
        return;

    int *c = g_hash_table_lookup(d->contagem, doc_number);
    if (!c) {
        c = g_new(int, 1);
        *c = 0;
        g_hash_table_insert(d->contagem, g_strdup(doc_number), c);
    }
    (*c)++;
}

/** 
 * =====================================================
 * QUERY 4 — IMPLEMENTAÇÃO PRINCIPAL
 * ===================================================== */

char *query4(const char *linhaComando,
             GestorPassengers *gestorPassageiros,
             GestorReservations *gestorReservas) {
    
     /** 
     * =================================================
     * FASE 0: Parse do filtro opcional de datas
     * =================================================
     */

    char data_ini_str[32] = "";
    char data_fim_str[32] = "";
    long inicio_dias = -1, fim_dias = -1;
    
    if (linhaComando && linhaComando[0] != '\0') {
        if (sscanf(linhaComando, "%31s %31s", data_ini_str, data_fim_str) == 2) {
            inicio_dias = parse_data_str(data_ini_str);
            fim_dias = parse_data_str(data_fim_str);
        }
    }
    
    /**  
     * =================================================
     * FASE 1 + 2: Usar cache de Top 10 semanal
     * =================================================
     */

    GHashTable *contagem_top10 = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, g_free
    );
    
    DadosTop10 dados_top = {
        .contagem = contagem_top10,
        .inicio_dias = inicio_dias,
        .fim_dias = fim_dias
    };
    
    gestor_reservations_foreach_top10(gestorReservas, conta_top10, &dados_top);

    /**  
     * =================================================
     * FASE 3: Encontrar passageiro vencedor
     * =================================================
     */

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
    
    /** 
     * =================================================
     * FASE 4: Construção do resultado
     * =================================================
     */
    
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
