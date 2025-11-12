#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <ctype.h> //para funcoes isupper e isdigit
#include <stdlib.h> //para atoi e atof
#include <glib.h>
#include "q2.h"
#include "errors.h"

//função auxiliar para libertar memória de uma aeronave
void libertaAeronave(void *data) {
    Aeronave *a = data;
    if (!a) return;
    g_free(a->identifier);
    g_free(a->manufacturer);
    g_free(a->model);
    g_free(a);
}

// --------- validações auxiliares no mesmo estilo ---------

//verifica se uma string é vazia ou só tem espaços
static int stringVaziaOuEspacos(const char *s) {
    if (!s) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

//verifica se ano de fabrico é válido (>= 1900 e <= 2025)
static int anoValido(int ano) {
    return (ano >= 1900 && ano <= 2025);
}

//verifica se capacidade e alcance são valores positivos
static int positivo(float n) {
    return (n > 0);
}

// --------------------------------------------------------------------

//carrega aeronaves de um ficheiro CSV para uma GHashTable
GHashTable* carregarAeronaves(const char *caminhoFicheiro) {
    FILE *f = fopen(caminhoFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro de aeronaves");
        exit(EXIT_FAILURE);
    }

    //cria HashTable (chave: identifier, valor: Aeronave*)
    GHashTable *tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeronave);

    char *linha = NULL;
    size_t tamanho = 0;

    //ignora cabeçalho
    ssize_t nread = getline(&linha, &tamanho, f);
    (void)nread;

    int numeroLinha = 2; //linha real do ficheiro (1 é o cabeçalho)

    while (getline(&linha, &tamanho, f) != -1) {
        linha[strcspn(linha, "\n")] = '\0';

        //divide a linha em campos separados por vírgulas
        gchar **campos = g_strsplit(linha, ",", 0);

        if (g_strv_length(campos) != 6) {
            errors_add("aircrafts.csv", numeroLinha, "linha incompleta");
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

        const char *identifier   = campos[0];
        const char *manufacturer = campos[1];
        const char *model        = campos[2];
        const char *year_str     = campos[3];
        const char *capacity_str = campos[4];
        const char *range_str    = campos[5];

        int campos_ok = !(stringVaziaOuEspacos(identifier) ||
                          stringVaziaOuEspacos(manufacturer) ||
                          stringVaziaOuEspacos(model) ||
                          stringVaziaOuEspacos(year_str) ||
                          stringVaziaOuEspacos(capacity_str) ||
                          stringVaziaOuEspacos(range_str));

        int id_ok  = identificadorValido(identifier);
        int ano    = atoi(year_str);
        int cap    = atoi(capacity_str);
        float ran  = atof(range_str);
        int ano_ok = anoValido(ano);
        int cap_ok = positivo(cap);
        int ran_ok = positivo(ran);

        if (campos_ok && id_ok && ano_ok && cap_ok && ran_ok) {
            Aeronave *a = g_new(Aeronave, 1);
            a->identifier   = g_strdup(identifier);
            a->manufacturer = g_strdup(manufacturer);
            a->model        = g_strdup(model);
            a->year         = ano;
            a->capacity     = cap;
            a->range        = ran;
            g_hash_table_insert(tabela, g_strdup(a->identifier), a);
        } else {
            if (!campos_ok)  errors_add("aircrafts.csv", numeroLinha, "campos vazios");
            else if (!id_ok) errors_add("aircrafts.csv", numeroLinha, "identifier inválido");
            else if (!ano_ok) errors_add("aircrafts.csv", numeroLinha, "ano inválido");
            else if (!cap_ok) errors_add("aircrafts.csv", numeroLinha, "capacity inválida");
            else if (!ran_ok) errors_add("aircrafts.csv", numeroLinha, "range inválido");
        }

        g_strfreev(campos);
        numeroLinha++;
    }

    free(linha);
    fclose(f);
    return tabela;
}

//verifica se o identificador da aeronave é válido (AA-1234)
int identificadorValido(const char *id) {
    if (!id || strlen(id) != 7) return 0;
    if (!isupper((unsigned char)id[0]) || !isupper((unsigned char)id[1])) return 0;
    if (id[2] != '-') return 0;
    for (int i = 3; i < 7; i++) {
        if (!isdigit((unsigned char)id[i])) return 0;
    }
    return 1;
}

//função auxiliar (comparação para ordenação)
int comparaContagens(const Contagem *a, const Contagem *b) {
    if (a->count != b->count)
        return b->count - a->count; //maior primeiro
    return g_strcmp0(a->identifier, b->identifier); //em caso de empate
}

//query 2 (top N aeronaves de um determinado fabricante com mais voos realizados)
void query2(const char *linhaComando, GHashTable *tabelaAeronaves, FILE *out) {
    int n;
    char fabricante[100] = "";
    if (sscanf(linhaComando, "%d %99s", &n, fabricante) < 1) {
        fprintf(out, "\n");
        return;
    }

    //cria lista com todos os elementos da hash table
    GList *lista = g_hash_table_get_values(tabelaAeronaves);

    //filtra por fabricante se especificado
    GList *filtrada = NULL;
    for (GList *l = lista; l != NULL; l = l->next) {
        Aeronave *a = l->data;
        if (strlen(fabricante) == 0 || g_strcmp0(a->manufacturer, fabricante) == 0) {
            Contagem *c = g_new(Contagem, 1);
            c->identifier = g_strdup(a->identifier);
            c->manufacturer = g_strdup(a->manufacturer);
            c->model = g_strdup(a->model);
            c->count = 1; //fase 1 ainda sem voos reais associados
            filtrada = g_list_prepend(filtrada, c);
        }
    }

    //ordena por count e depois por identifier
    filtrada = g_list_sort(filtrada, (GCompareFunc)comparaContagens);

    //imprime top N
    int printed = 0;
    for (GList *l = filtrada; l != NULL && printed < n; l = l->next, printed++) {
        Contagem *c = l->data;
        fprintf(out, "%s;%s;%s;%d\n", c->identifier, c->manufacturer, c->model, c->count);
    }

    //liberta memória
    for (GList *l = filtrada; l != NULL; l = l->next) {
        Contagem *c = l->data;
        g_free(c->identifier);
        g_free(c->manufacturer);
        g_free(c->model);
        g_free(c);
    }
    g_list_free(filtrada);
    g_list_free(lista);
}
