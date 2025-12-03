#include "validacoes_reservations.h"
#include "reservations.h"
#include "flights.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

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

bool v_is_reservation_id(const char *s){
    if(!s || strlen(s)!=10) return false;
    if(s[0] != 'R') return false;
    return is_digits(s+1);
}