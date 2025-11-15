#ifndef Q3_H
#define Q3_H

#include <stdio.h>
#include <glib.h>
#include "q1.h"

//voo
typedef struct voo {
    char *flight_id; //voo_id
    char *departure; //partida_prevista
    char *actual_departure; //partida_efetiva
    char *arrival; //chegada prevista
    char *actual_arrival; //chegada efetiva
    char *gate; //porta_embarque
    char *status; //--
    char *code_origin; //codigo IATA origem
    char *code_destination; //codigo IATA destino
    char *id_aircraft; //id_aeronave
    char *airline; //companhia aerea
    char *tracking_url;
} Voo;

//carrega voos de um ficheiro CSV para uma GHashTable
GHashTable* carregarVoos(const char *caminhoFicheiro);

//verifica se o identificador de voo é válido (7 caracteres alfanuméricos)
int idVooValido(const char *id);

//query 3 (aeroporto com mais partidas reais entre 2 datas, ignora cancelados)
void query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, GHashTable *tabelaAeroportos, FILE *out);

void libertaVoo (void *data);

#endif