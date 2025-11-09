#include <stdio.h>
#include <string.h>
#include <ctype.h> //para funcoes isupper e isdigit
#include <glib.h>
#include "q2.h"

//função auxiliar para libertar memória de uma aeronave
static void libertaAeronave(void *data) {
    Aeronave *a = data;
    if (!a) return;
    g_free(a->identifier);
    g_free(a->manufacturer);
    g_free(a->model);
    g_free(a);
}

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
    size_t tamanho = 0; //size_t -> número inteiro sem sinal

    //ignora designação dos dados (1ª linha -> "identifier","manufacturer","model","year","capacity","range")
    getline(&linha, &tamanho, f);

    while (getline(&linha, &tamanho, f) != -1) {
        linha[strcspn(linha, "\n")] = '\0'; // remove '\n'

        //divide a linha em campos separados por ';'
        gchar **campos = g_strsplit(linha, ";", 0); //gchar -> char definido pela glib
        if (g_strv_length(campos) == 6) {
            Aeronave *a = g_new(Aeronave, 1);
            a->identifier = g_strdup(campos[0]);
            a->manufacturer = g_strdup(campos[1]);
            a->model = g_strdup(campos[2]);
            a->year = atoi(campos[3]);
            a->capacity = atoi(campos[4]);
            a->range = atof(campos[5]);
            g_hash_table_insert(tabela, g_strdup(a->identifier), a);
        }
        g_strfreev(campos);
    }

    free(linha);
    fclose(f);
    return tabela;
}

//verifica se o identificador da aeronave é válido 
int identificadorValido(const char *id) {
    if (strlen(id) != 8) return 0;
    if (!isupper((unsigned char)id[0]) || !isupper((unsigned char)id[1])) return 0;
    if (id[2] != '-') return 0;
    for (int i = 3; i < 8; i++) {
        if (!isupper((unsigned char)id[i]) && !isdigit((unsigned char)id[i])) return 0;
    }
    return 1;
}

//função auxiliar (contagem de voos de uma aeronave)
static void contagem(GHashTable *contagens, const char *identifier) {
    gpointer valor = g_hash_table_lookup(contagens, identifier);
    if (valor) {
        int count = GPOINTER_TO_INT(valor) + 1;
        g_hash_table_replace(contagens, g_strdup(identifier), GINT_TO_POINTER(count));
    } else {
        g_hash_table_insert(contagens, g_strdup(identifier), GINT_TO_POINTER(1));
    }
}

//query 2 (top N aeronaves com mais voos realizados)
void query2(const char *linhaComando, GHashTable *tabelaAeronaves, FILE *out) {
    int n;
    char fabricante[100] = "";
    if (sscanf(linhaComando, "%d %99s", &n, fabricante) < 1) {
        fprintf(out, "\n");
        return;
    }

    FILE *f = fopen("dataset-fase-1/aircrafts.csv", "r");
    if (!f) {
        perror("Erro ao abrir aircrafts.csv");
        fprintf(out, "\n");
        return;
    }

    GHashTable *contagens = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, NULL);

    char *linha = NULL;
    size_t tamanho = 0;

    getline(&linha, &tamanho, f); //explicado anteriormente

    while (getline(&linha, &tamanho, f) != -1) {
        linha[strcspn(linha, "\n")] = '\0';
        gchar **campos = g_strsplit(linha, ";", 0);

        //assumindo ordem dos campos no ficheiro aircrafts
        if (g_strv_length(campos) > 7) {
            const char *aircraft_id = campos[1];
            const char *status = campos[7];

            if (g_strcmp0(status, "Cancelled") != 0)
                contagem(contagens, aircraft_id);
        }
        g_strfreev(campos);
    }

    fclose(f);
    free(linha);

    //cria lista de aeronaves com contagem
    GList *lista = NULL;
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        const char *id = key;
        Aeronave *a = g_hash_table_lookup(tabelaAeronaves, id);
        if (!a) continue;
        if (strlen(fabricante) > 0 && g_strcmp0(a->manufacturer, fabricante) != 0)
            continue;

        Contagem *c = g_new(Contagem, 1);
        c->identifier = g_strdup(id);
        c->manufacturer = g_strdup(a->manufacturer);
        c->model = g_strdup(a->model);
        c->count = GPOINTER_TO_INT(value);
        lista = g_list_prepend(lista, c);
    }

    //ordena por número de voos decrescente e por identifier crescente
    lista = g_list_sort(lista, (GCompareFunc)comparaContagens);

    //imprime top N
    int printed = 0;
    for (GList *l = lista; l != NULL && printed < n; l = l->next, printed++) {
        Contagem *c = l->data;
        fprintf(out, "%s;%s;%s;%d\n", c->identifier, c->manufacturer, c->model, c->count);
    }

    //liberta memória
    for (GList *l = lista; l != NULL; l = l->next) {
        Contagem *c = l->data;
        g_free(c->identifier);
        g_free(c->manufacturer);
        g_free(c->model);
        g_free(c);
    }
    g_list_free(lista);
    g_hash_table_destroy(contagens);
}

//função auxiliar (ordenação)
int comparaContagens(const Contagem *a, const Contagem *b) {
    if (a->count != b->count)
        return b->count - a->count; //maior primeiro
    return g_strcmp0(a->identifier, b->identifier); //empate -> ordem alfabética
}