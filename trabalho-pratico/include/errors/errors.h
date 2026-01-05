#ifndef ERRORS_H
#define ERRORS_H

/**
 * @file errors.h
 * @brief Funções utilitárias globais usadas por vários módulos do sistema.????????????????????????????
 *
 * Este módulo agrega funcionalidades como:
 *  - registo e escrita de erros.
 *
 * Nenhuma função aqui depende de entidades específicas do domínio.
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
