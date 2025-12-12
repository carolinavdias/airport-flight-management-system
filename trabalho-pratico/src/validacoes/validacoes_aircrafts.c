#include "validacoes/validacoes_aircrafts.h"
#include <string.h>

//AERONAVES -> VALIDAÇÃO SINTÁTICA

int valida_year(const char *s) {

    int year = 0;
    if (!s || strlen(s) != 4) return 0;
    for (int i = 0; i < 4; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
	else year = year * 10 + s[i]-'0';
    }
    return year <= 2025;
}
