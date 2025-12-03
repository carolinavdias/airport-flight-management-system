#ifndef READ_H
#define READ_H

#include <glib.h>

typedef struct {
    char dataset_dir[256];
} Contexto;

//AJUSTAR

int le_tabela(int opcao_inserida,
              Contexto ctx,
              GHashTable *tab_flights,
              GHashTable *tab_airports,
              GHashTable *tab_aircrafts,
              GHashTable *tab_passengers,
              GHashTable *tab_reservations);

int le_tabela (int opcao_inserida, Contexto ctx, GHashTable *tabela1, GHashTable *tabela2, GHashTable *tabela3, GHashTable *tabela4, GHashTable *tabela5);


#endif