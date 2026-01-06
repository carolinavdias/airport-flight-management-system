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
    int data_nascimento;     /**< Data de nascimento (YYYYMMDD) */
    const char *nacionalidade;     /**< Nacionalidade */
} Passageiros;

/* ============================================
 * GETTERS
 * ============================================ */

/**
 * As funções de acesso devolvem os valores dos campos internos.
 * 
 * Os getters que retornam strings devolvem cópias (g_strdup),
 * cabendo ao utilizador libertar a memória retornada.
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
 * Sempre que substituem uma string, libertam primeiro a memória antiga
 * para evitar fugas de memória.
 * 
 * A data de nascimento é convertida de "YYYY-MM-DD" para inteiro
 * ordenável "YYYYMMDD".
 */

void passenger_set_id (Passageiros *p, char *id, StringPool *pool) {
    p->id_passageiro =  string_pool_get(pool,id); //g_strdup(id);
}

void passenger_set_pn (Passageiros *p, char *pn, StringPool *pool) {
//    g_free(p->primeiro_nome);
    p->primeiro_nome = string_pool_get(pool,pn); //g_strdup(pn);
}

void passenger_set_un (Passageiros *p, char *un, StringPool *pool) {
   // g_free(p->ultimo_nome);
    p->ultimo_nome = string_pool_get(pool,un); //g_strdup(un);
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
    //g_free(p->nacionalidade);
    p->nacionalidade = string_pool_get(pool,nc); //g_strdup(nc);
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
//    if (a->id_passageiro) g_free(a->id_passageiro);
//    if (a->primeiro_nome) g_free(a->primeiro_nome);
//    if (a->ultimo_nome) g_free(a->ultimo_nome);
//    if (a->nacionalidade) g_free(a->nacionalidade);
    g_free(a);
}

