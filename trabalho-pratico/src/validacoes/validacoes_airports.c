#include "validacoes/validacoes_airports.h"
#include <ctype.h>
#include <string.h>
#include <glib.h>

//AEROPORTO -> VALIDAÇÃO SINTÁTICA

//valida o codigoIATA e faz o strdup
int valida_codigoIATA(const char *s) {
    if (!s) return 0;
    for (int i = 0; i < 3; i++) {
        if (s[i] == '\0') return 0; //string demasiado curta
        if (s[i] < 'A' || s[i] > 'Z') return 0; //caracter invalido
    }
    return s[3] == '\0'; //valido se !grande
}

// valida as coordenadas (latitude e longitude) e faz o atof (double)
int valida_coordenadas(const char* s, int versao) {
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

    double coordenada = atof(s); //converta par double

    switch (versao) {
	case 1: if (coordenada < -90 || coordenada > 90) return 0;
		break;
	case 2: if (coordenada < -180 || coordenada > 180) return 0;
		break;
    }
    return 1;
}

//valido o tipo do aeroporto e passa para a estrutura previamente definida para o tipo de aeroporto
int valida_tipo_aer(const char *s) {
    return s && (strcmp(s, "small_airport") == 0  ||
	         strcmp(s, "medium_airport") == 0 ||
	         strcmp(s, "large_airport") == 0  ||
	    	 strcmp(s, "heliport") == 0	  ||
	         strcmp(s, "seaplane_base") == 0   );
}