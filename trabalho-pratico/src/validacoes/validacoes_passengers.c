#include "validacoes/validacoes_passengers.h"

#include "utils/utils.h"

#include <string.h>
#include <ctype.h>

//PASSAGEIROS -> VALIDAÇÃO SINTÁTICA

//valida o id do passageiro e faz o atoi (int)
int valida_id_passageiro(const char *s) {

    if (!s) return 0;
    for (int i = 0; i < 9; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }
    return s[9] == '\0'; //string muito grande
}

//valida o Genero(passageiro) e passa para a estrutura previamente definida para o genero
int valida_genero(const char *s) {
    return s && strlen(s) == 1 && (s[0] == 'M' || s[0] == 'F' || s[0] == 'O');
}

//valida o email e faz o strdup (passageiros)
int valida_email(const char *s) {
     if (!s || strlen(s) == 0) return 0;
     int i = 0;
     int mark = i;
     //verificar username
     for (; s[i] != '@' && s[i] != '\0'; i++) {
        if (!(islower(s[i]) || isdigit(s[i]) || s[i] == '.')) return 0;
     }
     if (s[i] == '\0') return 0; // nada a seguir ao username ou sem '@'
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

     return 1;
}

int valida_Data (const char *s) {
    if (!s) return 0;

    int tamanho = 0;
    for (; s[tamanho] != '\0'; tamanho++);
    if (tamanho != 10) return 0; // tamanho errado

    // formato fixo
    if (s[4] != '-' || s[7] != '-') return 0;

    //verificar que são dígitos
    const int indices[] = {0,1,2,3,5,6,8,9};  // 8 dígitos para YYYY-MM-DD
    for (int i = 0; i < 8; i++) {  // 8 em vez de 12!
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

    return 1;
}


Passageiros *validacoes_campos_passengers(char **campos) {
    Passageiros *p = criaPassageiro();

    if (valida_id_passageiro(campos[0]) && //id_passageiro
        valida_Data(campos[3])          && //data de nascimento
        valida_genero(campos[5])        && //genero
        valida_email(campos[6])         )  //email
    {

        passenger_set_id(p,campos[0]);
        passenger_set_pn(p,campos[1]);
        passenger_set_un(p,campos[2]);
        passenger_set_dn(p,campos[3]);
        passenger_set_nc(p,campos[4]);

        return p;
    }
    else return NULL;
}
