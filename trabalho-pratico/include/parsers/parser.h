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
 * 
 * O parser deve ser chamado apenas uma vez antes das queries.
 */

/**
 * @brief Lê e processa todos os ficheiros CSV do dataset e constrói caches de queries.
 *
 * A função segue uma ordem lógica de dependência (ex: aeronaves antes de voos).
 * Realiza a validação de cada linha e, em caso de erro, regista a linha original 
 * num ficheiro de erros correspondente em "resultados/".
 *
 * Durante o processamento, são preenchidas estruturas auxiliares para otimizar:
 * - Q1: Contagens de passageiros por aeroporto.
 * - Q2: Frequência de uso de aeronaves (via GHashTable).
 * - Q3: Array de voos ordenado cronologicamente.
 * - Q4: Gastos semanais por passageiro.
 * - Q5: Atrasos médios por companhia.
 * - Q6: Destinos populares por nacionalidade.
 *
 * @param ctx   Contexto contendo o caminho para a diretoria do dataset.
 * @param V     Gestor de Voos (Flights) a ser populado.
 * @param AP    Gestor de Aeroportos (Airports) a ser populado.
 * @param AC    Gestor de Aeronaves (Aircrafts) a ser populado.
 * @param P     Gestor de Passageiros (Passengers) a ser populado.
 * @param R     Gestor de Reservas (Reservations) a ser populado.
 * @param pool  StringPool para armazenamento eficiente de strings repetidas.
 *
 * @return Um array de inteiros (int*) com 6 posições, alocado dinamicamente:
 * - [0]: Sucesso global (1 se todos os ficheiros foram processados, 0 caso contrário).
 * - [1]: Sucesso do ficheiro aircrafts.csv.
 * - [2]: Sucesso do ficheiro airports.csv.
 * - [3]: Sucesso do ficheiro flights.csv.
 * - [4]: Sucesso do ficheiro passengers.csv.
 * - [5]: Sucesso do ficheiro reservations.csv.
 */

int* read_csv (Contexto *ctx, GestorFlights *V, GestorAirports *AP, 
               GestorAircrafts *AC, GestorPassengers *P, GestorReservations *R, StringPool *pool);

#endif  
