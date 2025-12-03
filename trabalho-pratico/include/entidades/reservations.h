#ifndef RESERVATIONS_H
#define RESERVATIONS_H

typedef struct voos_reservados {
    char **lista_voos_reservados;
    int n_voos;
} Voos_reservados;

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

#endif