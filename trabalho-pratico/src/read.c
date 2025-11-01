#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>


typedef struct data_def {    j
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
    On_Time,
    Delayed,
    Cancelled,
} Estado;

typedef struct voos {
    char *voo_id;
    DataH partida_prevista; //A criar DataHora struct
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
    char *cidada_aer;
    char *pais_aer;
    double latitude; //latitude do aeroporto em graus decimais
    double longitude;
    char *codigo_ICAO_aer;
    char *tipo; //Militar, publico e privado
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
    char *id_pessoa;
    char *primeiro_nome;
    char *ultimo_nome;
    Data data_nascimento;
    char *nacionalidade;
    char *genero_pessoa;
    char *email_pessoa;
    int telefone_pessoa;
    char *morada_pessoa;
    //fotografia_pessoa (int);
} Passageiros;


typedef struct reservas {
    int id_reserva;
    int voos_reservados[10]; //limite de voos reservados a definir
    char *id_pessoa_reservou; //reserva em nome de
    char *lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade; //priority boarding
    //QR CODE associado à reserva (int);
} Reservas;

DataH parse_DataH (char *string) {
    DataH novo;
    sscanf (string, "%d-%d-%d %d:%d", &novo.data.ano, &novo.data.mes, &novo.data.dia, &novo.horas.hora, &novo.horas.mins);
    return novo;
}

Estado string_to_Estado (char *string) {
    if (strcmp(string, "On_Time") == 0) return On_Time;
    if (strcmp(string, "Delayed") == 0) return Delayed;
    if (strcmp(string, "Cancelled") == 0) return Cancelled;
    return Cancelled;
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


int main () {

    GHashTable *tabela = g_hash_table_new(g_str_hash, g_str_equal);

    FILE *ficheiro = fopen("flights_real.csv", "r");
    if (ficheiro == NULL) {
	perror ("Erro ao abrir o ficheiro.\n");
	return 1;
    }

    char *linha = NULL;
    size_t len = 0;

    getline(&linha, &len, ficheiro);

    while (getline(&linha, &len, ficheiro) != -1) {
	Voo *voo_atual = malloc(sizeof(Voo));
	int caso = 0;
	char *token;
	//char *token = strtok(linha, ",\n");
  	while ((token = strsep(&linha, ",\n")) != NULL) {
	    switch (caso) {
		case 0: voo_atual->voo_id = g_strdup(token); break;
		case 1: voo_atual->partida_prevista = parse_DataH(token); break;
		case 2: voo_atual->partida_efetiva = parse_DataH(token); break;
		case 3: voo_atual->chegada_prevista = parse_DataH(token); break;
		case 4: voo_atual->chegada_efetiva = parse_DataH(token); break;
		case 5: voo_atual->porta_embarque = g_strdup(token); break;
		case 6: voo_atual->estado = g_strdup(token); break;//string_to_Estado(token); break;
		case 7: voo_atual->codigo_IATA_aer_origem = g_strdup(token); break;
		case 8: voo_atual->codigo_IATA_aer_destino = g_strdup(token); break;
		case 9: voo_atual->id_aeronave = g_strdup(token); break;
		case 10: voo_atual->companhia_aerea = g_strdup(token); break;
		case 11: voo_atual->tracking_url = g_strdup(token); break;
	    }
            //printf("Campo: %s\n", token);
            //token = strtok(NULL, ",\n");
	    caso++;
	}
	g_hash_table_insert(tabela, voo_atual->voo_id, voo_atual);
	//printf ("----------\n");
    }

// depois, no main ou depois de preencher a hash table:
    g_hash_table_foreach(tabela, imprimir_voo, NULL);

    free(linha);
    fclose(ficheiro);
    return 0;
}
