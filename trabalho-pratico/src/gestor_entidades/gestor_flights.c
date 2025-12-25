#include "gestor_entidades/gestor_flights.h"
#include "entidades/flights.h"
#include <stdbool.h> //VERIFICAR SE gestor_flights_existe() É USADA
#include <string.h>
#include <stdio.h>

typedef struct gestor_flights {
    GHashTable *tabela_voos;
    Voo **array_ordenado;  //Array ordenado (recebido do parser)
    int num_voos;  
} GestorFlights;

GestorFlights *gestor_flights_novo() {
    GestorFlights *g = malloc(sizeof(GestorFlights));
    if (g == NULL) return NULL;
    g->tabela_voos = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaVoo);
    g->array_ordenado = NULL;  //inicializa
    g->num_voos = 0;           //inicializa
    return g;
}

void gestor_flights_destroy(GestorFlights *g) {
    if (!g) return;
    g_hash_table_destroy(g->tabela_voos);
    if (g->array_ordenado) free(g->array_ordenado);
    free(g);
}

void gestor_flights_inserir(GestorFlights *g, Voo *voo) {
    if (!g || !voo || !voo_get_flight_id(voo)) return;
    g_hash_table_insert(g->tabela_voos, strdup(voo_get_flight_id(voo)), voo);
}

//VERIFICAR SE É USADO
bool gestor_flights_existe(GestorFlights *g, const char *flight_id) {
    return g && flight_id && g_hash_table_contains(g->tabela_voos, flight_id);
}

const char *gestor_flights_obter_origem(GestorFlights *g, const char *flight_id) {
    if (!g) return NULL;
    Voo *v = g_hash_table_lookup(g->tabela_voos, flight_id);
    return v ? voo_get_code_origin(v) : NULL;
}

const char *gestor_flights_obter_destino(GestorFlights *g, const char *flight_id) {
    if (!g) return NULL;
    Voo *v = g_hash_table_lookup(g->tabela_voos, flight_id);
    return v ? voo_get_code_destination(v) : NULL;
}

GHashTable *gestor_flights_table(GestorFlights *g) {
    return (g != NULL) ? g->tabela_voos : NULL;
}

void gestor_flights_foreach(GestorFlights *g, void (*func)(Voo *, void *), void *user_data) {
    if (!g || !g->tabela_voos || !func) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, g->tabela_voos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        func((Voo *)value, user_data);
    }
}

//encapsulamento
void gestor_flights_print(GestorFlights *g) {
    if (!g || !g->tabela_voos) return;

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, g->tabela_voos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;
        printf("%s %s %s\n",
            voo_get_flight_id(v),
            voo_get_code_origin(v),
            voo_get_code_destination(v)
        );
    }
}

//recebe array ordenado do parser
void gestor_flights_set_array_ordenado(GestorFlights *g, Voo **array, int num_voos) {
    if (!g) return;
    g->array_ordenado = array;
    g->num_voos = num_voos;
}

//obtem array ordenado (para query3)
Voo **gestor_flights_get_array_ordenado(GestorFlights *g, int *num_voos) {
    if (!g) return NULL;
    if (num_voos) *num_voos = g->num_voos;
    return g->array_ordenado;
}