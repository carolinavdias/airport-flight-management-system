#include "validacoes_flights.h"
#include "flights.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <glib.h>

//VOOS -> VALIDAÇÃO SINTÁTICA
int valida_id_voo(char *s, char **voo_id)
{
    if (
        s[0] >= 'A' && s[0] <= 'Z' &&
        s[1] >= 'A' && s[1] <= 'Z' &&
        s[2] >= '0' && s[2] <= '9' &&
        s[3] >= '0' && s[3] <= '9' &&
        s[4] >= '0' && s[4] <= '9' &&
        s[5] >= '0' && s[5] <= '9' &&
        s[6] >= '0' && s[6] <= '9' &&
        (s[7] == '\0' || s[7] == '\n' || s[7] == '\r')
    ) {
    *voo_id = g_strdup(s);
    return 1;
    }

    return 0;
}

//Valida o id do voo
int valida_id_voo2 (char* string, char **voo_id) {
    if (string == NULL || strlen(string) != 7) return 0;
    else {
        for (int i = 0; i < 7; i++) {
            if ((i < 2 && (string[i] < 'A' || string[i] > 'Z')) ||
               (i >= 2 && (string[i] < '0' || string[i] > '9'))) return 0;
        }
    }
//Validação conluida

    *voo_id = g_strdup(string);
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
int valida_VOO (Voo voo, GHashTable *tabela) {
    //destino != origem
    if (strcmp (voo.code_origin, voo.code_destination) == 0) return 0;

    //cada voo tem uma aeronave correspondente EXISTENTE
    char *aeronave_chave = voo.id_aircraft;
    if (!g_hash_table_contains(tabela,aeronave_chave)) return 0;

    //if CANCELLED, actual departure e actual arrival == "N/A"
    if (voo.status == 2) {
        // CANCELLED: actual deve ser N/A (-2)
        if (voo.actual_departure != -2 || voo.actual_arrival != -2) return 0;
        // arrival deve ser DEPOIS de departure (pode ser igual)
        if (voo.arrival < voo.departure) return 0;
    }
    else {
        // arrival DEPOIS de departure (não pode ser igual)
        if (voo.arrival < voo.actual_departure || voo.actual_arrival < voo.actual_departure) return 0;

        //if DELAYED, actual departure/arrival >= departure/arrival (pode ser igual!)
        if (voo.status == 1) {
            if (voo.actual_departure < voo.departure || voo.actual_arrival < voo.arrival) return 0;
        }
    }
    return 1; //Válido!
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

//Valida o estado (voo) e passa para a estrutura previamente definida para estado
int valida_Estado2 (char *string, Estado *e) {
    if (string == NULL || strlen(string) == 0) return 0;
    if (strcmp(string, "On Time") == 0) *e = ESTADO_SCHEDULED;
    else if (strcmp(string, "Delayed") == 0) *e = ESTADO_DELAYED;
    else if (strcmp(string, "Cancelled") == 0) *e = ESTADO_CANCELLED;
    else return 0;

    return 1; //Estado válido
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