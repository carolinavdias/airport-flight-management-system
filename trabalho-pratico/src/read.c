#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include "read.h"
#include "q3.h"

// ---------------------------------------------------------
// Funções auxiliares e de validação
// ---------------------------------------------------------
// Este ficheiro serve só para as funções de leitura e validação,
// o main e a lógica principal estão no programaprincipal.c
// ---------------------------------------------------------

// devolve o tipo de mês (1 → 31 dias, 2 → 30 dias, 3 → fevereiro)
int qual_mes(int mes) {
    if (mes == 2) return 3; // fevereiro
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 2; // 30 dias
    return 1; // 31 dias
}

// verifica se a string é vazia ou só tem espaços (para evitar erros parvos)
int string_vazia_ou_espacos(const char *s) {
    if (s == NULL) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

// converte uma string para estrutura DataH (data + hora)
// exemplo: "2023-05-17 14:30" → {2023, 5, 17, 14, 30}
DataH parse_DataH(char *string) {
    DataH novo;
    int narg = sscanf(string, " %d-%d-%d %d:%d",
                      &novo.data.ano, &novo.data.mes, &novo.data.dia,
                      &novo.horas.hora, &novo.horas.mins);
    if (narg != 5) {
        // se a leitura falhar, mete tudo a 0
        novo.data.ano = novo.data.mes = novo.data.dia = 0;
        novo.horas.hora = novo.horas.mins = 0;
    }
    return novo;
}

// converte uma string numa estrutura Data
// (simples mas ajuda a validar se os valores fazem sentido)
Data parse_Data(char *string) {
    Data novo;
    int narg = sscanf(string, "%d-%d-%d", &novo.ano, &novo.mes, &novo.dia);
    if (narg != 3 || (novo.ano < 0 || novo.ano > 2025) ||
        (novo.mes < 1 || novo.mes > 12) || (novo.dia < 1 || novo.dia > 31)) {
        fprintf(stderr, "Formatação errada da Data: %s\n", string ? string : "(null)");
        novo.ano = novo.mes = novo.dia = 0;
    }
    return novo;
}

// converte string num email válido (ou mete um placeholder todo feio se estiver errado)
char* string_to_email(char *string) {
    if (string == NULL || strlen(string) == 0) return g_strdup("------@----.---");
    if (!strchr(string, '@')) return g_strdup("------@----.---");
    return g_strdup(string);
}

// valida código IATA (3 letras maiúsculas tipo "LIS" ou "OPO")
// se não tiver o formato certo devolve "---"
char* string_to_codigoIATA(char *string) {
    if (string == NULL || strlen(string) != 3) return g_strdup("---");
    for (int i = 0; i < 3; i++) {
        if (string[i] < 'A' || string[i] > 'Z') return g_strdup("---");
    }
    return g_strdup(string);
}

// traduz string para estado de voo (On_Time, Delayed, Cancelled)
// se for inválido devolve ESTADO_ERROR
Estado string_to_Estado(char *string) {
    if (!string) return ESTADO_ERROR;
    if (strcmp(string, "On_Time") == 0) return ESTADO_ON_TIME;
    if (strcmp(string, "Delayed") == 0) return ESTADO_DELAYED;
    if (strcmp(string, "Cancelled") == 0) return ESTADO_CANCELLED;
    return ESTADO_ERROR;
}

// traduz string para tipo de aeroporto (small_airport, heliport, etc)
Tipo_aeroporto valida_tipo(char *string) {
    if (!string) return TIPO_ERROR;
    if (strcmp(string, "small_airport") == 0) return TIPO_SMALL_AIRPORT;
    if (strcmp(string, "medium_airport") == 0) return TIPO_MEDIUM_AIRPORT;
    if (strcmp(string, "large_airport") == 0) return TIPO_LARGE_AIRPORT;
    if (strcmp(string, "heliport") == 0) return TIPO_HELIPORT;
    if (strcmp(string, "seaplane_base") == 0) return TIPO_SEAPLANE_BASE;
    return TIPO_ERROR;
}

// converte uma string em booleano (true/false)
// se for inválido, imprime aviso mas não crasha
bool string_to_bool(char *string, int versao) {
    (void)versao; // evitar warning, mas manter o parâmetro
    if (!string) return false;
    if (strcmp(string, "true") == 0) return true;
    if (strcmp(string, "false") == 0) return false;
    fprintf(stderr, "Aviso: valor inválido para booleano (%s)\n", string);
    return false;
}

// converte string em género (M, F ou O)
// se vier outra coisa qualquer → erro
Genero string_to_genero(char *string) {
    if (!string || strlen(string) != 1) return GENERO_ERROR;
    if (strcmp(string, "M") == 0) return GENERO_M;
    if (strcmp(string, "F") == 0) return GENERO_F;
    if (strcmp(string, "O") == 0) return GENERO_O;
    return GENERO_ERROR;
}

// valida ID de voo (formato tipo "TP1923" → 2 letras + 5 números)
// se estiver errado devolve "00-----" para não dar segmentation
char* string_to_id_voo(char *string) {
    if (!string || strlen(string) != 7) return g_strdup("00-----");
    for (int i = 0; i < 2; i++)
        if (string[i] < 'A' || string[i] > 'Z') return g_strdup("00-----");
    for (int i = 2; i < 7; i++)
        if (string[i] < '0' || string[i] > '9') return g_strdup("00-----");
    return g_strdup(string);
}

// ---------------------------------------------------------
// Funções auxiliares para debugging (imprimir voos, etc.)
// ---------------------------------------------------------
// dá jeito para testar se a hash table está a funcionar
void imprimir_voo(gpointer key, gpointer value, gpointer user_data) {
    (void)user_data; // para o compilador não chatear
    char *voo_id = (char *) key;
    Voo *v = (Voo *) value;

    printf("Voo ID: %s\n", voo_id);
    printf("Origem: %s | Destino: %s\n", v->origin, v->destination);
    printf("Companhia: %s | Estado: %s\n", v->airline, v->status);
    printf("--------------------------\n");
}

// ---------------------------------------------------------
// Nota final:
// Este ficheiro serve só para guardar as funções de leitura
// e validação. O main está em programaprincipal.c
// ---------------------------------------------------------
