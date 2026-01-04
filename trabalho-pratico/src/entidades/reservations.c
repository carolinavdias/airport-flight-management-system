#include "entidades/reservations.h"
#include "utils/utils.h"
#include <stdbool.h>
#include <stdlib.h>

/**
 * Contém a definição interna das estruturas `Reservas` e `Voos_reservados`,
 * bem como a implementação dos getters, setters e funções de criação e
 * destruição.
 */

// Estrutura que representa a lista de voos reservados 
/**
 * Estrutura interna que representa um passageiro.
 */

typedef struct voos_reservados { 
    const char **lista_voos_reservados;   /**< Array de IDs de voos */ 
    int n_voos;                     /**< Número de voos       */ 
} Voos_reservados;

/**
 * Estrutura interna que representa uma reserva.
 */

typedef struct reservas {
    const char *id_reserva;               // Identificador único da reserva
    Voos_reservados *reserva_lista;  // Lista de voos associados à reserva
    int id_pessoa_reservou;         // Identificador da pessoa em nome de quem a reserva foi efetuada
    double preco_reserva;           // Preço total da reserva
} Reservas;

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * Os getters permitem consultar os campos internos da reserva.
 *
 * Getters que retornam strings devolvem cópias (g_strdup), devendo o
 * utilizador libertar a memória retornada.
 * 
 * Getters que retornam ponteiros internos (`char **`) devolvem dados
 * pertencentes à estrutura e não devem ser libertados.
 * 
 * Getters numéricos devolvem os valores diretamente.
 */

int get_lista_n_voos (Voos_reservados *lvr) {
    return lvr->n_voos;
}

const char *r_get_id_reserva (const Reservas *r) {
    return r->id_reserva;
}

int r_get_id_pessoa_reservou (const Reservas *r) {
    return r->id_pessoa_reservou;
}

int r_get_lista_n_voos (const Reservas *r) {
    return r->reserva_lista->n_voos;
}

const char **r_get_lista_voos_reserv (const Reservas *r) {
    return r->reserva_lista->lista_voos_reservados;
}

double r_get_preco(const Reservas *r) {
    if (!r) return 0.0;
    return r->preco_reserva;
}

char *r_get_voo_por_indice(const Reservas *r, int indice) {
    if (!r) return NULL;
    if (indice < 0 || indice >= r->reserva_lista->n_voos) return NULL;
    if (!r->reserva_lista->lista_voos_reservados) return NULL;
    if (!r->reserva_lista->lista_voos_reservados[indice]) return NULL;
    
    // Retorna CÓPIA para encapsulamento - libertar com g_free()!
    return g_strdup(r->reserva_lista->lista_voos_reservados[indice]);
}

/* ============================================
 * SETTERS
 * ============================================ */

/**
 * Os setters atualizam os campos internos da reserva.
 *
 * Sempre que substituem uma string, libertam previamente a memória antiga
 * para evitar fugas de memória.
 * 
 * Campos numéricos são convertidos a partir de strings (ex.: preço, ID da pessoa).
 * 
 * Campos booleanos são interpretados a partir de `'t'` ou `'f'`.
 */

/* ============================================
 * USADO PARA SET
 * ============================================ */

Voos_reservados *cria0_lista_reserva (int n) {
    Voos_reservados *vr = calloc (1, sizeof *vr);
    vr->n_voos = n;
    vr->lista_voos_reservados = calloc (n, sizeof(char *));
    return vr;
}

void liberta_lista_reserva(void *data) {
    Voos_reservados *v = data;
    if (!v) return;
/*    if (v->lista_voos_reservados) {
        for (int i = 0; i < v->n_voos; i++) {
            if (v->lista_voos_reservados[i]) g_free(v->lista_voos_reservados[i]);
        }
        free(v->lista_voos_reservados);
    }*/
    free(v->lista_voos_reservados);
    g_free(v);
}

void set_lista_voos (Voos_reservados *vr, int i, char *s, GHashTable *lista_strings) {
    vr->lista_voos_reservados[i] = procura_string(lista_strings,s); //g_strdup(s);
}

void r_set_lista (Reservas *r, Voos_reservados *novo) {
    r->reserva_lista = novo;
}

void r_set_id_reserva (Reservas *r, char *s, GHashTable *lista_strings) {
    //g_free(r->id_reserva);
    r->id_reserva = procura_string(lista_strings,s); //g_strdup(s);
}

void r_set_id_pessoa_reservou (Reservas *r, char *s) {
    r->id_pessoa_reservou = atoi(s);
}

void r_set_preco (Reservas *r, char *s) {
    r->preco_reserva = atof(s);
}

/* ============================================
 * CRIA RESERVA
 * ============================================ */

Reservas *criaReserva () {
    Reservas *r = calloc (1, sizeof *r);
    r->reserva_lista->n_voos = 0;
    r->reserva_lista->lista_voos_reservados = NULL;
    return r;
}

/* ============================================
 * DESTRÓI RESERVA
 * ============================================ */

/**
 * Liberta todos os campos internos e a própria estrutura.
 */

void libertaReserva(void *data) {
    Reservas *a = data;
    if (!a) return;
    if (a->reserva_lista) liberta_lista_reserva (a->reserva_lista); 
    //if (a->id_reserva) g_free(a->id_reserva);
    g_free(a);
}
