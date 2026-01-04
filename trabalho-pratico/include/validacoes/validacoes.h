#ifndef VALIDACOES_H
#define VALIDACOES_H

/**
 * @file validacoes.h
 * @brief Funções genéricas de validação sintática.
 *
 * Este módulo contém funções de validação reutilizáveis por várias
 * entidades do sistema (voos, aeroportos, passageiros, reservas).
 * As validações aqui definidas não dependem de gestores nem de
 * estruturas de dados específicas.
 */

/* ============================================================
 * VALIDAÇÕES GENÉRICAS
 * ============================================================ */

/**
 * @brief Valida uma data no formato YYYY-MM-DD.
 *
 * Verifica:
 *  - formato fixo;
 *  - caracteres numéricos;
 *  - limites válidos para ano, mês e dia.
 *
 * @param s String da data.
 * @return 1 se válida, 0 caso contrário.
 */

int valida_Data(const char *s);

/**
 * @brief Valida uma data e hora no formato YYYY-MM-DD HH:MM.
 *
 * Verifica:
 *  - formato fixo;
 *  - caracteres numéricos;
 *  - limites válidos para data e hora;
 *  - data não futura (até 2025-09-30).
 *
 * @param s String da data e hora.
 * @return 1 se válida, 0 caso contrário.
 */

int valida_DataH(const char *s);

/**
 * @brief Valida um código IATA.
 *
 * O código deve conter exatamente 3 letras maiúsculas.
 *
 * @param s String do código IATA.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_codigoIATA(const char *s);

/**
 * @brief Valida o identificador de um voo.
 *
 * O ID deve ter o formato:
 *  - 2 letras maiúsculas (A–Z)
 *  - seguidas de 5 dígitos (0–9)
 *
 * @param s String contendo o ID do voo.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_id_voo (const char *s);

/**
 * @brief Valida o identificador do passageiro.
 *
 * O ID deve ter exatamente 9 dígitos numéricos.
 *
 * @param s String contendo o ID.
 * @return 1 se válido, 0 caso contrário.
 */

int valida_id_passageiro(const char *s);

#endif
