#include "validacoes_aircrafts.h"
#include "aircrafts.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


//AERONAVES -> VALIDAÇÃO SINTÁTICA

int valida_year (const char *s, int *year) {
    if (!s || strlen(s) != 4) return 0;
    for (int i = 0; i < 4; i++) {
        if (!isdigit(s[i])) return 0;
    }

//Validação concluida

    *year = atoi(s);
    if (*year < 0 || *year > 2025) return 0; //ano inválido

    return 1;
}

