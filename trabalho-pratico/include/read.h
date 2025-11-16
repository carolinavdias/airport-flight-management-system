#ifndef READ_H
#define READ_H

#include <glib.h>
#include <stdbool.h>
#include <stdint.h>

typedef struct {
    char dataset_dir[256];
} Contexto;

typedef struct data_def {
    int ano, mes, dia;
} Data;

typedef enum {
    GENERO_F,
    GENERO_M,
    GENERO_O,
    GENERO_ERROR
} Genero;

typedef struct voos_reservados {
    char **lista_voos_reservados;
    int n_voos;
} Voos_reservados;

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
    char *fotografia_passageiro;
} Passageiros;

typedef struct reservas {
    char *id_reserva;
    Voos_reservados reserva_lista;
    int id_pessoa_reservou;
    char *lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade;
    char *qr_code;
} Reservas;

typedef struct hora_def {
    int hora, mins;
} Hora;

typedef struct dataH {
    Data data;
    Hora horas;
} DataH;

// Funções de libertação de memória
void libertaAeronave(void *data);
void libertaAeroporto(void *data);
void libertaVoo(void *data);
void libertaPassageiro(void *data);
void libertaReserva(void *data);

// Função de leitura
int le_tabela(int opcao_inserida, Contexto ctx, GHashTable *tabela1, GHashTable *tabela2, GHashTable *tabela3, GHashTable *tabela4, GHashTable *tabela5);

#endif