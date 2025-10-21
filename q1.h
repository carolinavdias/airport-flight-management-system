#ifndef Q1_H
#define Q1_H

#include <stdio.h> //tipo FILE*

//aeroporto
typedef struct {
    char code[10];
    char name[80];
    char city[50];
    char country[50];
    char type[30];
} Aeroporto;

//variaveis existentes noutros ficheiros (dá conflito se tiver no ficheiro q1.)
extern Aeroporto aeroportos[1000]; //lista de aeroportos
extern int numAeroportos; //número de aeroportos armazenados na memória


//verifica se o código do aeroporto fornecido pelo utilizador está contido no ficheiro
void query1(const char* code, Aeroporto* aeroporto, int numAeroporto, FILE* out);

//verifica se o código introduzido é constituído por 3 letras maiúsculas
int codigoValido(const char* codigo);

#endif