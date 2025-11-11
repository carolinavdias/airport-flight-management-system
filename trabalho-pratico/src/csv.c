#define _POSIX_C_SOURCE 200809L
#include "csv.h"
#include <stdlib.h>
#include <string.h>

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
