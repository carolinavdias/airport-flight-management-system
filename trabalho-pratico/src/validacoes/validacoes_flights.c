#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include "validacoes_flights.h"
#include "gestor_aircrafts.h"
#include "flights.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>


//VOOS -> VALIDAÇÃO SINTÁTICA

//Valida o id do voo
int valida_id_voo (const char* s, char **voo_id) {
    if (!s) return 0; //tirar ou nao o strlen?
    for (int i = 0; i < 7; i++) {
        if (s[i] == '\0') return 0; //string demasiado curta
        if (i < 2) {
            if (s[i] < 'A' || s[i] > 'Z') return 0; //caracter invalido
        }
        else {
            if (s[i] < '0' || s[i] > '9') return 0; //caracter invalido
        }
    }
    if (s[7] != '\0') return 0; //string demasiado grande
//Validação conluida

    *voo_id = g_strdup(s);
    return 1;
}


//dia com mes e ano válidos ->auxiliar
int qual_mes (int ano, int mes) {
    if (mes == 2) {
        if (ano % 4 == 0) return 29;
        else return 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    return 31;
}

// Retorna 1 se válido; 0 se inválido.
// out = inteiro comparável crescente YYYYMMDDHHMM

int valida_DataH(const char *s, long long *out) {
    if (!s) return 0;

    int tamanho = 0;
    for (; s[tamanho] != '\0'; tamanho++);
    if (tamanho != 16) return 0; // tamanho errado

    // formato fixo
    if (s[4] != '-' || s[7] != '-' || s[10] != ' ' || s[13] != ':') return 0;

    //verificar que são dígitos
    const int indices[] = {0,1,2,3,5,6,8,9,11,12,14,15};
    for (int i = 0; i < 12; i++) {
        if (s[indices[i]] < '0' || s[indices[i]] >'9') return 0;
    }

    // extrair números
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');
    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    // validar ranges
    if (ano < 0 || ano > 2025) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > qual_mes(ano,mes)) return 0;
    if (hora < 0 || hora > 23) return 0;
    if (min < 0 || min > 59) return 0;

    // construir valor inteiro ordenável
    *out = ano * 100000000 +
           mes *   1000000 +
           dia *     10000 +
           hora *      100 +
           min;

    return 1;
}


//Função valida_estado de um voo
int valida_Estado(const char *s, Estado *e) {
    if (!s || strlen(s) == 0) return 0;

    if (strcmp(s, "On Time") == 0) {
        *e = ESTADO_ON_TIME;
        return 1;
    }
    if (strcmp(s, "Delayed") == 0) {
        *e = ESTADO_DELAYED;
        return 1;
    }
    if (strcmp(s, "Cancelled") == 0) {
        *e = ESTADO_CANCELLED;
        return 1;
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





//VOOS -> VALIDAÇÃO LÓGICA

int valida_VOO (Voo voo, GestorAircrafts *gestor_aeronaves) {
    //destino != origem
    if (strcmp (voo.code_origin, voo.code_destination) == 0) return 0;

    //verifica se aeronave correspondente existe
    if (!gestor_aircrafts_existe(gestor_aeronaves, voo.id_aircraft)) return 0;

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
