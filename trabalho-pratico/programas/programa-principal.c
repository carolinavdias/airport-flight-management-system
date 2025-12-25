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

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Contexto *ctx = cria_contexto();
    set_contexto(ctx, argv[1]);

    errors_begin();
    g_mkdir_with_parents("resultados", 0755);

    // Criar GESTORES
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

    int *read_check = read1(ctx, gestorVoos, gestorAeroportos, gestorAeronaves, gestorPassageiros, gestorReservas);

    // Inicializar arrays ordenados para queries otimizadas
    query2_init(gestorVoos);

    FILE *ficheiroComandos = fopen(argv[2], "r");
    if (!ficheiroComandos) {
        perror("Erro ao abrir o ficheiro de comandos");
        errors_write_csv("resultados/errors.csv");
        errors_end();
        query2_cleanup();
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
            case 1: {
                if (param && read_check[1]) {
                    char *resultado = query1(param, gestorAeroportos, gestorVoos);
                    if (resultado) {
                        fprintf(out, "%s", resultado);
                        free(resultado);
                    } else {
                        fprintf(out, "\n");
                    }
                } else {
                    fprintf(out, "\n");
                }
                break;
            }

            case 2: {
                if (param && read_check[2] && read_check[0]) {
                    char *resultado = query2(param, gestorAeronaves, gestorVoos);
                    if (resultado) {
                        fprintf(out, "%s", resultado);
                        free(resultado);
                    } else {
                        fprintf(out, "\n");
                    }
                } else {
                    fprintf(out, "\n");
                }
                break;
            }

            case 3: {
                char d1[16], d2[16], data_inicio[32], data_fim[32];
                if (param && (sscanf(param, "%31s %31s", d1, d2) == 2) && read_check[1] && read_check[0]) {
                    sprintf(data_inicio, "%s 00:00", d1);
                    sprintf(data_fim, "%s 23:59", d2);
                    char *resultado = query3(data_inicio, data_fim, gestorVoos, gestorAeroportos);
                    if (resultado) {
                        fprintf(out, "%s", resultado);
                        free(resultado);
                    } else {
                        fprintf(out, "\n");
                    }
                } else {
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

    free(linha);
    fclose(ficheiroComandos);

    errors_write_csv("resultados/errors.csv");
    errors_end();

    // Limpar queries
    query2_cleanup();

    // Libertar memória
    gestor_aircrafts_liberta(gestorAeronaves);
    gestor_flights_destroy(gestorVoos);
    gestor_airports_liberta(gestorAeroportos);
    gestor_passengers_destroy(gestorPassageiros);
    gestor_reservations_liberta(gestorReservas);

    return EXIT_SUCCESS;
}