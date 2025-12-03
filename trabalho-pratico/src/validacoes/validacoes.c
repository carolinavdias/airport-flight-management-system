#define _GNU_SOURCE
#define _XOPEN_SOURCE 700

#include "validacoes.h"
#include "flights.h"
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <glib.h>

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

static bool is_digits(const char *s){
    if(!s || !*s) return false;
    for(; *s; ++s) if(!isdigit((unsigned char)*s)) return false;
    return true;
}

static bool try_strptime(const char *s, const char *fmt, struct tm *out){
    if(!s || !out) return false;
    memset(out, 0, sizeof(*out));
    char *r = strptime(s, fmt, out);
    return r && *r=='\0';
}