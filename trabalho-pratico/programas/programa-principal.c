#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "queries/q1.h"
#include "queries/q2.h"
#include "queries/q3.h"

#include "parsers/parser.h"
#include "parsers/parser_flights.h"
#include "parsers/parser_airports.h"
#include "parsers/parser_aircrafts.h"
#include "parsers/parser_passengers.h"
#include "parsers/parser_reservations.h"

#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

#include "utils/utils.h"
#include "errors.h"

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Contexto *ctx = cria_contexto();
    set_contexto (ctx, argv[1]);
//    strncpy(ctx.dataset_dir, argv[1], sizeof(ctx.dataset_dir)-1);

    errors_begin();
    g_mkdir_with_parents("resultados", 0755);

    //criar GESTORES (não hash tables!)
    GestorAircrafts *gestorAeronaves = gestor_aircrafts_cria();
    GestorFlights *gestorVoos = gestor_flights_novo();
    GestorAirports *gestorAeroportos = gestor_airports_cria();
    GestorPassengers *gestorPassageiros = gestor_passengers_novo();
    GestorReservations *gestorReservas = gestor_reservations_cria();



    if (!gestorAeroportos || !gestorAeronaves || !gestorVoos || !gestorPassageiros || !gestorReservas) {
        errors_write_csv("resultados/errors.csv");
        errors_end();
        if (gestorAeronaves) gestor_aircrafts_liberta(gestorAeronaves);
        if (gestorVoos) gestor_flights_destroy(gestorVoos);
        if (gestorAeroportos) gestor_airports_liberta(gestorAeroportos);
        if (gestorPassageiros) gestor_passengers_destroy(gestorPassageiros);
        if (gestorReservas) gestor_reservations_liberta(gestorReservas);
        return EXIT_FAILURE;
    }

    int *read_check = read(ctx,gestorVoos, gestorAeroportos, gestorAeronaves, gestorPassageiros, gestorReservas);


/*
    gchar *caminhoAeroportos = g_build_filename(ctx.dataset_dir, "airports.csv", NULL);
    gchar *caminhoAeronaves  = g_build_filename(ctx.dataset_dir, "aircrafts.csv", NULL);
    gchar *caminhoVoos       = g_build_filename(ctx.dataset_dir, "flights.csv", NULL);
    gchar *caminhoPassageiros= g_build_filename(ctx.dataset_dir, "passengers.csv", NULL);
    gchar *caminhoReservas   = g_build_filename(ctx.dataset_dir, "reservations.csv", NULL);


//    printf ("build file name.\n");


    int le_1 = 0, le_2 = 0, le_3 = 0;

    errors_begin();
    g_mkdir_with_parents("resultados", 0755);

    //criar GESTORES (não hash tables!)
    GestorAircrafts *gestorAeronaves = gestor_aircrafts_cria();
    GestorFlights *gestorVoos = gestor_flights_novo();
    GestorAirports *gestorAeroportos = gestor_airports_cria();
    GestorPassengers *gestorPassageiros = gestor_passengers_novo();
    GestorReservations *gestorReservas = gestor_reservations_cria();


  //  printf ("criar ghashtables.\n");

    //carregar dados usando parsers
    if (g_file_test(caminhoAeronaves, G_FILE_TEST_EXISTS)) {
        le_3 = le_tabela_Aeronaves(ctx, gestorAeronaves);
    }
  //  printf ("aeronaves.\n");

    if (g_file_test(caminhoAeronaves, G_FILE_TEST_EXISTS) &&
        g_file_test(caminhoVoos, G_FILE_TEST_EXISTS)) {
        le_1 = le_tabela_Voos(ctx, gestorVoos, gestorAeronaves);
    }

   // printf ("Voos.\n");

    if (g_file_test(caminhoAeroportos, G_FILE_TEST_EXISTS)) {
//	printf ("Aeroportos.\n");
        le_2 = le_tabela_Aeroportos(ctx, gestorAeroportos);
    }

   // printf ("Aeroportos2.\n");
    
    if (g_file_test(caminhoPassageiros, G_FILE_TEST_EXISTS)) {
        le_tabela_Passageiros(ctx, gestorPassageiros);
    }
    
   // printf ("passageiros.\n");

    if (g_file_test(caminhoReservas, G_FILE_TEST_EXISTS) &&
        g_file_test(caminhoVoos, G_FILE_TEST_EXISTS) &&
        g_file_test(caminhoPassageiros, G_FILE_TEST_EXISTS)) {
        le_tabela_Reservas(ctx, gestorVoos, gestorPassageiros, gestorReservas);
    }


    g_free(caminhoAeroportos);
    g_free(caminhoAeronaves);
    g_free(caminhoVoos);
    g_free(caminhoPassageiros);
    g_free(caminhoReservas);


    if (!gestorAeroportos || !gestorAeronaves || !gestorVoos || !gestorPassageiros || !gestorReservas) {
        errors_write_csv("resultados/errors.csv");
        errors_end();
        if (gestorAeronaves) gestor_aircrafts_liberta(gestorAeronaves);
        if (gestorVoos) gestor_flights_destroy(gestorVoos);
        if (gestorAeroportos) gestor_airports_liberta(gestorAeroportos);
        if (gestorPassageiros) gestor_passengers_destroy(gestorPassageiros);
        if (gestorReservas) gestor_reservations_liberta(gestorReservas);
        return EXIT_FAILURE;
    }

*/
    //obter hash tables dos gestores para as queries

//    GHashTable *tabelaAeronaves = gestor_aircrafts_table(gestorAeronaves);
//    GHashTable *tabelaVoos = gestor_flights_table(gestorVoos);
//    GHashTable *tabelaAeroportos = gestor_airports_table(gestorAeroportos);


    //GHashTable *tabelaPassageiros = gestorPassageiros->tabela_passageiros;
    //GHashTable *tabelaReservas = gestor_reservations_table(gestorReservas);
//(tabelas ainda nao utilizadas nesta fase)


    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        errors_write_csv("resultados/errors.csv");
        errors_end();
        gestor_aircrafts_liberta(gestorAeronaves);
        gestor_flights_destroy(gestorVoos);
        gestor_airports_liberta(gestorAeroportos);
        gestor_passengers_destroy(gestorPassageiros);
        gestor_reservations_liberta(gestorReservas);
        return EXIT_FAILURE;
    }

    char *linha = NULL;
    size_t tamanho = 0;
    int numeroComando = 1;

    while (getline(&linha, &tamanho, ficheiroComandos) != -1) {
        linha[strcspn(linha, "\n")] = '\0';

        int idQuery = 0;
        char *param = NULL;

        if (sscanf(linha, "%d", &idQuery) == 1) {
            char *espaco = strchr(linha, ' ');
            if (espaco != NULL)
                param = espaco + 1;
        }

        gchar *nomeOutput = g_strdup_printf("resultados/command%d_output.txt", numeroComando);
        FILE *out = fopen(nomeOutput, "w");
        if (!out) {
            perror("Erro ao criar ficheiro de resultado");
            g_free(nomeOutput);
            numeroComando++;
            continue;
        }

        switch (idQuery) {
            case 1:
//		printf ("q1.\n");
                if (param && read_check[1])
                    query1(param, gestor_airports_table(gestorAeroportos) , out);
                else
                    fprintf(out, "\n");
                break;

            case 2:
//                printf ("q2.\n");
                if (param && read_check[2] && read_check[0])
                    query2(param, gestor_aircrafts_table(gestorAeronaves) , gestor_flights_table(gestorVoos) , out);
                else
                    fprintf(out, "\n");
                break;

            case 3: {
//                printf ("q3.\n");
                char d1[16], d2[16], data_inicio[32], data_fim[32];
                if (param && (sscanf(param, "%31s %31s", d1, d2) == 2) && read_check[1] && read_check[0]) {
                    sprintf(data_inicio, "%s 00:00", d1);
                    sprintf(data_fim, "%s 23:59", d2);
                    query3(data_inicio, data_fim, gestor_flights_table(gestorVoos) , gestor_airports_table(gestorAeroportos) , out);
                } else {
//		    printf ("aqui.\n");
                    fprintf(out, "\n");
                }
                break;
            }

            default:
                fprintf(out, "\n");
                break;
        }

        fclose(out);
        g_free(nomeOutput);
        numeroComando++;
    }

//printf ("Acabou queries.\n");
    free(linha);
    fclose(ficheiroComandos);

    errors_write_csv("resultados/errors.csv");
    errors_end();

//printf ("tudo bem.\n");
    //libertar memória
    gestor_aircrafts_liberta(gestorAeronaves);
  //  printf ("aeronaves destroi.\n");
    gestor_flights_destroy(gestorVoos);
//	printf ("voos_destroi.\n");
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    gestor_airports_liberta(gestorAeroportos);
  //      printf ("aeroportos_destroi.\n");

    gestor_passengers_destroy(gestorPassageiros);
  //      printf ("passageiros_destroi.\n");

    gestor_reservations_liberta(gestorReservas);
    //    printf ("reserv_destroi.\n");

//printf ("Fim.\n");
    return EXIT_SUCCESS;
}
