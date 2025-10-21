#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "q1.h"

#define MAX_AEROPORTOS 1000

Aeroporto aeroportos[MAX_AEROPORTOS];
int numAeroportos = 0;

void carregarAeroportos(const char* nomeFicheiro) {
    FILE* f = fopen(nomeFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro de aeroportos");
        exit(1);
    }

    char linha[512];
    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';
        char* campos[8];
        int totalCampos = 0;

        char* token = strtok(linha, ";");
        while (token && totalCampos < 8) {
            if (token[0] == '"') token++;
            char* aspas = strchr(token, '"');
            if (aspas) *aspas = '\0';

            campos[totalCampos++] = token;
            token = strtok(NULL, ";");
        }

        if (totalCampos >= 8) {
            Aeroporto novo;

            strncpy(novo.code, campos[0], sizeof(novo.code) - 1);
            novo.code[sizeof(novo.code) - 1] = '\0';

            strncpy(novo.name, campos[1], sizeof(novo.name) - 1);
            novo.name[sizeof(novo.name) - 1] = '\0';

            strncpy(novo.city, campos[2], sizeof(novo.city) - 1);
            novo.city[sizeof(novo.city) - 1] = '\0';

            strncpy(novo.country, campos[3], sizeof(novo.country) - 1);
            novo.country[sizeof(novo.country) - 1] = '\0';

            strncpy(novo.type, campos[7], sizeof(novo.type) - 1);
            novo.type[sizeof(novo.type) - 1] = '\0';

            int duplicado = 0;
            for (int i = 0; i < numAeroportos; i++) {
                if (strcmp(aeroportos[i].code, novo.code) == 0) {
                    duplicado = 1;
                    break;
                }
            }

            if (duplicado) {
                printf("Aviso: código duplicado ignorado: %s\n", novo.code);
                continue;
            }

            aeroportos[numAeroportos++] = novo;
            if (numAeroportos >= MAX_AEROPORTOS) break;
        }
    }

    fclose(f);
}

int main() {
    char comando[100];
    char codigo[10];

    carregarAeroportos("airports.csv");

    printf("Introduza o comando (ex: 1 LIS): ");
    fgets(comando, sizeof(comando), stdin);

    FILE* resultado = fopen("resultado.csv", "w");
    if (!resultado) {
        perror("Erro ao criar o ficheiro de resultado");
        return 1;
    }

    if (sscanf(comando, "1 %9s", codigo) == 1) {
        query1(codigo, aeroportos, numAeroportos, resultado);
    } else {
        fprintf(resultado, "\n");
    }

    fclose(resultado);
    return 0;
}