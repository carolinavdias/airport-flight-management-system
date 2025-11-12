#ifndef Q1_H
#define Q1_H

#include <stdio.h> //tipo FILE*
#include <glib.h>
#include <stdint.h> //++

//Tipo aeroporto - definição
typedef uint8_t Tipo_aeroporto;

#define SMALL_AIRPORT 0
#define MEDIUM_AIRPORT 1
#define LARGE_AIRPORT 2
#define HELIPORT 3
#define SEAPLANE_BASE 4
// ++

//aeroporto 
typedef struct aeroporto {
    char *code_IATA; //codigo_IATA_aer
    char *name; //name_aeroporto
    char *city; //cidade_aeroporto
    char *country; //pais_aeroporto
    double latitude; //latitude ++
    double longitude; //longitude ++
    char *code_ICAO; //codigo_ICAO_aeroporto ++
    //char *type; --
    Tipo_aeroporto type; // ++
} Aeroporto;

/*typedef struct aeroporto {
    char *codigo_IATA_aer;
    char *nome_aer;
    char *cidade_aer;
    char *pais_aer;
    double latitude;
    double longitude;
    char *codigo_ICAO_aer;
    Tipo_aeroporto tipo;
} Aeroporto;*/


//carrega aeroportos de um ficheiro CSV para uma GHashTable
GHashTable* carregarAeroportos(const char *caminhoFicheiro);

//verifica se o código introduzido é constituído por 3 letras maiúsculas
int codigoValido(const char* codigo);

//query 1 (dado um código, imprime as informações do aeroporto)
void query1(const char *code, GHashTable *tabelaAeroportos, FILE *out);

void libertaAeroporto(void *data);

#endif
