#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>

//Por fazer:
//ficheiro.h
//latitude/longitude, identificador reserva, numero documento, listas csv

typedef struct data_def {
   int ano, mes, dia;
} Data;

typedef struct hora_def {
   int hora, mins;
} Hora;

typedef struct dataH {
    Data data;
    Hora horas;
} DataH;

typedef enum {
    F,
    M,
    O,
    ERROR,
} Genero; //Genero_passageiro

typedef enum {
    On_Time,
    Delayed,
    Cancelled,
    Error,
} Estado;

typedef enum { //Perguntar se tem de ser como está escrito no documento ou se pode ser traduzido
    small_airport,
    medium_airport,
    large_airport,
    heliport,
    seaplane_base,
    error,
} Tipo_aeroporto;

typedef struct voos {
    char *voo_id;
    DataH partida_prevista; // DataHora struct
    DataH partida_efetiva;
    DataH chegada_prevista;
    DataH chegada_efetiva;
    char *porta_embarque;
    char *estado;//char* estado; //A desenvolver struct
    char *codigo_IATA_aer_origem;
    char *codigo_IATA_aer_destino;
    char *id_aeronave;
    char *companhia_aerea;
    char *tracking_url;
} Voo;


typedef struct aeroporto {
    char *codigo_IATA_aer;
    char *nome_aer;
    char *cidade_aer;
    char *pais_aer;
    double latitude; //latitude do aeroporto em graus decimais
    double longitude;
    char *codigo_ICAO_aer;
    Tipo_aeroporto tipo; //Militar, publico e privado
} Aeroporto;


typedef struct aeronave {
    char *id_aeronave;
    char *fabricante_nave;
    char *modelo_nave;
    int ano_fabricacao;
    int capacidade_max_pessoas;
    int alcance_max; // (em km)
} Aeronave;


typedef struct passageiros {
    char *id_passageiro;
    char *primeiro_nome;
    char *ultimo_nome;
    Data data_nascimento;
    char *nacionalidade;
    Genero genero_passageiro;
    char *email_passageiro;
    int telefone_passageiro;
    char *morada_passageiro;
    int fotografia_passageiro; //(int);
} Passageiros;


typedef struct reservas {
    int id_reserva;
    int voos_reservados[10]; //limite de voos reservados a definir
    char *id_pessoa_reservou; //reserva em nome de
    char *lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade; //priority boarding
    int qr_code;
    //QR CODE associado à reserva (int);
} Reservas;

int qual_mes (int mes) {
    if (mes == 2) return 3;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 2;
    else return 1;
}


int string_vazia_ou_espacos(const char *s) {
    if (s == NULL) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0; // encontrou algo que não é espaço
        s++;
    }
    return 1; // só espaços (ou vazio)
}


DataH parse_DataH (char *string) { // com validação incluida

    	DataH novo;
    	int narg = sscanf (string, " %d-%d-%d %d:%d", &novo.data.ano, &novo.data.mes, &novo.data.dia, &novo.horas.hora, &novo.horas.mins);
    	if (narg != 5 || (novo.data.ano < 0 || novo.data.ano > 2025) ||                                                                                             //validação ano
                     	 (novo.data.mes < 1 || novo.data.mes > 12 || (novo.data.ano == 2025 && novo.data.mes > 11)) ||                                              //validação mes
                     	 (novo.data.dia < 0 || (qual_mes(novo.data.mes) == 1 && novo.data.dia > 31) || (qual_mes(novo.data.mes) == 2 && novo.data.dia > 30)) ||     //validação dia
                      	 (qual_mes(novo.data.mes) == 3 && ((novo.data.ano % 4 == 0 && novo.data.dia > 28) || (novo.data.ano % 4 != 0 && novo.data.dia > 29))) ||   //validação especial (mês de Fevereiro)
		     	 (novo.horas.hora < 0 || novo.horas.hora > 23 || novo.horas.mins < 0 || novo.horas.mins > 59)) {
        	fprintf (stderr,"Formatação errada da DataHora, ou célula vazia.\nCorrija antes de ensirir novamente.\nDado incorreto: ");
        	//fprintf(stderr, "|%s|\n", string_vazia_ou_espacos(string) ? "(vazio)" : string);
    		if (string == NULL || strlen(string) == 0) {
    	    		printf("String vazia ou nula\n\n");
    		} else printf ("%s\n\n", string); //Acrescentar trim??
        	novo.data.ano = 0000;
        	novo.data.mes = 00;
        	novo.data.dia = 00;
        	novo.horas.hora = 00;
        	novo.horas.mins = 00;
    	}
 	return novo;
}

Data parse_Data (char *string) {
    Data novo;
    int narg = sscanf (string, "%d-%d-%d", &novo.ano, &novo.mes, &novo.dia);
    if (narg != 3 || (novo.ano < 0 || novo.ano > 2025) ||                                                                                             //validação ano
                     (novo.mes < 1 || novo.mes > 12 || (novo.ano == 2025 && novo.mes > 11)) ||                                              //validação mes
                     (novo.dia < 0 || (qual_mes(novo.mes) == 1 && novo.dia > 31) || (qual_mes(novo.mes) == 2 && novo.dia > 30)) ||     //validação dia
                     (qual_mes(novo.mes) == 3 && ((novo.ano % 4 == 0 && novo.dia > 28) || (novo.ano % 4 != 0 && novo.dia > 29)))) {
	fprintf (stderr, "Formatação errada da DataHora, ou célula vazia.\nCorrija antes de inserir novamente.\nDado incorreto: ");
	if (string == NULL || strlen(string) == 0) {
		printf ("String vazia ou nula\n\n");
	} else printf ("%s\n\n", string);
	novo.ano = 0000;
 	novo.mes = 00;
  	novo.dia = 00;
   }
   return novo;
}

char* string_to_email (char *string) { // função c/ traducao e validação token para Passageiro->email
    int i, j, k;
    int valido = 1; //default : valido
    char* email;

    if (string == NULL || strlen(string) == 0) valido = 0;

    for (i = 0; string[i] != '\0' && string[i] != '@'; i++) {
	if ((string[i] < 'a' || string[i] > 'z') && (string[i] < '0' || string[i] > '9')) valido = 0;
    }
    for (j = i+1; string[j] != '\0' && string[j] != '.'; j++) {
	if (string[j] < 'a' || string[j] > 'z') valido = 0;
    }
    if (j - i < 1) valido = 0; //lstring com menos de 1 caractere

    for (k = j+1; string[k] != '\0'; k++) {
	if (string[k] < 'a' || string[k] > 'z') valido = 0;
    }
    if (k - j < 2 || k - j > 3) valido = 0; //rstring com menos de 2 ou mais de 3 caracteres

//Verificação concluida

    if (valido) email = strdup(string);
    else {
	email = strdup("------@----.---");
	fprintf (stderr,"Formatação errada do Email, ou célula vazia.\nCorrija antes de ensirir novamente.\nDado incorreto: %s\n", email);
    }

    return email;
}


char* string_to_codigoIATA (char* string) { // funcao traducao e validacao token para codigo_IATA_aeroporto
    char* codigo_IATA; //codigo final
    int valido = 1; //default: válido
    if (string == NULL || strlen(string) != 3) valido = 0; // != 3 inclui string vazia
    for (int i = 0; i < 3; i++) {
	if (string[i] < 'A' || string[i] > 'Z') valido = 0;
    }

//Verificação concluida

    if (valido) codigo_IATA = strdup(string);
    else {
	codigo_IATA = "---";
        fprintf (stderr,"Formatação errada do código IATA do aeroporto, ou célula vazia.\nCorrija antes de ensirir novamente.\nDado incorreto: %s\n", codigo_IATA);
    }

    return codigo_IATA;
}


//Funcao a considerar mudança (usar strings ou usar estrutura pre-definida!!
Estado string_to_Estado (char *string) { // função traducao token para Voo->estado com default cancelled
    if (string == NULL || strlen(string) == 0) fprintf (stderr, "Designação de Estado : espaço vazio! Preencha antes de inserir novamente.\n");
    if (strcmp(string, "On_Time") == 0) return On_Time;
    if (strcmp(string, "Delayed") == 0) return Delayed;
    if (strcmp(string, "Cancelled") == 0) return Cancelled;
    fprintf (stderr, "Designação de Estado incorreta : %s . Corrija antes de inserir novamente.\n", string);
    string = strdup("error");
    return Error; //default
}

Tipo_aeroporto valida_tipo (char *string) {
    if (string == NULL || strlen(string) == 0) fprintf (stderr, "Designação de Tipo Aeroporto : espaço vazio! Preencha antes de inserir novamente.\n");
    if (strcmp(string, "small_airport") == 0) return small_airport;
    if (strcmp(string, "medium_airport") == 0) return medium_airport;
    if (strcmp(string, "large_airport") == 0) return large_airport;
    if (strcmp(string, "heliport") == 0) return heliport;
    if (strcmp(string, "seaplane_base") == 0) return seaplane_base;
    fprintf (stderr, "Designação de Tipo Aeroporto incorreta : %s . Corrija antes de inserir novamente.\n", string);
    string = strdup("error");
    return Error; //default
}

bool string_to_bool (char *string, int versao) {
    if (string == NULL || strlen(string) == 0) { // string vazia
	if (versao == 1) fprintf (stderr, "Designação para Bagagem Extra : espaço vazio! Preencha antes de inserir novamente.\n"); // para Reserva-> Bagagem Extra
 	else fprintf (stderr, "Designação para Prioridade : espaço vazio! Preencha antes de inserir novamente.\n"); // para Reserva-> Prioridade
	return 0; // por default
    }
    if (strcmp (string, "true") == 0) return 1;
    if (strcmp (string, "false") == 0) return 0;
    if (versao == 1) fprintf (stderr, "Designação para Bagagem Extra incorreta: %s. Corrija antes de inserir novamente.\n", string); //erro ao inserir Bagagem Extra
    else fprintf (stderr, "Designação para Prioridade incorreta: %s. Corrija antes de inserir novamente.\n", string); // erro ao inserir Prioridade
    return 0; // por default
}

Genero string_to_genero (char* string) {
    if (string == NULL || strlen(string) == 0 || strlen(string) != 1) {
	fprintf (stderr, "Designação para Género do Passageiro: espaço vazio! Preencha antes de inserir novamente.\n");
	return Error;
    }
    if (strcmp(string, "M") == 0) return M;
    if (strcmp(string, "F") == 0) return F;
    if (strcmp(string, "O") == 0) return O;
    fprintf (stderr, "Designação para Género do Passagiero incorreta. Corrija antes de inserir novamente.\n");
    return Error;
}


char* string_to_id_voo (char* string) {
    char* id_voo;
    int valido = 1; //default : valido
    if (string == NULL || strlen(string) != 7) valido = 0;
    for (int i = 0; i < 2; i++) {
	if (string[i] < 'A' || string[i] > 'Z') valido = 0;
    }
    for (int i = 2; i < 7; i++) {
	if (string[i] < '0' || string[i] > '9') valido = 0;
    }

//Validação conluida

    if (valido) id_voo = strdup (string);
    else {
        id_voo = "00-----";
        fprintf (stderr,"Formatação errada do identificador de Voo, ou célula vazia.\nCorrija antes de ensirir novamente.\nDado incorreto: %s\n", id_voo);
    }

    return id_voo;
}


void imprimir_voo(gpointer key, gpointer value, gpointer user_data) {
    char *voo_id = (char *) key;
    Voo *v = (Voo *) value;

    printf("Voo ID: %s\n", voo_id);
    printf("Partida prevista: %04d-%02d-%02d %02d:%02d\n",
           v->partida_prevista.data.ano,
           v->partida_prevista.data.mes,
           v->partida_prevista.data.dia,
           v->partida_prevista.horas.hora,
           v->partida_prevista.horas.mins);
    printf("Partida efetiva: %04d-%02d-%02d %02d:%02d\n",
           v->partida_efetiva.data.ano,
           v->partida_efetiva.data.mes,
           v->partida_efetiva.data.dia,
           v->partida_efetiva.horas.hora,
           v->partida_efetiva.horas.mins);
    printf("Partida prevista: %04d-%02d-%02d %02d:%02d\n",
           v->chegada_prevista.data.ano,
           v->chegada_prevista.data.mes,
           v->chegada_prevista.data.dia,
           v->chegada_prevista.horas.hora,
           v->chegada_prevista.horas.mins);
    printf("Partida efetiva: %04d-%02d-%02d %02d:%02d\n",
           v->chegada_efetiva.data.ano,
           v->chegada_efetiva.data.mes,
           v->chegada_efetiva.data.dia,
           v->chegada_efetiva.horas.hora,
           v->chegada_efetiva.horas.mins);
    printf("Porta embarque: %s\n", v->porta_embarque);
    printf("Estado: %s\n", v->estado); // podes criar função para imprimir o enum como string
    printf("Origem: %s | Destino: %s\n", v->codigo_IATA_aer_origem, v->codigo_IATA_aer_destino);
    printf("Aeronave: %s | Companhia: %s | Tracking: %s\n",
           v->id_aeronave, v->companhia_aerea, v->tracking_url);
    printf("----------\n");
}


<<<<<<< Updated upstream
//Validar email, id.voo e codigo iata
=======

int main () {
>>>>>>> Stashed changes

int main () { //Para opção inserir
// read (table, nome_ficheiro

/*

true main -> read function ----->
char* opcao;
printf ("O que deseja fazer??\n\nInserir\nProcurar\nModificar\nEliminar");
switch (opcao) {
	case "Inserir" :
	case (...)
*/
    int opcao_inserida;
    printf ("Insira o número correspondente ao que quer inserir.\n\n1 -> Voo\n2 -> Aeroporto\n3 -> Aeronave\n4 -> Passageiro\n5 -> Reserva\n\n");
    scanf ("%d", &opcao_inserida);
    putchar ('\n');

    //switch (opcao_inserir) {
	//case 1:
	if (opcao_inserida == 1) {

    		GHashTable *tabela1 = g_hash_table_new(g_str_hash, g_str_equal); //Voos ---> As tabelas vao ser iniciadas no main.c, aqui apenas para treino

    		FILE *ficheiro = fopen("flights_real.csv", "r");
    			if (ficheiro == NULL) {
			perror ("Erro ao abrir o ficheiro.\n");
			return 1;
    		}

    		char *linha = NULL;
    		size_t len = 0;

    		getline(&linha, &len, ficheiro); //Linha inicial do ficheiro.csv com os atributos

    		while (getline(&linha, &len, ficheiro) != -1) {
			Voo *voo_atual = malloc(sizeof(Voo));
			int caso = 0;
			char *token;
			//char *token = strtok(linha, ",\n");
  			while ((token = strsep(&linha, ",\n")) != NULL) {
	    			switch (caso) {
					case 0: voo_atual->voo_id = string_to_id_voo(token); break;
					case 1: voo_atual->partida_prevista = parse_DataH(token); break;
					case 2: voo_atual->partida_efetiva = parse_DataH(token); break;
					case 3: voo_atual->chegada_prevista = parse_DataH(token); break;
					case 4: voo_atual->chegada_efetiva = parse_DataH(token); break;
					case 5: voo_atual->porta_embarque = g_strdup(token); break;
					case 6: voo_atual->estado = g_strdup(token); break;//string_to_Estado(token); break;
					case 7: voo_atual->codigo_IATA_aer_origem = string_to_codigoIATA(token); break;
					case 8: voo_atual->codigo_IATA_aer_destino = string_to_codigoIATA(token); break;
					case 9: voo_atual->id_aeronave = g_strdup(token); break;
					case 10: voo_atual->companhia_aerea = g_strdup(token); break;
					case 11: voo_atual->tracking_url = g_strdup(token); break;
	    			}
            			//printf("Campo: %s\n", token);
            			//token = strtok(NULL, ",\n");
	    			caso++;
			}
			g_hash_table_insert(tabela1, voo_atual->voo_id, voo_atual);
			//printf ("----------\n");
    		}

		// depois, no main ou depois de preencher a hash table:
    		g_hash_table_foreach(tabela1, imprimir_voo, NULL);

    		free(linha);
    		fclose(ficheiro);

    	}
	else if (opcao_inserida == 2) {

                GHashTable *tabela2 = g_hash_table_new(g_str_hash, g_str_equal); //Voos

                FILE *ficheiro = fopen("flights_real.csv", "r");
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 1;
                }

                char *linha = NULL;
                size_t len = 0;

                getline(&linha, &len, ficheiro); //Linha inicial do ficheiro.csv com os atributos

                while (getline(&linha, &len, ficheiro) != -1) {
                        Aeroporto *aeroporto_atual = malloc(sizeof(Aeroporto));
                        int caso = 0;
                        char *token;
                        //char *token = strtok(linha, ",\n");
                        while ((token = strsep(&linha, ",\n")) != NULL) {
                                switch (caso) {
                                        case 0: aeroporto_atual->codigo_IATA_aer = string_to_codigoIATA(token); break;
                                        case 1: aeroporto_atual->nome_aer = g_strdup(token); break;
                                        case 2: aeroporto_atual->cidade_aer = g_strdup(token); break;
                                        case 3: aeroporto_atual->pais_aer = g_strdup(token); break;
                                        case 4: aeroporto_atual->latitude = atof(token); break;
                                        case 5: aeroporto_atual->longitude = atof(token); break;
                                        case 6: aeroporto_atual->codigo_ICAO_aer = g_strdup(token); break;
                                        case 7: aeroporto_atual->tipo = valida_tipo(token); break; //Ou string_to_tipo??
                                }
                                caso++;
                        }
                        g_hash_table_insert(tabela2, aeroporto_atual->codigo_IATA_aer, aeroporto_atual);
                        //printf ("----------\n");
                }

                // depois, no main ou depois de preencher a hash table:
                //g_hash_table_foreach(tabela2, imprimir_aeroporto, NULL); //imprimir aeroporto para verificação a fazer

                free(linha);
                fclose(ficheiro);

        } else if (opcao_inserida == 3) {

                GHashTable *tabela3 = g_hash_table_new(g_str_hash, g_str_equal); //Voos

                FILE *ficheiro = fopen("flights_real.csv", "r"); // a alterar
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 1;
                }

                char *linha = NULL;
                size_t len = 0;

                getline(&linha, &len, ficheiro); //Linha inicial do ficheiro.csv com os atributos

                while (getline(&linha, &len, ficheiro) != -1) {
                        Aeronave *aeronave_atual = malloc(sizeof(Aeronave));
                        int caso = 0;
                        char *token;
                        //char *token = strtok(linha, ",\n");
                        while ((token = strsep(&linha, ",\n")) != NULL) {
                                switch (caso) {
                                        case 0: aeronave_atual->id_aeronave = g_strdup(token); break;
                                        case 1: aeronave_atual->fabricante_nave = g_strdup(token); break;
                                        case 2: aeronave_atual->modelo_nave = g_strdup(token); break;
                                        case 3: if (atoi(token) < 0 || atoi(token) > 2025) aeronave_atual->ano_fabricacao = atoi(token);
						else {
							fprintf (stderr, "Designação incorreta de Ano de Fabricação: %s. Corrija antes de inserir novamente.\n", token);
							aeronave_atual->ano_fabricacao = -1;
						}
						break;
                                        case 4: aeronave_atual->capacidade_max_pessoas = atoi(token); break;
                                        case 5: aeronave_atual->alcance_max = atoi(token); break;
                                }
                                caso++;
                        }
                        g_hash_table_insert(tabela3, aeronave_atual->id_aeronave, aeronave_atual);
                        //printf ("----------\n");
                }

                // depois, no main ou depois de preencher a hash table:
                //g_hash_table_foreach(tabela3, imprimir_aeronave, NULL); //imprimir aeronave para verificação a fazer

               free(linha);
               fclose(ficheiro);

	} else if (opcao_inserida == 4) {

                GHashTable *tabela4 = g_hash_table_new(g_str_hash, g_str_equal); //Voos ---> As tabelas vao ser iniciadas no main.c, aqui apenas para treino

                FILE *ficheiro = fopen("flights_real.csv", "r");
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 1;
                }

                char *linha = NULL;
                size_t len = 0;

                getline(&linha, &len, ficheiro); //Linha inicial do ficheiro.csv com os atributos

                while (getline(&linha, &len, ficheiro) != -1) {
                        Passageiros *passageiro_atual = malloc(sizeof(Passageiros));
                        int caso = 0;
                        char *token;
                        //char *token = strtok(linha, ",\n");
                        while ((token = strsep(&linha, ",\n")) != NULL) {
                                switch (caso) {
                                        case 0: passageiro_atual->id_passageiro = g_strdup(token); break;
                                        case 1: passageiro_atual->primeiro_nome = g_strdup(token); break;
                                        case 2: passageiro_atual->ultimo_nome = g_strdup(token); break;
                                        case 3: passageiro_atual->data_nascimento = parse_Data(token); break; //data! a fazer/modificar funcao parse_dataH
                                        case 4: passageiro_atual->nacionalidade = g_strdup(token); break;
                                        case 5: passageiro_atual->genero_passageiro = string_to_genero(token); break; //Novo tipo de dado?
                                        case 6:	passageiro_atual->email_passageiro = string_to_email(token); break;//Novo tipo de dado??
                                        case 7: passageiro_atual->telefone_passageiro = atoi(token); break;
                                        case 8: passageiro_atual->morada_passageiro = g_strdup(token); break;
					case 9: passageiro_atual->fotografia_passageiro = atoi(token);
                                }
                                caso++;
                        }
                        g_hash_table_insert(tabela4, passageiro_atual->id_passageiro, passageiro_atual);
                }

                // depois, no main ou depois de preencher a hash table:
                //g_hash_table_foreach(tabela4, imprimir_passageiro, NULL); //imprimir passageiro para verificação a fazer

                free(linha);
                fclose(ficheiro);

        } else if (opcao_inserida == 5) {

                GHashTable *tabela5 = g_hash_table_new(g_str_hash, g_str_equal); //Voos ---> As tabelas vao ser iniciadas no main.c, aqui apenas para treino

                FILE *ficheiro = fopen("flights_real.csv", "r");
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 1;
                }

                char *linha = NULL;
                size_t len = 0;

                getline(&linha, &len, ficheiro); //Linha inicial do ficheiro.csv com os atributos

                while (getline(&linha, &len, ficheiro) != -1) {
                        Reservas *reserva_atual = malloc(sizeof(Reservas));
                        int caso = 0;
                        char *token;
			int indice_lista_voos_reserva = 0; //começa a preencher a lista do inicio
                        //char *token = strtok(linha, ",\n");
                        while ((token = strsep(&linha, ",\n")) != NULL) {
                                switch (caso) {
                                        case 0: reserva_atual->id_reserva = atoi(token); break;
                                        case 1: reserva_atual->voos_reservados[indice_lista_voos_reserva++] = atoi(token); break; //! Lista atributo
                                        case 2: reserva_atual->id_pessoa_reservou = g_strdup(token); break;
                                        case 3: reserva_atual->lugar_reservado = g_strdup(token); break;
                                        case 4: reserva_atual->preco_reserva = atof(token); break;
                                        case 5: reserva_atual->bagagem_extra = string_to_bool(token,1); break; //nova função string_to_bool a fazer
                                        case 6: reserva_atual->prioridade = string_to_bool(token,2); break;
                                        case 7: reserva_atual->qr_code = atoi(token); break; //qrcode identificado por int???
                                }
                                caso++;
                        }
                        g_hash_table_insert(tabela5, GINT_TO_POINTER(reserva_atual->id_reserva), reserva_atual);
                }
                //g_hash_table_foreach(tabela5, imprimir_reservas, NULL); //imprimir reservas para verificação

                free(linha);
                fclose(ficheiro);
	}
<<<<<<< Updated upstream
   	return 0;
}


//Podíamos criar um novo atributo a complementar sobre os países para onde é preciso o passaporte na viagem de voo
//Quando ocorre algum erro na validação do scanf, dar ao utilizador opção de corrigir na hora
//Usar typedef union para datas horas???
=======
	g_hash_table_insert(tabela, voo_atual->voo_id, voo_atual);
	//printf ("----------\n");
    }

// depois, no main ou depois de preencher a hash table:
    g_hash_table_foreach(tabela, imprimir_voo, NULL);

    free(linha);
    fclose(ficheiro);
    return 0;
}
>>>>>>> Stashed changes
