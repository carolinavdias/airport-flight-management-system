#include "entidades/flights.h"
#include "utils/utils.h"

/**
 * Contém a definição da estrutura interna Voo e a implementação
 * das funções de acesso (getters), modificação (setters), 
 * conversão de datas e libertação de memória associada aos voos 
 * lidos do ficheiro flights.csv.
 */

/**
 * Estrutura interna que representa um voo
 */

typedef struct voo {
    const char *flight_id;              /**< Identificador único do voo */
    long long departure;                /**< Data/hora prevista de partida */
    long long actual_departure;         /**< Data/hora efetiva de partida */
    long long arrival;                  /**< Data/hora prevista de chegada */
    long long actual_arrival;           /**< Data/hora efetiva de chegada */
    Estado status;                      /**< Estado atual do voo */
    const char *code_origin;            /**< Código IATA do aeroporto de origem */
    const char *code_destination;       /**< Código IATA do aeroporto de destino */
    const char *id_aircraft;            /**< Identificador da aeronave */
    const char *airline;                /**< Companhia aérea */
} Voo;

/* ============================================
 * GETTERS
 * ============================================ */


/**
 * As funções de acesso permitem consultar os campos internos
 * da estrutura Voo.  
 *
 * Os getters que retornam strings devolvem cópias (g_strdup),
 * cabendo ao utilizador libertar a memória retornada.
 * 
 * Os getters que retornam `const char *` devolvem ponteiros internos
 * que não devem ser libertados, sendo apenas para leitura.
 * 
 * Os campos numéricos (datas e horas no formato inteiro ordenável)
 * são devolvidos diretamente.
 *
 * Esta abordagem garante encapsulamento e evita que o utilizador
 * modifique acidentalmente os dados internos do voo.
 */

const char *voo_get_flight_id (const Voo *v) {
    return v->flight_id;
}

const char *voo_get_code_origin (const Voo *v) {
    return v->code_origin; 
}

long long voo_get_departure (const Voo *v) {
    return v->departure;
}

long long voo_get_actual_departure (const Voo *v) {
    return v->actual_departure;
}

Estado voo_get_status (const Voo *v) {
    return v->status;
}

const char *voo_get_id_aircraft (const Voo *v) {
    return v->id_aircraft;  
}

const char *voo_get_code_destination (const Voo *v) {
    return v->code_destination;  
}

long long voo_get_arrival (const Voo *v) {
    return v->arrival;
}

long long voo_get_actual_arrival (const Voo *v) {
    return v->actual_arrival;
}

const char *voo_get_airline(const Voo *v) {
    return v->airline;
}

/* ============================================
 * SETTERS
 * ============================================ */

/**
 * As funções de modificação atualizam os campos internos
 * da estrutura Voo.
 *
 * Sempre que substituem uma string, libertam previamente a memória
 * antiga com `g_free()` para evitar fugas de memória.
 * 
 * Os setters de datas e horas convertem strings no formato
 * `"YYYY-MM-DD HH:MM"` para um valor inteiro ordenável no formato
 * `YYYYMMDDHHMM`, permitindo comparações diretas.
 * 
 * O setter do estado (`status`) converte o texto do dataset para
 * o enum interno `Estado`, usando apenas o primeiro caractere.
 * 
 * Campos vazios no dataset são tratados explicitamente, sendo
 * representados pelo valor `-2` quando aplicável.
 *
 * Esta secção garante que todos os campos do voo são atualizados
 * de forma segura, consistente e sem fugas de memória.
 */


/**
 * O parâmetro versao indica qual dos campos deve ser atualizado:
 *  - 'o' → aeroporto de origem
 *  - 'd' → aeroporto de destino
 */

void voo_set_code (Voo *v, char *code, char versao, StringPool *pool) {
   switch (versao) {
	case 'o' : 
		   v->code_origin = string_pool_get(pool,code); 
		   break;
 	case 'd' : 
		   v->code_destination = string_pool_get(pool,code); 
		   break;
   }
}

void voo_set_flight_id (Voo *v, char *id, StringPool *pool) {
   v->flight_id = string_pool_get(pool,id); 
}

void voo_set_id_aircraft (Voo *v, char *id_airc, StringPool *pool) {
   v->id_aircraft = string_pool_get(pool,id_airc); 
}

void voo_set_airline (Voo *v, char *airl, StringPool *pool) {
//   g_free(v->airline);
   v->airline = string_pool_get(pool,airl); 
}

/**
 * Conversão do estado textual do dataset para enum interno
 * 
 * A decisão é feita com base no primeiro caractere:
 *  - 'O' → ON_TIME
 *  - 'D' → DELAYED
 *  - 'C' → CANCELLED
 */

void voo_set_status (Voo *v, char *status) {
   switch (status[0]) {
	case 'O' : v->status = ESTADO_ON_TIME;
	      break;
	case 'D' : v->status = ESTADO_DELAYED;
	      break;
	case 'C' : v->status = ESTADO_CANCELLED;
	      break;
   }
}

/* ============================================
 * USADO PARA SET
 * ============================================ */

/**
 * Converte uma data/hora textual para um valor inteiro ordenável
 * 
 * Formato esperado: YYYY-MM-DD HH:MM
 * O valor devolvido permite comparações diretas entre datas.
 */

long long converte_dataH (const char *s) {

    // extrair números
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');
    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    // construir valor inteiro ordenável
    return (long long)ano * 100000000 +
           (long long)mes *   1000000 +
           (long long)dia *     10000 +
           (long long)hora *      100 +
           (long long)min ;
}

/**
 * O parâmetro campo identifica o atributo a preencher:
 *  1 → departure
 *  2 → actual_departure
 *  3 → arrival
 *  4 → actual_arrival
 */

void voo_set_dataH (Voo *v, const char *s, int campo) {

    /**
     * Campo vazio indica ausência de valor no dataset.
     * O valor -2 é usado para representar dados não
     * disponíveis.
    */

    if (s[0] == '\0' && campo == 2) { v->actual_departure = (long long)-2; return; }
    if (s[0] == '\0' && campo == 4) { v->actual_arrival = (long long)-2; return; }

    switch (campo) {
        case 1 : v->departure = converte_dataH(s);
                 break;
        case 2 : v->actual_departure = converte_dataH(s);
                 break;
        case 3 : v->arrival = converte_dataH(s);
                 break;
        case 4 : v->actual_arrival = converte_dataH(s);
                 break;

    }
}

/* ============================================
 * CRIA VOO
 * ============================================ */

Voo *criaVoo () {
    Voo *v = calloc (1, sizeof *v);
    return v;
}

/* ============================================
 * DESTRÓI VOO
 * ============================================ */

/**
 * Liberta todos os campos internos e a própria estrutura
 */

void libertaVoo(void *data) {
    Voo *v = data;
    if (!v) return;
    g_free(v);
}

/**
 * Recebe dois elementos genéricos (void *) conforme exigido pelo qsort(),
 * que são convertidos para apontadores para Voo.
 *
 * A ordenação é feita por ordem crescente da data/hora de partida real,
 * permitindo percursos sequenciais eficientes nas queries.
 */

int compara_voos_por_data(const void *a, const void *b) {
    Voo *v1 = *(Voo **)a;
    Voo *v2 = *(Voo **)b;

    long long t1 = voo_get_actual_departure(v1);
    long long t2 = voo_get_actual_departure(v2);

    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}
