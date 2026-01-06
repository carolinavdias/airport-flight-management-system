#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stddef.h>
#include <glib.h>

/**
 * @file utils.h
 * @brief Funções utilitárias globais usadas por vários módulos do sistema.
 *
 * Este módulo agrega funcionalidades genéricas como:
 *  - gestão do contexto do dataset;
 *  - operações auxiliares com datas;
 *  - parsing de CSV;
 *  - registo e escrita de erros.
 *
 * Nenhuma função aqui depende de entidades específicas do domínio.
 */

/* ============================================================
 * CONTEXTO DO DATASET
 * ============================================================ */

/**
 * @struct contexto
 * @brief Estrutura que guarda informação global do programa.
 *
 * Contém apenas o diretório do dataset,
 */

typedef struct contexto Contexto;

/**
 * @brief Cria e inicializa um contexto vazio.
 *
 * @return Ponteiro para um novo Contexto.
 */

Contexto *cria_contexto ();

/**
 * @brief Obtém o diretório do dataset associado ao contexto.
 *
 * @param ctx Contexto.
 * @return String com o caminho do dataset.
 */

char *get_contexto (Contexto *ctx);

/**
 * @brief Define o diretório do dataset no contexto.
 *
 * @param ctx Contexto.
 * @param d Caminho para o diretório do dataset.
 */

void set_contexto (Contexto *ctx, const char *d);

/**
 * @brief Abre um ficheiro dentro do diretório do dataset.
 *
 * Constrói o caminho completo com base no contexto.
 *
 * @param ctx Contexto.
 * @param nome_ficheiro Nome do ficheiro.
 * @param modo Modo de abertura (ex: "r").
 * @return Ponteiro para FILE ou NULL em erro.
 */

FILE* abrir_ficheiro(Contexto *ctx, const char *nome_ficheiro, const char *modo);

/**
 * @brief Devolve o número de dias de um mês.
 *
 * Considera anos bissextos.
 *
 * @param ano Ano.
 * @param mes Mês (1–12).
 * @return Número de dias do mês.
 */

int qual_mes (int ano, int mes);

typedef struct _StringPool StringPool;

StringPool *cria_string_pool(void);

const char *string_pool_get(StringPool *pool, const char *s);

void string_pool_clear(StringPool *pool);

void string_pool_destroy(StringPool *pool);

/* ============================================================
 * GESTÃO DE ERROS
 * ============================================================ */

/**
 * @brief Inicializa/limpa a lista global de erros.
 */

void errors_begin(void);

/**
 * @brief Adiciona um erro à lista global.
 *
 * @param ficheiro Nome do ficheiro onde ocorreu o erro.
 * @param linha Número da linha.
 * @param mensagem Mensagem de erro.
 */

void errors_add(const char *ficheiro, int linha, const char *mensagem);

/**
 * @brief Escreve todos os erros num ficheiro CSV.
 *
 * O ficheiro terá o cabeçalho: file;line;message
 *
 * @param caminho Caminho do ficheiro CSV de saída.
 */

void errors_write_csv(const char *caminho);


/**
 * @brief Liberta toda a memória associada à lista de erros.
 */

void errors_end(void);

/* ============================================================
 * CSV
 * ============================================================ */

/**
 * @brief Divide uma linha CSV em campos individuais.
 *
 * Suporta campos com aspas e vírgulas internas.
 *
 * @param line Linha CSV.
 * @param fields Ponteiro para o array de campos (alocado).
 * @param count Número de campos encontrados.
 * @return 0 em sucesso, -1 em erro.
 */

int csv_split(const char *line, char ***fields, size_t *count);

/**
 * @brief Liberta a memória alocada por csv_split.
 *
 * @param fields Array de campos.
 * @param count Número de campos.
 */

void csv_free_fields(char **fields, size_t count);

#endif
