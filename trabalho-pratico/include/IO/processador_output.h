#ifndef PROCESSADOR_OUTPUT_H
#define PROCESSADOR_OUTPUT_H

#include <stdio.h>

/**
 * @file processador_output.h
 * @brief Módulo responsável pela criação e escrita de ficheiros de erro CSV.
 *
 * Este módulo fornece uma pequena abstração para gerar ficheiros
 * de erros por entidade (flights_errors.csv, passengers_errors.csv, etc.).
 *
 * Funcionalidades:
 *  - criar ficheiros de erro com header opcional;
 *  - escrever linhas inválidas no ficheiro;
 *  - fechar e libertar a estrutura associada.
 */

/**
 * @brief Estrutura opaca usada para gerir um ficheiro de erros.
 *
 * Contém internamente:
 *  - ponteiro para o ficheiro CSV;
 *  - flag que indica se o header já foi escrito.
 *
 * O conteúdo interno é privado ao módulo.
 */

typedef struct ErrorWriter ErrorWriter;

/**
 * @brief Cria um ficheiro CSV de erros para uma entidade.
 *
 * O ficheiro é criado dentro da pasta `resultados/` com o nome:
 *     <entity_name>_errors.csv
 *
 * Se for fornecido um header, este é escrito automaticamente.
 *
 * @param entity_name Nome da entidade (ex: "flights", "passengers").
 * @param header Header CSV a escrever no ficheiro (ou NULL).
 * @return Ponteiro para ErrorWriter, ou NULL em caso de erro.
 */

ErrorWriter* processador_output_create_error_file(const char *entity_name, const char *header);

/**
 * @brief Escreve uma linha de erro no ficheiro CSV.
 *
 * A função adiciona automaticamente uma quebra de linha.
 *
 * @param writer Estrutura ErrorWriter previamente criada.
 * @param line Linha a escrever no ficheiro.
 */

void processador_output_write_error(ErrorWriter *writer, const char *line);

/**
 * @brief Fecha o ficheiro de erros e liberta a estrutura associada.
 *
 * Deve ser chamada no final do processamento de cada entidade.
 *
 * @param writer Estrutura ErrorWriter a fechar.
 */

void processador_output_close_error_file(ErrorWriter *writer);

#endif