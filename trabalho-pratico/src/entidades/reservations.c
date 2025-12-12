#include "entidades/reservations.h"
#include <glib.h>
#include <stdbool.h>

typedef struct voos_reservados {
    char **lista_voos_reservados;
    int n_voos;
} Voos_reservados;

typedef struct reservas {
    char *id_reserva;
    Voos_reservados reserva_lista; //lista
    int id_pessoa_reservou; //reserva em nome de
    char *lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade; //priority boarding
    char *qr_code;
} Reservas;

//GETTERS

int get_lista_n_voos (Voos_reservados *lvr) {
    return lvr->n_voos;
}


char *r_get_id_reserva (Reservas *r) {
    return r->id_reserva;
}

int r_get_id_pessoa_reservou (Reservas *r) {
    return r->id_pessoa_reservou;
}

int r_get_lista_n_voos (Reservas *r) {
    return r->reserva_lista.n_voos;
}

char **r_get_lista_voos_reserv (Reservas *r) {
    return r->reserva_lista.lista_voos_reservados;
}

//SETTERS


Voos_reservados *cria0_lista_reserva (int n) {
    Voos_reservados *vr = calloc (1, sizeof *vr);
    vr->n_voos = n;
    vr->lista_voos_reservados = calloc (n, sizeof(char *));
    return vr;
}

void set_lista_voos (Voos_reservados *vr, int i, char *s) {
    vr->lista_voos_reservados[i] = g_strdup(s);
}

void r_set_lista (Reservas *r, Voos_reservados *novo) {
    r->reserva_lista = *novo;
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

//CRIA E DESTROI

Reservas *criaReserva () {
    Reservas *r = calloc (1, sizeof *r);
    r->reserva_lista.n_voos = 0;
    r->reserva_lista.lista_voos_reservados = NULL;
    return r;
}

void libertaReserva(void *data) {
    Reservas *a = data;
    if (!a) return;
    if (a->reserva_lista.lista_voos_reservados) {
        for (int i = 0; i < a->reserva_lista.n_voos; i++) {
            g_free(a->reserva_lista.lista_voos_reservados[i]);
        }
        free(a->reserva_lista.lista_voos_reservados);
    }
    g_free(a->lugar_reservado);
    g_free(a->qr_code);
    g_free(a);
}
