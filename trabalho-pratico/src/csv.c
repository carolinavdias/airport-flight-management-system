#define _POSIX_C_SOURCE 200809L
#include "csv.h"
#include <stdlib.h>
#include <string.h>
#include <glib.h>

int csv_split(const char *line, char ***out_fields, size_t *out_n){
    if(!line || !out_fields || !out_n) return -1;

    size_t cap=8, n=0;
    char **v = malloc(cap*sizeof(*v));
    if(!v) return -1;

    const char *p = line;
    while(1){
        if(n==cap){
            cap*=2;
            char **nv = realloc(v, cap*sizeof(*nv));
            if(!nv){ for(size_t i=0;i<n;i++) free(v[i]); free(v); return -1; }
            v = nv;
        }
        char *field=NULL; size_t flen=0, fcap=64;
        field = malloc(fcap);
        if(!field){ for(size_t i=0;i<n;i++) free(v[i]); free(v); return -1; }

        int quoted = 0;
        if(*p=='"'){ quoted=1; p++; }

        while(*p){
            if(quoted){
                if(*p=='"'){
                    if(p[1]=='"'){ // escaped quote
                        if(flen+1>=fcap){ fcap*=2; field=realloc(field,fcap); }
                        field[flen++]='"'; p+=2; continue;
                    } else { // end quote
                        p++; quoted=0; break;
                    }
                } else {
                    if(flen+1>=fcap){ fcap*=2; field=realloc(field,fcap); }
                    field[flen++]=*p++; continue;
                }
            } else {
                if(*p==',') break;
                if(*p=='\r'){ p++; continue; }
                if(*p=='\n') break;
                if(flen+1>=fcap){ fcap*=2; field=realloc(field,fcap); }
                field[flen++]=*p++; continue;
            }
        }
        field[flen]='\0';
        v[n++]=field;

        if(*p==','){ p++; continue; }
        while(*p=='\r') p++;
        if(*p=='\n') p++;
        break;
    }

    *out_fields = v;
    *out_n = n;
    return 0;
}

void csv_free_fields(char **fields, size_t n){
    if(!fields) return;
    for(size_t i=0;i<n;i++) free(fields[i]);
    free(fields);
}


// Função para parse CSV com aspas duplas como delimitadores

gchar **parse_csv_line(const gchar *line) {
    GPtrArray *tokens = g_ptr_array_new_with_free_func(g_free); // array de strings
    const gchar *p = line;
    while (*p != '\0') {
        // Avança as aspas iniciais
        if (*p == '"') p++;
        GString *token = g_string_new(NULL);

        int in_quotes = 1;
        while (*p != '\0' && in_quotes) {
            if (*p == '"') {
                //aspas do fim do campo
                in_quotes = 0;
            } else {
                g_string_append_c(token, *p);
            }
            p++;
        }

        g_ptr_array_add(tokens, g_string_free(token, FALSE));

        // p aponta para aspas do fim ou para o fim da string
        if (*p == '"') p++; // avançar depois da aspa final

        if (*p == ',') p++; // avançar depois da vírgula
    }

    // transformar GPtrArray em gchar**
    g_ptr_array_add(tokens, NULL);
    return (gchar **)g_ptr_array_free(tokens, FALSE);
}
