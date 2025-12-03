

typedef struct aeroporto {
    char *code_IATA; //codigo_IATA_aer
    char *name; //name_aeroporto
    char *city; //cidade_aeroporto
    char *country; //pais_aeroporto
    double latitude; //latitude ++
    double longitude; //longitude ++
    char *code_ICAO; //codigo_ICAO_aeroporto ++
    // char *type; 
    Tipo_aeroporto type; // ++
} Aeroporto;