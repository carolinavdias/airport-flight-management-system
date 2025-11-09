#ifndef Q3_H
#define Q3_H

#include <stdio.h>
#include <glib.h>

//voo
typedef struct voo {
    char *flight_id;
    char *departure;
    char *actual_departure;
    char *arrival;
    char *actual_arrival;
    char *gate;
    char *status;
    char *origin;
    char *destination;
    char *aircraft;
    char *airline;
    char *tracking_url;
} Voo;


//carrega voos de um ficheiro CSV para uma GHashTable
GHashTable* carregarVoos(const char *caminhoFicheiro);

//verifica se o identificador de voo é válido 
int idVooValido(const char *id);

//query 3 (aeroporto com mais partidas entre 2 datas)
void query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, FILE *out);

#endif