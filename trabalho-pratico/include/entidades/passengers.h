#ifndef PASSENGERS_H
#define PASSENGERS_H

#include <stdint.h>

typedef uint8_t Genero;

#define GENERO_F 0
#define GENERO_M 1
#define GENERO_O 2
#define GENERO_ERROR 3

typedef struct passageiros {
    int id_passageiro;
    char *primeiro_nome;
    char *ultimo_nome;
    int data_nascimento;
    char *nacionalidade;
    Genero genero_passageiro;
    char *email_passageiro;
    char *telefone_passageiro;
    char *morada_passageiro;
    char *fotografia_passageiro;
} Passageiros;

void libertaPassageiro(void *data);

#endif