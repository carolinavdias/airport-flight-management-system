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
    GENERO_F,
    GENERO_M,
    GENERO_O,
    GENERO_ERROR
} Genero;

typedef enum {
    ESTADO_ON_TIME,
    ESTADO_DELAYED,
    ESTADO_CANCELLED,
    ESTADO_ERROR
} Estado;

typedef enum {
    TIPO_SMALL_AIRPORT,
    TIPO_MEDIUM_AIRPORT,
    TIPO_LARGE_AIRPORT,
    TIPO_HELIPORT,
    TIPO_SEAPLANE_BASE,
    TIPO_ERROR
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


//funções auxiliares e de validação
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


DataH parse_DataH (char *string) {
    DataH novo;
    int narg = sscanf (string, " %d-%d-%d %d:%d", &novo.data.ano, &novo.data.mes,
                       &novo.data.dia, &novo.horas.hora, &novo.horas.mins);

    if (narg != 5 || (novo.data.ano < 0 || novo.data.ano > 2025) ||
        (novo.data.mes < 1 || novo.data.mes > 12) ||
        (novo.data.dia < 1 || novo.data.dia > 31) ||
        (novo.horas.hora < 0 || novo.horas.hora > 23) ||
        (novo.horas.mins < 0 || novo.horas.mins > 59)) {

        fprintf(stderr, "Formatação errada da DataHora: %s\n", string ? string : "(null)");
        novo.data.ano = novo.data.mes = novo.data.dia = 0;
        novo.horas.hora = novo.horas.mins = 0;
    }
    return novo;
}

Data parse_Data (char *string) {
    Data novo;
    int narg = sscanf (string, "%d-%d-%d", &novo.ano, &novo.mes, &novo.dia);
    if (narg != 3 || (novo.ano < 0 || novo.ano > 2025) ||
        (novo.mes < 1 || novo.mes > 12) || (novo.dia < 1 || novo.dia > 31)) {
        fprintf(stderr, "Formatação errada da Data: %s\n", string ? string : "(null)");
        novo.ano = novo.mes = novo.dia = 0;
    }
    return novo;
}

char* string_to_email (char *string) {
    if (string == NULL || strlen(string) == 0) return strdup("------@----.---");
    if (!strchr(string, '@')) return strdup("------@----.---");
    return strdup(string);
}

char* string_to_codigoIATA (char* string) {
    if (string == NULL || strlen(string) != 3) return strdup("---");
    for (int i = 0; i < 3; i++) {
        if (string[i] < 'A' || string[i] > 'Z') return strdup("---");
    }
    return strdup(string);
}

Estado string_to_Estado (char *string) {
    if (!string) return ESTADO_ERROR;
    if (strcmp(string, "On_Time") == 0) return ESTADO_ON_TIME;
    if (strcmp(string, "Delayed") == 0) return ESTADO_DELAYED;
    if (strcmp(string, "Cancelled") == 0) return ESTADO_CANCELLED;
    return ESTADO_ERROR;
}

Tipo_aeroporto valida_tipo (char *string) {
    if (!string) return TIPO_ERROR;
    if (strcmp(string, "small_airport") == 0) return TIPO_SMALL_AIRPORT;
    if (strcmp(string, "medium_airport") == 0) return TIPO_MEDIUM_AIRPORT;
    if (strcmp(string, "large_airport") == 0) return TIPO_LARGE_AIRPORT;
    if (strcmp(string, "heliport") == 0) return TIPO_HELIPORT;
    if (strcmp(string, "seaplane_base") == 0) return TIPO_SEAPLANE_BASE;
    return TIPO_ERROR;
}

bool string_to_bool (char *string, int versao) {
    if (!string) return false;
    if (strcmp(string, "true") == 0) return true;
    if (strcmp(string, "false") == 0) return false;
    fprintf(stderr, "Aviso: valor inválido para booleano (%s)\n", string);
    (void)versao; // evitar warning de parâmetro não usado
    return false;
}

Genero string_to_genero (char* string) {
    if (!string || strlen(string) != 1) return GENERO_ERROR;
    if (strcmp(string, "M") == 0) return GENERO_M;
    if (strcmp(string, "F") == 0) return GENERO_F;
    if (strcmp(string, "O") == 0) return GENERO_O;
    return GENERO_ERROR;
}

char* string_to_id_voo (char* string) {
    if (!string || strlen(string) != 7) return strdup("00-----");
    for (int i = 0; i < 2; i++)
        if (string[i] < 'A' || string[i] > 'Z') return strdup("00-----");
    for (int i = 2; i < 7; i++)
        if (string[i] < '0' || string[i] > '9') return strdup("00-----");
    return strdup(string);
}

void imprimir_voo(gpointer key, gpointer value, gpointer user_data) {
    (void)user_data; // evitar warning
    char *voo_id = (char *) key;
    Voo *v = (Voo *) value;

    printf("Voo ID: %s\n", voo_id);
    printf("Origem: %s | Destino: %s\n", v->codigo_IATA_aer_origem, v->codigo_IATA_aer_destino);
    printf("Companhia: %s | Estado: %s\n", v->companhia_aerea, v->estado);
    printf("----------\n");
}

// Este ficheiro contém apenas funções auxiliares de leitura e validação.
// O main() está em src/main.c
