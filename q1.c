#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "q1.h"

void query1(const char* code, Aeroporto* aeroporto, int numAeroporto, FILE* out) {
    // Verifica se o código tem exatamente 3 letras maiúsculas
    if (strlen(code) != 3) {
        fprintf(out, "\n");
        return;
    }

    for (int i = 0; i < 3; i++) {
        if (!isupper(code[i]) || !isalpha(code[i])) {
            fprintf(out, "\n");
            return;
        }
    }

    // Procura o aeroporto
    for (int j = 0; j < numAeroporto; j++) {
        if (strcmp(aeroporto[j].code, code) == 0) {
            fprintf(out, "%s;%s;%s;%s;%s\n",
                    aeroporto[j].code,
                    aeroporto[j].name,
                    aeroporto[j].city,
                    aeroporto[j].country,
                    aeroporto[j].type);
            return;
        }
    }

    // Código não encontrado
    fprintf(out, "\n");
}

//verifica se o código introduzido é constituído por 3 letras maiúsculas
int codigoValido(const char* codigo) {
    if (strlen(codigo) != 3) return 0;
    for (int i = 0; i < 3; i++) {
        if (!isupper(codigo[i]) || !isalpha(codigo[i])) return 0;
    }
    return 1;
}