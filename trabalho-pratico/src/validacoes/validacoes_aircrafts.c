#include "validacoes_aircrafts.h"
#include "aircrafts.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

//AERONAVES -> VALIDAÇÃO SINTÁTICA
int valida_year (char *string, int *year) {
    if (string == NULL || strlen(string) != 4) return 0;
    for (int i = 0; i < 4; i++) {
	if (!isdigit(string[i])) return 0;
    }

//Validação concluida

    *year = atoi(string);
    return 1;
}