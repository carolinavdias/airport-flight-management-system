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
 * @brief Fórmula de Sakamoto para dia da semana
 * @return 0=domingo, 1=segunda, ..., 6=sábado
 */
static int dia_da_semana(int ano, int mes, int dia) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (mes < 3) ano -= 1;
    return (ano + ano/4 - ano/100 + ano/400 + t[mes-1] + dia) % 7;
}

/**
 * @brief Verifica se ano é bissexto
 */
static int eh_bissexto(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}

/**
 * @brief Dias no mês
 */
static int dias_no_mes(int ano, int mes) {
    int dias[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    if (mes == 2 && eh_bissexto(ano)) return 29;
    return dias[mes - 1];
}

/**
 * @brief Converte data para número de dias desde 1 Jan 0001
 */
static long data_para_dias(int ano, int mes, int dia) {
    long total = 0;
    for (int a = 1; a < ano; a++) {
        total += eh_bissexto(a) ? 366 : 365;
    }
    for (int m = 1; m < mes; m++) {
        total += dias_no_mes(ano, m);
    }
    total += dia;
    return total;
}

/**
 * @brief Extrai ano, mês, dia de um datetime (formato YYYYMMDDHHmm)
 */
static void extrai_data(long long datetime, int *ano, int *mes, int *dia) {
    *ano = (int)(datetime / 100000000LL);
    *mes = (int)((datetime / 1000000LL) % 100);
    *dia = (int)((datetime / 10000LL) % 100);
}

/**
 * @brief Calcula ID único da semana (dias desde época do domingo dessa semana)
 */
static long obter_id_semana(long long datetime) {
    int ano, mes, dia;
    extrai_data(datetime, &ano, &mes, &dia);
    
    long dias = data_para_dias(ano, mes, dia);
    int dow = dia_da_semana(ano, mes, dia);  // 0=domingo
    
    // Retroceder para o domingo dessa semana
    return dias - dow;
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

/**
 * @brief Verifica se uma semana intersecta com o intervalo de filtro
 * Semana = [id_semana (domingo), id_semana + 6 (sábado)]
 */
static int semana_no_intervalo(long id_semana, long inicio_dias, long fim_dias) {
    // Sem filtro = todas as semanas
    if (inicio_dias < 0 || fim_dias < 0) return 1;
    
    long semana_domingo = id_semana;
    long semana_sabado = id_semana + 6;
    
    // Intersecta se: domingo <= fim_filtro E sábado >= inicio_filtro
    return (semana_domingo <= fim_dias && semana_sabado >= inicio_dias);
}

/* =====================================================
 * ESTRUTURAS AUXILIARES
 * ===================================================== */

typedef struct {
    char *doc_number;
    double total_gasto;
} GastoPassageiro;

/**
 * @brief Comparador: maior gasto primeiro, empate por doc_number crescente
 */
static int compara_gastos(const void *a, const void *b) {
    const GastoPassageiro *g1 = a;
    const GastoPassageiro *g2 = b;
    
    if (g1->total_gasto > g2->total_gasto) return -1;
    if (g1->total_gasto < g2->total_gasto) return 1;
    
    // Empate: menor document_number primeiro
    return strcmp(g1->doc_number, g2->doc_number);
}

/* =====================================================
 * DADOS PARA ITERAÇÃO
 * ===================================================== */

typedef struct {
    GHashTable *semanas;  // id_semana -> GHashTable(doc_number -> double*)
    GestorFlights *gestorVoos;
} DadosRecolha;

/**
 * @brief Callback para processar cada reserva
 */
static void processa_reserva(Reservas *r, void *user_data) {
    DadosRecolha *dados = user_data;
    
    // Obter primeiro voo da reserva usando função ENCAPSULADA
    char *voo_id = r_get_voo_por_indice(r, 0);
    if (!voo_id) return;
    
    Voo *voo = gestor_flights_procura(dados->gestorVoos, voo_id);
    g_free(voo_id);  // Libertar cópia!
    
    if (!voo) return;
    
    // NOTA: O enunciado da Q4 NÃO diz para excluir voos cancelados!
    // Diferente de Q1, Q3 e Q6 que dizem explicitamente.
    // Portanto, consideramos TODAS as reservas.
    
    // Obter departure (partida ESTIMADA, conforme enunciado)
    long long departure = voo_get_departure(voo);
    if (departure <= 0) return;
    
    // Calcular semana
    long id_semana = obter_id_semana(departure);
    
    // Obter ou criar tabela de gastos para esta semana
    GHashTable *gastos_semana = g_hash_table_lookup(dados->semanas, GINT_TO_POINTER(id_semana));
    if (!gastos_semana) {
        gastos_semana = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
        g_hash_table_insert(dados->semanas, GINT_TO_POINTER(id_semana), gastos_semana);
    }
    
    // Obter document_number (int) e formatar como string com 9 dígitos
    // O CSV tem "055895169", que é guardado como int 55895169
    // Para pesquisar no gestor_passengers, precisamos da string original
    int doc_int = r_get_id_pessoa_reservou(r);
    char doc_str[16];
    snprintf(doc_str, sizeof(doc_str), "%09d", doc_int);
    
    // Obter preço da reserva usando função NOVA
    double preco = r_get_preco(r);
    
    // Acumular gasto
    double *gasto = g_hash_table_lookup(gastos_semana, doc_str);
    if (!gasto) {
        gasto = g_new(double, 1);
        *gasto = 0.0;
        g_hash_table_insert(gastos_semana, g_strdup(doc_str), gasto);
    }
    *gasto += preco;
}

/* =====================================================
 * QUERY 4 - IMPLEMENTAÇÃO PRINCIPAL
 * ===================================================== */

char *query4(const char *linhaComando,
             GestorPassengers *gestorPassageiros,
             GestorFlights *gestorVoos,
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
    // FASE 1: Recolher gastos por passageiro, por semana
    // =========================================================
    GHashTable *semanas = g_hash_table_new_full(
        g_direct_hash, g_direct_equal, 
        NULL, (GDestroyNotify)g_hash_table_destroy
    );
    
    DadosRecolha dados = {
        .semanas = semanas,
        .gestorVoos = gestorVoos
    };
    
    // Usar função ENCAPSULADA para iterar
    gestor_reservations_foreach(gestorReservas, processa_reserva, &dados);
    
    // =========================================================
    // FASE 2: Calcular top 10 de cada semana (no intervalo)
    // =========================================================
    GHashTable *contagem_top10 = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, g_free
    );
    
    GHashTableIter iter_sem;
    gpointer key_sem, val_sem;
    g_hash_table_iter_init(&iter_sem, semanas);
    
    while (g_hash_table_iter_next(&iter_sem, &key_sem, &val_sem)) {
        long id_semana = GPOINTER_TO_INT(key_sem);
        GHashTable *gastos_semana = val_sem;
        
        // Verificar se semana intersecta com filtro
        if (!semana_no_intervalo(id_semana, inicio_dias, fim_dias)) {
            continue;
        }
        
        guint n_pass = g_hash_table_size(gastos_semana);
        if (n_pass == 0) continue;
        
        // Converter para array para ordenar
        GastoPassageiro *arr = g_new(GastoPassageiro, n_pass);
        
        GHashTableIter iter_g;
        gpointer key_doc, val_gasto;
        guint idx = 0;
        
        g_hash_table_iter_init(&iter_g, gastos_semana);
        while (g_hash_table_iter_next(&iter_g, &key_doc, &val_gasto)) {
            arr[idx].doc_number = key_doc;
            arr[idx].total_gasto = *((double *)val_gasto);
            idx++;
        }
        
        // Ordenar: maior gasto primeiro, empate por menor doc_number
        qsort(arr, n_pass, sizeof(GastoPassageiro), compara_gastos);
        
        // Incrementar contagem para top 10
        int limite = (n_pass < 10) ? (int)n_pass : 10;
        for (int j = 0; j < limite; j++) {
            int *count = g_hash_table_lookup(contagem_top10, arr[j].doc_number);
            if (!count) {
                count = g_new(int, 1);
                *count = 0;
                g_hash_table_insert(contagem_top10, g_strdup(arr[j].doc_number), count);
            }
            (*count)++;
        }
        
        g_free(arr);
    }
    
    // =========================================================
    // FASE 3: Encontrar passageiro com mais aparições
    // =========================================================
    if (g_hash_table_size(contagem_top10) == 0) {
        g_hash_table_destroy(contagem_top10);
        g_hash_table_destroy(semanas);
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
            if (asprintf(&resultado, "%s,%s,%s,%s,%s,%d\n",
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
    g_hash_table_destroy(semanas);
    
    return resultado;
}