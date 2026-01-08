#include "entidades/passengers.h"
#include "utils/utils.h"

/**
 * Este ficheiro contém a definição da estrutura interna Passageiros
 * e a implementação das funções de acesso (getters), modificação 
 * (setters), conversão de datas e libertação de memória associada 
 * aos passageiros lidos do ficheiro passengers.csv.
 */

/**
 * Estrutura interna que representa um passageiro.
 */

typedef struct passageiros {
    const char *id_passageiro;     /**< Identificador único do passageiro */
    const char *primeiro_nome;     /**< Primeiro nome */
    const char *ultimo_nome;       /**< Último nome */
    int data_nascimento;           /**< Data de nascimento (YYYYMMDD) */
    const char *nacionalidade;     /**< Nacionalidade */
} Passageiros;

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * As funções de acesso devolvem os valores dos campos internos.
 * 
 * Os getters retornam ponteiros const para strings geridas pela StringPool.
 * 
 * O getter do identificador devolve um ponteiro interno, pois este
 * campo é imutável após a criação.
 */

const char *passenger_get_id (const Passageiros *p) {
    return p->id_passageiro; 
}

const char *passenger_get_primeiro (const Passageiros *p) {
    return p->primeiro_nome;
}

const char *passenger_get_ultimo (const Passageiros *p) {
    return p->ultimo_nome;
}

int passenger_get_data (Passageiros *p) {
    return p->data_nascimento;
}

const char *passenger_get_nacionalidade (const Passageiros *p) {
    return p->nacionalidade;  
}

/* ============================================
 * SETTERS
 * ============================================ */

/**
 * As funções de modificação atualizam os campos internos.
 * 
 * As strings são geridas pelo StringPool, pelo que não é necessário
 * libertar memória antes de atualizar os campos.
 * 
 * A data de nascimento é convertida de "YYYY-MM-DD" para inteiro
 * ordenável "YYYYMMDD".
 */

void passenger_set_id (Passageiros *p, char *id, StringPool *pool) {
    p->id_passageiro =  string_pool_get(pool,id); 
}

void passenger_set_pn (Passageiros *p, char *pn, StringPool *pool) {
    p->primeiro_nome = string_pool_get(pool,pn); 
}

void passenger_set_un (Passageiros *p, char *un, StringPool *pool) {
    p->ultimo_nome = string_pool_get(pool,un); 
}


/**
 * A data textual no formato YYYY-MM-DD é convertida
 * para um inteiro ordenável no formato YYYYMMDD.
 */


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

void passenger_set_nc (Passageiros *p, char *nc, StringPool *pool) {
    p->nacionalidade = string_pool_get(pool,nc); 
}

/* ============================================
 * CRIA PASSAGEIRO
 * ============================================ */

Passageiros *criaPassageiro() {
    Passageiros *p = calloc (1, sizeof *p);
    return p;
}

/* ============================================
 * DESTRÓI PASSAGEIRO
 * ============================================ */

/**
 * Liberta todos os campos internos e a própria estrutura
 */

void libertaPassageiro(void *data) {
    Passageiros *a = data;
    if (!a) return;
    g_free(a);
}

