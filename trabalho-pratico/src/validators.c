#define _XOPEN_SOURCE 700
#include "validators.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

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
