#ifndef READ_H
#define READ_H

#include <glib.h>
#include <stdbool.h>
#include <stdint.h>


typedef struct {
    char dataset_dir[256];
} Contexto;

//estrutura para data (YYYY-MM-DD)
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
    Voos_reservados reserva_lista; //lista
    int id_pessoa_reservou; //reserva em nome de
    char *lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade; //priority boarding
    char *qr_code;
} Reservas;



//estrutura para hora (HH:MM)
typedef struct hora_def {
    int hora, mins;
} Hora;

//estrutura completa de data + hora
typedef struct dataH {
    Data data;
    Hora horas;
} DataH;

// =====================================================
// DECLARAÇÕES DAS FUNÇÕES DE LIBERTAÇÃO
// (Implementadas em read.c)
// =====================================================
void libertaReserva(void *data);
void libertaPassageiro(void *data);
void libertaVoo(void *data);
void libertaAeronave(void *data);
void libertaAeroporto(void *data);

int le_tabela (int opcao_inserida, Contexto ctx, GHashTable *tabela1, GHashTable *tabela2, GHashTable *tabela3, GHashTable *tabela4, GHashTable *tabela5);


#endif