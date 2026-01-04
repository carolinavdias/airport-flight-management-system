#ifndef PARSER_H
#define PARSER_H

#include "utils/utils.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * @file parser.h
 * @brief Leitura e processamento dos ficheiros CSV do dataset.
 *
 * Este módulo é responsável por:
 *  - Ler todos os ficheiros CSV do dataset
 *  - Validar cada linha usando o módulo de validações
 *  - Inserir entidades válidas nos respetivos gestores
 *  - Criar ficheiros de erro para linhas inválidas
 *  - Construir caches e estruturas auxiliares usadas pelas queries
 *
 * O parser executa apenas uma vez no início do programa,
 * permitindo que as queries sejam executadas de forma otimizada.
 */

/**
 * @brief Lê e processa todos os ficheiros CSV do dataset.
 *
 * A função lê os ficheiros:
 *  - aircrafts.csv
 *  - flights.csv
 *  - airports.csv
 *  - passengers.csv
 *  - reservations.csv
 *
 * Para cada ficheiro:
 *  - Ignora o header
 *  - Valida cada linha
 *  - Insere entidades válidas nos gestores
 *  - Regista linhas inválidas em ficheiros *_errors.csv
 *
 * Durante o parsing são também construídos vários caches:
 *  - Q1: contagem de partidas e chegadas por aeroporto
 *  - Q2: número de voos por aeronave
 *  - Q3: array de voos ordenado por data
 *  - Q4: top 10 de gastos semanais por passageiro
 *  - Q5: atrasos médios por companhia aérea
 *  - Q6: destinos mais comuns por nacionalidade
 *
 * @param ctx Contexto com o caminho base do dataset
 * @param V Gestor de voos
 * @param AP Gestor de aeroportos
 * @param AC Gestor de aeronaves
 * @param P Gestor de passageiros
 * @param R Gestor de reservas
 *
 * @return Array de inteiros (size 6):
 *         - índice 0: sucesso global
 *         - índices 1..5: sucesso individual de cada CSV
 *           (1 = sucesso, 0 = erro)
 *           O array deve ser libertado com free().
 */

int* read_csv (Contexto *ctx, GestorFlights *V, GestorAirports *AP, 
               GestorAircrafts *AC, GestorPassengers *P, GestorReservations *R);

#endif  
