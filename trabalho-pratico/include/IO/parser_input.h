#ifndef PARSER_INPUT_H
#define PARSER_INPUT_H

#include <stddef.h>

/**
 * @file parser_input.h
 * @brief Funções utilitárias para leitura de ficheiros CSV.
 *
 * Este módulo fornece funções genéricas para:
 *  - ler o header de um ficheiro CSV;
 *  - percorrer todas as linhas de um CSV e aplicar um callback;
 *  - dividir cada linha em campos e entregar ao módulo correspondente.
 *
 * É usado pelos parsers das entidades (flights, passengers, reservations, etc.).
 */

/**
 * @brief Tipo de função callback chamada para cada linha do CSV.
 *
 * A função callback recebe:
 *  - `fields`: array de strings com os campos já separados;
 *  - `n_fields`: número de campos;
 *  - `user_data`: ponteiro genérico passado pelo utilizador.
 */

typedef void (*LineCallback)(char **fields, size_t n_fields, void *user_data);

/**
 * @brief Lê um ficheiro CSV linha a linha e aplica um callback a cada linha.
 *
 * A função:
 *  - abre o ficheiro no diretório indicado;
 *  - descarta automaticamente o header;
 *  - lê cada linha seguinte;
 *  - divide a linha em campos usando `csv_split`;
 *  - chama o callback fornecido;
 *  - liberta os campos após o callback.
 *
 * @param dataset_dir Diretório onde se encontra o ficheiro.
 * @param filename Nome do ficheiro CSV.
 * @param callback Função a ser chamada para cada linha válida.
 * @param user_data Dados adicionais fornecidos ao callback.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */

int parser_input_read_csv(const char *dataset_dir, const char *filename, LineCallback callback, void *user_data);

/**
 * @brief Lê apenas o header de um ficheiro CSV.
 *
 * Copia a primeira linha do ficheiro (sem newline) para `header_out`.
 *
 * @param dataset_dir Diretório onde se encontra o ficheiro.
 * @param filename Nome do ficheiro CSV.
 * @param header_out Buffer onde o header será armazenado.
 * @param size Tamanho máximo do buffer.
 * @return 1 em caso de sucesso, 0 caso contrário.
 */

int parser_input_read_header(const char *dataset_dir, const char *filename, char *header_out, size_t size);

#endif