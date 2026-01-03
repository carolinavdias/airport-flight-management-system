#include "parsers/parser.h"

#include "entidades/flights.h"
#include "entidades/airports.h"
#include "entidades/aircrafts.h"
#include "entidades/passengers.h"
#include "entidades/reservations.h"

#include "validacoes/validacoes_flights.h"
#include "validacoes/validacoes_airports.h"
#include "validacoes/validacoes_aircrafts.h"
#include "validacoes/validacoes_passengers.h"
#include "validacoes/validacoes_reservations.h"
#include "utils/utils.h"

#include <stdio.h>
#include <glib.h>

/* Função auxiliar para calcular ID da semana (para cache Q4) */
static int dia_semana_parser(int ano, int mes, int dia) {
    static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if (mes < 3) ano -= 1;
    return (ano + ano/4 - ano/100 + ano/400 + t[mes-1] + dia) % 7;
}
static int bissexto_parser(int ano) {
    return (ano % 4 == 0 && ano % 100 != 0) || (ano % 400 == 0);
}
static long dias_desde_epoca_parser(int ano, int mes, int dia) {
    // Fórmula O(1) em vez de loop
    int a = ano - 1;
    long dias_anos = a * 365L + a/4 - a/100 + a/400;
    int dias_mes_acum[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    long dias_meses = dias_mes_acum[mes - 1];
    if (mes > 2 && ((ano % 4 == 0 && ano % 100 != 0) || ano % 400 == 0)) dias_meses++;
    return dias_anos + dias_meses + dia;
}
static long calcula_id_semana_parser(long long datetime) {
    int ano = (int)(datetime / 100000000LL);
    int mes = (int)((datetime / 1000000LL) % 100);
    int dia = (int)((datetime / 10000LL) % 100);
    long dias = dias_desde_epoca_parser(ano, mes, dia);
    int dow = dia_semana_parser(ano, mes, dia);
    return dias - dow;
}


int compara_voos_por_data(const void *a, const void *b) {
    Voo *v1 = *(Voo **)a;
    Voo *v2 = *(Voo **)b;

    long long t1 = voo_get_actual_departure(v1);
    long long t2 = voo_get_actual_departure(v2);

    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

/*
//Função auxiliar adiciona um voo ao array a ser usado na Q5
int adiciona_voo_para_Q5 (Estrt_aux_q5 *lista, char *hora_efetiva, char *hora_prevista, char *airline) {
    int i = 0;
    ContagemQ5 *arr = get_array(lista);
    int num_voos = get_numv(lista);

    for (; i < n; i++) {
        if (strcmp((*arr)[i].airline,airline) == 0) {
            float d = (*arr)[i].delay_avg;
            int current_count = (*arr)[i].delay_count;
            int new_delay = get_time_datah(hora_efetiva) - get_time_datah(hora_prevista); // to float
	    float new_avg = (d * current_count + new_delay) / (current_count + 1);
            set_contagem_avg(arr[i],new_avg); //(*arr)[i].delay_avg
	    set_contagem_count(arr[i],
            (*arr)[i].delay_count++;
            return n;
        }
    }
    ContagemQ5 *tmp = realloc(*arr,(n+1) * sizeof(ContagemQ5));
    if (!tmp) {
        return -1;
    }
    *arr = tmp;

    int new_delay = get_time_datah(hora_efetiva) - get_time_datah(hora_prevista);
    (*arr)->airline = strdup(airline);
    (*arr)->delay_avg = new_delay;
    (*arr)->delay_count = 1;

    return n + 1;
}
*/

int* read_csv (Contexto *ctx, GestorFlights *V, GestorAirports *AP, GestorAircrafts *AC, GestorPassengers *P, GestorReservations *R) {
    // Inicializar contagens de passageiros por aeroporto (para Q1 otimizada)
    gestor_airports_init_contagens(AP);


    gchar *caminhoAeroportos = g_build_filename(get_contexto(ctx), "airports.csv", NULL);
    gchar *caminhoAeronaves  = g_build_filename(get_contexto(ctx), "aircrafts.csv", NULL);
    gchar *caminhoVoos       = g_build_filename(get_contexto(ctx), "flights.csv", NULL);
    gchar *caminhoPassageiros= g_build_filename(get_contexto(ctx), "passengers.csv", NULL);
    gchar *caminhoReservas   = g_build_filename(get_contexto(ctx), "reservations.csv", NULL);

    //int le_1 = 0, le_2 = 0, le_3 = 0, le_4 = 0, le_5 = 0 ;
    int *resultados_read = malloc (6 * sizeof(int));
    resultados_read[0] = 1;
    resultados_read[1] = 0;
    resultados_read[2] = 0;
    resultados_read[3] = 0;
    resultados_read[4] = 0;
    resultados_read[5] = 0;

    //carrega dados

    if (g_file_test(caminhoAeronaves, G_FILE_TEST_EXISTS)   &&
	g_file_test(caminhoVoos, G_FILE_TEST_EXISTS)	    &&
	g_file_test(caminhoAeroportos, G_FILE_TEST_EXISTS)  &&
	g_file_test(caminhoPassageiros, G_FILE_TEST_EXISTS) &&
	g_file_test(caminhoReservas, G_FILE_TEST_EXISTS)    )
    {

        const char *csv_file_names[] = {"","aircrafts.csv","flights.csv","airports.csv","passengers.csv","reservations.csv"};
	const char *csv_error_names[] = {"",
					 "resultados/aircrafts_errors.csv",
					 "resultados/flights_errors.csv",
					 "resultados/airports_errors.csv",
					 "resultados/passengers_errors.csv",
					 "resultados/reservations_errors.csv"};

        for (int c = 1; c < 6; c++) {

	    if (c == 2 && !resultados_read[1]) continue;
	    if (c == 5 && (!resultados_read[2] || !resultados_read[4])) continue;

            int MAX_LINHA = 512;
            gchar buffer[MAX_LINHA];


            FILE *ficheiro = abrir_ficheiro(ctx, csv_file_names[c], "r");
            if (ficheiro == NULL) {
                resultados_read[c] = 0;
                continue;
            }

            int no_header = 1;
            char header[MAX_LINHA];
	    int erro_fatal = 0;

            if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
            else {
                buffer[strcspn(buffer,"\n")] = '\0';
                strcpy(header, buffer);
            }

            FILE *ficheiro_erros = NULL;
            int header_escrito = 0;

            //array local
            int capacidade_array = 10000;
            int num_voos_array = 0;
	    Voo **array_voos = NULL;

	    //tabela de contagens para Q2
	    GHashTable *contagens = NULL;

	    if (c == 2) {
	 	array_voos = malloc(capacidade_array * sizeof(Voo *));
                contagens = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

		if (!array_voos || !contagens) {
		    fclose(ficheiro);
		    resultados_read[c] = 0;
		    continue;
	        }

	    }

//            Estrt_aux_q5 *lista_Q5 = init_lista();

           while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {

               buffer[strcspn(buffer, "\n")] = '\0';
               char **campos = NULL;
               size_t n_campos = 0;

	       int linha_valida = 1;

               if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0; //return ?

               switch (c) {
                   case 1:
                   	Aeronave *aeronave_atual = validacoes_campos_aircrafts(campos);
                   	if (aeronave_atual) gestor_aircrafts_insere(AC, aeronave_atual);
                   	else linha_valida = 0;
                   	//csv_file_error_name = strdup("resultados/aircrafts_errors.csv");
                   	break;
                   case 2:

                   	Voo *voo_atual = validacoes_campos_flights(campos, AC);
                   	if (voo_atual) {
                            //insere no gestor (hash table)
                            gestor_flights_inserir(V, voo_atual);

                            //CONTA voos por aircraft (para Q2) - só voos não cancelados
                            if (voo_get_status(voo_atual) != ESTADO_CANCELLED) {
                            	const char *aircraft_id = voo_get_id_aircraft(voo_atual);
                            	if (aircraft_id) {
                                    gpointer ptr = g_hash_table_lookup(contagens, aircraft_id);
                                    int count = ptr ? GPOINTER_TO_INT(ptr) : 0;
                                    g_hash_table_insert(contagens, g_strdup(aircraft_id), GINT_TO_POINTER(count + 1));
                            	}
                            }

                            //ADICIONA ao array local (se válido para query3)
                            if (voo_get_status(voo_atual) != ESTADO_CANCELLED && voo_get_code_origin(voo_atual)) {
                            	if (num_voos_array >= capacidade_array) {
                                    capacidade_array *= 2;
                                    Voo **temp = realloc(array_voos, capacidade_array * sizeof(Voo *));
				    if (!temp) {
					erro_fatal = 1;
					break;
				    } else {
					array_voos = temp;
				    }
                            	}
                            	array_voos[num_voos_array++] = voo_atual;
                            }
                            // Popular cache Q5: atrasos por airline (so voos Delayed)
                            if (voo_get_status(voo_atual) == ESTADO_DELAYED) {
                                char *airline = voo_get_airline(voo_atual);
                                if (airline) {
                                    long long dep = voo_get_departure(voo_atual);
                                    long long act = voo_get_actual_departure(voo_atual);
                                    int dep_hora = (dep % 10000) / 100;
                                    int dep_min = dep % 100;
                                    int act_hora = (act % 10000) / 100;
                                    int act_min = act % 100;
                                    int dep_total = dep_hora * 60 + dep_min;
                                    int act_total = act_hora * 60 + act_min;
                                    int delay = act_total - dep_total;
                                    if (delay < 0) delay += 24 * 60;
                                    gestor_flights_add_atraso_q5(V, airline, delay);
                                    g_free(airline);
                                }
                            }
                   	} else linha_valida = 0;
                   	//csv_file_error_name = strdup("resultados/flights_errors.csv");
                   	break;

                   case 3:
                   	Aeroporto *aeroporto_atual = validacoes_campos_airports(campos);
                   	if (aeroporto_atual) gestor_airports_insere(AP, aeroporto_atual);
                   	else linha_valida = 0;
                   	//csv_file_error_name = strdup("resultados/airports_errors.csv");
                   	break;
                   case 4:
                   	Passageiros *passageiro_atual = validacoes_campos_passengers(campos);
                   	if (passageiro_atual) gestor_passengers_inserir(P, passageiro_atual);
                   	else linha_valida = 0;
                   	//csv_file_error_name = strdup("resultados/passengers_errors.csv");
                   	break;
                   case 5:
                        Reservas *reserva_atual = validacoes_campos_reservations(campos,V,P);
                        if (reserva_atual) {
                            gestor_reservations_insere(R, reserva_atual);
                            // Popular cache Q4: gasto por semana por passageiro
                            char *primeiro_voo_id = r_get_voo_por_indice(reserva_atual, 0);
                            if (primeiro_voo_id) {
                                Voo *primeiro_voo = gestor_flights_procura(V, primeiro_voo_id);
                                if (primeiro_voo) {
                                    long long departure = voo_get_departure(primeiro_voo);
                                    long id_semana = calcula_id_semana_parser(departure);
                                    int doc_int = r_get_id_pessoa_reservou(reserva_atual);
                                    char doc_str[16];
                                    snprintf(doc_str, sizeof(doc_str), "%09d", doc_int);
                                    double preco = r_get_preco(reserva_atual);
                                    gestor_reservations_add_gasto_q4(R, id_semana, doc_str, preco);
                                }
                                g_free(primeiro_voo_id);
                            }
                            int n_voos = r_get_lista_n_voos(reserva_atual);
                            for (int iv = 0; iv < n_voos; iv++) {
                                char *voo_id = r_get_voo_por_indice(reserva_atual, iv);
                                if (voo_id) {
                                    Voo *voo = gestor_flights_procura(V, voo_id);
                                    if (voo && voo_get_status(voo) != ESTADO_CANCELLED) {
                                        const char *origem = voo_get_code_origin(voo);
                                        const char *destino = voo_get_code_destination(voo);
                                        if (origem) gestor_airports_incrementa_partidas(AP, origem);
                                        if (destino) gestor_airports_incrementa_chegadas(AP, destino);
                                    }
                                    g_free(voo_id);
                                }
                            }
                        } else linha_valida = 0;
                        //csv_file_error_name = strdup("resultados/reservations_errors.csv");
                        break;

           	}
		if (erro_fatal) {
	            break;
	   	}

           	if (!linha_valida) {
                    if (!ficheiro_erros) {
                        ficheiro_erros = fopen(csv_error_names[c], "w");
		 	if (!ficheiro_erros) {
			    erro_fatal = 1;
			    break;
			}
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
           	}

                if (campos) csv_free_fields(campos, n_campos);
           }

    	   if (erro_fatal) {
    		if (c == 2) {
        	    free(array_voos);
        	    g_hash_table_destroy(contagens);
    		}
    		resultados_read[c] = 0;
		continue;
	   }
	   else {
		if (c == 2) {
                    //ORDENA array 1 ÚNICA VEZ (no parser!)
                    qsort(array_voos, num_voos_array, sizeof(Voo *), compara_voos_por_data);

                    //passa array ordenado para o gestor
                    gestor_flights_set_array_ordenado(V, array_voos, num_voos_array);

                    //passa tabela de contagens para o gestor
                    gestor_flights_set_contagens_aircraft(V, contagens);
		}
	   }

           if (ficheiro_erros) fclose(ficheiro_erros);
           if (ficheiro) fclose(ficheiro);

     	   resultados_read[c] = 1;

      }

   }


    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);
    g_free(caminhoVoos);
    g_free(caminhoPassageiros);
    g_free(caminhoReservas);


    return resultados_read;
}


/*

           if (c == 2 )
             if (!erro_fatal) {
                //ORDENA array 1 ÚNICA VEZ (no parser!)
                qsort(array_voos, num_voos_array, sizeof(Voo *), compara_voos_por_data);

                //passa array ordenado para o gestor
                gestor_flights_set_array_ordenado(V, array_voos, num_voos_array);

                //passa tabela de contagens para o gestor
                gestor_flights_set_contagens_aircraft(V, contagens);
             } else {
                free(array_voos);
                g_hash_table_destroy(contagens);
             }
           }





    if (g_file_test(caminhoAeronaves, G_FILE_TEST_EXISTS)) {
        le_3 = le_tabela_Aeronaves(ctx,AC);
    }

    if (g_file_test(caminhoVoos, G_FILE_TEST_EXISTS) && le_3) {
        le_1 = le_tabela_Voos (ctx, V, AC);
    }

    if (g_file_test(caminhoAeroportos, G_FILE_TEST_EXISTS)) {
        le_2 = le_tabela_Aeroportos(ctx,AP);
    }
    if (g_file_test(caminhoPassageiros, G_FILE_TEST_EXISTS)) {
        le_4 = le_tabela_Passageiros(ctx,P);
    }
    if (g_file_test(caminhoReservas, G_FILE_TEST_EXISTS) && le_1 && le_4) {
        le_5 = le_tabela_Reservas(ctx,V,P,R);
    }



int *le_tabela_Voos(Contexto *ctx, GestorFlights *V, GestorAircrafts *AC) {

    int *resultados = [1,0,0,0,0,0];
    char csv_file_names[[]] = ["aircrafts.csv","flights.csv","airports.csv","passengers.csv","reservations.csv"];
    //char *csv_file_error_name = malloc(40);

    for (int c = 1; resultados[c-1] = 1 && c < 6; c++) {

        int MAX_LINHA = 512;
        gchar buffer[MAX_LINHA];

        //char *csv_file_name = malloc(20);
        char *csv_file_error_name = malloc(40);

        FILE *ficheiro = abrir_ficheiro(ctx, csv_file_names[c], "r");
        if (ficheiro == NULL) {
            resultados[c-1] = 0;
            continue;
        }

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

        //tabela de contagens para Q2
        GHashTable *contagens = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

        Estrt_aux *lista_Q5 = cria_estrutura_auxiliarpQ5();

        while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {

           buffer[strcspn(buffer, "\n")] = '\0';
           char **campos = NULL;
           size_t n_campos = 0;

           if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0; //return ?

           int linha_valida = 1;

           //Voo *voo_atual = criaVoo();

           switch (c) {
                case 1:
                   Aeronave *aeronave_atual = validacoes_campos_aircrafts(campos);
                   if (aeronave_atual) gestor_aircrafts_insere(AC, aeronave_atual);
                   else linha_valida = 0;
                   csv_file_error_name = strdup("resultados/aircrafts_errors.csv");
                   break;
                case 2:

                   Voo *voo_atual = validacoes_campos_flights(campos);
                   if (voo_atual) {
                        //insere no gestor (hash table)
                        gestor_flights_inserir(V, voo_atual);

                        //CONTA voos por aircraft (para Q2) - só voos não cancelados
                        if (voo_get_status(voo_atual) != ESTADO_CANCELLED) {
                            const char *aircraft_id = voo_get_id_aircraft(voo_atual);
                            if (aircraft_id) {
                                gpointer ptr = g_hash_table_lookup(contagens, aircraft_id);
                                int count = ptr ? GPOINTER_TO_INT(ptr) : 0;
                                g_hash_table_insert(contagens, g_strdup(aircraft_id), GINT_TO_POINTER(count + 1));
                            }
                        }

                        //ADICIONA ao array local (se válido para query3)
                        if (voo_get_status(voo_atual) != ESTADO_CANCELLED && voo_get_code_origin(voo_atual)) {
                            if (num_voos_array >= capacidade_array) {
                                capacidade_array *= 2;
                                array_voos = realloc(array_voos, capacidade_array * sizeof(Voo *));
                            }
                            array_voos[num_voos_array++] = voo_atual;
                        }

                        //ADICIONA ao array (válido para Q5)
                        if (voo_get_status(voo_atual) == ESTADO_DELAYED) {
                            int new_count = adiciona_voo_para_Q5(lista_airlines_atrasos,num_voos_Q5,campos[2],campos[1]);
                            num_voos_Q5 = new_count;
                        }

                   } else linha_valida = 0;
                   csv_file_error_name = strdup("resultados/flights_errors.csv");
                   break;


                case 3:
                   Aeroporto *aeroporto_atual = validacoes_campos_airports(campos);
                   if (aeroporto_atual) gestor_airports_insere(AP, aeroporto_atual);
                   else linha_valida = 0;
                   csv_file_error_name = strdup("resultados/airports_errors.csv");
                   break;
                case 4:
                   Passageiro *passageiro_atual = validacoes_campos_passengers(campos);
                   if (passageiro_atual) gestor_passengers_insere(P, passageiro_atual);
                   else linha_valida = 0;
                   csv_file_error_name = strdup("resultados/passengers_errors.csv");
                   break;
                case 5:
                   Reserva *reserva_atual = validacoes_campos_reservations(campos);
                   if (reserva_atual) gestor_reservations_insere(R, reserva_atual);
                            // Popular cache Q4: gasto por semana por passageiro
                            char *primeiro_voo_id = r_get_voo_por_indice(reserva_atual, 0);
                            if (primeiro_voo_id) {
                                Voo *primeiro_voo = gestor_flights_procura(V, primeiro_voo_id);
                                if (primeiro_voo) {
                                    long long departure = voo_get_departure(primeiro_voo);
                                    long id_semana = calcula_id_semana_parser(departure);
                                    int doc_int = r_get_id_pessoa_reservou(reserva_atual);
                                    char doc_str[16];
                                    snprintf(doc_str, sizeof(doc_str), "%09d", doc_int);
                                    double preco = r_get_preco(reserva_atual);
                                    gestor_reservations_add_gasto_q4(R, id_semana, doc_str, preco);
                                }
                                g_free(primeiro_voo_id);
                            }
                   else linha_valida = 0;
                   csv_file_error_name = strdup("resultados/reservations_errors.csv");
                   break;

           }

           if (!linha_valida) {
                if (!ficheiro_erros) {
                    ficheiro_erros = fopen(csv_file_error_name, "w");
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
           }

           if (campos) csv_free_fields(campos, n_campos);
        }

        if (ficheiro_erros) fclose(ficheiro_erros);
        fclose(ficheiro);

        if (c == 2) {
            //ORDENA array 1 ÚNICA VEZ (no parser!)
            qsort(array_voos, num_voos_array, sizeof(Voo *), compara_voos_por_data);

            //passa array ordenado para o gestor
            gestor_flights_set_array_ordenado(V, array_voos, num_voos_array);

            //passa tabela de contagens para o gestor
            gestor_flights_set_contagens_aircraft(V, contagens);
        }
    resultados[c] = 1;
    }
    return resultados;
}

*/
