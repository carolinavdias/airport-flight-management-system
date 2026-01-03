#ifndef PROCESSADOR_INPUT_INTERATIVO_H
#define PROCESSADOR_INPUT_INTERATIVO_H

#include "gestor_entidades/gestor.h"

/**
 * @file processador_input_interativo.h
 * @brief Processador de comandos do modo interativo.
 *
 * Este módulo é responsável por:
 *  - ler input do utilizador;
 *  - interpretar comandos;
 *  - validar sintaticamente os argumentos;
 *  - despachar a execução para as queries apropriadas.
 *
 * Não executa lógica de negócio nem acede diretamente às entidades.
 */

/**
 * @brief Inicia o ciclo principal do modo interativo.
 *
 * O ciclo termina quando o utilizador escreve "quit" ou "exit".
 *
 * @param gestor Gestor geral do sistema.
 */
void modo_interativo(GestorGeral *gestor);

#endif
