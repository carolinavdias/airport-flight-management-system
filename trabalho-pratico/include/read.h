#ifndef READ_H
#define READ_H

typedef struct {
    char dataset_dir[256];
} Contexto;

#endif

//deixar read.h apenas com a def de Contexto uma vez que esta a ser usado em todos os parsers, prog-prinicpal,etc...!