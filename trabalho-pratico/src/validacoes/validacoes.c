#include "validacoes/validacoes.h"
#include "utils/utils.h"

#include <string.h>
#include <ctype.h>
#include <stdlib.h>

/* ============================================================
 * VALIDAÇÕES GENÉRICAS
 * ============================================================ */

/**
 * Valida uma data no formato YYYY-MM-DD.
 */

int valida_Data(const char *s) {
    if (!s) return 0;

    int tamanho = strlen(s);
    if (tamanho != 10) return 0;

    // formato fixo
    if (s[4] != '-' || s[7] != '-') return 0;

    // verificar dígitos
    const int indices[] = {0,1,2,3,5,6,8,9};
    for (int i = 0; i < 8; i++) {
        if (!isdigit(s[indices[i]])) return 0;
    }

    // extrair valores
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');

    // validar limites
    if (ano < 0 || ano > 2025) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > qual_mes(ano, mes)) return 0;

    return 1;
}

/**
 * Valida uma data e hora no formato YYYY-MM-DD HH:MM.
 */

int valida_DataH(const char *s) {
    if (!s) return 0;

    if (strlen(s) != 16) return 0;

    // formato fixo
    if (s[4] != '-' || s[7] != '-' || s[10] != ' ' || s[13] != ':')
        return 0;

    // verificar dígitos
    const int indices[] = {0,1,2,3,5,6,8,9,11,12,14,15};
    for (int i = 0; i < 12; i++) {
        if (!isdigit(s[indices[i]])) return 0;
    }

    // extrair valores
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');
    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    // validar limites básicos
    if (ano < 0 || ano > 2025) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > 31) return 0;
    if (hora < 0 || hora > 23) return 0;
    if (min < 0 || min > 59) return 0;

    // impedir datas futuras (data atual fixa do projeto)
    int ano_atual = 2025, mes_atual = 9, dia_atual = 30;

    if (ano > ano_atual) return 0;
    if (ano == ano_atual && mes > mes_atual) return 0;
    if (ano == ano_atual && mes == mes_atual && dia > dia_atual) return 0;

    return 1;
}

/**
 * Valida um código IATA (3 letras maiúsculas).
 */

int valida_codigoIATA(const char *s) {
    if (!s) return 0;

    for (int i = 0; i < 3; i++) {
        if (s[i] < 'A' || s[i] > 'Z') return 0;
    }

    return s[3] == '\0';
}
