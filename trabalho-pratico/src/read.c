#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include <stdint.h>
//#include "read.h"


//gcc -g -Wall -Wextra read.c -o programa $(pkg-config --cflags --libs glib-2.0)
//valgrind --leak-check=full ./programa
//gcc read.c -o programa $(pkg-config --cflags --libs glib-2.0)


//Por acabar:
//Voos->e_maybe ->logica
//passageiro->email->função segmentation core dumped resolver
//reservas->seg fault total opççao 5
//Validação lógica -> tirar de comentario
//Adaptar funções git

#define MAX_LINHAS 200000000
gchar buffer[MAX_LINHAS];

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


typedef uint8_t Genero;

#define O 0
#define F 1
#define M 2

typedef uint8_t Estado;

#define ESTADO_ON_TIME 0
#define ESTADO_DELAYED 1
#define ESTADO_CANCELLED 2

typedef uint8_t Tipo_aeroporto;

#define SMALL_AIRPORT 0
#define MEDIUM_AIRPORT 1
#define LARGE_AIRPORT 2
#define HELIPORT 3
#define SEAPLANE_BASE 4

typedef struct voos {
    char *voo_id;
    char *partida_prevista; // DataHora struct
    char *partida_efetiva;
    char *chegada_prevista;
    char *chegada_efetiva;
    char *porta_embarque;
    Estado estado;//char* estado; //A desenvolver struct
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
    int id_passageiro;
    char *primeiro_nome;
    char *ultimo_nome;
    Data data_nascimento;
    char *nacionalidade;
    Genero genero_passageiro;
    char *email_passageiro;
    char *telefone_passageiro;
    char *morada_passageiro;
    char *fotografia_passageiro; //(int);
} Passageiros;


typedef struct reservas {
    char *id_reserva;
    char **voos_reservados; //lista
    char *id_pessoa_reservou; //reserva em nome de
    char *lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade; //priority boarding
    char *qr_code;
} Reservas;

int qual_mes (int mes) {
    if (mes == 2) return 3;
    if (mes == 4 || mes == 6 || mes == 9 || mes == 11) return 2;
    else return 1;
}


// Função para parse CSV simples com aspas
gchar **parse_csv_line(const gchar *line) {
    GPtrArray *tokens = g_ptr_array_new_with_free_func(g_free); // array de strings
    const gchar *p = line;
    while (*p != '\0') {
        // Pular aspas iniciais
        if (*p == '"') p++;
        
//        const gchar *start = p;
        GString *token = g_string_new(NULL);

        int in_quotes = 1;
        while (*p != '\0' && in_quotes) {
            if (*p == '"') {
                // aspas de fechamento
                in_quotes = 0;
            } else {
                g_string_append_c(token, *p);
            }
            p++;
        }

        g_ptr_array_add(tokens, g_string_free(token, FALSE));

        // p agora aponta para aspas de fechamento ou fim da string
        if (*p == '"') p++; // avançar depois da aspa final

        if (*p == ',') p++; // avançar depois da vírgula
    }

    // transformar GPtrArray em gchar**
    g_ptr_array_add(tokens, NULL); // NULL-terminator
    return (gchar **)g_ptr_array_free(tokens, FALSE);
}


int valida_DataH (char *string, char **datah) { // com validação incluida

    	DataH novo;
    	int narg = sscanf (string, "%d-%d-%d %d:%d", &novo.data.ano, &novo.data.mes, &novo.data.dia, &novo.horas.hora, &novo.horas.mins);
    	if (narg != 5 || (novo.data.ano < 0 || novo.data.ano > 2025) ||                                                                                             //validação ano
                     	 (novo.data.mes < 1 || novo.data.mes > 12 || (novo.data.ano == 2025 && novo.data.mes > 11)) ||                                              //validação mes
                     	 (novo.data.dia < 0 || (qual_mes(novo.data.mes) == 1 && novo.data.dia > 31) || (qual_mes(novo.data.mes) == 2 && novo.data.dia > 30)) ||     //validação dia
                      	 (qual_mes(novo.data.mes) == 3 && ((novo.data.ano % 4 == 0 && novo.data.dia > 28) || (novo.data.ano % 4 != 0 && novo.data.dia > 29))) ||   //validação especial (mês de Fevereiro)
		     	 (novo.horas.hora < 0 || novo.horas.hora > 23 || novo.horas.mins < 0 || novo.horas.mins > 59))

		return 0;

	*datah = strdup(string);
	return 1;
}


int valida_Data (char *string, Data *data) {
    Data novo;
    int narg = sscanf (string, "%d-%d-%d", &novo.ano, &novo.mes, &novo.dia);
    if (narg != 3 || (novo.ano < 0 || novo.ano > 2025) ||                                                                                             //validação ano
                     (novo.mes < 1 || novo.mes > 12 || (novo.ano == 2025 && novo.mes > 11)) ||                                              //validação mes
                     (novo.dia < 0 || (qual_mes(novo.mes) == 1 && novo.dia > 31) || (qual_mes(novo.mes) == 2 && novo.dia > 30)) ||     //validação dia
                     (qual_mes(novo.mes) == 3 && ((novo.ano % 4 == 0 && novo.dia > 28) || (novo.ano % 4 != 0 && novo.dia > 29))))

		return 0;

    *data = novo;
    return 1;
}


int valida_email2 (char *string, char **email) { // função c/ traducao e validação token para Passageiro->email
    int i, j, k;

    if (string == NULL || strlen(string) == 0) return 0;

    for (i = 0; string[i] != '\0' && string[i] != '@'; i++) {
	if ((string[i] < 'a' || string[i] > 'z') && (string[i] < '0' || string[i] > '9')) return 0;
    }
    if (i == 1) return 0; //0 caracteres //modificado

    for (j = i+1; string[j] != '\0' && string[j] != '.'; j++) {
	if (string[j] < 'a' || string[j] > 'z') return 0;
    }
    if (j - i < 1) return 0; //lstring com menos de 1 caractere

    for (k = j+1; string[k] != '\0'; k++) {
	if (string[k] < 'a' || string[k] > 'z') return 0;
    }
    if (k - j < 2 || k - j > 3) return 0; //rstring com menos de 2 ou mais de 3 caracteres

//Verificação concluida

    *email = strdup(string);
    return 1;
}


int valida_email(const char *string, char **email) {
    if (string == NULL || strlen(string) == 0) return 0;

    // 1. Separar username e domínio pelo '@'
    const char *at = strchr(string, '@');
    if (!at || at == string || *(at + 1) == '\0') return 0; // não tem '@' ou '@' no início

    // username: do início até '@'
    for (const char *p = string; p < at; p++) {
        if (!(islower(*p) || isdigit(*p) || *p == '.')) {
            return 0; // caractere inválido
        }
    }

    // domínio: depois do '@'
    const char *dominio = at + 1;
    const char *dot = strchr(dominio, '.');
    if (!dot || dot == dominio || *(dot + 1) == '\0') return 0;//dot == dominio + strlen(dominio) - 1) return 0; // domínio inválido

    // lstring: antes do '.'
    for (const char *p = dominio; p < dot; p++) {
        if (!islower(*p)) return 0;
    }

    // rstring: depois do '.'
    size_t rlen = strlen(dot + 1);
    if (rlen < 2 || rlen > 3) return 0; // tamanho inválido
    for (const char *p = dot + 1; *p; p++) {
        if (!islower(*p)) return 0;
    }

    *email = strdup(string);
    return 1; // email válido
}



//['94747652,98523485']
//Passaar para o formato de uma lista
int valida_voos_reservados(char *string, char ***lista) {
    if (string == NULL || strlen(string) < 3) return 0; //[] invalido
    int len = strlen(string);
    if (string[0] != '[' || string[len-1] != ']') return 0;

//Passar para o formato de lista de lista de string
    int i = 0;
    while (string != NULL) {
	char *token = strsep(&string,"',\t");
	(*lista)[i] = strdup(token);
    }

    // *lista = strdup(string);
    return 1;
}

/*
    int length = strlen(string);

    // Lista com 1 elemento
    if (string[0] == '[' && string[length - 1] == ']') {
        char *conteudo = strndup(string + 2, length - 4); // tira [" e "]
        lista[0] = conteudo;
        return 1;
    }

    // Lista com 2 elementos
    if (string[0] == '[' && string[length - 1] != ']') {
        char *primeira = strndup(string + 2, length - 3); // até antes da vírgula
        lista[0] = primeira;

        char *token2 = remover_aspas(strsep(linha, ",\n"));
        if (token2 == NULL) return 0;

        int length2 = strlen(token2);
        if (token2[length2 - 1] == ']') {
            char *segunda = strndup(token2 + 1, length2 - 3); // tira aspas e ]
            lista[1] = segunda;
        }
        return 1;
    }

    return 0;
}
*/

int valida_codigoIATA (char* string, char **codigo_IATA) { // funcao traducao e validacao token para codigo_IATA_aeroporto
    //char* codigo_IATA; //codigo final
    if (string == NULL || strlen(string) != 3) return 0; // != 3 inclui string vazia
    for (int i = 0; i < 3; i++) {
	if (string[i] < 'A' || string[i] > 'Z') return 0;
    }

//Verificação concluida

    *codigo_IATA = strdup(string);

    return 1; //codigo_IATA valido
}



int valida_id_reserva (char* string, char **id_reserva) {
    if (string == NULL || strlen(string) != 10 || string[0] != 'R') return 0;
    for (int i = 1; i < 10; i++) {
	if (string[i] < '0' || string[i] > '9') return 0;
    }

//Validaçao concluida

    *id_reserva = strdup(string);
    return 1;
}

int valida_id_passageiro(char* string, int *id_passageiro) {

    if (string == NULL || strlen(string) != 9) return 0;
    for (int i = 0; i < 9; i++) {
	if (string[i] < '0' || string[i] > '9') return 0;
    }

//Validação concluida

    *id_passageiro = atoi(string);
    return 1;
}

int valida_coordenadas (const char* string, int versao, double coordenada) {
//versao 1. latitude
//versao 2. longitude
    if (string == NULL || strlen(string) == 0) {
	return 0;
    }
    int length = strlen(string);


    int contador;
    for (int i = 0; i < length; i++) {
	if ((string[i] == '-' && i != 0) || string[i] < '0' || string[i] > '9' || string[i] != '.') return 0;
	if (string[i] == '.') contador++;
    }
    if (contador > 1) return 0;


    coordenada = atof(string);
    switch (versao) {
	case 1: if (coordenada < -90 || coordenada > 90) return 0;
	  	break;
	case 2: if (coordenada < -180 || coordenada > 180) return 0;
		break;
    }

    return 1;
}


//Funcao a considerar mudança (usar strings ou usar estrutura pre-definida!!
int valida_Estado (char *string, Estado *e) { // função traducao token para Voo->estado com default cancelled
    if (string == NULL || strlen(string) == 0) return 0;
    if (strcmp(string, "On Time") == 0) *e = ESTADO_ON_TIME;
    else if (strcmp(string, "Delayed") == 0) *e = ESTADO_DELAYED;
    else if (strcmp(string, "Cancelled") == 0) *e = ESTADO_CANCELLED;
    else return 0;

//    fprintf (stderr, "Designação de Estado incorreta : %s . Corrija antes de inserir novamente.\n", string);
//    string = strdup("error");
    return 1; //Estado válido
}

int valida_tipo_aer (char *string, Tipo_aeroporto *t) {
    if (string == NULL || strlen(string) == 0) return 0;
    if (strcmp(string, "small_airport") == 0) *t = SMALL_AIRPORT;
    else if (strcmp(string, "medium_airport") == 0) *t = MEDIUM_AIRPORT;
    else if (strcmp(string, "large_airport") == 0) *t = LARGE_AIRPORT;
    else if (strcmp(string, "heliport") == 0) *t = HELIPORT;
    else if (strcmp(string, "seaplane_base") == 0) *t = SEAPLANE_BASE;
    else return 0;

    return 1;
}

int valida_bool (char *string, bool *b) {
    if (string == NULL || strlen(string) == 0) { // string vazia
	return 0; // por default
    }
    if (strcmp (string, "true") == 0) *b = 1;
    else if (strcmp (string, "false") == 0) *b = 0;
    else return 0;

    return 1;
}

int valida_genero (char* string, Genero *g) {
    if (string == NULL || strlen(string) != 1) return 0;
    if (strcmp(string, "M") == 0) *g = M;
    else if (strcmp(string, "F") == 0) *g = F;
    else if (strcmp(string, "O") == 0) *g = O;
    else return 0;

    return 1;
}


int valida_id_voo (char* string) {
   // char* id_voo;
    int valido = 1; //default : valido
    if (string == NULL || strlen(string) != 7) valido = 0;
    else {
	for (int i = 0; i < 7; i++) {
	    if ((i < 2 && (string[i] < 'A' || string[i] > 'Z')) ||
	       (i >= 2 && (string[i] < '0' || string[i] > '9'))) valido = 0;
        }
    }
//Validação conluida

    if (!valido) return 0;
    return 1;
}



//VALIDAÇÕES LÓGICAS -> pode ser que mudemos isto de módulo
//Voos e reservas

int compara_dataH (char *datah1, char *datah2) { //se 1 for não anterior a 2, entao retorna 1 else retorna 0
//Se ano1<ano2, ent 2 vem depois de 1 : errado
//else ano1>ano2 : certo || ano1 == ano2 : proxima
    DataH data1;
    sscanf (datah1, "%d-%d-%d %d:%d", &data1.data.ano, &data1.data.mes, &data1.data.dia, &data1.horas.hora, &data1.horas.mins);
    DataH data2;
    sscanf (datah2, "%d-%d-%d %d:%d", &data2.data.ano, &data2.data.mes, &data2.data.dia, &data2.horas.hora, &data2.horas.mins);
    if (data1.data.ano > data2.data.ano) return 1;
    else if (data1.data.ano == data2.data.ano) {
	     if (data1.data.mes > data2.data.mes) return 1;
	     else if (data1.data.mes == data2.data.mes) {
			if (data1.data.dia > data2.data.dia) return 1;
			else if (data1.data.dia == data2.data.dia) {
				if (data1.horas.hora > data2.horas.hora) return 1;
				else if (data1.horas.hora == data2.horas.hora) {
					if (data1.horas.mins >= data2.horas.mins) return 1; //enunciado: "data nao anterior"
				}
			}
	    }
    }

    return 0;
//Usar constantes para evitar tantos acessos continuos ao mesmo valor na memoria??
}





/*

int valida_VOO (Voo voo) {
    //destino != origem
    if (strcmp (voo.codigo_IATA_aer_origem, voo.codigo_IATA_aer_destino) == 0) return 0;

    //cada voo tem uma aeronave correspondente EXISTENTE
    char *aeronave_chave = voo.id_aeronave;
    if (!g_hash_table_contains(tabela3,aeronave_chave)) return 0;

    //if CANCELLED, actual departure e actual arrival == "N/A"
    if (voo.estado == 2) {
	if (strcmp(voo.partida_efetiva, "N/A") != 0 || strcmp(voo.chegada_efetiva, "N/A") != 0) return 0;
    }
    else {

        //arrival >= departure
    	if (!compara_dataH(voo.chegada_prevista, voo.partida_prevista) ||
            !compara_dataH(voo.chegada_prevista, voo.partida_efetiva)  || //Não tem de ser -> REVER
            !compara_dataH(voo.chegada_efetiva, voo.partida_prevista)  ||
            !compara_dataH(voo.chegada_efetiva, voo.partida_efetiva)) return 0;

	//if DELAYED, actual departure/arrival >= departure/arrival
     	if (voo.estado == 1) {
                if (!compara_dataH(voo.partida_efetiva,voo.partida_prevista) ||
                    !compara_dataH(voo.chegada_efetiva,voo.chegada_prevista)) return 0;
	}
    }

    return 1; //Válido!
}

int valida_RESERVA (Reservas reserva) {
    //flights id -> lista de 1 ou 2 voos EXSITENTES
    int length_vr = sizeof (reserva.voos_reservados) / sizeof (reserva.voos_reservados[0]);
    if (length_vr < 1 || length_vr > 2) return 0;
    else {
	for (int i = 0; i < length_vr; i++) {
		char *voo_chave = reserva.voos_reservados[i];
		if (!g_hash_table_contains(tabela1,voo_chave)) return 0;
	}
    }

    //document number -> passageiro EXISTENTE
    char* passageiro_chave = reserva.id_pessoa_reservou;
    if (!g_hash_table_contains(tabela4,passageiro_chave)) return 0;

    //if (flights ids == 2) -> destination1 == departure2, i.e., simulando uma escala
    if (length_vr == 2) {
	Voo *voo1 = g_hash_table_lookup(tabela1,reserva.voos_reservados[0]);
	Voo *voo2 = g_hash_table_lookup(tabela1,reserva.voos_reservados[1]);
	if (strcmp(voo1->codigo_IATA_aer_destino, voo2->codigo_IATA_aer_origem) != 0) return 0;
    }

    return 1; //Reserva válida!

}

*/

//Validar email, id.voo e codigo iata

int read () { //Para opção inserir
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
    if (opcao_inserida == 5) printf ("Entrou na opçao 5.\n");
    putchar ('\n');
    //int linhas_com_sucesso = 0;
    //int linhas_totais = 0;

    //switch (opcao_inserir) {
	//case 1:
	if (opcao_inserida == 1) {

    		GHashTable *tabela1 = g_hash_table_new(g_str_hash, g_str_equal); //Voos ---> As tabelas vao ser iniciadas no main.c, aqui apenas para treino

    		FILE *ficheiro = fopen("flights.csv", "r");
    			if (ficheiro == NULL) {
			perror ("Erro ao abrir o ficheiro.\n");
			return 0;
    		}

                int linhas_totais = 0;
                int linhas_com_sucesso = 0;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                fgets(buffer,sizeof(buffer),ficheiro);

                while (fgets(buffer, sizeof(buffer),ficheiro)) {
                        Voo *voo_atual = malloc(sizeof(Voo));
                        linhas_totais++;
                        int linha_valida = 1;
			int e_maybe = -1; // verificação datas baseado no estado do voo
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha
                        //for (int i = 0; campos[i] != NULL; i++) {
                        //      printf("%s\n", campos[i]);
                        //}
                        //g_ptr_array_add(todas_as_linhas,campos); //Guarda o array de strings
                        voo_atual->voo_id = strdup(campos[0]);
			if (linha_valida) {
				if (!valida_DataH(campos[1],&voo_atual->partida_prevista)) linha_valida = 0;
			}
			if (linha_valida) { //Falta validação logica voo cancelado
				if (strcmp(campos[2],"N/A") == 0) {
				 	e_maybe = 2;
					voo_atual->partida_efetiva = g_strdup(campos[2]);
				} else {
					if (!valida_DataH(campos[2],&voo_atual->partida_efetiva)) linha_valida = 0;
				}
			}
			if (linha_valida) {
				if (!valida_DataH(campos[3],&voo_atual->chegada_prevista)) linha_valida = 0;
 			}
			if (linha_valida) {
				if (e_maybe == 2) {
					if (strcmp(campos[4],"N/A") != 0) linha_valida = 0;
					else voo_atual->chegada_efetiva = g_strdup(campos[4]);
				} else {
					if (!valida_DataH(campos[4],&voo_atual->chegada_efetiva)) linha_valida = 0;
				}
			}
			if (linha_valida) voo_atual->porta_embarque = g_strdup(campos[5]);
			if (linha_valida) {
				if (!valida_Estado(campos[6],&voo_atual->estado)) linha_valida = 0;
				if (voo_atual->estado == 2 && e_maybe != 2) linha_valida = 0; //data n/a, para outro estado que nao cancelled
			}
			if (linha_valida) {
				if (!valida_codigoIATA(campos[7],&voo_atual->codigo_IATA_aer_origem)) linha_valida = 0;
			}
                        if (linha_valida) {
                                if (!valida_codigoIATA(campos[8],&voo_atual->codigo_IATA_aer_destino)) linha_valida = 0;
                        }
			if (linha_valida) voo_atual->id_aeronave = g_strdup(campos[9]);
			if (linha_valida) voo_atual->companhia_aerea = g_strdup(campos[10]);
			if (linha_valida) voo_atual->tracking_url = g_strdup(campos[11]);

			//if (linha_valida) printf("Linha %d:\n%s\nD\nD\nD\nD\nOT\n\n", linhas_totais,voo_atual->voo_id);




                        if (!linha_valida) {
                                FILE *ficheiro_erros = fopen ("flights_errors.csv", "a+");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_voos_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
			else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela1, voo_atual->voo_id, voo_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela dos voos %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depoisd e usar 
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);

    	}
	else if (opcao_inserida == 2) {

                GHashTable *tabela2 = g_hash_table_new(g_str_hash, g_str_equal); //Voos

                FILE *ficheiro = fopen("airports.csv", "r");
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 0;
                }

                int linhas_totais = 0;
                int linhas_com_sucesso = 0;

		GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                fgets(buffer,sizeof(buffer),ficheiro);

                while (fgets(buffer, sizeof(buffer),ficheiro)) {
                        Aeroporto *aeroporto_atual = malloc(sizeof(Aeroporto));
                        linhas_totais++;
                        int linha_valida = 1;
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha
                        //for (int i = 0; campos[i] != NULL; i++) {
                        //      printf("%s\n", campos[i]);
                        //}
                        //g_ptr_array_add(todas_as_linhas,campos); //Guarda o array de strings
                        if (!valida_codigoIATA(campos[0],&aeroporto_atual->codigo_IATA_aer)) linha_valida = 0;// = g_strdup(campos[0]);
                        if (linha_valida) aeroporto_atual->nome_aer = g_strdup(campos[1]);
                        if (linha_valida) aeroporto_atual->cidade_aer = g_strdup(campos[2]);
			if (linha_valida) aeroporto_atual->pais_aer = g_strdup(campos[3]);
                        //if (linha_valida) {
			//	if (!valida_coordenadas(campos[4],1,aeroporto_atual->latitude)) linha_valida = 0; // = atof(campos[4]);
			//}
			if (linha_valida) aeroporto_atual->latitude = atof(campos[4]);
			//if (linha_valida) {
                        //      if (!valida_coordenadas(campos[5],2,aeroporto_atual->longitude)) linha_valida = 0; // = atof(campo>
                        //}

                        if (linha_valida) aeroporto_atual->longitude = atof(campos[5]);
			if (linha_valida) aeroporto_atual->codigo_ICAO_aer = g_strdup(campos[6]);
			if (linha_valida) {
				if (!valida_tipo_aer(campos[7],&aeroporto_atual->tipo)) linha_valida = 0;
			}
			//if (linha_valida) aeroporto_atual->tipo = M;

			if (linha_valida) printf("Linha %d:\n%s\n%s\n%s\n%s\n%f\n%f\n%s\nM\n\n", linhas_totais,aeroporto_atual->codigo_IATA_aer,aeroporto_atual->nome_aer,aeroporto_atual->cidade_aer ,aeroporto_atual->pais_aer,aeroporto_atual->latitude,aeroporto_atual->longitude,aeroporto_atual->codigo_ICAO_aer);

                        if (!linha_valida) {
                                FILE *ficheiro_erros = fopen ("airports_errors.csv", "a+");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_aeroportos_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela2, aeroporto_atual->codigo_IATA_aer, aeroporto_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela dos voos %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depoisd e usar
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);

        } else if (opcao_inserida == 3) {

                GHashTable *tabela3 = g_hash_table_new(g_str_hash, g_str_equal); //Aeronaves

                FILE *ficheiro = fopen("aircrafts.csv", "r"); // a alterar
                        if (ficheiro == NULL) {
                        perror ("Erro ao abrir o ficheiro.\n");
                        return 0;
                }

		int linhas_totais = 0;
		int linhas_com_sucesso = 0;

		GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
		fgets(buffer,sizeof(buffer),ficheiro);

		while (fgets(buffer, sizeof(buffer),ficheiro)) {
			Aeronave *aeronave_atual = malloc(sizeof(Aeronave));
			linhas_totais++;
			int linha_valida = 1;
			buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
			gchar **campos = parse_csv_line(buffer); //faz o parsing da linha
			//for (int i = 0; campos[i] != NULL; i++) {
			//	printf("%s\n", campos[i]);
			//}
			//g_ptr_array_add(todas_as_linhas,campos); //Guarda o array de strings
			aeronave_atual->id_aeronave = g_strdup(campos[0]);
			aeronave_atual->fabricante_nave = g_strdup(campos[1]);
			aeronave_atual->modelo_nave = g_strdup(campos[2]);
      			//linha_valida = 0;
			if (atoi(campos[3]) >= 0 || atoi(campos[3]) <= 2025) aeronave_atual->ano_fabricacao = atoi(campos[3]);
                        else linha_valida = 0;
			//}
			//if (linha_valida) aeronave_atual->ano_fabricacao = atoi(campos[3]); //falta validação
			if (linha_valida) aeronave_atual->capacidade_max_pessoas = atoi(campos[4]);
			if (linha_valida) aeronave_atual->alcance_max = atoi(campos[5]);
                        if (linha_valida) printf("Linha %d:\n%s\n%s\n%s\n%d\n%d\n%d\n\n", linhas_totais, aeronave_atual->id_aeronave,aeronave_atual->fabricante_nave,aeronave_atual->modelo_nave,aeronave_atual->ano_fabricacao,aeronave_atual->capacidade_max_pessoas,aeronave_atual->alcance_max);


                        if (!linha_valida) {
                                FILE *ficheiro_erros = fopen ("aircrafts_errors.csv", "a+");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_aeronave_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        else {
                                linhas_com_sucesso++;
				g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela3, aeronave_atual->id_aeronave, aeronave_atual);
                        }
		}
	        printf ("Foram inseridas com sucesso na tabela dos voos %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

		for (guint i = 0; i < todas_as_linhas->len; i++) {
			gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
			g_strfreev(campos); //libertar depoisd e usar 
		}

		g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

		fclose(ficheiro);

	} else if (opcao_inserida == 4) {

                GHashTable *tabela4 = g_hash_table_new(g_str_hash, g_str_equal); //Passageiros

                FILE *ficheiro = fopen("passengers.csv", "r");
                        if (ficheiro == NULL) {
                             	perror ("Erro ao abrir o ficheiro.\n");
                        	return 0;
		}

                int linhas_totais = 0;
                int linhas_com_sucesso = 0;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                fgets(buffer,sizeof(buffer),ficheiro);

                while (fgets(buffer, sizeof(buffer),ficheiro)) {
                        Passageiros *passageiro_atual = malloc(sizeof(Passageiros));
                        linhas_totais++;
                        int linha_valida = 1;
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha
                        //for (int i = 0; campos[i] != NULL; i++) {
                        //      printf("%s\n", campos[i]);
                        //}
                        //g_ptr_array_add(todas_as_linhas,campos); //Guarda o array de strings

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
			//if (linha_valida) passageiro_atual->email_passageiro = g_strdup("mine"); //campos[6]);
			if (linha_valida) { //problema
				printf("%d\n",linha_valida);
				if (!valida_email(campos[6],&passageiro_atual->email_passageiro)) linha_valida = 0;
				printf("%d\n", linha_valida);
			}
			if (linha_valida) passageiro_atual->telefone_passageiro = strdup(campos[7]);
			if (linha_valida) passageiro_atual->morada_passageiro = g_strdup(campos[8]);
			if (linha_valida) passageiro_atual->fotografia_passageiro = g_strdup(campos[9]);

//			if (linha_valida) printf("1\n");
//			else printf("0\n");

                        if (!linha_valida) {
                                FILE *ficheiro_erros = fopen ("passengers_errors.csv", "a+");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_passageiros_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
                                g_hash_table_insert(tabela4, GINT_TO_POINTER (passageiro_atual->id_passageiro), passageiro_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela dos voos %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depois e usar
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);

        } else if (opcao_inserida == 5) {
		printf("Entrou na opcao 5");
                GHashTable *tabela5 = g_hash_table_new(g_str_hash, g_str_equal); //Voos ---> As tabelas vao ser iniciadas no main.c, aqui apenas para treino

                FILE *ficheiro = fopen("reservations.csv", "r");
                if (ficheiro == NULL) {
                    perror ("Erro ao abrir o ficheiro.\n");
                    return 0;
                }

		int linhas_totais = 0;
                int linhas_com_sucesso = 0;

                GPtrArray *todas_as_linhas = g_ptr_array_new(); //lista de arrays de string iniciar
                fgets(buffer,sizeof(buffer),ficheiro);
		printf("Fez o fets");

                while (fgets(buffer, sizeof(buffer),ficheiro)) {
			printf("Entrou no while");
			Reservas *reserva_atual = malloc(sizeof(Reservas));
                        linhas_totais++;
                        int linha_valida = 1;
                        buffer[strcspn(buffer,"\n")] = '\0'; //remove \n do final
                        gchar **campos = parse_csv_line(buffer); //faz o parsing da linha

			if (!valida_id_reserva(campos[0],&reserva_atual->id_reserva)) linha_valida = 0;
			if (linha_valida) {
				if (!valida_voos_reservados(campos[1],&reserva_atual->voos_reservados)) linha_valida = 0;
			}
			if (linha_valida) reserva_atual->id_pessoa_reservou = g_strdup(campos[2]);
			if (linha_valida) reserva_atual->lugar_reservado = g_strdup(campos[3]);
			if (linha_valida) reserva_atual->preco_reserva = atof(campos[4]);
			if (linha_valida) {
				if (!valida_bool(campos[5],&reserva_atual->bagagem_extra)) linha_valida = 0;
                        }
			if (linha_valida) {
				if (!valida_bool(campos[6],&reserva_atual->prioridade)) linha_valida = 0;
                        }
			if (linha_valida) reserva_atual->qr_code = strdup(campos[7]);


                        if (!linha_valida) {
                                FILE *ficheiro_erros = fopen ("reservations_errors.csv", "a+");
                                if (ficheiro_erros == NULL) {
                                        perror ("Erro ao abrir o ficheiro_reservas_erros.\n");
                                        return 0;
                                }
                                fputs(buffer,ficheiro_erros);
                                fprintf(ficheiro_erros, "\n");
                                fclose(ficheiro_erros);

                        }
                        else {
                                linhas_com_sucesso++;
                                g_ptr_array_add(todas_as_linhas,campos);
				g_hash_table_insert(tabela5, GINT_TO_POINTER(reserva_atual->id_reserva), reserva_atual);
                        }
                }
                printf ("Foram inseridas com sucesso na tabela dos voos %d linhas de %d.\n", linhas_com_sucesso, linhas_totais);

                for (guint i = 0; i < todas_as_linhas->len; i++) {
                        gchar **campos = g_ptr_array_index(todas_as_linhas,i); //associa os campos *a linha
                        g_strfreev(campos); //libertar depoisd e usar
                }

                g_ptr_array_free(todas_as_linhas,TRUE); //liberta a lista

                fclose(ficheiro);
	}
   	return 1;
}


int main() {

    int a = read();
    if (!a) return 1;
    return 0;
}

//Podíamos criar um novo atributo a complementar sobre os países para onde é preciso o passaporte na viagem de voo
//Quando temos erros no input (ex. flights com erros), imprimimos algo ou interrompemos a impressao por completo, dizendo apenas o que ta mal???
//Critérios de avaliação: uso de memória, programa de testes, mais oq??
//Problemas das aspas. é algo para resolver ou algo que nem deveria estar a acontecer??
//Ficheiro flights_sem erros -> todo: dá merda; uns 5 inicio -> da tudo bem! 	whyyyyy??? :((
//Os ficheiros com/sem erros é suposto o utilizador escolher, ou mudar no código???





/*

main ()

GHashTable *tabela1 = g_hash_table_new(g_str_hash, g_str_equal); -> voos
GHashTable *tabela2 = g_hash_table_new(g_str_hash, g_str_equal); -> aeroportos
GHashTable *tabela3 = g_hash_table_new(g_str_hash, g_str_equal); -> aeronaves
GHashTable *tabela4 = g_hash_table_new(g_str_hash, g_str_equal); -> passageiros
GHashTable *tabela5 = g_hash_table_new(g_str_hash, g_str_equal); -> reservas

read(ficheiro2,tabela2);
read(ficheiro3,tabela3);
read(ficheiro4,tabela4);
read(ficheiro1,tabela1);
read(ficheiro5,tabela5);

*/
