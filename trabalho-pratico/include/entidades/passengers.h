#ifndef PASSENGERS_H
#define PASSENGERS_H

#include <stdint.h>

typedef uint8_t Genero;

#define GENERO_F 0
#define GENERO_M 1
#define GENERO_O 2

typedef struct passageiros Passageiros;

int passenger_get_id (Passageiros *p);

void passenger_set_id (Passageiros *p, char *id);
void passenger_set_pn (Passageiros *p, char *pn);
void passenger_set_un (Passageiros *p, char *un);
void passenger_set_dn (Passageiros *p, char *dn);
void passenger_set_nc (Passageiros *p, char *nc);
void passanger_set_ge (Passageiros *p, char *ge);
void passenger_set_em (Passageiros *p, char *em);
void passenger_set_tl (Passageiros *p, char *tl);
void passenger_set_md (Passageiros *p, char *md);
void passenger_set_ft (Passageiros *p, char *ft);

Passageiros *criaPassageiro();
void libertaPassageiro(void *data);

#endif
