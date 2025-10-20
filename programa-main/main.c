#include <stdio.h>

typedef struct voos {
    int voo_id;
    DataH partida_prevista; //A criar DataHora struct
    DataH partida_efetiva;
    DataH chegada_prevista;
    DataH chegada_efetiva;
    int porta_embarque;
    char* estado; //A desenvolver struct
    char* codigo_IATA_aer_origem;
    char* codigo_IATA_aer_destino;
    int id_aeronave;
    char* companhia_area;
    char* tracking_url;
} Voo;


typedef struct aeroporto {
    char* codigo_IATA_aer;
    char* nome_aer;
    char* cidada_aer;
    char* pais_aer;
    double latitude; //latitude do aeroporto em graus decimais
    double longitude;
    char* codigo_ICAO_aer;
    char* tipo; //Militar, publico e privado
} Aeroporto;


typedef struct aeronave {
    id_aeronave;
    char* fabricante_nave;
    char* modelo_nave;
    int ano_fabricacao;
    int capacidade_max_pessoas;
    int alcance_max; // (em km)
} Aeronave;



typedef struct passageiros {
    char* id_pessoa;
    char* primeiro_nome;
    char* ultimo nome;
    Data data_nascimento;
    char* nacionalidade;
    char* genero_pessoa;
    char* email_pessoa;
    int telefone_pessoa;
    char* morada_pessoa;
    //fotografia_pessoa (int);
} Passageiros;


typedef struct reservas {
    int id_reserva;
    int voos_reservados[10]; //limite de voos reservados a definir
    char* id_pessoa_reservou; //reserva em nome de
    char* lugar_reservado;
    double preco_reserva;
    bool bagagem_extra;
    bool prioridade; //priority boarding
    //QR CODE associado à reserva (int);
} Reservas;


