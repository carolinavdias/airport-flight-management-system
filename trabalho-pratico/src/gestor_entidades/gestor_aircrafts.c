#include "gestor_entidades/gestor_aircrafts.h"
#include <stdlib.h>
#include <glib.h>
//#include <string.h>

//tabela guarda todas as aeronaves
typedef struct gestor_aircrafts {
    GHashTable *tabela;
} GestorAircrafts; 

GestorAircrafts *gestor_aircrafts_cria(void) {
    GestorAircrafts *g = malloc(sizeof(GestorAircrafts));
    if (g == NULL) return NULL;
    g->tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeronave);
    return g;
}

void gestor_aircrafts_insere(GestorAircrafts *g, Aeronave *a) { 
    if (!g || !a) return; 
    
    const char *id_int = aircraft_get_identifier(a); 
    if (!id_int) return; 
    
    char *id = g_strdup(id_int); 
    g_hash_table_insert(g->tabela, id, a); 
}

Aeronave *gestor_aircrafts_procura(GestorAircrafts *g, const char *identifier) {
    if (g == NULL || identifier == NULL) return NULL;
    return g_hash_table_lookup(g->tabela, identifier);
}

int gestor_aircrafts_existe(GestorAircrafts *g, const char *identifier) {
    return g && g->tabela && identifier &&
           g_hash_table_contains(g->tabela, identifier);
}

void gestor_aircrafts_foreach(GestorAircrafts *g, AircraftIterFunc f, void *user_data) {
    if (!g || !g->tabela || !f) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, g->tabela);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        f((Aeronave *)value, user_data);
    }
}

void gestor_aircrafts_liberta(GestorAircrafts *g) {
    if (g == NULL) return;
    if (g->tabela != NULL) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}