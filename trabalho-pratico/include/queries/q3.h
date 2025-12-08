#ifndef Q3_H
#define Q3_H

#include <stdio.h>
#include <glib.h>
#include <stdint.h>
#include "entidades/flights.h"

typedef struct contagens2 {
    char *code;
    int count;
} Contagens2;


//carrega voos de um ficheiro CSV para uma GHashTable
GHashTable* carregarVoos(const char *caminhoFicheiro);

//verifica se o identificador de voo é válido (7 caracteres alfanuméricos)
int idVooValido(const char *id);

//query 3 (aeroporto com mais partidas reais entre 2 datas, ignora cancelados)
void query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, GHashTable *tabelaAeroportos, FILE *out);

void libertaVoo (void *data);

#endif
