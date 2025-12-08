#ifndef AIRPORTS_H
#define AIRPORTS_H

typedef uint8_t Tipo_aeroporto;

#define TIPO_SMALL_AIRPORT 0
#define TIPO_MEDIUM_AIRPORT 1
#define TIPO_LARGE_AIRPORT 2
#define TIPO_HELIPORT 3
#define TIPO_SEAPLANE_BASE 4


typedef struct aeroporto {
    char *code_IATA; //codigo_IATA_aer
    char *name; //name_aeroporto
    char *city; //cidade_aeroporto
    char *country; //pais_aeroporto
    double latitude; //latitude ++
    double longitude; //longitude ++
    char *code_ICAO; //codigo_ICAO_aeroporto ++
    Tipo_aeroporto type; // ++
    int arrival_count;
    int departure_count;
} Aeroporto;

void libertaAeroporto(void *data);

#endif
