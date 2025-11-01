#include <stdio.h>
#include <string.h>
#include <ctype.h> //para funcoes isupper e isalpha
#include <glib.h>
#include "q1.h"

// função auxiliar para libertar memória de um aeroporto
//static -> recomendado para modularidade e encapsulamento (torna a função visível só dentro do ficheiro)
static void free_aeroporto(void *data) {
    Aeroporto *a = data;
    g_free(a->code);
    g_free(a->name);
    g_free(a->city);
    g_free(a->country);
    g_free(a->type);
    g_free(a);
}

//carrega aeroportos de um ficheiro CSV para uma GHashTable
GHashTable* carregarAeroportos(const char *caminhoFicheiro) {
    FILE *f = fopen(caminhoFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro de aeroportos");
        exit(EXIT_FAILURE);
    }

    // chave: code, valor: Aeroporto*
    GHashTable *tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_aeroporto);

    char linha[512];
    fgets(linha, sizeof(linha), f); //lê uma linha e guarda na string

    while (fgets(linha, sizeof(linha), f)) {
        linha[strcspn(linha, "\n")] = '\0';

        char code[10], name[80], city[50], country[50], type[30];
        // code;name;city;country;latitude;longitude;icao;type
        if (sscanf(linha, "%9[^;];%79[^;];%49[^;];%49[^;];%*[^;];%*[^;];%*[^;];%29[^\n]",
                   code, name, city, country, type) == 5) {

            if (codigoValido(code)) {
                Aeroporto *a = g_new(Aeroporto, 1);
                a->code = g_strdup(code);
                a->name = g_strdup(name);
                a->city = g_strdup(city);
                a->country = g_strdup(country);
                a->type = g_strdup(type);

                g_hash_table_insert(tabela, g_strdup(a->code), a);
            }
        }
    }

    fclose(f);
    return tabela;
}

//verifica se o código é válido (3 letras maiúsculas)
int codigoValido(const char *codigo) {
    if (strlen(codigo) != 3) return 0;
    for (int i = 0; i < 3; i++) {
        if (!isupper(codigo[i]) || !isalpha(codigo[i])) return 0;
    }
    return 1;
}

//query 1 usando glib
void query1(const char *code, GHashTable *tabelaAeroportos, FILE *out) {
    if (!codigoValido(code)) {
        fprintf(out, "\n");
        return;
    }

    Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, code);
    if (a != NULL) {
        fprintf(out, "%s;%s;%s;%s;%s\n",
                a->code, a->name, a->city, a->country, a->type);
    } else {
        fprintf(out, "\n");
    }
}