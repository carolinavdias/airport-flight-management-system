#ifndef Q2_H
#define Q2_H

#include <glib.h>
#include "gestor_entidades/gestor_aircrafts.h"  

// Inicializa
void query2_init(GHashTable *tabelaVoos);

// Limpeza
void query2_cleanup(void);

// Query 2 - ASSINATURA ATUALIZADA
char *query2(const char *linhaComando, GestorAircrafts *gestorAeronaves, GHashTable *tabelaVoos);

#endif