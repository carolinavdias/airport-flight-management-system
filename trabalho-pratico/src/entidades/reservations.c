#include "entidades/reservations.h"
#include <glib.h>
#include <stdbool.h>
#include <stdlib.h>

/**
 * Contém a definição interna das estruturas `Reservas` e `Voos_reservados`,
 * bem como a implementação dos getters, setters e funções de criação e
 * destruição.
 */

// Estrutura que representa a lista de voos reservados 

typedef struct voos_reservados { 
    char **lista_voos_reservados;   // Array de IDs de voos 
    int n_voos;                     // Número de voos 
} Voos_reservados;

// Estrutura que representa uma reserva

typedef struct reservas {
    char *id_reserva;               // Identificador único da reserva
    Voos_reservados *reserva_lista;  // Lista de voos associados à reserva
    int id_pessoa_reservou;         // Identificador da pessoa em nome de quem a reserva foi efetuada
    char *lugar_reservado;          // Lugar atribuído ao passageiro no voo
    double preco_reserva;           // Preço total da reserva
    bool bagagem_extra;             // Indica se foi requisitada bagagem extra
    bool prioridade;                // Indica se é necessária prioridade de embarque
    char *qr_code;                  // Código QR associado à reserva
} Reservas;

// ===================================================
// GETTERS
// ===================================================

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

char *r_get_id_reserva (Reservas *r) {
    return g_strdup(r->id_reserva);
}

int r_get_id_pessoa_reservou (Reservas *r) {
    return r->id_pessoa_reservou;
}

int r_get_lista_n_voos (Reservas *r) {
    return r->reserva_lista->n_voos;
}

char **r_get_lista_voos_reserv (Reservas *r) {
    return r->reserva_lista->lista_voos_reservados;
}

double r_get_preco(Reservas *r) {
    if (!r) return 0.0;
    return r->preco_reserva;
}

char *r_get_voo_por_indice(Reservas *r, int indice) {
    if (!r) return NULL;
    if (indice < 0 || indice >= r->reserva_lista->n_voos) return NULL;
    if (!r->reserva_lista->lista_voos_reservados) return NULL;
    if (!r->reserva_lista->lista_voos_reservados[indice]) return NULL;
    
    // Retorna CÓPIA para encapsulamento - libertar com g_free()!
    return g_strdup(r->reserva_lista->lista_voos_reservados[indice]);
}

// ===================================================
// SETTERS
// ===================================================

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

// ===================================================
// USADO PARA SET
// ===================================================

Voos_reservados *cria0_lista_reserva (int n) {
    Voos_reservados *vr = calloc (1, sizeof *vr);
    vr->n_voos = n;
    vr->lista_voos_reservados = calloc (n, sizeof(char *));
    return vr;
}

void liberta_lista_reserva(void *data) {
    Voos_reservados *v = data;
    if (!v) return;
    if (v->lista_voos_reservados) {
        for (int i = 0; i < v->n_voos; i++) {
            if (v->lista_voos_reservados[i]) g_free(v->lista_voos_reservados[i]);
        }
        free(v->lista_voos_reservados);
    }
    g_free(v);
}

void set_lista_voos (Voos_reservados *vr, int i, char *s) {
    vr->lista_voos_reservados[i] = g_strdup(s);
}

void r_set_lista (Reservas *r, Voos_reservados *novo) {
    r->reserva_lista = novo;
}

void r_set_id_reserva (Reservas *r, char *s) {
    g_free(r->id_reserva);
    r->id_reserva = g_strdup(s);
}

void r_set_id_pessoa_reservou (Reservas *r, char *s) {
    r->id_pessoa_reservou = atoi(s);
}

void r_set_lugar (Reservas *r, char *s) {
    g_free(r->lugar_reservado);
    r->lugar_reservado = g_strdup(s);
}

void r_set_preco (Reservas *r, char *s) {
    r->preco_reserva = atof(s);
}

void r_set_bools (Reservas *r, char *s, int versao) {
     switch (s[0]) {
	case 't' : if (versao == 1) r->bagagem_extra = true;
		   else r->prioridade = true;
		   break;
	case 'f' : if (versao == 1) r->bagagem_extra = false;
		   else r->prioridade = false;
	    	   break;
     }
}

void r_set_qr_code (Reservas *r, char *s) {
     g_free(r->qr_code);
     r->qr_code = g_strdup(s);
}

// ===================================================
// CRIA RESERVA
// ===================================================

Reservas *criaReserva () {
    Reservas *r = calloc (1, sizeof *r);
    r->reserva_lista->n_voos = 0;
    r->reserva_lista->lista_voos_reservados = NULL;
    return r;
}

// ===================================================
// DESTRÓI RESERVA
// ===================================================

// Liberta todos os campos internos e a própria estrutura

void libertaReserva(void *data) {
    Reservas *a = data;
    if (!a) return;
    if (a->reserva_lista->lista_voos_reservados) {
        for (int i = 0; i < a->reserva_lista->n_voos; i++) {
            g_free(a->reserva_lista->lista_voos_reservados[i]);
        }
        free(a->reserva_lista->lista_voos_reservados);
    }
    g_free(a->id_reserva);
    g_free(a->lugar_reservado);
    g_free(a->qr_code);
    g_free(a);
}
