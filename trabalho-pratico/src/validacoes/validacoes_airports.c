#include "validacoes/validacoes_airports.h"
#include "validacoes/validacoes.h"
#include <ctype.h>
#include <string.h>
#include <glib.h>

/* ============================================
 * AEROPORTO -> VALIDAÇÃO SINTÁTICA
 * ============================================ */

/**
 * Valida uma coordenada geográfica (latitude ou longitude).
 *
 * A função verifica:
 *  - formato numérico válido;
 *  - apenas um ponto decimal;
 *  - sinal negativo apenas na primeira posição;
 *  - limites adequados conforme o tipo (versao).
 */

static int valida_coordenadas(const char* s, int versao) {
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
	case 1:  // latitude
        if (coordenada < -90 || coordenada > 90) return 0;
		break;
	case 2:  // longitude
        if (coordenada < -180 || coordenada > 180) return 0;
		break;
    }
    return 1;
}

/**
 * Valida o tipo de aeroporto.
 *
 * Apenas os tipos definidos na especificação são aceites.
 */

static int valida_tipo_aer(const char *s) {
    return s && (strcmp(s, "small_airport") == 0  ||
	         strcmp(s, "medium_airport") == 0 ||
	         strcmp(s, "large_airport") == 0  ||
	    	 strcmp(s, "heliport") == 0	  ||
	         strcmp(s, "seaplane_base") == 0   );
}

/**
 * Valida todos os campos essenciais de um aeroporto.
 *
 * Se forem válidos, cria e preenche a estrutura Aeroporto.
 * Caso contrário, devolve NULL.
 */

Aeroporto *validacoes_campos_airports(char **campos, StringPool *pool) {
    Aeroporto *ap = criaAeroporto();

    if (valida_codigoIATA(campos[0])    && // codigo_IATA
	valida_coordenadas(campos[4],1) &&     // latitude
	valida_coordenadas(campos[5],2) &&     // longitude
        valida_tipo_aer(campos[7])   )     // tipo_aeroporto
    {

        airport_set_code_IATA(ap,campos[0],pool);
        airport_set_name(ap,campos[1],pool);
        airport_set_city(ap,campos[2],pool);
        airport_set_country(ap,campos[3],pool);
        airport_set_type(ap,campos[7]);

        return ap;

    } else {
	libertaAeroporto(ap);
	return NULL;
    }
}
