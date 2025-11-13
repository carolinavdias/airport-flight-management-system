#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include <time.h> //converte strings de datas em valores comparáveis
#include "q3.h"
#include "errors.h"

//função auxiliar para libertar memória de um voo
void libertaVoo (void *data) {
    Voo *v = data;
    if (!v) return;
    g_free(v->flight_id);
    g_free(v->departure);
    g_free(v->actual_departure);
    g_free(v->arrival);
    g_free(v->actual_arrival);
    g_free(v->gate);
//    g_free(v->status);
    g_free(v->code_origin);
    g_free(v->code_destination);
    g_free(v->id_aircraft);
    g_free(v->airline);
    g_free(v->tracking_url);
    g_free(v);
}


//verifica se o estado do voo é válido
static int estadoValido(const char *estado) {
    if (!estado) return 0;
    return (strcmp(estado, "On_Time") == 0 ||
            strcmp(estado, "Delayed") == 0 ||
            strcmp(estado, "Cancelled") == 0 ||
            strcmp(estado, "Scheduled") == 0);
}

//verifica se código IATA tem 3 letras maiúsculas
static int codigoIATAValido(const char *codigo) {
    if (!codigo || strlen(codigo) != 3) return 0;
    for (int i = 0; i < 3; i++)
        if (!isupper((unsigned char)codigo[i]) || !isalpha((unsigned char)codigo[i])) return 0;
    return 1;
}

// --------------------------------------------------------------------

//carrega voos de um ficheiro CSV para uma GHashTable
GHashTable* carregarVoos(const char *caminhoFicheiro) {
    FILE *f = fopen(caminhoFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro de voos");
        return NULL;
    }

    GHashTable *tabela = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaVoo);
    char *linha = NULL;
    size_t len = 0;
    ssize_t nread;
    int numeroLinha = 0;

    // ignora cabeçalho
    __attribute__((unused)) ssize_t _unused = getline(&linha, &len, f);


    while ((nread = getline(&linha, &len, f)) != -1) {
        linha[strcspn(linha, "\r\n")] = '\0'; // remove \n e \r

        // separa campos com g_strsplit (GLib)
        gchar **campos = g_strsplit(linha, ",", 12);
        int n_campos = g_strv_length(campos);

        if (n_campos < 12) {
            g_strfreev(campos);
            numeroLinha++;
            continue;
        }

        for (int i = 0; i < n_campos; i++) {
            g_strstrip(campos[i]);
            size_t l = strlen(campos[i]);
            if (l > 0 && campos[i][0] == '"') memmove(campos[i], campos[i] + 1, l - 1);
            if (l > 1 && campos[i][strlen(campos[i]) - 1] == '"') campos[i][strlen(campos[i]) - 1] = '\0';
        }

        if (!idVooValido(campos[0]) ||
            !codigoIATAValido(campos[7]) ||
            !codigoIATAValido(campos[8]) ||
            !estadoValido(campos[6])) {
            errors_add("flights.csv", numeroLinha, "dados inválidos");
            g_strfreev(campos);
            numeroLinha++;
            continue;
        }

        Voo *v = g_new(Voo, 1);
        v->flight_id        = g_strdup(campos[0]);
        v->departure        = g_strdup(campos[1]);
        v->actual_departure = g_strdup(campos[2]);
        v->arrival          = g_strdup(campos[3]);
        v->actual_arrival   = g_strdup(campos[4]);
        v->gate             = g_strdup(campos[5]);
        v->status           = atoi(campos[6]);
        v->code_origin      = g_strdup(campos[7]);
        v->code_destination = g_strdup(campos[8]);
        v->id_aircraft      = g_strdup(campos[9]);
        v->airline          = g_strdup(campos[10]);
        v->tracking_url     = g_strdup(campos[11]);

        g_hash_table_insert(tabela, g_strdup(v->flight_id), v);
        g_strfreev(campos);
        numeroLinha++;

        if (numeroLinha % 100000 == 0) {
            printf("[INFO] Linhas processadas: %d\n", numeroLinha);
            fflush(stdout);
        }
    }

    free(linha);
    fclose(f);
    printf("[INFO] Voos carregados: %d\n", numeroLinha);
    return tabela;
}


//verifica se o identificador de voo é válido 
int idVooValido(const char *id) {
    if (!id) return 0;
    int len = strlen(id);
    if (len < 5 || len > 8) return 0;

    //permite formatos TP1234 ou TP-1234
    int i = 0;
    if (!isupper((unsigned char)id[i++]) || !isupper((unsigned char)id[i++])) return 0;
    if (id[i] == '-') i++;
    for (; i < len; i++) {
        if (!isdigit((unsigned char)id[i])) return 0;
    }
    return 1;
}

//função auxiliar (converte uma data no formato "YYYY-MM-DD" ou "YYYY-MM-DD HH:MM:SS")
static time_t parseDate(const char *dateStr) {
    struct tm tm = {0};
    if (!dateStr) return (time_t)-1;

    // tenta com hora e segundos
    if (strptime(dateStr, "%Y-%m-%d %H:%M:%S", &tm) != NULL)
        return mktime(&tm);

    // tenta apenas data
    if (strptime(dateStr, "%Y-%m-%d", &tm) != NULL)
        return mktime(&tm);

    return (time_t)-1;
}

//função auxiliar (verifica se uma data está entre 2 limites)
static int dataEntre(const char *data, const char *inicio, const char *fim) {
    time_t t_data = parseDate(data);
    time_t t_inicio = parseDate(inicio);
    time_t t_fim = parseDate(fim);

    if (t_data == (time_t)-1 || t_inicio == (time_t)-1 || t_fim == (time_t)-1)
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
    gpointer key, value;
    g_hash_table_iter_init(&iter, tabelaVoos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;

        //usa partida real se existir, senão usa partida planeada
        const char *partida = (v->actual_departure && strlen(v->actual_departure) > 0)
                                ? v->actual_departure : v->departure;

        if (dataEntre(partida, data_inicio, data_fim)) {
            int *count = g_hash_table_lookup(contagens, v->code_origin);
            if (count) {
                (*count)++;
            } else {
                int *novo = g_new(int, 1);
                *novo = 1;
                g_hash_table_insert(contagens, g_strdup(v->code_origin), novo);
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
        fprintf(out, "%s,%d\n", aeroportoTop, maxPartidas);
    else
        fprintf(out, "\n");

    g_hash_table_destroy(contagens);
}
