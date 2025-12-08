#include "validacoes_passengers.h"
#include "passengers.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

//PASSAGEIROS -> VALIDAÇÃO SINTÁTICA

//Valida o id do passageiro e faz o atoi (int)
int valida_id_passageiro(const char* s, int *id_passageiro) {

    if (!s || strlen(s) != 9) return 0;
    for (int i = 0; i < 9; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }

//Validação concluida

    *id_passageiro = atoi(s);
    return 1;
}

//Valida o Genero(passageiro) e passa para a estrutura previamente definida para o genero
int valida_genero(const char *s, Genero *g) {
    if (!s || strlen(s) != 1) {
        return 0;
    }
    switch (s[0]) {
        case 'M' : *g = GENERO_M;
                   return 1;
        case 'F' : *g = GENERO_F;
                   return 1;
        case 'O' : *g = GENERO_O;
                   return 1;
        default : return 0;
   }
}


//Valida o email e faz o strdup (passageiros)
int valida_email(const char *string, char **email) {
     if (!s || strlen(s) == 0) return 0;
     int i = 0;
     int mark = i;
     //verificar username
     for (; s[i] != '@' && s[i] != '\0'; i++) {
        if (!(islower(s[i]) || isdigit(s[i]) || s[i] == '.')) return 0;
     }
     if (s[i] == '\0') return 0; // nada a seguir ao username
     int j = i + 1;
     if (j - mark < 2) return 0;//username nulo -> inválido
     mark = j;
     for (; s[j] != '.' && s[j] != '\0'; j++) {
        if (s[j] < 'a' || s[j] > 'z') return 0; //lstring inválida
     }
     if (s[j] == '\0') return 0; //dominio invalido
     int k = j + 1;
     if (k - mark < 2) return 0; //lstring vazia
     mark = k;
     for (; s[k] != '\0'; k++) {
        if (s[k] < 'a' || s[k] > 'z') return 0; //rstring sintaxe invalida
     }
     if (k - mark < 2 || k - mark > 3) return 0; // rstring tamanho invalido

     *email = g_strdup(s);
     return 1;
}

int valida_Data (const char *s, int *data) {
    if (!s) return 0;

    int tamanho = 0;
    for (; s[tamanho] != '\0'; tamanho++);
    if (tamanho != 10) return 0; // tamanho errado

    // formato fixo
    if (s[4] != '-' || s[7] != '-') return 0;

    //verificar que são dígitos
    const int indices[] = {0,1,2,3,5,6,8,9,11,12,14,15};
    for (int i = 0; i < 12; i++) {
        if (s[indices[i]] < '0' || s[indices[i]] >'9') return 0;
    }

    // extrair números
    int ano  = (s[0]-'0')*1000 + (s[1]-'0')*100 + (s[2]-'0')*10 + (s[3]-'0');
    int mes  = (s[5]-'0')*10   + (s[6]-'0');
    int dia  = (s[8]-'0')*10   + (s[9]-'0');

    // validar ranges
    if (ano < 0 || ano > 2025) return 0;
    if (mes < 1 || mes > 12) return 0;
    if (dia < 1 || dia > qual_mes(ano,mes)) return 0;

    // construir valor inteiro ordenável
    *data = ano * 10000 +
           mes *   100 +
           dia;

    return 1;
}

