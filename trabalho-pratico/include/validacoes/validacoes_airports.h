#ifndef VALIDACOES_AIRPORTS_H
#define VALIDACOES_AIRPORTS_H

int valida_codigoIATA (const char *s);
int valida_coordenadas (const char* s, int versao);
int valida_tipo_aer(const char *s);

#endif