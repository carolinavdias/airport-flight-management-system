#include "entidades/passengers.h"
#include <glib.h>

typedef struct passageiros {
    int id_passageiro;
    char *primeiro_nome;
    char *ultimo_nome;
    int data_nascimento;
    char *nacionalidade;
} Passageiros;

//GETTERS
int passenger_get_id (Passageiros *p) {
    return p->id_passageiro;
}

char *passenger_get_primeiro (Passageiros *p) {
    return p->primeiro_nome;
}

char *passenger_get_ultimo (Passageiros *p) {
    return p->ultimo_nome;
}

int passenger_get_data (Passageiros *p) {
    return p->data_nascimento;
}

char *passenger_get_nacionalidade (Passageiros *p) {
    return p->nacionalidade;
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
    g_free(a);
}