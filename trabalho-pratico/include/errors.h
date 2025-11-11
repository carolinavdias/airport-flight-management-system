#ifndef ERRORS_H
#define ERRORS_H

//API do módulo de erros (lista global interna)
// iniciar/limpar a lista global de erros
void errors_begin(void);

// adicionar um erro (ficheiro, nº da linha, mensagem)
void errors_add(const char *ficheiro, int linha, const char *mensagem);

// escrever todos os erros num CSV (cabeçalho: file;line;message)
void errors_write_csv(const char *caminho);

// libertar memória (esvaziar lista global)
void errors_end(void);

#endif
