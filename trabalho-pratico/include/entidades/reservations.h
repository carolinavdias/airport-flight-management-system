#ifndef RESERVATIONS_H
#define RESERVATIONS_H

typedef struct voos_reservados Voos_reservados;

typedef struct reservas Reservas;

/* ============================================
 * GETTERS EXISTENTES (não alterar assinaturas)
 * ============================================ */

char *r_get_id_reserva (Reservas *r);
int r_get_id_pessoa_reservou (Reservas *r);  //retorna int
int r_get_lista_n_voos (Reservas *r);
char **r_get_lista_voos_reserv (Reservas *r);

/* ============================================
 * NOVOS GETTERS PARA FASE 2 (encapsulamento)
 * ============================================ */

/**
 * @brief Obtém o preço da reserva
 * @param r Ponteiro para a reserva
 * @return Preço da reserva (double)
 */
double r_get_preco(Reservas *r);

/**
 * @brief Obtém o ID de um voo da reserva por índice (encapsulado)
 * @param r Ponteiro para a reserva
 * @param indice Índice do voo (0 ou 1)
 * @return Cópia do ID do voo (libertar com g_free!) ou NULL se inválido
 */
char *r_get_voo_por_indice(Reservas *r, int indice);

/* ============================================
 * SETTERS (sem alterações)
 * ============================================ */

Voos_reservados *cria0_lista_reserva (int n);

void set_lista_voos (Voos_reservados *vr, int i, char *s);
void r_set_lista (Reservas *r, Voos_reservados *novo);

void r_set_id_reserva (Reservas *r, char *s);
void r_set_id_pessoa_reservou (Reservas *r, char *s);
void r_set_lugar (Reservas *r, char *s);
void r_set_preco (Reservas *r, char *s);
void r_set_bools (Reservas *r, char *s, int versao);
void r_set_qr_code (Reservas *r, char *s);

/* ============================================
 * CRIA E DESTROI (sem alterações)
 * ============================================ */

Reservas *criaReserva ();
void libertaReserva(void *data);

#endif