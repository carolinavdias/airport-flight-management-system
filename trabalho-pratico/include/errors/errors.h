#ifndef ERRORS_H
#define ERRORS_H

/**
 * @file errors.h
 * @brief Módulo de registo e gestão global de erros.
 *
 * Este módulo fornece funções para:
 *  - iniciar e limpar a lista global de erros;
 *  - registar erros ocorridos durante o processamento dos ficheiros;
 *  - exportar os erros para um ficheiro CSV;
 *  - libertar toda a memória associada ao sistema de erros.
 *
 * Não depende de nenhuma entidade específica do domínio.
 */

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


#endif
