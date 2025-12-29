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
//#include "queries/q4.h"
//#include "queries/q5.h"
#include "queries/q6.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

//função para aplicar separador conforme formato
static void aplica_formato(char *str, bool formato_alternativo) {
    if (!str) return;
    
    char separador = formato_alternativo ? '=' : ';';
    
    //troca TODAS as vírgulas pelo separador correto
    for (int i = 0; str[i]; i++) {
        if (str[i] == ',') {
            str[i] = separador;
        }
    }
}

//função para extrair número da query
static int extrai_numero_query(const char *comando, bool *formato_alternativo) {
    if (!comando || !comando[0]) return 0;
    
    *formato_alternativo = false;
    
    //conta dígitos iniciais
    int i = 0;
    while (comando[i] && comando[i] >= '0' && comando[i] <= '9') {
        i++;
    }
    
    //se o próximo caractere é 'S' e é o fim da string
    if (comando[i] == 'S' && comando[i+1] == '\0') {
        *formato_alternativo = true;
    }
    
    //converte apenas a parte numérica
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

//interpretador
void interpreta_comando(const char *comando,
                        const char *param,
                        FILE *out,
                        GestorAirports *gestorAeroportos,
                        GestorFlights *gestorVoos,
                        GestorAircrafts *gestorAeronaves,
                        GestorPassengers *gestorPassageiros,
                        GestorReservations *gestorReservas) {
    
    if (!comando || !out) {
        fprintf(out, "\n");
        return;
    }

    bool formato_alternativo = false;
    int query_num = extrai_numero_query(comando, &formato_alternativo);

    //QUERY 1 - USA A FUNÇÃO query1() já implementada
    if (query_num == 1) {
        if (param && param[0]) {
            //chama query1() (já conta passageiros corretamente!)
            char *resultado = query1(param, gestorAeroportos, gestorVoos, gestorPassageiros);
            
            if (resultado && resultado[0] != '\n') {
                //zplica formato alternativo se necessário
                aplica_formato(resultado, formato_alternativo);
                fprintf(out, "%s", resultado);
                free(resultado);
            } else {
                fprintf(out, "\n");
                if (resultado) free(resultado);
            }
        } else {
            fprintf(out, "\n");
        }
    }
    //QUERY 2
    else if (query_num == 2) {
        if (param && param[0]) {
            char *resultado = query2(param, gestorAeronaves, gestorVoos);
            
            if (resultado && resultado[0] != '\n') {
                aplica_formato(resultado, formato_alternativo);
                fprintf(out, "%s", resultado);
                free(resultado);
            } else {
                fprintf(out, "\n");
                if (resultado) free(resultado);
            }
        } else {
            fprintf(out, "\n");
        }
    }
    //QUERY 3
    else if (query_num == 3) {
        if (param && param[0]) {
            char d1[32] = {0}, d2[32] = {0};
            char data_inicio[64] = {0}, data_fim[64] = {0};
            
            if (sscanf(param, "%31s %31s", d1, d2) == 2) {
                //adiciona hora
                snprintf(data_inicio, sizeof(data_inicio), "%s 00:00", d1);
                snprintf(data_fim, sizeof(data_fim), "%s 23:59", d2);
                
                char *resultado = query3(data_inicio, data_fim, gestorVoos, gestorAeroportos);
                
                if (resultado && resultado[0] != '\n') {
                    aplica_formato(resultado, formato_alternativo);
                    fprintf(out, "%s", resultado);
                    free(resultado);
                } else {
                    fprintf(out, "\n");
                    if (resultado) free(resultado);
                }
            } else {
                fprintf(out, "\n");
            }
        } else {
            fprintf(out, "\n");
        }
    }
    //QUERY 6
    else if (query_num == 6) {
        if (param && param[0]) {
            char *resultado = query6(param, gestorPassageiros, gestorVoos, gestorReservas);
            
            if (resultado && resultado[0] != '\n') {
                aplica_formato(resultado, formato_alternativo);
                fprintf(out, "%s", resultado);
                free(resultado);
            } else {
                fprintf(out, "\n");
                if (resultado) free(resultado);
            }
        } else {
            fprintf(out, "\n");
        }
    }
    //comando não reconhecido
    else {
        fprintf(out, "\n");
    }
}