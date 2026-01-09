#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>

#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"
#include "gestor_entidades/gestor.h"

#include "utils/utils.h"
#include "parsers/parser.h"
#include "queries/interpreter.h"

/**
 * Este módulo:
 *  - inicializa gestores e caches
 *  - lê o dataset completo
 *  - processa o ficheiro de comandos
 *  - escreve os resultados em ficheiros individuais
 *  - gera o ficheiro de erros
 */

/**
 * Função auxiliar para processar linha de comando.
 */

static void processa_linha_comando(const char *linha_completa, 
                                   FILE *out,
                                   GestorAirports *gestorAeroportos,
                                   GestorFlights *gestorVoos,
                                   GestorAircrafts *gestorAeronaves,
                                   GestorPassengers *gestorPassageiros,
                                   GestorReservations *gestorReservas)
{
    if (!linha_completa || !linha_completa[0]) {
        fprintf(out, "\n");
        return;
    }

    char *linha = g_strdup(linha_completa);
    char *comando = linha;
    char *param = NULL;

    char *espaco = strchr(linha, ' ');
    if (espaco) {
        *espaco = '\0';
        param = espaco + 1;
    }

    // AGORA PASSAMOS gestorReservas AO INTERPRETADOR
    interpreta_comando(comando, param, out, 
                       gestorAeroportos, gestorVoos, gestorAeronaves,
                       gestorPassageiros, gestorReservas);

    g_free(linha);
}

/**
 * Função main.
 */

int main(int argc, char **argv) {

    if (argc != 3) {
        fprintf(stderr, "Uso: %s <dataset-fase-1> <ficheiro_comandos>\n", argv[0]);
        return EXIT_FAILURE;
    }

    Contexto *ctx = cria_contexto();
    if (!ctx) return EXIT_FAILURE;
    
    set_contexto(ctx, argv[1]);

    errors_begin();
    g_mkdir_with_parents("resultados", 0755);

    StringPool *pool = cria_string_pool();
    if (!pool) return EXIT_FAILURE;

    // Criar GESTORES
    GestorAircrafts *gestorAeronaves = gestor_aircrafts_cria();
    GestorFlights *gestorVoos = gestor_flights_novo();
    GestorAirports *gestorAeroportos = gestor_airports_cria();
    GestorPassengers *gestorPassageiros = gestor_passengers_novo();
    GestorReservations *gestorReservas = gestor_reservations_cria();   // <-- NECESSÁRIO
    gestor_reservations_init_cache_q4(gestorReservas);  // Cache para Q4
    gestor_flights_init_cache_q5(gestorVoos);  // Cache para Q5
    gestor_passengers_init_cache_q6(gestorPassageiros);  // Cache para Q6

    if (!gestorAeroportos || !gestorAeronaves || !gestorVoos ||
        !gestorPassageiros || !gestorReservas)
    {
        errors_write_csv("resultados/errors.csv");
        errors_end();
        if (gestorAeronaves) gestor_aircrafts_liberta(gestorAeronaves);
        if (gestorVoos) gestor_flights_destroy(gestorVoos);
        if (gestorAeroportos) gestor_airports_liberta(gestorAeroportos);
        if (gestorPassageiros) gestor_passengers_destroy(gestorPassageiros);
        if (gestorReservas) gestor_reservations_liberta(gestorReservas);
        return EXIT_FAILURE;
    }
    // Carregar dados (agora incluindo reservas)
    int *resultados_read = read_csv(ctx, gestorVoos, gestorAeroportos, gestorAeronaves,
         gestorPassageiros, gestorReservas, pool);
    gestor_reservations_finaliza_cache_q4(gestorReservas);  // Pre-calcular top 10 para Q4
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
        free(ctx);
        return EXIT_FAILURE;
    }

    char *linha = NULL;
    size_t tamanho = 0;
    int numeroComando = 1;

    while (getline(&linha, &tamanho, ficheiroComandos) != -1) {
        linha[strcspn(linha, "\n")] = '\0';
        if (linha[0] == '\0') continue;

        gchar *nomeOutput = g_strdup_printf("resultados/command%d_output.txt", numeroComando);
        FILE *out = fopen(nomeOutput, "w");
        if (!out) {
            perror("Erro ao criar ficheiro de resultado");
            g_free(nomeOutput);
            numeroComando++;
            continue;
        }

        // AGORA PASSAMOS gestorReservas
        processa_linha_comando(linha, out,
                               gestorAeroportos, gestorVoos,
                               gestorAeronaves, gestorPassageiros,
                               gestorReservas);

        fclose(out);
        g_free(nomeOutput);
        numeroComando++;
    }

    free(resultados_read);
    free(linha);
    fclose(ficheiroComandos);

    errors_write_csv("resultados/errors.csv");
    errors_end();

    // Libertar memória
    gestor_aircrafts_liberta(gestorAeronaves);
    gestor_flights_destroy(gestorVoos);
    gestor_airports_liberta(gestorAeroportos);
    gestor_passengers_destroy(gestorPassageiros);
    gestor_reservations_liberta(gestorReservas);
    
    free(ctx);
    string_pool_destroy(pool);

    return EXIT_SUCCESS;
}
