#include "validacoes_airports.h"
#include "airports.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>

//AEROPORTO -> VALIDAÇÃO SINTÁTICA
//Valida o codigoIATA e faz o strdup
int valida_codigoIATA (char* string, char **codigo_IATA) {
    if (string[0] >= 'A' && string[0] <= 'Z' &&
	string[1] >= 'A' && string[1] <= 'Z' &&
	string[2] >= 'A' && string[2] <= 'Z' &&
	string[3] == '\0') {
	*codigo_IATA = g_strdup(string);
	return 1;
    }

    return 0;
}

// Valida as coordenadas (latitude e longitude) e faz o atof (double)
int valida_coordenadas (const char* string, int versao, double *coordenada) {
//versao 1. latitude
//versao 2. longitude
    if (string == NULL || strlen(string) == 0) {
        return 0;
    }
    int length = strlen(string);


    int contador = 0;
    for (int i = 0; i < length; i++) {
    	if (string[i] == '-' && i != 0) return 0;
    	if (!isdigit(string[i]) && string[i] != '.' && string[i] != '-') return 0;
        if (string[i] == '.') contador++;
    }
    if (contador > 1) return 0;


    *coordenada = atof(string);
    switch (versao) {
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
        *t = TIPO_ERROR;
        return 0;
    }

    if (strcmp(string, "small_airport") == 0) *t = TIPO_SMALL_AIRPORT;
    else if (strcmp(string, "medium_airport") == 0) *t = TIPO_MEDIUM_AIRPORT;
    else if (strcmp(string, "large_airport") == 0) *t = TIPO_LARGE_AIRPORT;
    else if (strcmp(string, "heliport") == 0) *t = TIPO_HELIPORT;
    else if (strcmp(string, "seaplane_base") == 0) *t = TIPO_SEAPLANE_BASE;
    else if (strcmp(string, "closed_airport") == 0) *t = TIPO_CLOSED_AIRPORT;
    else {
        *t = TIPO_ERROR;
        return 0;
    }

    return 1;
}

bool v_is_iata3(const char *s){
    if(!s || strlen(s)!=3) return false;
    for(int i=0;i<3;i++) if(!isalpha((unsigned char)s[i])) return false;
    return true; // normaliza para maiúsculas no chamador se necessário
}

bool v_is_airport_type(const char *s){
    if(!s) return false;
    return strcmp(s,"small_airport")==0 || strcmp(s,"medium_airport")==0 ||
           strcmp(s,"large_airport")==0 || strcmp(s,"heliport")==0 ||
           strcmp(s,"seaplane_base")==0;
}

bool v_is_lat(const char *s){
    if(!s || !*s) return false;
    char *end=NULL; double v=strtod(s,&end);
    if(end==s || *end!='\0') return false;
    return v>=-90.0 && v<=90.0;
}

bool v_is_lng(const char *s){
    if(!s || !*s) return false;
    char *end=NULL; double v=strtod(s,&end);
    if(end==s || *end!='\0') return false;
    return v>=-180.0 && v<=180.0;
}