#include "validacoes_airports.h"
#include "airports.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>


//AEROPORTO -> VALIDAÇÃO SINTÁTICA


//Valida o codigoIATA e faz o strdup
int valida_codigoIATA (const char* s, char **codigo_IATA) {
    if (!s) return 0; //tirar ou nao o strlen?
    for (int i = 0; i < 3; i++) {
        if (s[i] == '\0') return 0; //string demasiado curta
        if (s[i] < 'A' || s[i] > 'Z') return 0; //caracter invalido
    }
    if (s[3] != '\0') return 0; //string demasiado grande
//Validação conluida

    *codigo_IATA = g_strdup(s);
    return 1;
}


// Valida as coordenadas (latitude e longitude) e faz o atof (double)
int valida_coordenadas (const char* s, int versao, double *coordenada) {
//versao 1. latitude
//versao 2. longitude
    if (!s) return 0;
    int length = strlen(s);

    int contador = 0;
    for (int i = 0; i < length; i++) {
        if (s[i] == '-' && i != 0) return 0; //coordenada negativa
        if (!isdigit(s[i]) && s[i] != '.' && s[i] != '-') return 0; //admite numeros, '.' e '-'
        if (s[i] == '.') contador++; //conta os '.' para cofirmar que, se existir, aprenas há 1
    }
    if (contador > 1) return 0;


    *coordenada = atof(s); //converta par double
    switch (versao) { //valida tendo em conta a versao
        case 1: if (*coordenada < -90 || *coordenada > 90) return 0;
                break;
        case 2: if (*coordenada < -180 || *coordenada > 180) return 0;
                break;
    }

    return 1;
}



//Valido o tipo do aeroporto e passa para a estrutura previamente definida para o tipo de aeroporto
int valida_tipo_aer(const char *string, Tipo_aeroporto *t) {
    if (string == NULL || strlen(string) == 0) {
        return 0;
    }

    if (strcmp(string, "small_airport") == 0) {
        *t = TIPO_SMALL_AIRPORT;
        return 1;
    }
    if (strcmp(string, "medium_airport") == 0) {
        *t = TIPO_MEDIUM_AIRPORT;
        return 1;
    }
    if (strcmp(string, "large_airport") == 0) {
        *t = TIPO_LARGE_AIRPORT;
        return 1;
    }
    if (strcmp(string, "heliport") == 0) {
        *t = TIPO_HELIPORT;
        return 1;
    }
    if (strcmp(string, "seaplane_base") == 0) {
        *t = TIPO_SEAPLANE_BASE;
        return 1;
    }
    return 0;
}

