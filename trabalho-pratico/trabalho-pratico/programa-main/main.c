#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct data_def {
   int ano, mes, dia;
} Data;

typedef struct hora_def {
   int hora, mins;
} Hora;

typedef struct dataH {
    Data data;
    Hora hora;
} DataH;

typedef struct voos {
    int voo_id;
    DataH partida_prevista; //A criar DataHora struct
    DataH partida_efetiva;
    DataH chegada_prevista;
    DataH chegada_efetiva;
    int porta_embarque;
    char* estado; //A desenvolver struct
    char* codigo_IATA_aer_origem;
    char* codigo_IATA_aer_destino;
    int id_aeronave;
    char* companhia_area;
    char* tracking_url;
} Voo;


typedef struct aeroporto {
    char* codigo_IATA_aer;
    char* nome_aer;
    char* cidada_aer;
    char* pais_aer;
    double latitude; //latitude do aeroporto em graus decimais
    double longitude;
    char* codigo_ICAO_aer;
    char* tipo; //Militar, publico e privado
} Aeroporto;


typedef struct aeronave {
    int id_aeronave;
    char* fabricante_nave;
    char* modelo_nave;
    int ano_fabricacao;
    int capacidade_max_pessoas;
    int alcance_max; // (em km)
} Aeronave;


typedef struct passageiros {
    char* id_pessoa;
    char* primeiro_nome;
    char* ultimo_nome;
    Data data_nascimento;
    char* nacionalidade;
    char* genero_pessoa;
    char* email_pessoa;
    int telefone_pessoa;
    char* morada_pessoa;
    //fotografia_pessoa (int);
} Passageiros;


typedef struct reservas {
    int id_reserva;
    int voos_reservados[10]; //limite de voos reservados a definir
    char* id_pessoa_reservou; //reserva em nome de
    char* lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade; //priority boarding
    //QR CODE associado à reserva (int);
} Reservas;


int main () {
    FILE *ficheiro = fopen("flights.csv", "r");
    if (ficheiro == NULL) {
	perror ("Erro ao abrir o ficheiro.\n");
	return 1;
    }

    char *linha = NULL;
    size_t len = 0;

    while (getline(&linha, &len, ficheiro) != -1) {
	char *token = strtok(linha, ",\n");
  	while (token) {
	    printf("Campo: %s\n", token);
	    token = strtok(NULL, ",\n");
	}
	printf ("----------\n");
    }

    free(linha);
    fclose(ficheiro);
    return 0;
}
