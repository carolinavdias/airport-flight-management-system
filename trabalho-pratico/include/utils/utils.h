#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stddef.h>

typedef struct contexto Contexto;
Contexto *cria_contexto ();
char *get_contexto (Contexto *ctx);
void set_contexto (Contexto *ctx, char *d);

FILE* abrir_ficheiro(Contexto *ctx, const char *nome_ficheiro, const char *modo);
int qual_mes (int ano, int mes);

//===================================
//ERRORS
//===================================

//API do módulo de erros (lista global interna)
// iniciar/limpar a lista global de erros
void errors_begin(void);

// adicionar um erro (ficheiro, nº da linha, mensagem)
void errors_add(const char *ficheiro, int linha, const char *mensagem);

// escrever todos os erros num CSV (cabeçalho: file;line;message)
void errors_write_csv(const char *caminho);

// libertar memória (esvaziar lista global)
void errors_end(void);

//===================================
//CSV
//===================================

// divide linha CSV em campos
int csv_split(const char *line, char ***fields, size_t *count);

// liberta campos alocados
void csv_free_fields(char **fields, size_t count);

#endif