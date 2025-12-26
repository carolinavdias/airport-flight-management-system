#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <stdbool.h>

#include "queries/interpreter.h"
#include "queries/q1.h"
#include "queries/q2.h"
#include "queries/q3.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"

// Função auxiliar para converter Tipo_aeroporto para string
static const char* tipo_aeroporto_para_string(int tipo) {
    switch (tipo) {
        case TIPO_SMALL_AIRPORT:   return "small_airport";
        case TIPO_MEDIUM_AIRPORT:  return "medium_airport";
        case TIPO_LARGE_AIRPORT:   return "large_airport";
        case TIPO_HELIPORT:        return "heliport";
        case TIPO_SEAPLANE_BASE:   return "seaplane_base";
        default:                   return "unknown";
    }
}

// Função para aplicar separador conforme formato
static void aplica_formato(char *str, bool formato_alternativo) {
    if (!str) return;
    
    char separador = formato_alternativo ? '=' : ';';
    
    for (int i = 0; str[i]; i++) {
        if (str[i] == ',') {
            str[i] = separador;
        }
    }
}

// Função para extrair número da query de forma segura
static int extrai_numero_query(const char *comando, bool *formato_alternativo) {
    if (!comando || !comando[0]) return 0;
    
    *formato_alternativo = false;
    
    // Contar dígitos iniciais
    int i = 0;
    while (comando[i] && comando[i] >= '0' && comando[i] <= '9') {
        i++;
    }
    
    // Se o próximo caractere é 'S' e é o fim da string
    if (comando[i] == 'S' && comando[i+1] == '\0') {
        *formato_alternativo = true;
    }
    
    // Converter apenas a parte numérica
    char num_str[16];
    int copy_len = i;
    if (copy_len >= (int)sizeof(num_str)) {
        copy_len = sizeof(num_str) - 1;
    }
    
    for (int j = 0; j < copy_len; j++) {
        num_str[j] = comando[j];
    }
    num_str[copy_len] = '\0';
    
    return atoi(num_str);
}

// Função principal do interpretador (CORRIGIDA)
void interpreta_comando(const char *comando,
                        const char *param,
                        FILE *out,
                        GestorAirports *gestorAeroportos,
                        GestorFlights *gestorVoos,
                        GestorAircrafts *gestorAeronaves,
                        GestorPassengers *gestorPassageiros) {
    
    (void)gestorPassageiros; // Silencia warning de parâmetro não usado
    
    if (!comando || !out) return;

    bool formato_alternativo = false;
    int query_num = extrai_numero_query(comando, &formato_alternativo);

    // Processa cada query
    if (query_num == 1 && param && param[0]) {
        // Query 1: Formato "code;name;city;country;type"
        Aeroporto *a = gestor_airports_procura(gestorAeroportos, param);
        if (a) {
            char linha[512];
            
            // Obter tipo do aeroporto como número (0-4)
            int tipo = airport_get_type(a); // Deve retornar 0, 1, 2, 3 ou 4
            const char *tipo_str = tipo_aeroporto_para_string(tipo);
            
            // Formatar com vírgulas
            int written = snprintf(linha, sizeof(linha), "%s,%s,%s,%s,%s\n",
                    airport_get_code_IATA(a),
                    airport_get_name(a),
                    airport_get_city(a),
                    airport_get_country(a),
                    tipo_str);
            
            if (written > 0 && (size_t)written < sizeof(linha)) {
                aplica_formato(linha, formato_alternativo);
                fputs(linha, out);
            } else {
                fputs("\n", out);
            }
        } else {
            fputs("\n", out);
        }
    }
    else if (query_num == 2 && param && param[0]) {
        // Query 2: Processamento específico
        char *resultado = query2(param, gestorAeronaves, gestorVoos);
        if (resultado) {
            aplica_formato(resultado, formato_alternativo);
            fputs(resultado, out);
            free(resultado);
        } else {
            fputs("\n", out);
        }
    }
    else if (query_num == 3 && param && param[0]) {
        // Query 3: Processa duas datas
        char data_inicio[32] = {0}, data_fim[32] = {0};
        if (sscanf(param, "%31s %31s", data_inicio, data_fim) == 2) {
            char *resultado = query3(data_inicio, data_fim, gestorVoos, gestorAeroportos);
            if (resultado) {
                aplica_formato(resultado, formato_alternativo);
                fputs(resultado, out);
                free(resultado);
            } else {
                fputs("\n", out);
            }
        } else {
            fputs("\n", out);
        }
    }
    else {
        // Comando não reconhecido ou sem parâmetros
        fputs("\n", out);
    }
}