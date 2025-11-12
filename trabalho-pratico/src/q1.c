#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <ctype.h> //para funcoes isupper e isalpha
#include <stdlib.h>
#include <glib.h>
#include "q1.h"
#include "errors.h"

//função auxiliar para libertar memória de um aeroporto
//static -> recomendado para modularidade e encapsulamento (torna a função visível só dentro do ficheiro)
//        -> utilizado para funções auxiliares
void libertaAeroporto(void *data) {
    Aeroporto *a = data;
    if (!a) return;
    g_free(a->code_IATA);
    g_free(a->name);
    g_free(a->city);
    g_free(a->country);
    g_free(a->code_ICAO); //++
//    g_free(a->type);
    g_free(a);
}

// --------- validações auxiliares no mesmo estilo ---------

//verifica se o tipo do aeroporto é válido
//(small_airport, medium_airport, large_airport, heliport, seaplane_base, closed_airport)
static int tipoValido(const char *type) {
    if (!type) return 0;
    return (strcmp(type, "small_airport")  == 0) ||
           (strcmp(type, "medium_airport") == 0) ||
           (strcmp(type, "large_airport")  == 0) ||
           (strcmp(type, "heliport")       == 0) ||
           (strcmp(type, "seaplane_base")  == 0) ||
           (strcmp(type, "closed_airport") == 0);
}

//verifica se uma string é vazia ou só tem espaços
static int stringVaziaOuEspacos(const char *s) {
    if (!s) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

// --------------------------------------------------------------------

//carrega aeroportos de um ficheiro CSV para uma GHashTable
GHashTable* carregarAeroportos(const char *caminhoFicheiro) {
    FILE *f = fopen(caminhoFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro de aeroportos");
        exit(EXIT_FAILURE);
    }

    //chave: code, valor: Aeroporto*
    GHashTable *tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeroporto);

    char *linha = NULL;
    size_t tamanho = 0;

    //ignora cabeçalho
    ssize_t nread = getline(&linha, &tamanho, f);
    (void)nread;

    int numeroLinha = 2; //linha real do ficheiro (1 é o cabeçalho)

    while ((nread = getline(&linha, &tamanho, f)) != -1) {
        linha[strcspn(linha, "\n")] = '\0';

        //divide pelos separadores de vírgula
        gchar **campos = g_strsplit(linha, ",", 0);

        //valida número de campos
        if (g_strv_length(campos) < 8) {
            errors_add("airports.csv", numeroLinha, "linha incompleta");
            g_strfreev(campos);
            numeroLinha++;
            continue;
        }

        //remove aspas e espaços
        for (int i = 0; campos[i] != NULL; i++) {
            campos[i] = g_strstrip(campos[i]);
            size_t len = strlen(campos[i]);
            if (len > 0 && campos[i][0] == '"') memmove(campos[i], campos[i] + 1, len - 1);
            if (len > 1 && campos[i][strlen(campos[i]) - 1] == '"') campos[i][strlen(campos[i]) - 1] = '\0';
        }

        const char *code    = campos[0];
        const char *name    = campos[1];
        const char *city    = campos[2];
        const char *country = campos[3];
        const char *type    = campos[7];

        int campos_ok = !(stringVaziaOuEspacos(code) ||
                          stringVaziaOuEspacos(name) ||
                          stringVaziaOuEspacos(city) ||
                          stringVaziaOuEspacos(country) ||
                          stringVaziaOuEspacos(type));

        if (campos_ok && codigoValido(code) && tipoValido(type)) {
            Aeroporto *a = g_new(Aeroporto, 1);
            a->code_IATA = g_strdup(code);
            a->name    = g_strdup(name);
            a->city    = g_strdup(city);
            a->country = g_strdup(country);
            a->type    = atoi(type); //g_strdup
            g_hash_table_insert(tabela, g_strdup(a->code_IATA), a);
        } else {
            //mensagem de erro mais informativa
            if (!codigoValido(code))
                errors_add("airports.csv", numeroLinha, "código inválido");
            else if (!tipoValido(type))
                errors_add("airports.csv", numeroLinha, "tipo inválido");
            else
                errors_add("airports.csv", numeroLinha, "campos vazios");
        }

        g_strfreev(campos);
        numeroLinha++;
    }

    free(linha);
    fclose(f);
    return tabela;
}

//verifica se o código do aeroporto é válido (3 letras maiúsculas)
int codigoValido(const char *codigo) {
    if (!codigo || strlen(codigo) != 3) return 0;
    for (int i = 0; i < 3; i++) {
        if (!isupper((unsigned char)codigo[i]) || !isalpha((unsigned char)codigo[i])) return 0;
    }
    return 1;
}

//query 1 (dado um código de aeroporto, procura-o na tabela e imprime as suas informações)
void query1(const char *code, GHashTable *tabelaAeroportos, FILE *out) {
    if (!codigoValido(code)) {
        fprintf(out, "\n");
        return;
    }

    Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, code);
    if (a != NULL) {
        fprintf(out, "%s;%s;%s;%s;%d\n",
                a->code_IATA, a->name, a->city, a->country, a->type);
    } else {
        fprintf(out, "\n");
    }
}
