#include "validacoes/validacoes_passengers.h"
#include "entidades/passengers.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

// função auxiliar - dias num mês
static int qual_mes(int ano, int mes) {
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
        return bissexto ? 29 : 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    return 31;
}

int valida_id_passageiro(const char* string, int *id_passageiro) {
    if (!string || strlen(string) == 0) return 0;
    for (size_t i = 0; i < strlen(string); i++) {
        if (!isdigit((unsigned char)string[i])) return 0;
    }
    *id_passageiro = atoi(string);
    return 1;
}

int valida_email(const char *string, char **email) {
    if (!string || strlen(string) == 0) return 0;
    
    const char *at = strchr(string, '@');
    if (!at || at == string) return 0;
    
    const char *dot = strrchr(at, '.');
    if (!dot || dot == at + 1 || dot[1] == '\0') return 0;
    
    *email = strdup(string);
    return 1;
}

int valida_genero(const char *s, Genero *genero) {
    if (!s) return 0;
    if (strcmp(s, "M") == 0) {
        *genero = GENERO_M;
        return 1;
    }
    if (strcmp(s, "F") == 0) {
        *genero = GENERO_F;
        return 1;
    }
    if (strcmp(s, "N/A") == 0) {
        *genero = GENERO_O;
        return 1;
    }
    *genero = GENERO_ERROR;
    return 0;
}

int valida_Data(const char *s, int *data) {
    if (!s) return 0;
    
    int ano, mes, dia;
    if (sscanf(s, "%d-%d-%d", &ano, &mes, &dia) != 3) return 0;
    
    if (ano < 1900 || ano > 2100) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > qual_mes(ano, mes)) return 0;
    
    *data = ano * 10000 + mes * 100 + dia;
    return 1;
}