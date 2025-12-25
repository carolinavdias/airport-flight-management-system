#define _POSIX_C_SOURCE 200809L
#include "parsers/parser_flights.h"

#include "entidades/flights.h"
#include "validacoes/validacoes_flights.h"
#include "validacoes/validacoes_airports.h"
#include "utils/utils.h"
#include <stdlib.h>

static int compara_voos_por_data(const void *a, const void *b) {
    Voo *v1 = *(Voo **)a;
    Voo *v2 = *(Voo **)b;
    
    long long t1 = voo_get_actual_departure(v1);
    long long t2 = voo_get_actual_departure(v2);
    
    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

int le_tabela_Voos(Contexto *ctx, GestorFlights *V, GestorAircrafts *AC) {

    int MAX_LINHA = 2000;
    gchar buffer[MAX_LINHA];

    FILE *ficheiro = abrir_ficheiro(ctx, "flights.csv", "r");
    if (ficheiro == NULL) return 0;

    int no_header = 1;
    char header[MAX_LINHA];

    if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
    else {
        buffer[strcspn(buffer,"\n")] = '\0';
        strcpy(header, buffer);
    }

    FILE *ficheiro_erros = NULL;
    int header_escrito = 0;

    //array local
    int capacidade_array = 10000;
    Voo **array_voos = malloc(capacidade_array * sizeof(Voo *));
    int num_voos_array = 0;

    while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
        Voo *voo_atual = criaVoo();

        int linha_valida = 1;
        int e_maybe = -1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;

        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

	//FLIGHT_ID
	if (linha_valida && valida_id_voo(campos[0])) voo_set_flight_id (voo_atual,campos[0]);
	else linha_valida = 0;

	//DEPARTURE
	if (linha_valida && valida_DataH(campos[1])) voo_set_dataH(voo_atual,campos[1],1);
	else linha_valida = 0;

	//ACTUAL_DEPARTURE
        if (linha_valida) {
	    if (strcmp(campos[2], "N/A") == 0) {
                 e_maybe = 2;
		 voo_set_dataH(voo_atual,"",2);
            } else {
		if (valida_DataH(campos[2])) voo_set_dataH(voo_atual,campos[2],2);
		else linha_valida = 0;
	    }
        }

        //CODE_ORIGIN
        if (linha_valida && valida_codigoIATA(campos[7])) voo_set_code(voo_atual,campos[7],'o');
	else linha_valida = 0;

        //STATUS
        if (linha_valida) {
           if (valida_Estado(campos[6])) {
		voo_set_status(voo_atual,campos[6]);
                if (voo_get_status(voo_atual) == ESTADO_CANCELLED && e_maybe != 2) linha_valida = 0;
	   }
	   else linha_valida = 0;
        }

	//ID_AIRCRAFT
        if (linha_valida) voo_set_id_aircraft(voo_atual,campos[9]);

	//ARRIVAL
        if (linha_valida && valida_DataH(campos[3])) voo_set_dataH(voo_atual,campos[3],3);
	else linha_valida = 0;

	//ACTUAL_ARRIVAL
        if (linha_valida) {
            if (e_maybe == 2) {
                 if (strcmp(campos[4], "N/A") != 0) linha_valida = 0;
                 else voo_set_dataH(voo_atual,"",4);
            } else {
		 if (valida_DataH(campos[4])) voo_set_dataH(voo_atual,campos[4],4);
		 else linha_valida = 0;
	    }
        }
        //if (linha_valida) voo_set_gate(voo_atual,campos[5]);

	//CODE_DESTINATION
        if (linha_valida && valida_codigoIATA(campos[8])) voo_set_code(voo_atual,campos[8],'d');
	else linha_valida = 0;


	//AIRLINE
        if (linha_valida) voo_set_airline(voo_atual,campos[10]);
        
/*
	//TRACKING_URL
        if (linha_valida) voo_set_tracking_url(voo_atual,campos[11]);
*/
        //VALIDAÇÃO LÓGICA
        if (linha_valida && !valida_VOO(voo_atual, AC)) linha_valida = 0;

        if (!linha_valida) {
            if (!ficheiro_erros) {
                ficheiro_erros = fopen("resultados/flights_errors.csv", "w");
                if (ficheiro_erros && !header_escrito) {
                    fputs(header, ficheiro_erros);
                    fputc('\n', ficheiro_erros);
                    header_escrito = 1;
                }
            }
            if (ficheiro_erros) {
                fputs(buffer, ficheiro_erros);
                fputc('\n', ficheiro_erros);
            }
            libertaVoo(voo_atual);
        } else {
            //insere no gestor (hash table)
            gestor_flights_inserir(V, voo_atual);
            
            //ADICIONA ao array local (se válido para query3)
            if (voo_get_status(voo_atual) != ESTADO_CANCELLED && voo_get_code_origin(voo_atual)) {
                //expaner array se necessário
                if (num_voos_array >= capacidade_array) {
                    capacidade_array *= 2;
                    array_voos = realloc(array_voos, capacidade_array * sizeof(Voo *));
                }
                array_voos[num_voos_array++] = voo_atual;
            }
        }
        
        if (campos) csv_free_fields(campos, n_campos);
    }
    
    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    
    //ORDENA array 1 ÚNICA VEZ (no parser!)
    qsort(array_voos, num_voos_array, sizeof(Voo *), compara_voos_por_data);
    
    //passa array ordenado para o gestor
    gestor_flights_set_array_ordenado(V, array_voos, num_voos_array);
    
    return 1;
}