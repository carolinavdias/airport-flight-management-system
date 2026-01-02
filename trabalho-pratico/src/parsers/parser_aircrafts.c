/*#define _POSIX_C_SOURCE 200809L

#include "parsers/parser_aircrafts.h"
#include "entidades/aircrafts.h"
#include "validacoes/validacoes_aircrafts.h"
#include "utils/utils.h"
#include <string.h>
#include <stdlib.h>
#include <glib.h>

#define _POSIX_C_SOURCE 200809L

#include "parsers/parser_aircrafts.h"
#include "entidades/aircrafts.h"
#include "validacoes/validacoes_aircrafts.h"
#include "utils/utils.h"
#include <string.h>
#include <stdlib.h>
#include <glib.h>

int le_tabela_Aeronaves(Contexto *ctx, GestorAircrafts *AC) {

    int MAX_LINHA = 2000;
    gchar buffer[MAX_LINHA];

    FILE *ficheiro = abrir_ficheiro(ctx, "aircrafts.csv", "r");
    if (!ficheiro) return 0;

    // Ler header
    if (!fgets(buffer, sizeof(buffer), ficheiro))
        return 0;

    // Processar linhas
    while (fgets(buffer, sizeof(buffer), ficheiro)) {

        buffer[strcspn(buffer, "\n")] = '\0';   // remover newline

        // Criar aeronave (a função já valida e envia para errors.csv)
        Aeronave *a = criaAeronave(buffer);

        if (a != NULL) {
            // Linha válida → inserir no gestor
            gestor_aircrafts_insere(AC, a);
        }
        // Se for NULL, a função criaAeronave já tratou do erro
    }

    fclose(ficheiro);
    return 1;
}
*/
