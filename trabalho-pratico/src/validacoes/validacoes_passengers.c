#include "validacoes/validacoes_passengers.h"
#include "validacoes/validacoes.h"
#include "utils/utils.h"

#include <string.h>
#include <ctype.h>

/* ============================================
 * PASSAGEIROS -> VALIDAÇÃO SINTÁTICA
 * ============================================ */

/**
 * Valida o Genero(passageiro) e passa para a estrutura previamente definida para o genero
 */

int valida_genero(const char *s) {
    return s && strlen(s) == 1 && (s[0] == 'M' || s[0] == 'F' || s[0] == 'O');
}

/**
 * Valida o email e faz o strdup (passageiros)
 */

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

Passageiros *validacoes_campos_passengers(char **campos, GHashTable *lista_strings) {
    Passageiros *p = criaPassageiro();

    if (valida_id_passageiro(campos[0]) && //id_passageiro
        valida_Data(campos[3])          && //data de nascimento
        valida_genero(campos[5])        && //genero
        valida_email(campos[6])         )  //email
    {

        passenger_set_id(p,campos[0],lista_strings);
        passenger_set_pn(p,campos[1],lista_strings);
        passenger_set_un(p,campos[2],lista_strings);
        passenger_set_dn(p,campos[3]);
        passenger_set_nc(p,campos[4],lista_strings);

        return p;
    } else {
	libertaPassageiro(p);
	return NULL;
    }
}
