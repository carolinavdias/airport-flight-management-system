#ifndef AIRPORTS_H
#define AIRPORTS_H

typedef enum {
    TIPO_SMALL_AIRPORT,
    TIPO_MEDIUM_AIRPORT,
    TIPO_LARGE_AIRPORT,
    TIPO_HELIPORT,
    TIPO_SEAPLANE_BASE,
    TIPO_CLOSED_AIRPORT,
    TIPO_ERROR
} Tipo_aeroporto;

typedef struct aeroporto {
    char *code_IATA; //codigo_IATA_aer
    char *name; //name_aeroporto
    char *city; //cidade_aeroporto
    char *country; //pais_aeroporto
    double latitude; //latitude ++
    double longitude; //longitude ++
    char *code_ICAO; //codigo_ICAO_aeroporto ++
    Tipo_aeroporto type; // ++
} Aeroporto;

void libertaAeroporto(void *data);

#endif