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
//    float range; --
    int range; //++
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

//verifica se o identificador é válido (formato tipo PT-AB12)
int identificadorValido(const char *id);

//query 2 (top N aeronaves com mais voos realizados, filtradas por fabricante opcional)
void query2(const char *linhaComando, GHashTable *tabelaAeronaves, FILE *out);

//função auxiliar (ordenação por número de voos e depois por identificador)
int comparaContagens(const Contagem *a, const Contagem *b);

void libertaAeronave(void *data);

#endif
