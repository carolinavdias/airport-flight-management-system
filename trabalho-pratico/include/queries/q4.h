#ifndef Q4_H
#define Q4_H

#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_reservations.h"

/**
 * @brief Query 4 - Passageiro mais frequente no top 10 de gastos semanais
 * 
 * Calcula o top 10 de passageiros com mais gastos (soma dos preços das
 * reservas) por semana (domingo a sábado). Retorna o passageiro que mais
 * vezes apareceu no top 10.
 * 
 * Conforme enunciado:
 * - A semana é determinada pelo campo departure (partida estimada) do voo
 * - Se o filtro captar uma semana parcialmente, essa semana também conta
 * - Em caso de empate, prevalece o menor document_number
 * - O enunciado NÃO menciona exclusão de voos cancelados (diferente de Q1/Q3/Q6)
 * 
 * @param linhaComando Argumentos opcionais: begin_date end_date (YYYY-MM-DD)
 * @param gestorPassageiros Gestor de passageiros
 * @param gestorVoos Gestor de voos
 * @param gestorReservas Gestor de reservas
 * @return String com resultado (formato com vírgulas, interpreter converte)
 *         document_number,first_name,last_name,dob,nationality,count_top_10
 *         Libertar com free()!
 */
char *query4(const char *linhaComando,
             GestorPassengers *gestorPassageiros,
//             GestorFlights *gestorVoos,
             GestorReservations *gestorReservas);

#endif
