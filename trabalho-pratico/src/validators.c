#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include "validators.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


//VOOS -> VALIDAÇÃO SINTÁTICA


//Valida o id do voo
int valida_id_voo (char* string, char **voo_id) {
    if (string == NULL || strlen(string) != 7) return 0;
    else {
        for (int i = 0; i < 7; i++) {
            if ((i < 2 && (string[i] < 'A' || string[i] > 'Z')) ||
               (i >= 2 && (string[i] < '0' || string[i] > '9'))) return 0;
        }
    }
//Validação conluida

    *voo_id = g_strdup(string);
    return 1;
}



//Auxiliar de parseData/parseDatah
int qual_mes (int mes) {
    if (mes == 2) return 3;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 2;
    else return 1;
}

//Valida a datah (ano-mes-dia horas:mins) e passa para a estrutura previamente definida para DataH
int valida_DataH (char *string, char **datah) { // com validação incluida
	if (string == NULL || strlen(string) != 16) return 0;
	if (string[4] != '-' || string[7] != '-' || string[10] != ' ' || string[13] != ':') return 0;
        //for (int i = 0; i < 16; i++) {
		//if (!isdigit(string[i]) && (i != 

        DataH novo;
        int narg = sscanf (string, "%d-%d-%d %d:%d", &novo.data.ano, &novo.data.mes, &novo.data.dia, &novo.horas.hora, &novo.horas.mins);
        if (narg != 5 || (novo.data.ano < 0 || novo.data.ano > 2025) ||                                                                                             //validação ano
                         (novo.data.mes < 1 || novo.data.mes > 12 || (novo.data.ano == 2025 && novo.data.mes > 11)) ||                                              //validação mes
                         (novo.data.dia < 0 || (qual_mes(novo.data.mes) == 1 && novo.data.dia > 31) || (qual_mes(novo.data.mes) == 2 && novo.data.dia > 30)) ||     //validação dia
                         (qual_mes(novo.data.mes) == 3 && ((novo.data.ano % 4 == 0 && novo.data.dia > 29) || (novo.data.ano % 4 != 0 && novo.data.dia > 28))) ||   //validação especial (mês de Fevereiro)
                         (novo.horas.hora < 0 || novo.horas.hora > 23 || novo.horas.mins < 0 || novo.horas.mins > 59))

                return 0;

        *datah = g_strdup(string);
        return 1;
}

//Valida a data (ano-mes-dia) e passa para a estrutura previamente definida para Data
int valida_Data (char *string, Data *data) {
    Data novo;
    int narg = sscanf (string, "%d-%d-%d", &novo.ano, &novo.mes, &novo.dia);
    if (narg != 3 || (novo.ano < 0 || novo.ano > 2025) ||                                                                                             //validação ano
                     (novo.mes < 1 || novo.mes > 12 || (novo.ano == 2025 && novo.mes > 11)) ||                                              //validação mes
                     (novo.dia < 0 || (qual_mes(novo.mes) == 1 && novo.dia > 31) || (qual_mes(novo.mes) == 2 && novo.dia > 30)) ||     //validação dia
                     (qual_mes(novo.mes) == 3 && ((novo.ano % 4 == 0 && novo.dia > 29) || (novo.ano % 4 != 0 && novo.dia > 28))))

                return 0;

    *data = novo;
    return 1;
}


//Valida o estado (voo) e passa para a estrutura previamente definida para estado
int valida_Estado (char *string, Estado *e) {
    if (string == NULL || strlen(string) == 0) return 0;
    if (strcmp(string, "On Time") == 0) *e = ESTADO_ON_TIME;
    else if (strcmp(string, "Delayed") == 0) *e = ESTADO_DELAYED;
    else if (strcmp(string, "Cancelled") == 0) *e = ESTADO_CANCELLED;
    else return 0;

    return 1; //Estado válido
}




//AEROPORTO -> VALIDAÇÃO SINTÁTICA


//Valida o codigoIATA e faz o strdup
int valida_codigoIATA (char* string, char **codigo_IATA) { // funcao traducao e validacao token para codigo_IATA_aeroporto
    //char* codigo_IATA; //codigo final
    if (string == NULL || strlen(string) != 3) return 0;
    for (int i = 0; i < 3; i++) {
        if (string[i] < 'A' || string[i] > 'Z') return 0;
    }

    //Verificação concluida

    *codigo_IATA = g_strdup(string);

    return 1; //codigo_IATA valido
}


// Valida as coordenadas (latitude e longitude) e faz o atof (double)
int valida_coordenadas (const char* string, int versao, double coordenada) {
//versao 1. latitude
//versao 2. longitude
    if (string == NULL || strlen(string) == 0) {
        return 0;
    }
    int length = strlen(string);


    int contador = 0;
    for (int i = 0; i < length; i++) {
        if ((string[i] == '-' && i != 0) || string[i] < '0' || string[i] > '9' || string[i] != '.') return 0;
        if (string[i] == '.') contador++;
    }
    if (contador > 1) return 0;


    coordenada = atof(string);
    switch (versao) {
        case 1: if (coordenada < -90 || coordenada > 90) return 0;
                break;
        case 2: if (coordenada < -180 || coordenada > 180) return 0;
                break;
    }

    return 1;
}



//Valido o tipo do aeroporto e passa para a estrutura previamente definida para o tipo de aeroporto
int valida_tipo_aer(const char *string, Tipo_aeroporto *t) {
    if (string == NULL || strlen(string) == 0) {
        *t = TIPO_ERROR;
        return 0;
    }

    if (strcmp(string, "small_airport") == 0) *t = TIPO_SMALL_AIRPORT;
    else if (strcmp(string, "medium_airport") == 0) *t = TIPO_MEDIUM_AIRPORT;
    else if (strcmp(string, "large_airport") == 0) *t = TIPO_LARGE_AIRPORT;
    else if (strcmp(string, "heliport") == 0) *t = TIPO_HELIPORT;
    else if (strcmp(string, "seaplane_base") == 0) *t = TIPO_SEAPLANE_BASE;
    else if (strcmp(string, "closed_airport") == 0) *t = TIPO_CLOSED_AIRPORT;
    else {
        *t = TIPO_ERROR;
        return 0;
    }

    return 1;
}


//AERONAVES -> VALIDAÇÃO SINTÁTICA

int valida_year (char *string, int *year) {
    if (string == NULL || strlen(string) != 4) return 0;
    for (int i = 0; i < 4; i++) {
	if (!isdigit(string[i])) return 0;
    }

//Validação concluida

    *year = atoi(string);
    return 1;
}


//PASSAGEIROS -> VALIDAÇÃO SINTÁTICA

//Valida o id do passageiro e faz o atoi (int)
int valida_id_passageiro(char* string, int *id_passageiro) {

    if (string == NULL || strlen(string) != 9) return 0;
    for (int i = 0; i < 9; i++) {
        if (string[i] < '0' || string[i] > '9') return 0;
    }

//Validação concluida

    *id_passageiro = atoi(string);
    return 1;
}


//Valida o Genero(passageiro) e passa para a estrutura previamente definida para o genero
int valida_genero(const char *string, Genero *g) {
    if (string == NULL || strlen(string) != 1) {
        *g = GENERO_ERROR;
        return 0;
    }

    if (strcmp(string, "M") == 0) *g = GENERO_M;
    else if (strcmp(string, "F") == 0) *g = GENERO_F;
    else if (strcmp(string, "O") == 0) *g = GENERO_O;
    else {
        *g = GENERO_ERROR;
        return 0;
    }

    return 1;
}


//Valida o email e faz o strdup (passageiros)
int valida_email(char *string, char **email) {
     if (string == NULL || strlen(string) == 0) return 0;
     int i = 0;
     int mark = i;
     //verificar username
     for (; string[i] != '@' && string[i] != '\0'; i++) {
        if (!(islower(string[i]) || isdigit(string[i]) || string[i] == '.')) return 0; //estranho
     }
     if (string[i] == '\0') return 0; // nada a seguir ao username
     int j = i + 1;
     if (j - mark < 2) return 0;//username nulo -> inválido
     mark = j;
     for (; string[j] != '.' && string[j] != '\0'; j++) {
        if (string[j] < 'a' || string[j] > 'z') return 0; //lstring inválida
     }
     if (string[j] == '\0') return 0; //dominio invalido
     int k = j + 1;
     if (k - mark < 2) return 0; //lstring vazia
     mark = k;
     for (; string[k] != '\0'; k++) {
        if (string[k] < 'a' || string[k] > 'z') return 0; //rstring sintaxe invalida
     }
     if (k - mark < 2 || k - mark > 3) return 0; // rstring tamanho invalido

     *email = g_strdup(string);
     return 1;
}


//RESERVAS -> VALIDAÇÃO SINTÁTICA


//Valida o id da reserva e faz o strdup
int valida_id_reserva (char* string, char **id_reserva) {
    if (string == NULL || strlen(string) != 10 || string[0] != 'R') return 0;
    for (int i = 1; i < 10; i++) {
        if (string[i] < '0' || string[i] > '9') return 0;
    }

    //Validaçao concluida

    *id_reserva = g_strdup(string);
    return 1;
}


//Valida a lista dos voos reservados, passa para o formato de uma lista e atribui a "Reserva"
int valida_voos_reservados(char *string, Voos_reservados *lista) { //char ***lista
    if (string == NULL || strlen(string) < 3) return 0; //[] invalido
    int len = strlen(string);
    if (string[0] != '[' || string[len-1] != ']') return 0; //verifica se tem os parenteses retos no inicio e no final
    //Voos_reservados novo;

    //Limpar a string
    char *string_voos = g_strdup(string + 1); // pula '['
    string_voos[strlen(string_voos)-1] = '\0';      // remove ']'

    // contar voos
    int n = 1;
    for (int i = 0; string_voos[i]; i++) {
        if (string_voos[i] == ',') n++;
    }

    Voos_reservados novo;
    novo.n_voos = n;
    novo.lista_voos_reservados = malloc(n * sizeof(char *));

    char *ptr = string_voos;
    for (int i = 0; i < n; i++) {
        char *token = strsep(&ptr,",");
        while (*token == ' ' || *token == '\'') token++; //tira os espaços e aspas simples
        char *end = token + strlen(token) - 1; //
        while (*end == ' ' || *end == '\'') *end-- = '\0';
        (novo.lista_voos_reservados)[i] = g_strdup(token);
    }

    *lista = novo;
    for (int i = 0; i < lista->n_voos; i++) {
	lista->lista_voos_reservados[i] = g_strdup(novo.lista_voos_reservados[i]);
    }
    //printf("%s\n", lista->lista_voos_reservados[0]);
/*    for (int i = 0; i < novo.n_voos; i++) {
	g_free(novo.lista_voos_reservados[i]);
	printf("%s\n", lista->lista_voos_reservados[i]);
    }
*/
    g_free(string_voos);
    return 1;
}


//Valida BAGAGEM/PRIORIDADE das Reservas e passa a string para um bool nos respetivos campos em Reserva
int valida_bool (char *string, bool *b) {
    if (string == NULL || strlen(string) == 0) { // string vazia
        return 0; // por default
    }
    if (strcmp (string, "true") == 0) *b = 1;
    else if (strcmp (string, "false") == 0) *b = 0;
    else return 0;

    return 1;
}

//FIM DA VALIDAÇÃO SINTÁTICA



//VALIDAÇÃO LÓGICA


//Função auxiliar a validação logica dos voos
int compara_dataH (char *datah1, char *datah2) { //se 1 for não anterior a 2, entao retorna 1 else retorna 0
//Se ano1<ano2, ent 2 vem depois de 1 : errado
//else ano1>ano2 : certo || ano1 == ano2 : proxima
    DataH data1;
    sscanf (datah1, "%d-%d-%d %d:%d", &data1.data.ano, &data1.data.mes, &data1.data.dia, &data1.horas.hora, &data1.horas.mins);
    DataH data2;
    sscanf (datah2, "%d-%d-%d %d:%d", &data2.data.ano, &data2.data.mes, &data2.data.dia, &data2.horas.hora, &data2.horas.mins);
    if (data1.data.ano > data2.data.ano) return 1;
    else if (data1.data.ano == data2.data.ano) {
             if (data1.data.mes > data2.data.mes) return 1;
             else if (data1.data.mes == data2.data.mes) {
                        if (data1.data.dia > data2.data.dia) return 1;
                        else if (data1.data.dia == data2.data.dia) {
                                if (data1.horas.hora > data2.horas.hora) return 1;
                                else if (data1.horas.hora == data2.horas.hora) {
                                        if (data1.horas.mins >= data2.horas.mins) return 1; //enunciado: "data nao anterior"
                                }
                        }
            }
    }

    return 0;
//Usar constantes para evitar tantos acessos continuos ao mesmo valor na memoria??
}


//VOOS -> VALIDAÇÃO LÓGICA

int valida_VOO (Voo voo, GHashTable *tabela) {
    //destino != origem
    if (strcmp (voo.code_origin, voo.code_destination) == 0) return 0;

    //cada voo tem uma aeronave correspondente EXISTENTE
    char *aeronave_chave = voo.id_aircraft;
    if (!g_hash_table_contains(tabela,aeronave_chave)) return 0; //tabela3

    //if CANCELLED, actual departure e actual arrival == "N/A"
    if (voo.status == 2) {
        if (strcmp(voo.actual_departure, "N/A") != 0 || strcmp(voo.actual_arrival, "N/A") != 0) return 0;
    }
    else {

        //arrival >= departure
        if (!compara_dataH(voo.arrival, voo.departure) ||
            !compara_dataH(voo.actual_arrival, voo.actual_departure)) return 0;

        //if DELAYED, actual departure/arrival >= departure/arrival
        if (voo.status == 1) {
                if (!compara_dataH(voo.actual_departure,voo.departure) ||
                    !compara_dataH(voo.actual_arrival,voo.arrival)) return 0;
        }
    }

    return 1; //Válido!
}


//RESERVAS -> VALIDAÇÃO LÓGICA


int valida_RESERVA (Reservas reserva, GHashTable *tabela_v, GHashTable *tabela_p) {
    //flights id -> lista de 1 ou 2 voos EXSITENTES
//    int length_vr = sizeof (reserva.lista_voos_reservados) / sizeof (reserva.voos_reservados[0]);
    int length_vr = reserva.reserva_lista.n_voos;
    if (length_vr < 1 || length_vr > 2) {printf("N"); return 0;}
    else {
        for (int i = 0; i < length_vr; i++) {
                char *voo_chave = g_strdup(reserva.reserva_lista.lista_voos_reservados[i]);
		//printf("%s\n", reserva.reserva_lista.lista_voos_reservados[i]);
		//printf("%s\n", voo_chave);
                if (!g_hash_table_contains(tabela_v,voo_chave)) return 0; // {printf("C_V"); return 0;}
        }
    }

    //document number -> passageiro EXISTENTE
    int passageiro_chave = reserva.id_pessoa_reservou;
    if (!g_hash_table_contains(tabela_p,GINT_TO_POINTER(passageiro_chave))) {printf("C_P"); return 0;}

    //if (flights ids == 2) -> destination1 == departure2, i.e., simulando uma escala
    if (length_vr == 2) {
        Voo *voo1 = g_hash_table_lookup(tabela_v,reserva.reserva_lista.lista_voos_reservados[0]);
        Voo *voo2 = g_hash_table_lookup(tabela_v,reserva.reserva_lista.lista_voos_reservados[1]);
        if (strcmp(voo1->code_destination, voo2->code_origin) != 0) {printf("L"); return 0;}
    }

    return 1; //Reserva válida!

}


//FIM DA VALIDAÇÃO LÓGICA

static bool is_digits(const char *s){
    if(!s || !*s) return false;
    for(; *s; ++s) if(!isdigit((unsigned char)*s)) return false;
    return true;
}

bool v_is_iata3(const char *s){
    if(!s || strlen(s)!=3) return false;
    for(int i=0;i<3;i++) if(!isalpha((unsigned char)s[i])) return false;
    return true; // normaliza para maiúsculas no chamador se necessário
}

bool v_is_airport_type(const char *s){
    if(!s) return false;
    return strcmp(s,"small_airport")==0 || strcmp(s,"medium_airport")==0 ||
           strcmp(s,"large_airport")==0 || strcmp(s,"heliport")==0 ||
           strcmp(s,"seaplane_base")==0;
}

bool v_is_lat(const char *s){
    if(!s || !*s) return false;
    char *end=NULL; double v=strtod(s,&end);
    if(end==s || *end!='\0') return false;
    return v>=-90.0 && v<=90.0;
}

bool v_is_lng(const char *s){
    if(!s || !*s) return false;
    char *end=NULL; double v=strtod(s,&end);
    if(end==s || *end!='\0') return false;
    return v>=-180.0 && v<=180.0;
}

bool v_is_flight_id(const char *s){
    if(!s || strlen(s)!=7) return false;
    return isalpha((unsigned char)s[0]) && isalpha((unsigned char)s[1]) &&
           isdigit((unsigned char)s[2]) && isdigit((unsigned char)s[3]) &&
           isdigit((unsigned char)s[4]) && isdigit((unsigned char)s[5]) &&
           isdigit((unsigned char)s[6]);
}

bool v_is_reservation_id(const char *s){
    if(!s || strlen(s)!=10) return false;
    if(s[0] != 'R') return false;
    return is_digits(s+1);
}

bool v_is_document_number(const char *s){
    return s && strlen(s)==9 && is_digits(s);
}

bool v_is_email(const char *s){
    if(!s) return false;                  // validação simples (suficiente para Fase 1)
    const char *at = strchr(s,'@'); if(!at || at==s) return false;
    const char *dot = strrchr(at+1,'.'); if(!dot || dot==at+1 || dot[1]=='\0') return false;
    return true;
}

bool v_is_gender(const char *s){
    return s && (strcmp(s,"M")==0 || strcmp(s,"F")==0 || strcmp(s,"O")==0);
}

bool v_is_status_valid(const char *s){
    if(!s) return false;
    return strcmp(s,"Scheduled")==0 || strcmp(s,"Delayed")==0 || strcmp(s,"Cancelled")==0;
}

static bool try_strptime(const char *s, const char *fmt, struct tm *out){
    if(!s || !out) return false;
    memset(out, 0, sizeof(*out));
    char *r = strptime(s, fmt, out);
    return r && *r=='\0';
}

bool v_parse_date(const char *s, struct tm *out){
    if(!s || !out) return false;
    // YYYY-MM-DD
    if(try_strptime(s, "%Y-%m-%d", out)) return true;
    // YYYY-MM-DD hh:mm
    if(try_strptime(s, "%Y-%m-%d %H:%M", out)) return true;
    // opcionalmente com segundos
    if(try_strptime(s, "%Y-%m-%d %H:%M:%S", out)) return true;
    return false;
}
