#define _POSIX_C_SOURCE 200809L

#include "queries/q2.h"
#include <glib.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "entidades/flights.h"
#include "entidades/aircrafts.h"
#include "gestor_entidades/gestor_aircrafts.h" 
#include "gestor_entidades/gestor_flights.h" 

/** 
 * Estrutura auxiliar que guarda a informação de cada aeronave
 * e o número de voos associados.
 */

typedef struct contagem { 
    const char *identifier;    /**< Identificador único do equipamento. */ 
    const char *manufacturer;  /**< Nome do fabricante do equipamento. */ 
    const char *model;         /**< Modelo específico do equipamento. */ 
    int count;           /**< Número de ocorrências registadas. */ 
} Contagem;

/**
 * =====================================================
 * FUNÇÕES AUXILIARES
 * ===================================================== */

/**
 * Remove espaços em branco no início e no fim de uma string 
 */

static void trim(char *s) {
    if (!s) return;
    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);

    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

/** 
 * Função de comparação usada na ordenação:
 * - Primeiro pelo número de voos (ordem decrescente)
 * - Em caso de empate, pelo identificador da aeronave
 */

int comparaContagens(const Contagem *a, const Contagem *b) {
    if (a->count != b->count)
        return b->count - a->count;
    return g_strcmp0(a->identifier, b->identifier);
}

/**
 * =====================================================
 * CALLBACK PARA PROCESSAR CADA AERONAVE
 * ===================================================== */

/**
 * Estrutura que agrega os dados necessários durante a iteração
 * pelas aeronaves.
 */

typedef struct { 
    const char *fabricante_lower;  /**< Nome do fabricante em minúsculas, usado para comparação case-insensitive. */ 
    int usar_filtro;               /**< Flag que indica se o filtro deve ser aplicado (1) ou ignorado (0). */ 
    GList *resultado;              /**< Lista ligada (GLib) onde serão acumulados os resultados filtrados. */ 
    GestorFlights *gestorVoos;     /**< Apontador para o gestor de voos, usado para consultar contagens e metadados. */ 
} DadosFiltro;

/**
 * Callback chamada para cada aeronave existente no gestor.
 * Aplica o filtro por fabricante (se existir) e cria uma
 * estrutura Contagem com a informação relevante.
 */

static void processa_aeronave(Aeronave *a, void *user_data) {
    DadosFiltro *dados = user_data;

    // guardar ponteiro para o fabricante (string interna, não libertar)
    const char *manuf_str = aircraft_get_manufacturer(a);
    
    // filtro
    if (dados->usar_filtro) {
        if (g_ascii_strcasecmp(manuf_str, dados->fabricante_lower) != 0) {
            //g_free(manuf_str);
            return;
        }
    }

    //guardar getters para libertar
    const char *id_str = aircraft_get_identifier(a);
    const char *model_str = aircraft_get_model(a);
    
    //busca contagem na tabela PRÉ-CONSTRUÍDA (não reconstrói!)
    int count = 0;
    if (dados->gestorVoos) {
        int count_val = gestor_flights_get_contagem_aircraft(dados->gestorVoos, id_str);
        count = count_val;
    }

    Contagem *c = g_new(Contagem, 1);
    c->identifier   = id_str;
    c->manufacturer = manuf_str;
    c->model        = model_str;
    c->count        = count;

    dados->resultado = g_list_prepend(dados->resultado, c);
}

/** 
 * Liberta a memória associada a uma estrutura Contagem 
 */

static void free_contagem(void *data) {
    Contagem *c = data;
//    g_free(c->identifier);
//    g_free(c->manufacturer);
//    g_free(c->model);
    g_free(c);
}

/**
 * =====================================================
 * QUERY 2 — IMPLEMENTAÇÃO OTIMIZADA
 * ===================================================== */

/** 
 * Utiliza contagens de voos pré-calculadas durante o parsing,
 * evitando percorrer todos os voos para cada aeronave.
 */
char *query2(const char *linhaComando, GestorAircrafts *gestorAeronaves, GestorFlights *gestorVoos) {
    
    int N;
    char fabricante_raw[200] = "";

    int arg = sscanf(linhaComando, "%d %[^\n]", &N, fabricante_raw);
    if (arg < 1 || N <= 0) {
        return strdup("\n");
    }

    if (arg == 2) trim(fabricante_raw);
    else fabricante_raw[0] = '\0';

    int usar_filtro = (fabricante_raw[0] != '\0');
    gchar *fabricante_lower = usar_filtro ? g_ascii_strdown(fabricante_raw, -1) : NULL;

    //obtém tabela PRÉ-CONSTRUÍDA do gestor (não reconstrói!) -otimização
    //usa gestor diretamente (encapsulado)

    //prepara dados para o foreach
    DadosFiltro dados = {
        .fabricante_lower = fabricante_lower,
        .usar_filtro = usar_filtro,
        .resultado = NULL,
        .gestorVoos = gestorVoos  //passa gestor encapsulado
    };

    //usa foreach em vez de iterar diretamente
    gestor_aircrafts_foreach(gestorAeronaves, processa_aeronave, &dados);

    if (fabricante_lower) g_free(fabricante_lower);

    // Ordenar
    dados.resultado = g_list_sort(dados.resultado, (GCompareFunc) comparaContagens);

    // Construir string de resultado
    if (dados.resultado == NULL) {
        return strdup("\n");
    }

    size_t buffer_size = 4096;
    char *output = malloc(buffer_size);
    if (!output) {
        g_list_free_full(dados.resultado, free_contagem);
        return strdup("\n");
    }
    
    output[0] = '\0';
    size_t current_pos = 0;

    int printed = 0;
    for (GList *l = dados.resultado; l != NULL && printed < N; l = l->next, printed++) {
        Contagem *c = l->data;
        
        char linha[512];
        int len = snprintf(linha, sizeof(linha), "%s;%s;%s;%d\n",
                          c->identifier, c->manufacturer, c->model, c->count);
        
        if (current_pos + len + 1 > buffer_size) {
            buffer_size *= 2;
            char *new_output = realloc(output, buffer_size);
            if (!new_output) {
                free(output);
                g_list_free_full(dados.resultado, free_contagem);
                return strdup("\n");
            }
            output = new_output;
        }
        
        strcpy(output + current_pos, linha);
        current_pos += len;
    }

    if (printed == 0) {
        free(output);
        g_list_free_full(dados.resultado, free_contagem);
        return strdup("\n");
    }

    g_list_free_full(dados.resultado, free_contagem);
    
    return output;
}

