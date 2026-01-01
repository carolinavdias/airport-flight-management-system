#define _XOPEN_SOURCE 700
#include "validacoes/validacoes_flights.h"
#include "utils/utils.h"
#include <string.h>

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
    return (s[7] == '\0' || s[8] == '\0');
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

    // validar limites básicos 
    if (ano < 0 || ano > 2025) return 0; 
    if (mes < 1 || mes > 12) return 0; 
    if (dia < 1 || dia > 31) return 0; //ignora dias por mês 
    if (hora < 0 || hora > 23) return 0; 
    if (min < 0 || min > 59) return 0;

    // validar que não é futura (data atual = 2025-09-30) 
    int ano_atual = 2025, mes_atual = 9, dia_atual = 30; 
    
    if (ano > ano_atual) return 0; 
    if (ano == ano_atual && mes > mes_atual) return 0; 
    if (ano == ano_atual && mes == mes_atual && dia > dia_atual) return 0; 
    
    return 1;
}

//É NECESSARIA ESTA FUNCAO ????????
//função valida_estado de um voo
int valida_Estado(const char *s) {
    if (!s || strlen(s) == 0) return 0;
    return (strcmp(s, "On Time") == 0 || strcmp(s, "Delayed") == 0 || strcmp(s, "Cancelled") == 0);
}

//VOOS -> VALIDAÇÃO LÓGICA
int valida_VOO (Voo *voo, GestorAircrafts *gestor_aeronaves) {
    
    //DESTINATION != ORIGIN
    if (strcmp(voo_get_code_origin(voo), voo_get_code_destination(voo)) == 0) {
        return 0;
    }
    
    //AIRCRAFT EXISTE
    if (gestor_aeronaves && voo_get_id_aircraft(voo)) {
        if (!gestor_aircrafts_existe(gestor_aeronaves, voo_get_id_aircraft(voo))) {
            return 0;
        }
    }
    
    //ARRIVAL >= DEPARTURE
    if (voo_get_arrival(voo) < voo_get_departure(voo)) {
        return 0;
    }
    
    //se CANCELLED, actual_departure E actual_arrival devem ser -2 (N/A)
    if (voo_get_status(voo) == ESTADO_CANCELLED) {
        if (voo_get_actual_departure(voo) != -2 || 
            voo_get_actual_arrival(voo) != -2) {
            return 0;
        }
    }
    
    //se NÃO CANCELLED: actual_arrival >= actual_departure
    if (voo_get_status(voo) != ESTADO_CANCELLED) {
        if (voo_get_actual_departure(voo) != -2 && 
            voo_get_actual_arrival(voo) != -2) {
            if (voo_get_actual_arrival(voo) < voo_get_actual_departure(voo)) {
                return 0;
            }
        }
    }
    
    // se DELAYED, actual >= scheduled
    if (voo_get_status(voo) == ESTADO_DELAYED) {
        // actual_departure >= departure
        if (voo_get_actual_departure(voo) != -2 && 
            voo_get_actual_departure(voo) < voo_get_departure(voo)) {
            return 0;
        }
        //actual_arrival >= arrival
        if (voo_get_actual_arrival(voo) != -2 && 
            voo_get_actual_arrival(voo) < voo_get_arrival(voo)) {
            return 0;
        }
    }
    
    return 1; //válido
}