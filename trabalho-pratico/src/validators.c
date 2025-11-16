#define _GNU_SOURCE
#define _XOPEN_SOURCE 700
#include "validators.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


//VOOS -> VALIDAÇÃO SINTÁTICA

int valida_id_voo(char *s, char **voo_id)
{
    if (
        s[0] >= 'A' && s[0] <= 'Z' &&
        s[1] >= 'A' && s[1] <= 'Z' &&
        s[2] >= '0' && s[2] <= '9' &&
        s[3] >= '0' && s[3] <= '9' &&
        s[4] >= '0' && s[4] <= '9' &&
        s[5] >= '0' && s[5] <= '9' &&
        s[6] >= '0' && s[6] <= '9' &&
        (s[7] == '\0' || s[7] == '\n' || s[7] == '\r')
    ) {
    *voo_id = g_strdup(s);
    return 1;
    }

    return 0;
}

//Valida o id do voo
int valida_id_voo2 (char* string, char **voo_id) {
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
int qual_mes2 (int mes) {
    if (mes == 2) return 3;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 2;
    else return 1;
}

//dia com mes válido
int qual_mes (int ano, int mes) {
    if (mes == 2) {
	if (ano % 4 == 0) return 29;
	else return 28;
    }
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 30;
    return 31;
}

/*
static time_t parseDateH(char *dateStr) {
    struct tm tm = {0};
    if (!dateStr) return (time_t)-1;

    // tenta com hora e segundos
    if (strptime(dateStr, "%Y-%m-%d %H:%M", &tm) != NULL)
        return mktime(&tm);

    return (time_t)-1;
}
*/

//função auxiliar (converte uma data no formato "YYYY-MM-DD" ou "YYYY-MM-DD HH:MM:SS")
time_t parseDate_(const char *dateStr) {
    struct tm tm = {0};
    if (!dateStr) return (time_t)-1;

    // tenta com hora e segundos
    if (strptime(dateStr, "%Y-%m-%d %H:%M", &tm) != NULL)
        return mktime(&tm);

    if (strptime(dateStr, "%Y-%m-%d", &tm) != NULL)
        return mktime(&tm);


    return (time_t)-1;
}

time_t fast_convert(const DataH *d) {
    struct tm tm = {0};
    tm.tm_year = d->data.ano - 1900;
    tm.tm_mon  = d->data.mes - 1;
    tm.tm_mday = d->data.dia;
    tm.tm_hour = d->horas.hora;
    tm.tm_min  = d->horas.mins;
    tm.tm_sec  = 0;

    return mktime(&tm);
}

int valida_DataH2(const char *s, time_t *out)
{
    if (!s) return 0;

    // formato: YYYY-MM-DD HH:MM  (16 chars)
    if (strlen(s) != 16) return 0;

    // valida caracteres fixos
    if (s[4] != '-' || s[7] != '-' || s[10] != ' ' || s[13] != ':')
        return 0;

    // extrai tudo manualmente (ULTRA-RÁPIDO)
    DataH novo;

    novo.data.ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    novo.data.mes  = (s[5]-'0')*10   + (s[6]-'0');
    novo.data.dia  = (s[8]-'0')*10   + (s[9]-'0');
    novo.horas.hora= (s[11]-'0')*10  + (s[12]-'0');
    novo.horas.mins= (s[14]-'0')*10  + (s[15]-'0');

    // valida ano
    if (novo.data.ano < 1000 || novo.data.ano > 2025) return 0;

    // valida mês
    if (novo.data.mes < 1 || novo.data.mes > 12) return 0;

    // valida dias
    int maxdias = qual_mes(novo.data.ano,novo.data.mes);

    if (novo.data.dia < 1 || novo.data.dia > maxdias) return 0;

    // valida hora e minuto
    if (novo.horas.hora < 0 || novo.horas.hora > 23) return 0;
    if (novo.horas.mins < 0 || novo.horas.mins > 59) return 0;

   *out = fast_convert(&novo);
    return 1;
}


// Retorna 1 se válido; 0 se inválido.
// out = inteiro comparável crescente YYYYMMDDHHMM

int valida_DataH(const char *s, int *out)
{
    if (!s) return 0;

    // verificar que existem pelo menos 16 chars válidos
    if (!(s[0] && s[1] && s[2] && s[3] &&
          s[4] && s[5] && s[6] && s[7] &&
          s[8] && s[9] &&
          s[10] && s[11] && s[12] && s[13] &&
          s[14] && s[15]))
        return 0;

    // formato fixo
    if (s[4] != '-' || s[7] != '-' || s[10] != ' ' || s[13] != ':')
        return 0;

    // verificar que são dígitos
    if (!(
        s[0]>='0' && s[0]<='9' && s[1]>='0' && s[1]<='9' &&
        s[2]>='0' && s[2]<='9' && s[3]>='0' && s[3]<='9' &&
        s[5]>='0' && s[5]<='9' && s[6]>='0' && s[6]<='9' &&
        s[8]>='0' && s[8]<='9' && s[9]>='0' && s[9]<='9' &&
        s[11]>='0' && s[11]<='9' && s[12]>='0' && s[12]<='9' &&
        s[14]>='0' && s[14]<='9' && s[15]>='0' && s[15]<='9'
    )) return 0;

    // extrair números
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');
    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    // validar ranges
    if (ano < 0 || ano > 2025) return 0;
    if (mes < 1 || mes > 12) return 0;

    static const int mdias[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int maxd = mdias[mes];
    if (mes == 2 && (ano % 4 == 0)) maxd = 29;

    if (dia < 1 || dia > maxd) return 0;
    if (hora < 0 || hora > 23) return 0;
    if (min < 0 || min > 59) return 0;

    // construir valor inteiro ordenável
    *out = ano * 100000000 +
           mes *   1000000 +
           dia *     10000 +
           hora *       100 +
           min;

    return 1;
}

//BOA FUNCIONANDO
int valida_DataH9(const char *s, time_t *out)
{
    if (!s) return 0;

    // Formato obrigatório: "YYYY-MM-DD HH:MM" (16 chars)
    // Verificação SEM strlen (muito mais rápido)
    if (!(s[0] && s[1] && s[2] && s[3] &&
          s[4]=='-' &&
          s[5] && s[6] &&
          s[7]=='-' &&
          s[8] && s[9] &&
          s[10]==' ' &&
          s[11] && s[12] &&
          s[13]==':' &&
          s[14] && s[15] &&
          (s[16]=='\0' || s[16]=='\n' || s[16]=='\r')))
        return 0;

    // Extrair valores manualmente (SUPER RÁPIDO)
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');
    int hora = (s[11]-'0')*10  + (s[12]-'0');
    int min  = (s[14]-'0')*10  + (s[15]-'0');

    // Validações rápidas
    if (ano < 0 || ano > 2025) return 0;
    if (mes < 1 || mes > 12) return 0;

    // Dias no mês (a tua função qual_mes faz isto, mas aqui é mais rápido)
    int dias_mes[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int maxdia = dias_mes[mes];
    if (mes == 2 && ano % 4 == 0) maxdia = 29; // ano bissexto

    if (dia < 1 || dia > maxdia) return 0;
    if (hora < 0 || hora > 23) return 0;
    if (min < 0 || min > 59) return 0;

    // Construir struct tm
    struct tm tm = {0};
    tm.tm_year = ano - 1900;
    tm.tm_mon  = mes - 1;
    tm.tm_mday = dia;
    tm.tm_hour = hora;
    tm.tm_min  = min;
    tm.tm_sec  = 0;

    // Converter p/ time_t (inevitável, mas ok)
    *out = mktime(&tm);
    return (*out != (time_t)-1);
}

//Valida a datah (ano-mes-dia horas:mins) e passa para a estrutura previamente definida para DataH
int valida_DataH3 (char *string, time_t *datah) { // com validação incluida
    if (string == NULL || (strlen(string) != 16 && strlen(string) != 19)) return 0;
	if (string[4] != '-' || string[7] != '-' || string[10] != ' ' || string[13] != ':') return 0;

        DataH novo;
        int narg = sscanf (string, "%d-%d-%d %d:%d", &novo.data.ano, &novo.data.mes, &novo.data.dia, &novo.horas.hora, &novo.horas.mins);
        if (narg != 5 || (novo.data.ano < 0 || novo.data.ano > 2025) ||                                                                                             //validação ano
                         (novo.data.mes < 1 || novo.data.mes > 12 || (novo.data.ano == 2025 && novo.data.mes > 11)) ||                                              //validação mes
                         (novo.data.dia < 0 || (qual_mes2(novo.data.mes) == 1 && novo.data.dia > 31) || (qual_mes2(novo.data.mes) == 2 && novo.data.dia > 30)) ||     //validação dia
                         (qual_mes2(novo.data.mes) == 3 && ((novo.data.ano % 4 == 0 && novo.data.dia > 29) || (novo.data.ano % 4 != 0 && novo.data.dia > 28))) ||   //validação especial (mês de Fevereiro)
                         (novo.horas.hora < 0 || novo.horas.hora > 23 || novo.horas.mins < 0 || novo.horas.mins > 59))

                return 0;
	*datah = fast_convert(&novo);
        return 1;
}


bool v_parse_date2(const char *s, time_t *out){
    if(!s || !out) return false;

    struct tm tm;
    memset(&tm, 0, sizeof(tm));

    // tenta "YYYY-MM-DD HH:MM"
    char *end = strptime(s, "%Y-%m-%d %H:%M", &tm);
    if(!end || *end != '\0') return false;

    // mktime valida datas impossíveis?
    // NÃO! Ajusta automaticamente → precisamos verificar.
    time_t t = mktime(&tm);
    if (t == -1) return false;

    // reconstrói data e compara com a original
    struct tm tm2 = *localtime(&t);
    if (tm.tm_year != tm2.tm_year ||
        tm.tm_mon  != tm2.tm_mon  ||
        tm.tm_mday != tm2.tm_mday ||
        tm.tm_hour != tm2.tm_hour ||
        tm.tm_min  != tm2.tm_min)
        return false;   // data inválida (ex: 2025-02-30)

    *out = t;
    return true;
}


//Valida a data (ano-mes-dia) e passa para a estrutura previamente definida para Data
int valida_Data (char *string, Data *data) {
    Data novo;
    int narg = sscanf (string, "%d-%d-%d", &novo.ano, &novo.mes, &novo.dia);
    if (narg != 3 || (novo.ano < 0 || novo.ano > 2025) ||                                                                                             //validação ano
                     (novo.mes < 1 || novo.mes > 12 || (novo.ano == 2025 && novo.mes > 11)) ||                                              //validação mes
                     (novo.dia < 0 || (qual_mes2(novo.mes) == 1 && novo.dia > 31) || (qual_mes2(novo.mes) == 2 && novo.dia > 30)) ||     //validação dia
                     (qual_mes2(novo.mes) == 3 && ((novo.ano % 4 == 0 && novo.dia > 29) || (novo.ano % 4 != 0 && novo.dia > 28))))

                return 0;

    *data = novo;
    return 1;
}

int valida_Estado(const char *s, Estado *e)
{
    if (!s) return 0;

    if (s[0] == 'O' && memcmp(s, "On Time", 7) == 0 && s[7] == '\0') {
        *e = ESTADO_ON_TIME; return 1;
    }

    if (s[0] == 'D' && memcmp(s, "Delayed", 7) == 0 && s[7] == '\0') {
        *e = ESTADO_DELAYED; return 1;
    }

    if (s[0] == 'C' && memcmp(s, "Cancelled", 9) == 0 && s[9] == '\0') {
        *e = ESTADO_CANCELLED; return 1;
    }

    return 0;
}

//Valida o estado (voo) e passa para a estrutura previamente definida para estado
int valida_Estado2 (char *string, Estado *e) {
    if (string == NULL || strlen(string) == 0) return 0;
    if (strcmp(string, "On Time") == 0) *e = ESTADO_ON_TIME;
    else if (strcmp(string, "Delayed") == 0) *e = ESTADO_DELAYED;
    else if (strcmp(string, "Cancelled") == 0) *e = ESTADO_CANCELLED;
    else return 0;

    return 1; //Estado válido
}




//AEROPORTO -> VALIDAÇÃO SINTÁTICA


//Valida o codigoIATA e faz o strdup
int valida_codigoIATA (char* string, char **codigo_IATA) {
    if (string[0] >= 'A' && string[0] <= 'Z' &&
	string[1] >= 'A' && string[1] <= 'Z' &&
	string[2] >= 'A' && string[2] <= 'Z' &&
	string[3] == '\0') {
	*codigo_IATA = g_strdup(string);
	return 1;
    }

    return 0;
}


// Valida as coordenadas (latitude e longitude) e faz o atof (double)
int valida_coordenadas (const char* string, int versao, double *coordenada) {
//versao 1. latitude
//versao 2. longitude
    if (string == NULL || strlen(string) == 0) {
        return 0;
    }
    int length = strlen(string);


    int contador = 0;
    for (int i = 0; i < length; i++) {
    	if (string[i] == '-' && i != 0) return 0;
    	if (!isdigit(string[i]) && string[i] != '.' && string[i] != '-') return 0;
        if (string[i] == '.') contador++;
    }
    if (contador > 1) return 0;


    *coordenada = atof(string);
    switch (versao) {
        case 1: if (*coordenada < -90 || *coordenada > 90) return 0;
                break;
        case 2: if (*coordenada < -180 || *coordenada > 180) return 0;
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
        if (!(islower(string[i]) || isdigit(string[i]) || string[i] == '.')) return 0;
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
int valida_voos_reservados(char *string, Voos_reservados *lista) {
    if (string == NULL || strlen(string) < 3) return 0; //[] invalido
    int len = strlen(string);
    if (string[0] != '[' || string[len-1] != ']') return 0; //verifica se tem os parenteses retos no inicio e no final

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
        char *token = strsep(&ptr, ",");
        while (*token == ' ' || *token == '\'') token++;
        char *end = token + strlen(token) - 1;
        while (end >= token && (*end == ' ' || *end == '\'')) *end-- = '\0';

	if (!valida_id_voo(token,&novo.lista_voos_reservados[i])) return 0; //flight id invalido
    }

    *lista = novo;
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
}


//VOOS -> VALIDAÇÃO LÓGICA

int valida_VOO (Voo voo, GHashTable *tabela) {
    //destino != origem
    if (strcmp (voo.code_origin, voo.code_destination) == 0) return 0;

    //cada voo tem uma aeronave correspondente EXISTENTE
    char *aeronave_chave = voo.id_aircraft;
    if (!g_hash_table_contains(tabela,aeronave_chave)) return 0;

    //if CANCELLED, actual departure e actual arrival == "N/A"
    if (voo.status == 2) {
        // CANCELLED: actual deve ser N/A (-2)
        if (voo.actual_departure != -2 || voo.actual_arrival != -2) return 0;
        // arrival deve ser DEPOIS de departure (pode ser igual)
        if (voo.arrival < voo.departure) return 0;
    }
    else {
        // arrival DEPOIS de departure (não pode ser igual)
        if (voo.arrival < voo.actual_departure || voo.actual_arrival < voo.actual_departure) return 0;

        //if DELAYED, actual departure/arrival >= departure/arrival (pode ser igual!)
        if (voo.status == 1) {
            if (voo.actual_departure < voo.departure || voo.actual_arrival < voo.arrival) return 0;
        }
    }

    return 1; //Válido!
}


//RESERVAS -> VALIDAÇÃO LÓGICA


int valida_RESERVA (Reservas reserva, GHashTable *tabela_v, GHashTable *tabela_p) {
    //flights id -> lista de 1 ou 2 voos EXSITENTES
    int length_vr = reserva.reserva_lista.n_voos;
    if (length_vr < 1 || length_vr > 2) return 0;
    else {
        for (int i = 0; i < length_vr; i++) {
                char *voo_chave = reserva.reserva_lista.lista_voos_reservados[i];
                if (!g_hash_table_contains(tabela_v, voo_chave)) return 0;
        }
    }

    //document number -> passageiro EXISTENTE
    int passageiro_chave = reserva.id_pessoa_reservou;
    if (!g_hash_table_contains(tabela_p,GINT_TO_POINTER(passageiro_chave))) return 0;

    //if (flights ids == 2) -> destination1 == departure2, i.e., simulando uma escala
    if (length_vr == 2) {
        Voo *voo1 = g_hash_table_lookup(tabela_v,reserva.reserva_lista.lista_voos_reservados[0]);
        Voo *voo2 = g_hash_table_lookup(tabela_v,reserva.reserva_lista.lista_voos_reservados[1]);
        if (strcmp(voo1->code_destination, voo2->code_origin) != 0) return 0;
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
    if(!s) return false;
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