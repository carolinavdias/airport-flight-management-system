#include "validacoes_passangers.h"
#include "passangers.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

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

bool v_is_gender(const char *s){
    return s && (strcmp(s,"M")==0 || strcmp(s,"F")==0 || strcmp(s,"O")==0);
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