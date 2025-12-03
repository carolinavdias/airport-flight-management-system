#ifndef UTILS_H
#define UTILS_H

#include <ctype.h>
#include <stdlib.h>

//funcoes auxiliares usadas em varias partes do projeto
static inline int is_digits(const char *s) {
    if (s == NULL) return 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if (!isdigit(s[i])) return 0;
    }
    return 1;
}

#endif