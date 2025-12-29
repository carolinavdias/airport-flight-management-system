#ifndef RESERVATIONS_H
#define RESERVATIONS_H

typedef struct voos_reservados Voos_reservados;

typedef struct reservas Reservas;


//int get_lista_n_voos (Voos_reservados *lvr);
char *r_get_id_reserva (Reservas *r);
int r_get_id_pessoa_reservou (Reservas *r);  //retorna int
int r_get_lista_n_voos (Reservas *r);
char **r_get_lista_voos_reserv (Reservas *r);

//void cria0_lista_voos (Voos_reservados *vr, int n);
Voos_reservados *cria0_lista_reserva (int n);

void set_lista_voos (Voos_reservados *vr, int i, char *s);
void r_set_lista (Reservas *r, Voos_reservados *novo);

void r_set_id_reserva (Reservas *r, char *s);
void r_set_id_pessoa_reservou (Reservas *r, char *s);
void r_set_lugar (Reservas *r, char *s);
void r_set_preco (Reservas *r, char *s);
void r_set_bools (Reservas *r, char *s, int versao);
void r_set_qr_code (Reservas *r, char *s);

Reservas *criaReserva ();
//Voos_reservados *cria_voos_reservados ();
void libertaReserva(void *data);

#endif