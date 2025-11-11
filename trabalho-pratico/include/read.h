#ifndef READ_H
#define READ_H

#include <glib.h>
#include <stdbool.h>

//estrutura para data (YYYY-MM-DD)
typedef struct data_def {
    int ano, mes, dia;
} Data;

//estrutura para hora (HH:MM)
typedef struct hora_def {
    int hora, mins;
} Hora;

//estrutura completa de data + hora
typedef struct dataH {
    Data data;
    Hora horas;
} DataH;

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

//tipo de aeroporto
typedef enum {
    TIPO_SMALL_AIRPORT,
    TIPO_MEDIUM_AIRPORT,
    TIPO_LARGE_AIRPORT,
    TIPO_HELIPORT,
    TIPO_SEAPLANE_BASE,
    TIPO_ERROR
} Tipo_aeroporto;

//funções de leitura e validação
Data parse_Data(char *string);
DataH parse_DataH(char *string);
char* string_to_email(char *string);
char* string_to_codigoIATA(char *string);
char* string_to_id_voo(char *string);
Genero string_to_genero(char *string);
Tipo_aeroporto valida_tipo(char *string);
Estado string_to_Estado(char *string);
bool string_to_bool(char *string, int versao);

//funções de carga (ficheiros CSV)
GHashTable* carregarVoos(const char *caminho);
GHashTable* carregarAeroportos(const char *caminho);
GHashTable* carregarAeronaves(const char *caminho);

#endif
