/*#define _POSIX_C_SOURCE 200809L

#include "parsers/parser_reservations.h"
#include "entidades/reservations.h"
#include "validacoes/validacoes_reservations.h"
#include "validacoes/validacoes_passengers.h"
#include "utils/utils.h"
#include <glib.h>

int le_tabela_Reservas(Contexto *ctx, GestorFlights *V, GestorPassengers *P, GestorReservations *R) {

    int MAX_LINHA = 2000;
    gchar buffer[MAX_LINHA];

    FILE *ficheiro = abrir_ficheiro(ctx, "reservations.csv", "r");
    if (ficheiro == NULL) return 0;

    int no_header = 1;
    char header[MAX_LINHA];

    if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
    else {
        buffer[strcspn(buffer,"\n")] = '\0';
        strcpy(header, buffer);
    }

    FILE *ficheiro_erros = NULL;
    int header_escrito = 0;

    while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
        Reservas *reserva_atual = criaReserva();

        int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

	//ID_RESERVA
        if (linha_valida && valida_id_reserva(campos[0])) r_set_id_reserva(reserva_atual,campos[0]);
	else linha_valida = 0;

	//VOOS_RESERVADOS
        if (linha_valida && !valida_set_voos_reservados(campos[1],reserva_atual)) linha_valida = 0;

	//ID_PASSAGEIRO_RESERVOU
        if (linha_valida && valida_id_passageiro(campos[2])) r_set_id_pessoa_reservou(reserva_atual,campos[2]);
	else linha_valida = 0;

	//LUGAR RESERVADO
        if (linha_valida) r_set_lugar(reserva_atual,campos[3]);

	//PRECO RESERVA
	if (linha_valida) r_set_preco(reserva_atual,campos[4]);

	//BAGAGEM EXTRA
        if (linha_valida && valida_bool(campos[5])) r_set_bools(reserva_atual,campos[5],1);
	else linha_valida = 0;

	//PRIORIDADE
        if (linha_valida && valida_bool(campos[6])) r_set_bools(reserva_atual,campos[6],2);
	else linha_valida = 0;

	//QR CODE
        if (linha_valida) r_set_qr_code(reserva_atual,campos[7]);

        //validação lógica (usa gestores V e P)
        if (linha_valida && !valida_RESERVA(reserva_atual, V, P)) linha_valida = 0;

        if (!linha_valida) {
             if (!ficheiro_erros) {
                 ficheiro_erros = fopen("resultados/reservations_errors.csv", "w");
                 if (ficheiro_erros && !header_escrito) {
                 fputs(header, ficheiro_erros);
                 fputc('\n', ficheiro_erros);
                 header_escrito = 1;
             }
        }
        if (ficheiro_erros) {
             fputs(buffer, ficheiro_erros);
             fputc('\n', ficheiro_erros);
        }
        libertaReserva(reserva_atual);
        } else {
            gestor_reservations_insere(R, reserva_atual);
        }
        if (campos) csv_free_fields(campos, n_campos);
    }

    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}
*/
