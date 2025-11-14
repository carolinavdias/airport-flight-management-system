#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include <stdint.h>
#include "read.h"
#include "validators.h"
#include "csv.h"
#include "q1.h"
#include "q2.h"
#include "q3.h"



#define MAX_LINHA 20000
gchar buffer[MAX_LINHA];

// ---------------------------------------------------------
// Funções auxiliares e de validação
// ---------------------------------------------------------
// Este ficheiro serve só para as funções de leitura e validação,
// o main e a lógica principal estão no programaprincipal.c
// ---------------------------------------------------------





/*
// devolve o tipo de mês (1 → 31 dias, 2 → 30 dias, 3 → fevereiro)
int qual_mes(int mes) {
    if (mes == 2) return 3; // fevereiro
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 2; // 30 dias
    return 1; // 31 dias
}
*/

/*
// verifica se a string é vazia ou só tem espaços (para evitar erros parvos)
int string_vazia_ou_espacos(const char *s) {
    if (s == NULL) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

// converte uma string para estrutura DataH (data + hora)
// exemplo: "2023-05-17 14:30" → {2023, 5, 17, 14, 30}
DataH parse_DataH(char *string) {
    DataH novo;
    int narg = sscanf(string, " %d-%d-%d %d:%d",
                      &novo.data.ano, &novo.data.mes, &novo.data.dia,
                      &novo.horas.hora, &novo.horas.mins);
    if (narg != 5) {
        // se a leitura falhar, mete tudo a 0
        novo.data.ano = novo.data.mes = novo.data.dia = 0;
        novo.horas.hora = novo.horas.mins = 0;
    }
    return novo;
}

// converte uma string numa estrutura Data
// (simples mas ajuda a validar se os valores fazem sentido)
Data parse_Data(char *string) {
    Data novo;
    int narg = sscanf(string, "%d-%d-%d", &novo.ano, &novo.mes, &novo.dia);
    if (narg != 3 || (novo.ano < 0 || novo.ano > 2025) ||
        (novo.mes < 1 || novo.mes > 12) || (novo.dia < 1 || novo.dia > 31)) {
        fprintf(stderr, "Formatação errada da Data: %s\n", string ? string : "(null)");
        novo.ano = novo.mes = novo.dia = 0;
    }
    return novo;
}

// converte string num email válido (ou mete um placeholder todo feio se estiver errado)
char* string_to_email(char *string) {
    if (string == NULL || strlen(string) == 0) return g_strdup("------@----.---");
    if (!strchr(string, '@')) return g_strdup("------@----.---");
    return g_strdup(string);
}

// valida código IATA (3 letras maiúsculas tipo "LIS" ou "OPO")
// se não tiver o formato certo devolve "---"
char* string_to_codigoIATA(char *string) {
    if (string == NULL || strlen(string) != 3) return g_strdup("---");
    for (int i = 0; i < 3; i++) {
        if (string[i] < 'A' || string[i] > 'Z') return g_strdup("---");
    }
    return g_strdup(string);
}

// traduz string para estado de voo (On_Time, Delayed, Cancelled)
// se for inválido devolve ESTADO_ERROR
Estado string_to_Estado(char *string) {
    if (!string) return ESTADO_ERROR;
    if (strcmp(string, "On_Time") == 0) return ESTADO_ON_TIME;
    if (strcmp(string, "Delayed") == 0) return ESTADO_DELAYED;
    if (strcmp(string, "Cancelled") == 0) return ESTADO_CANCELLED;
    return ESTADO_ERROR;
}

// traduz string para tipo de aeroporto (small_airport, heliport, etc)
Tipo_aeroporto valida_tipo(char *string) {
    if (!string) return TIPO_ERROR;
    if (strcmp(string, "small_airport") == 0) return TIPO_SMALL_AIRPORT;
    if (strcmp(string, "medium_airport") == 0) return TIPO_MEDIUM_AIRPORT;
    if (strcmp(string, "large_airport") == 0) return TIPO_LARGE_AIRPORT;
    if (strcmp(string, "heliport") == 0) return TIPO_HELIPORT;
    if (strcmp(string, "seaplane_base") == 0) return TIPO_SEAPLANE_BASE;
    return TIPO_ERROR;
}

// converte uma string em booleano (true/false)
// se for inválido, imprime aviso mas não crasha
bool string_to_bool(char *string, int versao) {
    (void)versao; // evitar warning, mas manter o parâmetro
    if (!string) return false;
    if (strcmp(string, "true") == 0) return true;
    if (strcmp(string, "false") == 0) return false;
    fprintf(stderr, "Aviso: valor inválido para booleano (%s)\n", string);
    return false;
}

// converte string em género (M, F ou O)
// se vier outra coisa qualquer → erro
Genero string_to_genero(char *string) {
    if (!string || strlen(string) != 1) return GENERO_ERROR;
    if (strcmp(string, "M") == 0) return GENERO_M;
    if (strcmp(string, "F") == 0) return GENERO_F;
    if (strcmp(string, "O") == 0) return GENERO_O;
    return GENERO_ERROR;
}

// valida ID de voo (formato tipo "TP1923" → 2 letras + 5 números)
// se estiver errado devolve "00-----" para não dar segmentation
char* string_to_id_voo(char *string) {
    if (!string || strlen(string) != 7) return g_strdup("00-----");
    for (int i = 0; i < 2; i++)
        if (string[i] < 'A' || string[i] > 'Z') return g_strdup("00-----");
    for (int i = 2; i < 7; i++)
        if (string[i] < '0' || string[i] > '9') return g_strdup("00-----");
    return g_strdup(string);
}

// ---------------------------------------------------------
// Funções auxiliares para debugging (imprimir voos, etc.)
// ---------------------------------------------------------
// dá jeito para testar se a hash table está a funcionar
void imprimir_voo(gpointer key, gpointer value, gpointer user_data) {
    (void)user_data; // para o compilador não chatear
    char *voo_id = (char *) key;
    Voo *v = (Voo *) value;

    printf("Voo ID: %s\n", voo_id);
    printf("Origem: %s | Destino: %s\n", v->origin, v->destination);
    printf("Companhia: %s | Estado: %s\n", v->airline, v->status);
    printf("--------------------------\n");
}

// ---------------------------------------------------------
// Nota final:
// Este ficheiro serve só para guardar as funções de leitura
// e validação. O main está em programaprincipal.c
// ---------------------------------------------------------
*/




/*

int read () { //Para opção inserir
// read (table, nome_ficheiro)

    GHashTable *tabela1 = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *tabela2 = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *tabela3 = g_hash_table_new(g_str_hash, g_str_equal);
    GHashTable *tabela4 = g_hash_table_new(g_direct_hash, g_direct_equal);
    GHashTable *tabela5 = g_hash_table_new(g_str_hash, g_str_equal);
  int continua = 1; //flag para read no while, começa em 1
  while (continua) {


    int opcao_inserida;
    printf ("Insira o número correspondente ao que quer inserir.\n\n1 -> Voo\n2 -> Aeroporto\n3 -> Aeronave\n4 -> Passageiro\n5 -> Reserva\n\n");
    if (scanf ("%d", &opcao_inserida) != 1) {
        printf("Erro a ler input.\n");
        return 0;
    }

    putchar ('\n');
*/


void libertaReserva(void *data) {
    Reservas *a = data;
    if (!a) return;

    g_free(a->id_reserva);

    if (a->reserva_lista.lista_voos_reservados) {
        for (int i = 0; i < a->reserva_lista.n_voos; i++) {
            g_free(a->reserva_lista.lista_voos_reservados[i]);
        }
        free(a->reserva_lista.lista_voos_reservados);  // libertar o array
    }

    g_free(a->lugar_reservado);
    g_free(a->qr_code);
    g_free(a);
}



void libertaPassageiro(void *data) {
    Passageiros *a = data;
    if (!a) return;
    g_free(a->primeiro_nome);
    g_free(a->ultimo_nome);
    g_free(a->nacionalidade);
    g_free(a->email_passageiro);
    g_free(a->telefone_passageiro);
    g_free(a->morada_passageiro);
    g_free(a->fotografia_passageiro);
    g_free(a);
}

FILE *abrir_ficheiro (Contexto *ctx, const char *nome_ficheiro, const char *modo) {
    char path[1024];
    snprintf (path, sizeof(path), "%s/%s", ctx->dataset_dir, nome_ficheiro);
    FILE *ficheiro = fopen (path,modo);
    if (ficheiro == NULL) {
    	perror ("Erro ao abrir o ficheiro.\n");
    	return NULL;
    }
    return ficheiro;
}

int le_tabela (int opcao_inserida, Contexto ctx, GHashTable *tabela1, GHashTable *tabela2, GHashTable *tabela3, GHashTable *tabela4, GHashTable *tabela5) {

        if (opcao_inserida == 1) {

	printf("Entrou no 1.\n");
/*
                FILE *ficheiro = fopen("dataset/flights.csv", "r");
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 0;
                }
*/

		FILE *ficheiro = abrir_ficheiro (&ctx,"flights.csv", "r");
		if (ficheiro == NULL) return 0;


                int linhas_totais = 0;
                int linhas_com_sucesso = 0;
                int no_header = 1;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                if (fgets(buffer,sizeof(buffer),ficheiro) == NULL) no_header = 0;
                //if (fgets(buffer,sizeof(buffer),ficheiro) == NULL) break;

                while (fgets(buffer,sizeof(buffer),ficheiro) && no_header) {
              //while (fgets(buffer, sizeof(buffer),ficheiro)) {
                        Voo *voo_atual = malloc(sizeof(Voo));
	/*		voo_atual->flight_id = NULL;
			voo_atual->departure = NULL;
			voo_atual->actual_departure = NULL;
			voo_atual->arrival = NULL;
			voo_atual->actual_arrival = NULL;
			voo_atual->gate = NULL;
			voo_atual->status = 0;
			voo_atual->code_origin = NULL;
			voo_atual->code_destination = NULL;
			voo_atual->id_aircraft = NULL;
			voo_atual->airline = NULL;
			voo_atual->tracking_url = NULL;
*/
                        linhas_totais++;
                        int linha_valida = 1;
                        int e_maybe = -1; // verificação datas baseado no estado do voo
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha

                        //validação dos campos

                        if (!valida_id_voo(campos[0],&voo_atual->flight_id)) linha_valida = 0;
			//printf("%s-%d\n", campos[0], linha_valida);

                        if (linha_valida) {
                                if (!valida_DataH(campos[1],&voo_atual->departure)) linha_valida = 0;
			//	printf("%s-%d\n", campos[1], linha_valida);
                        }
                        if (linha_valida) {
                                if (strcmp(campos[2],"N/A") == 0) {
                                        e_maybe = 2;
                                        voo_atual->actual_departure = g_strdup(campos[2]);
                                } else {
                                        if (!valida_DataH(campos[2],&voo_atual->actual_departure)) linha_valida = 0;
                                }
                        }
                        if (linha_valida) {
                                if (!valida_DataH(campos[3],&voo_atual->arrival)) linha_valida = 0;
                        }
                        if (linha_valida) {
                                if (e_maybe == 2) {
                                        if (strcmp(campos[4],"N/A") != 0) linha_valida = 0;
                                        else voo_atual->actual_arrival = g_strdup(campos[4]);
                                } else {
                                        if (!valida_DataH(campos[4],&voo_atual->actual_arrival)) linha_valida = 0;
                                }
                        }
                        if (linha_valida) voo_atual->gate = g_strdup(campos[5]);
                        if (linha_valida) {
                                if (!valida_Estado(campos[6],&voo_atual->status)) linha_valida = 0;
                                if (voo_atual->status == 2 && e_maybe != 2) linha_valida = 0; //data n/a, para outro estado que nao cancelled
                        }
                        if (linha_valida) {
                                if (!valida_codigoIATA(campos[7],&voo_atual->code_origin)) linha_valida = 0;
                        }
                        if (linha_valida) {
                                if (!valida_codigoIATA(campos[8],&voo_atual->code_destination)) linha_valida = 0;
                        }
                        if (linha_valida) voo_atual->id_aircraft = g_strdup(campos[9]);
                        if (linha_valida) voo_atual->airline = g_strdup(campos[10]);
                        if (linha_valida) voo_atual->tracking_url = g_strdup(campos[11]);

                        //if (linha_valida) printf("Linha %d:\n%s\nD\nD\nD\nD\nOT\n\n", linhas_totais,voo_atual->voo_id);


                        //Validação lógica
                        if (linha_valida) {
				if (!valida_VOO (*voo_atual,tabela3)) linha_valida = 0;
			}


                        // if !valida escreve no ficheiro_erros
                        if (!linha_valida && no_header) {
                                FILE *ficheiro_erros = fopen("resultados/flights_errors.csv", "a");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_voos_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        //if valida adiciona à tabela
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela1, g_strdup(voo_atual->flight_id), voo_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela dos voos %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos à linha
                        g_strfreev(campos); //libertar depois e usar
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);

        }
        else if (opcao_inserida == 2) {
/*
                FILE *ficheiro = fopen("airports.csv", "r");
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 0;
                }
*/
                FILE *ficheiro = abrir_ficheiro (&ctx,"airports.csv", "r");
                if (ficheiro == NULL) return 0;

                int linhas_totais = 0;
                int linhas_com_sucesso = 0;
		int no_header = 1;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                if (fgets(buffer,sizeof(buffer),ficheiro) == NULL) no_header = 0;

                while (fgets(buffer, sizeof(buffer),ficheiro) && no_header) {
                        Aeroporto *aeroporto_atual = malloc(sizeof(Aeroporto));
			aeroporto_atual->code_IATA = NULL;
			aeroporto_atual->name = NULL;
			aeroporto_atual->city = NULL;
			aeroporto_atual->country = NULL;
			aeroporto_atual->latitude = 0.0;
			aeroporto_atual->longitude = 0.0;
			aeroporto_atual->code_ICAO = NULL;
			aeroporto_atual->type = TIPO_ERROR;

                        linhas_totais++;
                        int linha_valida = 1;
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha

                        //validação dos campos

                        if (!valida_codigoIATA(campos[0],&aeroporto_atual->code_IATA)) linha_valida = 0;
                        if (linha_valida) aeroporto_atual->name = g_strdup(campos[1]);
                        if (linha_valida) aeroporto_atual->city = g_strdup(campos[2]);
                        if (linha_valida) aeroporto_atual->country = g_strdup(campos[3]);
                        if (linha_valida) aeroporto_atual->latitude = atof(campos[4]);
                        if (linha_valida) aeroporto_atual->longitude = atof(campos[5]);
                        if (linha_valida) aeroporto_atual->code_ICAO = g_strdup(campos[6]);
                        if (linha_valida) {
                                if (!valida_tipo_aer(campos[7],&aeroporto_atual->type)) linha_valida = 0;
                        }

                        //if (linha_valida) printf("Linha %d:\n%s\n%s\n%s\n%s\n%f\n%f\n%s\nM\n\n", linhas_totais,aeroporto_atual->codigo_IATA_aer,aeroporto_atual->nome_aer,aeroporto_atual->cidade_aer ,aeroporto_atual->pais_aer,aeroporto_atual->latitude,aeroporto_atual->longitude,aeroporto_atual->codigo_ICAO_aer);


                        //if !valida escreve no ficheiro_erros
                        if (!linha_valida && no_header) {
                                FILE *ficheiro_erros = fopen("resultados/airports_errors.csv", "a");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_aeroportos_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        //if valida adiciona à tabela
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela2, g_strdup(aeroporto_atual->code_IATA), aeroporto_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela dos aeroportos %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depoisd e usar
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);

        } else if (opcao_inserida == 3) {

/*
                FILE *ficheiro = fopen("dataset/aircrafts.csv", "r"); // a alterar
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 0;
                }
*/
                FILE *ficheiro = abrir_ficheiro (&ctx,"aircrafts.csv", "r");
                if (ficheiro == NULL) return 0;

                int linhas_totais = 0;
                int linhas_com_sucesso = 0;
		int no_header = 1;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                if (fgets(buffer,sizeof(buffer),ficheiro) == NULL) no_header = 0;

                while (fgets(buffer, sizeof(buffer),ficheiro) && no_header) {
                        Aeronave *aeronave_atual = malloc(sizeof(Aeronave));
			aeronave_atual->identifier = NULL;
			aeronave_atual->manufacturer = NULL;
			aeronave_atual->model = NULL;
			aeronave_atual->year = -1;
			aeronave_atual->capacity = -1;
			aeronave_atual->range = -1;

                        linhas_totais++;
                        int linha_valida = 1;
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha

                        //validação e atribuição dos campos
                        aeronave_atual->identifier = g_strdup(campos[0]);
                        aeronave_atual->manufacturer = g_strdup(campos[1]);
                        aeronave_atual->model = g_strdup(campos[2]);
			if (!valida_year(campos[3],&aeronave_atual->year)) linha_valida = 0;
                        //if (atoi(campos[3]) >= 0 && atoi(campos[3]) <= 2025) aeronave_atual->year = atoi(campos[3]);
                        //else linha_valida = 0;
			//printf("%s-%ld\n", campos[3], strlen(campos[3])); //'
                        if (linha_valida) aeronave_atual->capacity = atoi(campos[4]);
                        if (linha_valida) aeronave_atual->range = atoi(campos[5]);


                        //if !valida escreve no ficheiro_erros
                        if (!linha_valida && no_header) {
                                FILE *ficheiro_erros = fopen("resultados/aircrafts_errors.csv", "a");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_aeronave_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        //if valida adiciona à tabela
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela3, g_strdup(aeronave_atual->identifier), aeronave_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela das aeronaves %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depoisd e usar 
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista
                fclose(ficheiro);

        } else if (opcao_inserida == 4) {
/*
                FILE *ficheiro = fopen("passengers.csv", "r");
                        if (ficheiro == NULL) {
                                perror ("Erro ao abrir o ficheiro.\n");
                                return 0;
                }
*/
                FILE *ficheiro = abrir_ficheiro (&ctx,"passengers.csv", "r");
                if (ficheiro == NULL) return 0;

                int linhas_totais = 0;
                int linhas_com_sucesso = 0;
		int no_header = 1;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                if (fgets(buffer,sizeof(buffer),ficheiro) == NULL) no_header = 0;

                while (fgets(buffer, sizeof(buffer),ficheiro) && no_header) {
                        Passageiros *passageiro_atual = malloc(sizeof(Passageiros));
			passageiro_atual->id_passageiro = -1;
			passageiro_atual->primeiro_nome = NULL;
			passageiro_atual->ultimo_nome = NULL;
			//Data data_nascimento;
    			passageiro_atual->nacionalidade = NULL;
    			//Genero genero_passageiro;
    			passageiro_atual->email_passageiro = NULL;
    			passageiro_atual->telefone_passageiro = NULL;
			passageiro_atual->morada_passageiro = NULL;
			passageiro_atual->fotografia_passageiro = NULL;


                        linhas_totais++;
                        int linha_valida = 1;
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha

                        //validação e atribuição dos campos
                        if (!valida_id_passageiro(campos[0],&passageiro_atual->id_passageiro)) linha_valida = 0;
                        if (linha_valida) passageiro_atual->primeiro_nome = g_strdup(campos[1]);
                        if (linha_valida) passageiro_atual->ultimo_nome = g_strdup(campos[2]);
                        if (linha_valida) {
                                if (!valida_Data(campos[3],&passageiro_atual->data_nascimento)) linha_valida = 0;
                        }
                        if (linha_valida) passageiro_atual->nacionalidade = g_strdup(campos[4]);
                        if (linha_valida) {
                                if (!valida_genero(campos[5],&passageiro_atual->genero_passageiro)) linha_valida = 0;
                        }
                        if (linha_valida) {
                                if (!valida_email(campos[6],&passageiro_atual->email_passageiro)) linha_valida = 0;
                        }
                        if (linha_valida) passageiro_atual->telefone_passageiro = g_strdup(campos[7]);
                        if (linha_valida) passageiro_atual->morada_passageiro = g_strdup(campos[8]);
                        if (linha_valida) passageiro_atual->fotografia_passageiro = g_strdup(campos[9]);


                        //if !valida escreve no ficheiro_erros
                        if (!linha_valida && no_header) {
                                FILE *ficheiro_erros = fopen("resultados/passengers_errors.csv", "a");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_passageiros_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);
                                free(passageiro_atual);

                        }
                        //if valida adiciona à tabela
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela4, GINT_TO_POINTER (passageiro_atual->id_passageiro), passageiro_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela dos passageiros %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depois e usar
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);

        } else if (opcao_inserida == 5) {
                FILE *ficheiro = abrir_ficheiro(&ctx, "reservations.csv", "r");
                if (ficheiro == NULL) return 0;

/*
                FILE *ficheiro = abrir_ficheiro (&ctx,"reservations.csv", "r");
                if (ficheiro == NULL) return 0;
//                printf("Entrou na opcao 5.\n");
*/
                int linhas_totais = 0;
                int linhas_com_sucesso = 0;
		int no_header = 1;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                if (fgets(buffer,sizeof(buffer),ficheiro) == NULL) no_header = 0;
                //printf("%d.\n", no_header);


                while (fgets(buffer, sizeof(buffer),ficheiro) && no_header) {
  //              printf("Entrou na opcao 5.\n");

                        Reservas *reserva_atual = malloc(sizeof(Reservas));
			reserva_atual->id_reserva = NULL;
			//    Voos_reservados reserva_lista; //lista
    			//int id_pessoa_reservou; //reserva em nome de
    			reserva_atual->lugar_reservado = NULL;
			reserva_atual->preco_reserva = -1.0;
			//bagagem_extra;
    			//bool prioridade; //priority boarding
    			reserva_atual->qr_code = NULL;;


			//reserva_atual->reserva_lista.n_voos = 0;
			//reserva_atual->reserva_lista.lista_voos_reservados = NULL;

                        linhas_totais++;
                        int linha_valida = 1;
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha
//			printf("%s\n", campos[1]);

                        //validação e atribuição dos campos
                        if (!valida_id_reserva(campos[0],&reserva_atual->id_reserva)) linha_valida = 0;

                        if (linha_valida) {
                                if (!valida_voos_reservados(campos[1],&reserva_atual->reserva_lista)) linha_valida = 0; //voos_reservados
				//printf("linhavalida%d\n", linha_valida);
                        }
                        if (linha_valida) reserva_atual->id_pessoa_reservou = atoi(campos[2]);
                        if (linha_valida) reserva_atual->lugar_reservado = g_strdup(campos[3]);
                        if (linha_valida) reserva_atual->preco_reserva = atof(campos[4]);

                        if (linha_valida) {
                                if (!valida_bool(campos[5],&reserva_atual->bagagem_extra)) linha_valida = 0;
                        }
//printf("%d\n", linha_valida);
                        if (linha_valida) {
                                if (!valida_bool(campos[6],&reserva_atual->prioridade)) linha_valida = 0;
                        }
//printf("linhavalida%d\n", linha_valida);
                        if (linha_valida) reserva_atual->qr_code = g_strdup(campos[7]);

                        //Validação Lógica
			if (linha_valida) {
                        	if (!valida_RESERVA (*reserva_atual,tabela1,tabela4)) linha_valida = 0;
			}
//printf("linhavalida%d\n", linha_valida);

                        //if !valida escreve no ficheiro_erros
                        if (!linha_valida && no_header) {
                                FILE *ficheiro_erros = fopen("resultados/reservations_errors.csv", "a");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_reservas_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        //if valida adiciona à tabela
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela5, g_strdup(reserva_atual->id_reserva), reserva_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela das reservas %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depoisd e usar
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);
        }
//    }
        return 1;
}