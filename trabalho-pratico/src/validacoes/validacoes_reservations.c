#include "validacoes_reservations.h"
#include "reservations.h"
#include "flights.h"
#include "gestor_flights.h"
#include "gestor_passengers.h"
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <glib.h>

//RESERVAS -> VALIDAÇÃO SINTÁTICA


//Valida o id da reserva e faz o strdup
int valida_id_reserva (const char *s, char **id_reserva) {
    if (!s || strlen(s) != 10 || s[0] != 'R') return 0;
    for (int i = 1; i < 10; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }

    //Validaçao concluida

    *id_reserva = g_strdup(s);
    return 1;
}


//Valida a lista dos voos reservados, passa para o formato de uma lista e atribui a "Reserva"
int valida_voos_reservados(const char *s, Voos_reservados *lista) {
    if (!s || strlen(s) < 3) return 0; //[] invalido
    int len = strlen(s);
    if (s[0] != '[' || s[len-1] != ']') return 0; //verifica se tem os parenteses retos no inicio e no final

    //Limpar a string
    char *string_voos = g_strdup(s + 1); // pula '['
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
int valida_bool (const char *s, bool *b) {
    if (!s || strlen(s) == 0) return 0; // string nula

    if (strcmp (s, "true") == 0) *b = 1;
    else if (strcmp (s, "false") == 0) *b = 0;
    else return 0;

    return 1;
}


//RESERVAS -> VALIDAÇÃO LÓGICA

/*
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
*/



//RESERVAS -> VALIDAÇÃO LÓGICA
int valida_RESERVA(Reservas reserva, GestorFlights *gestor_voos, GestorPassengers *gestor_passageiros) {
    int length_vr = reserva.reserva_lista.n_voos;
    if (length_vr < 1 || length_vr > 2) return 0;

    // Verifica se voos existem (USA O GESTOR!)
    for (int i = 0; i < length_vr; i++) {
        char *voo_id = reserva.reserva_lista.lista_voos_reservados[i];
        if (!gestor_flights_existe(gestor_voos, voo_id))
            return 0;
    }

    // Verifica se passageiro existe (USA O GESTOR!)
    if (!gestor_passengers_existe(gestor_passageiros, reserva.id_pessoa_reservou))
        return 0;

    // Validar escala (se 2 voos)
    if (length_vr == 2) {
        const char *dest1 = gestor_flights_obter_destino(gestor_voos, reserva.reserva_lista.lista_voos_reservados[0]);
        const char *orig2 = gestor_flights_obter_origem(gestor_voos, reserva.reserva_lista.lista_voos_reservados[1]);

        if (!dest1 || !orig2 || strcmp(dest1, orig2) != 0) return 0;
    }

    return 1;
}
