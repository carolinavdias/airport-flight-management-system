#ifndef Q2_H
#define Q2_H

#include <stdio.h> //tipo FILE*
#include <glib.h>

//aeronave
typedef struct aeronave {
    char *identifier;
    char *manufacturer;
    char *model;
    int year;
    int capacity;
    float range;
} Aeronave;

//contagem
typedef struct {
    char *identifier;
    char *manufacturer;
    char *model;
    int count;
} Contagem;

//carrega aeronaves de um ficheiro CSV para uma GHashTable
GHashTable* carregarAeronaves(const char *caminhoFicheiro);

//verifica se o identificador é válido
int identificadorValido(const char *id);

//query 2 (imprime informações de uma aeronave com base no identificador)
void query2(const char *linhaComando, GHashTable *tabelaAeronaves, FILE *out);

//função auxiliar (ordenação)
int comparaContagens(const Contagem *a, const Contagem *b);

#endif