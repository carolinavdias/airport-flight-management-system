#include "validacoes_flights.h"
#include "gestor_aircrafts.h"
#include "flights.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <glib.h>

//VOOS -> VALIDAÇÃO SINTÁTICA
int valida_id_voo(const char *string, char **flight_id) {
    if (string == NULL) return 0;
    
    int len = strlen(string);
    // Verifica o formato: 2 letras seguidas de 5 dígitos
    if (len != 7) return 0;
    
    // Primeiros 2 caracteres devem ser letras maiúsculas
    for (int i = 0; i < 2; i++) {
        if (!isupper(string[i])) return 0;
    }
    
    // Últimos 5 caracteres devem ser dígitos
    for (int i = 2; i < 7; i++) {
        if (!isdigit(string[i])) return 0;
    }
    
    // Validação concluída
    *flight_id = g_strdup(string);
    return 1;
}

//Função auxiliar a validação logica dos voos
int compara_dataH (char *datah1, char *datah2) { //se 1 for não anterior a 2, entao retorna 1 else retorna 0
//Se ano1<ano2, ent 2 vem depois de 1 : errado
//else ano1>ano2 : certo || ano1 == ano2 : proxima
    DataH data1;
    sscanf (datah1, "%d-%d-%d %d:%d", &data1.data.ano, &data1.data.mes, &data1.data.dia, &data1.horas.hora, &data1.horas.mins);
    DataH data2;
    sscanf (datah2, "%d-%d-%d %d:%d", &data2.data.ano, &data2.data.mes, &data2.data.dia, &data2.horas.hora, &data2.horas.mins);
    if (data1.data.ano > data2.data.ano) return 1;
    else if (data1.data.ano == data2.data.ano) {
             if (data1.data.mes > data2.data.mes) return 1;
             else if (data1.data.mes == data2.data.mes) {
                        if (data1.data.dia > data2.data.dia) return 1;
                        else if (data1.data.dia == data2.data.dia) {
                                if (data1.horas.hora > data2.horas.hora) return 1;
                                else if (data1.horas.hora == data2.horas.hora) {
                                        if (data1.horas.mins >= data2.horas.mins) return 1; //enunciado: "data nao anterior"
                                }
                        }
            }
    }

    return 0;
}

//VOOS -> VALIDAÇÃO LÓGICA
int valida_VOO(Voo voo, GestorAircrafts *gestor_aeronaves) {
    // destino != origem
    if (strcmp(voo.code_origin, voo.code_destination) == 0) return 0;

    //verifica se aeronave existe 
    if (!gestor_aircrafts_existe(gestor_aeronaves, voo.id_aircraft))
        return 0;
    
    if (voo.status == ESTADO_CANCELLED) {
        if (voo.actual_departure != -2 || voo.actual_arrival != -2) return 0;
        if (voo.arrival < voo.departure) return 0;
    } else {
        if (voo.arrival < voo.actual_departure || 
            voo.actual_arrival < voo.actual_departure) return 0;
        
        if (voo.status == ESTADO_DELAYED) {
            if (voo.actual_departure < voo.departure || 
                voo.actual_arrival < voo.arrival) return 0;
        }
    }
    
    return 1;
}

int valida_Estado(const char *s, Estado *e)
{
    if (!s) return 0;

    if (s[0] == 'O' && memcmp(s, "On Time", 7) == 0 && s[7] == '\0') {
        *e = ESTADO_SCHEDULED; return 1;
    }

    if (s[0] == 'D' && memcmp(s, "Delayed", 7) == 0 && s[7] == '\0') {
        *e = ESTADO_DELAYED; return 1;
    }

    if (s[0] == 'C' && memcmp(s, "Cancelled", 9) == 0 && s[9] == '\0') {
        *e = ESTADO_CANCELLED; return 1;
    }

    return 0;
}

bool v_is_flight_id(const char *s){
    if(!s || strlen(s)!=7) return false;
    return isalpha((unsigned char)s[0]) && isalpha((unsigned char)s[1]) &&
           isdigit((unsigned char)s[2]) && isdigit((unsigned char)s[3]) &&
           isdigit((unsigned char)s[4]) && isdigit((unsigned char)s[5]) &&
           isdigit((unsigned char)s[6]);
}

bool v_is_status_valid(const char *s){
    if(!s) return false;
    return strcmp(s,"Scheduled")==0 || strcmp(s,"Delayed")==0 || strcmp(s,"Cancelled")==0;
}