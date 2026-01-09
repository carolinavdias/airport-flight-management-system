#include "IO/processador_input_interativo.h"
#include "utils/utils.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FOR_INPUT 1024

/* ============================================================
 * FUNÇÕES INTERNAS
 * ============================================================ */

/**
 * Remove o newline final de uma string.
 */

static void trim_newline(char *s) {
    if (!s) return;
    size_t len = strlen(s);
    if (len && s[len - 1] == '\n')
        s[len - 1] = '\0';
}

/**
 * Processa um comando individual.
 */

static int processar_comando(GestorGeral *gestor, char *input) {
    (void)gestor; // TODO: usar para chamar queries
    if (!input || !*input) return 1;

    /* comandos de saída */
    if (strcmp(input, "quit") == 0 || strcmp(input, "exit") == 0) {
        printf("A sair do modo interativo...\n");
        return 0;
    }

    /* separar comando e argumentos */
    char *cmd = strtok(input, " ");
    char *args = strtok(NULL, "");

    if (!cmd) return 1;

    if (strcmp(cmd, "Q1") == 0) {
        // exemplo
        printf("[INFO] Query 1 chamada com args: %s\n", args ? args : "(nenhum)");
        // chama executor_query1(gestor, args);
    }
    else if (strcmp(cmd, "Q2") == 0) {
        printf("[INFO] Query 2 chamada com args: %s\n", args ? args : "(nenhum)");
        // executor_query2(gestor, args);
    }
    else {
        printf("Comando desconhecido: %s\n", cmd);
    }

    return 1;
}

/* ============================================================
 * API PÚBLICA
 * ============================================================ */

/**
 * Inicia o ciclo principal do modo interativo.
 */

void modo_interativo(GestorGeral *gestor) {
    char buffer[MAX_FOR_INPUT];

    printf("Modo interativo iniciado.\n");
    printf("Escreva 'quit' ou 'exit' para terminar.\n\n");

    while (1) {
        printf("> ");
        if (!fgets(buffer, sizeof(buffer), stdin))
            break;

        trim_newline(buffer);

        if (!processar_comando(gestor, buffer))
            break;
    }
}
