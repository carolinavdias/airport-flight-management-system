#include "parsers/parser.h"

#include <stdio.h>
#include <glib.h>

#include "parsers/parser_flights.h"
#include "parsers/parser_airports.h"
#include "parsers/parser_aircrafts.h"
#include "parsers/parser_passengers.h"
#include "parsers/parser_reservations.h"

int* read1 (Contexto *ctx, GestorFlights *V, GestorAirports *AP, GestorAircrafts *AC, GestorPassengers *P, GestorReservations *R) {

    gchar *caminhoAeroportos = g_build_filename(get_contexto(ctx), "airports.csv", NULL);
    gchar *caminhoAeronaves  = g_build_filename(get_contexto(ctx), "aircrafts.csv", NULL);
    gchar *caminhoVoos       = g_build_filename(get_contexto(ctx), "flights.csv", NULL);
    gchar *caminhoPassageiros= g_build_filename(get_contexto(ctx), "passengers.csv", NULL);
    gchar *caminhoReservas   = g_build_filename(get_contexto(ctx), "reservations.csv", NULL);

    int le_1 = 0, le_2 = 0, le_3 = 0, le_4 = 0, le_5 = 0 ;

    //carrega dados
    if (g_file_test(caminhoAeronaves, G_FILE_TEST_EXISTS)) {
        le_3 = le_tabela_Aeronaves(ctx,AC);
    }

    if (g_file_test(caminhoVoos, G_FILE_TEST_EXISTS) && le_3) {
        le_1 = le_tabela_Voos (ctx, V, AC);
    }

    if (g_file_test(caminhoAeroportos, G_FILE_TEST_EXISTS)) {
        le_2 = le_tabela_Aeroportos(ctx,AP);
    }
    if (g_file_test(caminhoPassageiros, G_FILE_TEST_EXISTS)) {
        le_4 = le_tabela_Passageiros(ctx,P);
    }
    if (g_file_test(caminhoReservas, G_FILE_TEST_EXISTS) && le_1 && le_4) {
        le_5 = le_tabela_Reservas(ctx,V,P,R);
    }

    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);
    g_free(caminhoVoos);
    g_free(caminhoPassageiros);
    g_free(caminhoReservas);

    int *resultados_read = malloc (5 * sizeof(int));
    resultados_read[0] = le_1;
    resultados_read[1] = le_2;
    resultados_read[2] = le_3;
    resultados_read[3] = le_4;
    resultados_read[4] = le_5;

    return resultados_read;
}