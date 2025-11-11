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

//query 1 (dado um código, imprime as informações do aeroporto)
void query1(const char *code, GHashTable *tabelaAeroportos, FILE *out);

#endif
