#include "entidades/passengers.h"
#include <glib.h>

typedef struct passageiros {
    int id_passageiro;
    char *primeiro_nome;
    char *ultimo_nome;
    int data_nascimento;
    char *nacionalidade;
    Genero genero_passageiro;
    char *email_passageiro;
    char *telefone_passageiro;
    char *morada_passageiro;
    char *fotografia_passageiro;
} Passageiros;

//GETTERS
int passenger_get_id (Passageiros *p) {
    return p->id_passageiro;
}

//SETTERS
void passenger_set_id (Passageiros *p, char *id) {
    p->id_passageiro = atoi(id);
}

void passenger_set_pn (Passageiros *p, char *pn) {
    g_free(p->primeiro_nome);
    p->primeiro_nome = g_strdup(pn);
}

void passenger_set_un (Passageiros *p, char *un) {
    g_free(p->ultimo_nome);
    p->ultimo_nome = g_strdup(un);
}

void passenger_set_dn (Passageiros *p, char *dn) {

    // extrair números
    int ano  = (dn[0]-'0')*1000 + (dn[1]-'0')*100 + (dn[2]-'0')*10 + (dn[3]-'0');
    int mes  = (dn[5]-'0')*10   + (dn[6]-'0');
    int dia  = (dn[8]-'0')*10   + (dn[9]-'0');

    // construir valor inteiro ordenável
    int data = ano * 10000 +
           mes *   100 +
           dia;

    p->data_nascimento = data;
}

void passenger_set_nc (Passageiros *p, char *nc) {
    g_free(p->nacionalidade);
    p->nacionalidade = g_strdup(nc);
}

void passanger_set_ge (Passageiros *p, char *ge) {
    switch (ge[0]) {
        case 'M' : p->genero_passageiro = GENERO_M;
		   break;
        case 'F' : p->genero_passageiro = GENERO_F;
		   break;
        case 'O' : p->genero_passageiro = GENERO_O;
		   break;
    }
}

void passenger_set_em (Passageiros *p, char *em) {
    g_free(p->email_passageiro);
    p->email_passageiro = strdup(em);
}

void passenger_set_tl (Passageiros *p, char *tl) {
    g_free(p->telefone_passageiro);
    p->telefone_passageiro = g_strdup(tl);
}

void passenger_set_md (Passageiros *p, char *md) {
    g_free(p->morada_passageiro);
    p->morada_passageiro = g_strdup(md);
}

void passenger_set_ft (Passageiros *p, char *ft) {
    g_free(p->fotografia_passageiro);
    p->fotografia_passageiro = g_strdup(ft);
}

//CRIA E DESTROI
Passageiros *criaPassageiro() {
    Passageiros *p = calloc (1, sizeof *p);
    return p;
}

void libertaPassageiro(void *data) {
    Passageiros *a = data;
    if (!a) return;
    g_free(a->primeiro_nome);
    g_free(a->ultimo_nome);
    g_free(a->nacionalidade);
    g_free(a->email_passageiro);
    g_free(a->telefone_passageiro);
    g_free(a->morada_passageiro);
    g_free(a->fotografia_passageiro);
    g_free(a);
}