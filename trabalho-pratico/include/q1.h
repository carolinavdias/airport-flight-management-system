#ifndef Q1_H
#define Q1_H

#include <stdio.h> //tipo FILE*
#include <glib.h>

//aeroporto 
typedef struct aeroporto {
    char *code;
    char *name;
    char *city;
    char *country;
    char *type;
} Aeroporto;

//carrega aeroportos de um ficheiro CSV para uma GHashTable
GHashTable* carregarAeroportos(const char *caminhoFicheiro);

//verifica se o código introduzido é constituído por 3 letras maiúsculas
int codigoValido(const char* codigo);

//verifica se o código do aeroporto fornecido pelo utilizador está contido no ficheiro
void query1(const char *code, GHashTable *tabelaAeroportos, FILE *out);

#endif