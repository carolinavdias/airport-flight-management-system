#ifndef READ_H
#define READ_H

#include <glib.h>
#include <stdbool.h>

//estrutura para data (YYYY-MM-DD)
typedef struct data_def {
    int ano, mes, dia;
} Data;

typedef uint8_t Genero;
#define O 0
#define F 1
#define M 2


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

/*
//género
typedef enum {
    GENERO_F,
    GENERO_M,
    GENERO_O,
    GENERO_ERROR
} Genero;


//estado do voo
typedef enum {
    ESTADO_ON_TIME,
    ESTADO_DELAYED,
    ESTADO_CANCELLED,
    ESTADO_ERROR
} Estado;
*/

/*
//tipo de aeroporto
typedef enum {
    TIPO_SMALL_AIRPORT,
    TIPO_MEDIUM_AIRPORT,
    TIPO_LARGE_AIRPORT,
    TIPO_HELIPORT,
    TIPO_SEAPLANE_BASE,
    TIPO_ERROR
} Tipo_aeroporto;
*/




/*
//funções de leitura e validação
Data parse_Data(char *string);
DataH parse_DataH(char *string);
char* string_to_email(char *string);
char* string_to_codigoIATA(char *string);
char* string_to_id_voo(char *string);
Genero string_to_genero(char *string);
//Tipo_aeroporto valida_tipo(char *string);
Estado string_to_Estado(char *string);
bool string_to_bool(char *string, int versao);

//funções de carga (ficheiros CSV)
GHashTable* carregarVoos(const char *caminho);
GHashTable* carregarAeroportos(const char *caminho);
GHashTable* carregarAeronaves(const char *caminho);
*/


void libertaReserva(void *data);
int read (int opcao_inserida, GHashTable *tabela1, GHashTable *tabela2, GHashTable *tabela3, GHashTable *tabela4, GHashTable *tabela5);


#endif
