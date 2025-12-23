#ifndef PASSENGERS_H
#define PASSENGERS_H

#include <stdint.h>

typedef struct passageiros Passageiros;

int passenger_get_id (Passageiros *p);
char *passenger_get_primeiro (Passageiros *p);
char *passenger_get_ultimo (Passageiros *p);
int passenger_get_data (Passageiros *p);
char *passenger_get_nacionalidade (Passageiros *p);

void passenger_set_id (Passageiros *p, char *id);
void passenger_set_pn (Passageiros *p, char *pn);
void passenger_set_un (Passageiros *p, char *un);
void passenger_set_dn (Passageiros *p, char *dn);
void passenger_set_nc (Passageiros *p, char *nc);

Passageiros *criaPassageiro();
void libertaPassageiro(void *data);

#endif