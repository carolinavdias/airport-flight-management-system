#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include "validacoes/validacoes_flights.h"
#include "utils/utils.h"

//VOOS -> VALIDAÇÃO SINTÁTICA


//valida o id do voo
int valida_id_voo (const char *s) {
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
    return s[7] == '\0';
}


//retorna 1 se válido; 0 se inválido.
//out = inteiro comparável crescente YYYYMMDDHHMM

int valida_DataH(const char *s) { //, long long *out) {
    if (!s) return 0;

    int tamanho = 0;
    for (; s[tamanho] != '\0'; tamanho++);
    if (tamanho != 16) return 0; // tamanho errado

    //formato fixo
    if (s[4] != '-' || s[7] != '-' || s[10] != ' ' || s[13] != ':') return 0;

    //verifica que são dígitos
    const int indices[] = {0,1,2,3,5,6,8,9,11,12,14,15};
    for (int i = 0; i < 12; i++) {
        if (s[indices[i]] < '0' || s[indices[i]] >'9') return 0;
    }

    //extrai números
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');
    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    return !(ano < 0 || ano > 2025 ||
	     mes < 1 || mes > 12    ||
	     dia < 1 || dia > qual_mes(ano,mes) ||
	     hora < 0 || hora > 23  ||
	     min < 0 || min > 59);
}

//função valida_estado de um voo
int valida_Estado(const char *s) {
    if (!s || strlen(s) == 0) return 0;
    return (strcmp(s, "On Time") == 0 || strcmp(s, "Delayed") == 0 || strcmp(s, "Cancelled") == 0);
}

//VOOS -> VALIDAÇÃO LÓGICA

int valida_VOO (Voo *voo, GestorAircrafts *gestor_aeronaves) {
    //destino != origem
    if (strcmp (voo_get_code_origin(voo), voo_get_code_destination(voo)) == 0) return 0;

    //verifica se aeronave correspondente existe
    if (!gestor_aircrafts_existe(gestor_aeronaves, voo_get_id_aircraft(voo))) return 0;

    //if CANCELLED, actual departure e actual arrival == "N/A"
    if (voo_get_status(voo) == 2) {
        // CANCELLED: actual deve ser N/A (-2)
        if (voo_get_actual_departure(voo) != -2 || voo_get_actual_arrival(voo) != -2) return 0;
        // arrival deve ser DEPOIS de departure (pode ser igual)
        if (voo_get_arrival(voo) < voo_get_departure(voo)) return 0;
    }
    else {
        // arrival DEPOIS de departure (não pode ser igual)
        if (voo_get_arrival(voo) < voo_get_actual_departure(voo) || voo_get_actual_arrival(voo) < voo_get_actual_departure(voo)) return 0;

        //if DELAYED, actual departure/arrival >= departure/arrival (pode ser igual!)
        if (voo_get_status(voo) == 1) {
            if (voo_get_actual_departure(voo) < voo_get_departure(voo) || voo_get_actual_arrival(voo) < voo_get_arrival(voo)) return 0;
        }
    }

    return 1; //Válido!
}