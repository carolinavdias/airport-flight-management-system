#include "validacoes/validacoes_reservations.h"
#include "validacoes/validacoes_flights.h"
#include "validacoes/validacoes_passengers.h"

#include <string.h>
#include <stdio.h>
#include <glib.h>

//RESERVAS -> VALIDAÇÃO SINTÁTICA

//valida o id da reserva e faz o strdup
int valida_id_reserva (const char *s) {
    if (!s || strlen(s) != 10 || s[0] != 'R') return 0;
    for (int i = 1; i < 10; i++) {
        if (s[i] < '0' || s[i] > '9') return 0;
    }
    return 1;
}

//valida a lista dos voos reservados, passa para o formato de uma lista e atribui a "Reserva"
int valida_set_voos_reservados(const char *s, Reservas *r) {
    if (!s || strlen(s) < 3) return 0; //[] invalido
    int len = strlen(s);
    if (s[0] != '[' || s[len-1] != ']') return 0; //verifica se tem os parenteses retos no inicio e no final

    //limpa a string
    char *string_voos = g_strdup(s + 1); // pula '['
    string_voos[strlen(string_voos)-1] = '\0';      // remove ']'

    //conta voos
    int n = 1;
    for (int i = 0; string_voos[i]; i++) {
        if (string_voos[i] == ',') n++;
    }

    Voos_reservados *novo = cria0_lista_reserva(n);

    char *ptr = string_voos;
    for (int i = 0; i < n; i++) {
        char *token = strsep(&ptr, ",");
        while (*token == ' ' || *token == '\'') token++;
        char *end = token + strlen(token) - 1;
        while (end >= token && (*end == ' ' || *end == '\'')) *end-- = '\0';

        if (!valida_id_voo(token)) { //flight id invalido
	   g_free(string_voos);
	   //free(novo)
           return 0; //liberta string_voos e novo?
	}
	set_lista_voos(novo,i,token);
    }

    r_set_lista(r, novo);
    //free(novo);  //libertar a estrutura Voos_reservados após copiar!
    g_free(string_voos);
    return 1;
}

//valida BAGAGEM/PRIORIDADE das Reservas e passa a string para um bool nos respetivos campos em Reserva
int valida_bool (const char *s) {
    return s && (strcmp (s, "true") == 0 || strcmp (s, "false") == 0);
}

//RESERVAS -> VALIDAÇÃO LÓGICA

int valida_RESERVA(Reservas *reserva, GestorFlights *gestor_voos, GestorPassengers *gestor_passageiros) {
    int length_vr = r_get_lista_n_voos(reserva);
    if (length_vr < 1 || length_vr > 2) return 0;

    //verifica se voos existem (USA O GESTOR!)
    for (int i = 0; i < length_vr; i++) {
        char *voo_id = (r_get_lista_voos_reserv(reserva))[i];
        if (!gestor_flights_existe(gestor_voos, voo_id))
            return 0;
    }

    //verifica se passageiro existe (USA O GESTOR!)
    //FIX: Converter int para string
    int id_pessoa = r_get_id_pessoa_reservou(reserva);
    char id_str[32];
    snprintf(id_str, sizeof(id_str), "%09d", id_pessoa);
    
    if (!gestor_passengers_existe(gestor_passageiros, id_str))
        return 0;

    //valida escala (se 2 voos)
    if (length_vr == 2) {
        const char *dest1 = gestor_flights_obter_destino(gestor_voos, (r_get_lista_voos_reserv(reserva))[0]);
        const char *orig2 = gestor_flights_obter_origem(gestor_voos, (r_get_lista_voos_reserv(reserva))[1]);

        if (!dest1 || !orig2 || strcmp(dest1, orig2) != 0) return 0;
    }

    return 1;
}


Reservas *validacoes_campos_reservations(char **campos, GestorFlights *V, GestorPassengers *P) {
    Reservas *r = criaReserva();

    if (valida_id_reserva(campos[0]) 	        && //id_reserva
	valida_set_voos_reservados(campos[1],r) && //lista voos reservados
	valida_id_passageiro(campos[2]) 	&& //ide pessoa q reservou
	valida_bool(campos[5])  		&& //bagagem extra
	valida_bool(campos[6])			)  //prioridade
    {

	r_set_id_reserva(r,campos[0]);
	r_set_id_pessoa_reservou(r,campos[2]);
	r_set_lugar(r,campos[3]);
	r_set_preco(r,campos[4]);
	r_set_bools(r,campos[5],1);
	r_set_bools(r,campos[6],2);
	r_set_qr_code(r,campos[7]);

	if (!valida_RESERVA(r, V, P)) return NULL;

        return r;
    }
    else return NULL;
}
