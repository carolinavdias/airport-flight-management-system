#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include <time.h> //converte srings de datas em valores comparáveis
#include "q3.h"

//função auxiliar para libertar memória de um voo
static void free_voo(void *data) {
    Voo *v = data;
    if (!v) return;
    g_free(v->flight_id);
    g_free(v->departure);
    g_free(v->actual_departure);
    g_free(v->arrival);
    g_free(v->actual_arrival);
    g_free(v->gate);
    g_free(v->status);
    g_free(v->origin);
    g_free(v->destination);
    g_free(v->aircraft);
    g_free(v->airline);
    g_free(v->tracking_url);
    g_free(v);
}

//função auxiliar para remover aspas
static void removeQuotes(char *str) {
    size_t len = strlen(str);
    if (len > 0 && str[0] == '"') memmove(str, str + 1, len - 1);
    if (len > 1 && str[strlen(str) - 1] == '"') str[strlen(str) - 1] = '\0';
}

//carrega voos de um ficheiro CSV para uma GHashTable
GHashTable* carregarVoos(const char *caminhoFicheiro) {
    FILE *f = fopen(caminhoFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro de voos");
        exit(EXIT_FAILURE);
    }

    //cria HashTable (chave: flight_id, valor: Voo*)
    GHashTable *tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, free_voo);

    char *linha = NULL;
    size_t len = 0; //size_t -> número inteiro sem sinal
    ssize_t nread; //quantos caracteres foram lidos (de uma linha) -> ssize_t (tipo de dado inteiro com sinal)

    getline(&linha, &len, f); //ignora designação dos dados (1ª linha)

    //lê linha a linha
    while ((nread = getline(&linha, &len, f)) != -1) {
        linha[strcspn(linha, "\n")] = '\0';

        char *campos[12];
        int i = 0;
        //ponteiro para o inicio (token -> pedaço de uma string)
        char *token = strtok(linha, ","); //devolve primeiro token (biblioteca string.h)
        while (token && i < 12) {
            removeQuotes(token);
            campos[i++] = token;
            token = strtok(NULL, ","); //devolve token seguinte
        }

        if (i == 12) {
            Voo *v = g_new(Voo, 1);
            v->flight_id        = g_strdup(campos[0]);
            v->departure         = g_strdup(campos[1]);
            v->actual_departure  = g_strdup(campos[2]);
            v->arrival           = g_strdup(campos[3]);
            v->actual_arrival    = g_strdup(campos[4]);
            v->gate              = g_strdup(campos[5]);
            v->status            = g_strdup(campos[6]);
            v->origin            = g_strdup(campos[7]);
            v->destination       = g_strdup(campos[8]);
            v->aircraft          = g_strdup(campos[9]);
            v->airline           = g_strdup(campos[10]);
            v->tracking_url      = g_strdup(campos[11]);

            g_hash_table_insert(tabela, g_strdup(v->flight_id), v);
        }
    }

    free(linha);
    fclose(f);
    return tabela;
}

//verifica se o identificador de voo é válido 
int idVooValido(const char *id) {
    if (strlen(id) != 7) return 0; //tem de ter exatamente 7 caracteres
    for (int i = 0; id[i]; i++) {
        if (!isalnum((unsigned char) id[i])) return 0; //letras e números
    }
    return 1;
}

//função auxiliar (converte uma data no formato "YYYY-MM-DD HH:MM:SS")
static time_t parseDate(const char *dateStr) {
    struct tm tm = {0};
    if (strptime(dateStr, "%Y-%m-%d %H:%M:%S", &tm) == NULL) //converte string -> struct tm
        return (time_t) -1;
    return mktime(&tm); //converte struct tm -> time_t
}

//função auxiliar (verifica se uma data está entre 2 limites)
static int dataEntre(const char *data, const char *inicio, const char *fim) {
    time_t t_data = parseDate(data);
    time_t t_inicio = parseDate(inicio);
    time_t t_fim = parseDate(fim);

    if (t_data == (time_t) -1 || t_inicio == (time_t) -1 || t_fim == (time_t) -1)
        return 0;

    return (t_data >= t_inicio && t_data <= t_fim);
}

//query 3 (aeroporto com mais partidas entre 2 datas)
void query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, FILE *out) {
    if (!data_inicio || !data_fim || !tabelaVoos) {
        fprintf(out, "\n");
        return;
    }

    if (parseDate(data_inicio) == (time_t)-1 || parseDate(data_fim) == (time_t)-1) {
        fprintf(out, "\n");
        return;
    }

    GHashTable *contagens = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);

    GHashTableIter iter;
    gpointer key, value; //tipo genérico de ponteiro (glib)
    g_hash_table_iter_init(&iter, tabelaVoos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;
        //usa partida real
        if (dataEntre(v->actual_departure, data_inicio, data_fim)) {
            int *count = g_hash_table_lookup(contagens, v->origin);
            if (count) {
                (*count)++;
            } else {
                int *novo = g_new(int, 1);
                *novo = 1;
                g_hash_table_insert(contagens, g_strdup(v->origin), novo);
            }
        }
    }

    //encontra o aeroporto com mais partidas
    char *aeroportoTop = NULL;
    int maxPartidas = 0;

    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        int *count = value;
        if (*count > maxPartidas) {
            maxPartidas = *count;
            aeroportoTop = key;
        }
    }

    if (aeroportoTop)
        fprintf(out, "%s;%d\n", aeroportoTop, maxPartidas);
    else
        fprintf(out, "\n");

    g_hash_table_destroy(contagens);
}

//VER MAIN E ULTIMAS FUNCOES DESTE FICHEIRO