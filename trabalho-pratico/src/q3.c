#define _XOPEN_SOURCE 700
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include <time.h> //converte strings de datas em valores comparáveis
#include "q3.h"
#include "validators.h"
#include "q1.h"
#include "errors.h"

//função auxiliar para libertar memória de um voo
void libertaVoo (void *data) {
    Voo *v = data;
    if (!v) return;
    g_free(v->flight_id);
  //  g_free(v->departure);
//    g_free(v->actual_departure);
    //g_free(v->arrival);
    //g_free(v->actual_arrival);
    g_free(v->gate);
    g_free(v->code_origin);
    g_free(v->code_destination);
    g_free(v->id_aircraft);
    g_free(v->airline);
    g_free(v->tracking_url);
    g_free(v);
}


//verifica se o estado do voo é válido
/*static int estadoValido(const char *estado) {
    if (!estado) return 0;
    return (strcmp(estado, "On Time") == 0 ||
            strcmp(estado, "Delayed") == 0 ||
            strcmp(estado, "Cancelled") == 0 ||
            strcmp(estado, "Scheduled") == 0);
}*/

//verifica se código IATA tem 3 letras maiúsculas
/*static int codigoIATAValido(const char *codigo) {
    if (!codigo || strlen(codigo) != 3) return 0;
    for (int i = 0; i < 3; i++)
        if (!isupper((unsigned char)codigo[i]) || !isalpha((unsigned char)codigo[i])) return 0;
    return 1;
}*/

// --------------------------------------------------------------------

//carrega voos de um ficheiro CSV para uma GHashTable
/*GHashTable* carregarVoos(const char *caminhoFicheiro) {
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
*/

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

/*
//função auxiliar (converte uma data no formato "YYYY-MM-DD" ou "YYYY-MM-DD HH:MM:SS")
static time_t parseDate_(const char *dateStr) {
    struct tm tm = {0};
    if (!dateStr) return (time_t)-1;

    // tenta com hora e segundos
    if (strptime(dateStr, "%Y-%m-%d %H:%M:%S", &tm) != NULL)
        return mktime(&tm);

    if (strptime(dateStr, "%Y-%m-%d", &tm) != NULL)
        return mktime(&tm);


    return (time_t)-1;
}
*/
//função auxiliar (verifica se uma data está entre 2 limites)

/*static int dataEntre(const char *data, const char *inicio, const char *fim) {
    time_t t_data = parseDate_(data);
    time_t t_inicio = parseDate_(inicio);
    time_t t_fim = parseDate_(fim);

    if (t_data == (time_t)-1 || t_inicio == (time_t)-1 || t_fim == (time_t)-1)
        return 0;

    return (t_data >= t_inicio && t_data <= t_fim);
}*/

/*
//antiga
//query 3 (aeroporto com mais partidas entre 2 datas)
void query31(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, FILE *out) {
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
//void query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, FILE *out) {
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


//Nova para merge
void query32(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, GHashTable *tabelaAeroportos, FILE *out) {
    if (!data_inicio || !data_fim || !tabelaVoos) {
        fprintf(out, "\n");
        return;
    }

    time_t t_inicio = parseDate(data_inicio);
    time_t t_fim    = parseDate(data_fim);

    if (t_inicio == (time_t)-1 || t_fim == (time_t)-1) {
        fprintf(out, "\n");
        return;
    }

    Cache temporária: key = flight_id, value = time_t convertido para pointer 
    GHashTable *ts_cache = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, NULL);

     1) Pré-processamento — calcular timestamp uma vez por voo 
    GHashTableIter it;
    gpointer key, val;
    g_hash_table_iter_init(&it, tabelaVoos);

    while (g_hash_table_iter_next(&it, &key, &val)) {
        Voo *v = val;

        time_t t;
        if (v->actual_departure && strlen(v->actual_departure) > 0)
            t = parseDate(v->actual_departure);
        else
            t = parseDate(v->departure);

        g_hash_table_insert(ts_cache, v->flight_id, GINT_TO_POINTER(t));
    }

     2) Tabela de contagens 
    GHashTable *contagens = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, g_free);

    g_hash_table_iter_init(&it, tabelaVoos);

    while (g_hash_table_iter_next(&it, &key, &val)) {
        Voo *v = val;

        if (v->status == 2)   // Cancelado
            continue;

        time_t t = GPOINTER_TO_INT(
            g_hash_table_lookup(ts_cache, v->flight_id)
        );

        if (t == (time_t)-1) continue;

        if (t >= t_inicio && t <= t_fim) {
            int *count = g_hash_table_lookup(contagens, v->code_origin);
            if (count)
                (*count)++;
            else {
                int *novo = g_new(int, 1);
                *novo = 1;
                g_hash_table_insert(contagens, g_strdup(v->code_origin), novo);
            }
        }
    }

     3) Encontrar o aeroporto top (lexicográfico em caso de empate) 
    char *top = NULL;
    int max = 0;

    g_hash_table_iter_init(&it, contagens);
    while (g_hash_table_iter_next(&it, &key, &val)) {
        char *airport = key;
        int count = *(int*)val;

        if (count > max ||
            (count == max && top && strcmp(airport, top) < 0)) {
            top = airport;
            max = count;
        }
    }

    if (top) {
	Aeroporto *found = g_hash_table_lookup(tabelaAeroportos,top);
//	g_hash_table_get_value
        fprintf(out, "%s,%s,%s,%s,%d\n", top, found->name, found->city, found->country, max);
    } else
        fprintf(out, "\n");

    g_hash_table_destroy(ts_cache);
    g_hash_table_destroy(contagens);
}

char *estado_to_string (Estado e) {
	if (e == 0) return "On Time";
	if (e == 1) return "Delayed";
	if (e == 2) return "Cancelled";
	return NULL;
}


static void debugVoo(Voo *v) {
    printf("DEBUG Voo: id=%s, status='%s', origin=%s, departure='%s'\n", 
           v->flight_id, estado_to_string(v->status), v->code_origin, v->actual_departure);
}



//antiga
//query 3 (aeroporto com mais partidas entre 2 datas)
void query35(const char *data_inicio, const char *data_fim, 
           GHashTable *tabelaVoos, GHashTable *tabelaAeroportos, FILE *out) {
    
    if (!data_inicio || !data_fim || !tabelaVoos || !tabelaAeroportos) {
        fprintf(out, "\n");
        return;
    }

    time_t t_inicio = parseDate(data_inicio);
    time_t t_fim = parseDate(data_fim);

    if (t_inicio == (time_t)-1 || t_fim == (time_t)-1) {
        fprintf(out, "\n");
        return;
    }

    GHashTable *contagens = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    const char *top_code = NULL;  // ⚠️ MUDADO: agora é const char*
    int max_count = 0;

    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, tabelaVoos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;

        // Ignorar voos cancelados
 //       if (v->status && strcmp(v->status, "Cancelled") == 0) {
	if (v->status == 2) { 
           continue;
        }

        // Usar partida REAL
        const char *partida = (v->actual_departure && strlen(v->actual_departure) > 0) 
                            ? v->actual_departure : v->departure;
        if (!partida) continue;

        time_t t_partida = parseDate(partida);
        if (t_partida == (time_t)-1) continue;

        // Verificar intervalo
        if (t_partida >= t_inicio && t_partida <= t_fim) {
            int *count = g_hash_table_lookup(contagens, v->code_origin);

            if (count) {
                (*count)++;
            } else {
                int *novo = g_new(int, 1);
                *novo = 1;
                g_hash_table_insert(contagens, g_strdup(v->code_origin), novo);
                count = novo;
            }

            // ATUALIZAR MÁXIMO - APENAS REFERÊNCIA, SEM DUPLICAR
            if (*count > max_count || 
                (*count == max_count && top_code && strcmp(v->code_origin, top_code) < 0)) {
                top_code = v->code_origin;  // ⚠️ APENAS REFERÊNCIA, não duplica!
                max_count = *count;
            }
        }
    }

    // OUTPUT
    if (top_code && max_count > 0) {
        Aeroporto *aero = g_hash_table_lookup(tabelaAeroportos, top_code);
        if (aero) {
            fprintf(out, "%s,%s,%s,%s,%d\n", 
                   aero->code_IATA, aero->name, aero->city, aero->country, max_count);
        } else {
            fprintf(out, "\n");
        }
        // ⚠️ NÃO PRECISA g_free(top_code) - é apenas referência
    } else {
        fprintf(out, "\n");
    }

    g_hash_table_destroy(contagens);
}



//--

void query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, GHashTable *tabelaAeroportos, FILE *out) {

    if (!data_inicio || !data_fim || !tabelaVoos || !tabelaAeroportos) {
        fprintf(out, "\n");
        return;
    }

    time_t t_inicio = parseDate(data_inicio);
    time_t t_fim = parseDate(data_fim);

    if (t_inicio == (time_t)-1 || t_fim == (time_t)-1) {
        fprintf(out, "\n");
        return;
    }

    GHashTable *contagens = g_hash_table_new_full( g_str_hash, g_str_equal, g_free, NULL);

    GList *listaVoos = g_hash_table_get_values(tabelaVoos);

    for (GList *l = listaVoos; l != NULL; l = l->next) {
        Voo *v = l->data;

        // ignorar cancelados e fora do intervalo de datas
        if (v->status == 2 || (v->status != 2 && !dataEntre(v->actual_departure, data_inicio, data_fim)))
            continue;

        // ignorar voos sem aeronave associada
        if (!v->code_origin || strlen(v->code_origin) == 0)
            continue;

        gpointer val = g_hash_table_lookup(contagens, v->code_origin);

        if (val == NULL) {
            g_hash_table_insert(contagens,g_strdup(v->code_origin), GINT_TO_POINTER(1));
        } else {
            int novo = GPOINTER_TO_INT(val) + 1;
            g_hash_table_replace(contagens, g_strdup(v->code_origin), GINT_TO_POINTER(novo));
        }
    }

    g_list_free(listaVoos);

    if (!contagens) {
	fprintf (out,"\n");
	return;
    }

    Glist *contagens_list = g_hash_table_get_value(contagens);
    contagens_list = g_list_sort(contagens_list, (GCompareFunc) comparaContagens);

    int count = 0;
    char *a = contagens_list->data;
    Aeroporto *a_max = lookup(tabelaAeroportos, a);
    gpointer val = g_hash_table_lookup(contagens, a_max);
        if (val) count = GPOINTER_TO_INT(val);


    fprintf ("%s,%s,%s,%s,%d\n", a_max->code, a_max->name, a_max->city, a_max->country, count);
}




static gint comparaAeroportos(const gpointer a, const gpointer b, gpointer user_data) {
    GHashTable *contagens = user_data;

    int ca = GPOINTER_TO_INT(g_hash_table_lookup(contagens, a));
    int cb = GPOINTER_TO_INT(g_hash_table_lookup(contagens, b));

    // primeiro ordenar por count desc
    if (ca != cb) return (cb - ca);

    // em empate, ordenar alfabeticamente o código
    return strcmp((const char *)a, (const char *)b);
}


void query3(const char *data_inicio, const char *data_fim,
            GHashTable *tabelaVoos, GHashTable *tabelaAeroportos, FILE *out)
{
    if (!data_inicio || !data_fim || !tabelaVoos || !tabelaAeroportos) {
        fprintf(out, "\n");
        return;
    }

    time_t t_inicio = parseDate(data_inicio);
    time_t t_fim    = parseDate(data_fim);

    if (t_inicio == (time_t)-1 || t_fim == (time_t)-1) {
        fprintf(out, "\n");
        return;
    }

    // chave = código do aeroporto, valor = count (int*)
    GHashTable *contagens = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, g_free
    );

    // obter lista de voos
    GList *listaVoos = g_hash_table_get_values(tabelaVoos);

    for (GList *l = listaVoos; l != NULL; l = l->next) {
        Voo *v = l->data;

        if (v->status == 2)  // cancelado
            continue;

        if (!v->actual_departure)
            continue;

        time_t t_partida = parseDate(v->actual_departure);
        if (t_partida == (time_t)-1)
            continue;

        if (t_partida < t_inicio || t_partida > t_fim)
            continue;

        if (!v->code_origin)
            continue;

        int *count = g_hash_table_lookup(contagens, v->code_origin);

        if (!count) {
            int *novo = g_new(int, 1);
            *novo = 1;
            g_hash_table_insert(contagens, g_strdup(v->code_origin), novo);
        } else {
            (*count)++;
        }
    }

    g_list_free(listaVoos);

    // Se não há contagens → output vazio
    if (g_hash_table_size(contagens) == 0) {
        fprintf(out, "\n");
        g_hash_table_destroy(contagens);
        return;
    }

    // transformar contagens em lista
    GList *keys = g_hash_table_get_keys(contagens);

    keys = g_list_sort_with_data(keys,(GCompareDataFunc) comparaAeroportos, contagens);
    if (!keys) {fprintf (out,"\n"); return; }
    // ordenar aeroportos por count desc e depois código asc
    keys = g_list_sort(keys, (GCompareFunc) (int(const char *a, const char *b)) {
        int ca = GPOINTER_TO_INT(g_hash_table_lookup(contagens, a));
        int cb = GPOINTER_TO_INT(g_hash_table_lookup(contagens, b));

        if (ca != cb) return cb - ca;  // maior primeiro
        return strcmp(a, b);           // empate → alfabético
    }));

    // melhor aeroporto = primeiro elemento da lista
    char *best_code = keys->data;
    int best_count = GPOINTER_TO_INT(g_hash_table_lookup(contagens, best_code));


    Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, best_code);

    if (a)
        fprintf(out, "%s,%s,%s,%s,%d\n",
                a->code_IATA, a->name, a->city, a->country, best_count);
    else
        fprintf(out, "\n");

    // libertar
    g_list_free(keys);
    g_hash_table_destroy(contagens);
}
*/

void query39(const char *data_inicio, const char *data_fim,
            GHashTable *tabelaVoos,
            GHashTable *tabelaAeroportos,
            FILE *out)
{
    if (!data_inicio || !data_fim || !tabelaVoos || !tabelaAeroportos) {
        fprintf(out, "\n");
        return;
    }

    time_t t_inicio = parseDate_(data_inicio);
    time_t t_fim    = parseDate_(data_fim);
    //time_t t_inicio;
    //int tem_inicio_valido = v_parse_date2(data_inicio,&t_inicio);

    //time_t t_fim;
    //int tem_fim_valido = v_parse_date2(data_fim,&t_fim);

    //if (!tem_inicio_valido || !tem_fim_valido) {
    if (t_inicio == (time_t)-1 || t_fim == (time_t)-1) {
        fprintf(out, "\n");
        return;
    }

    // contagens[code_origin] = número de partidas
    GHashTable *contagens = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free,     // libera a key (g_strdup)
        g_free      // libera o int*
    );

    // ---------------------------
    // PASSO 1 – Contar partidas
    // ---------------------------
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, tabelaVoos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;

        if (v->status == 2) continue;       // Cancelado não conta
        //if (!v->actual_departure) continue; // Safety

        time_t t_partida = v->actual_departure;
        if (t_partida == (time_t)-1) continue;
	//if (v->actual_departure == (time_t)-1) continue;

        // dentro do intervalo?
        if (t_partida < t_inicio || t_partida > t_fim)
            continue;

        if (!v->code_origin) continue;

        // atualizar contagem
        int *ptr = g_hash_table_lookup(contagens, v->code_origin);

        if (!ptr) {
            int *novo = g_new(int, 1);
            *novo = 1;
            g_hash_table_insert(contagens, g_strdup(v->code_origin), novo);
        } else {
            (*ptr)++;
        }
    }

    // ---------------------------
    // PASSO 2 – Encontrar o máximo
    // ---------------------------
    const char *melhor = NULL;
    int max = 0;

    g_hash_table_iter_init(&iter, contagens);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *code = key;
        int count = *((int *)value);

        if (count > max) {
            max = count;
            melhor = code;
        } 
        else if (count == max && melhor != NULL) {
            if (strcmp(code, melhor) < 0) {
                melhor = code;
            }
        }
    }

    // ---------------------------
    // PASSO 3 – Output
    // ---------------------------
    if (!melhor || max == 0) {
        fprintf(out, "\n");
    } else {
        Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, melhor);
        if (a) {
            fprintf(out, "%s,%s,%s,%s,%d\n",
                a->code_IATA, a->name, a->city, a->country, max);
        } else {
            fprintf(out, "\n");
        }
    }

    g_hash_table_destroy(contagens);
}


void query3(const char *data_inicio, const char *data_fim,
            GHashTable *tabelaVoos,
            GHashTable *tabelaAeroportos,
            FILE *out)
{
    if (!data_inicio || !data_fim || !tabelaVoos || !tabelaAeroportos) {
        fprintf(out, "\n");
        return;
    }

    time_t t_inicio = parseDate_(data_inicio);
    time_t t_fim    = parseDate_(data_fim);

    if (t_inicio == (time_t)-1 || t_fim == (time_t)-1) {
        fprintf(out, "\n");
        return;
    }

    // contagens[code_origin] = número de partidas
    GHashTable *contagens = g_hash_table_new_full(
        g_str_hash, g_str_equal,
        g_free,     // libera a key (g_strdup)
        g_free      // libera o int*
    );

    // ---------------------------
    // PASSO 1 – Contar partidas
    // ---------------------------
    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, tabelaVoos);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;

        if (v->status == 2) continue;       // Cancelado não conta
        if (!v->actual_departure) continue; // Safety

        time_t t_partida = v->actual_departure;
        if (t_partida == (time_t)-1) continue;

        // dentro do intervalo?
        if (t_partida < t_inicio || t_partida > t_fim)
            continue;

        if (!v->code_origin) continue;

        // atualizar contagem
        int *ptr = g_hash_table_lookup(contagens, v->code_origin);

        if (!ptr) {
            int *novo = g_new(int, 1);
            *novo = 1;
            g_hash_table_insert(contagens, g_strdup(v->code_origin), novo);
        } else {
            (*ptr)++;
        }
    }

    // ---------------------------
    // PASSO 2 – Encontrar o máximo
    // ---------------------------
    const char *melhor = NULL;
    int max = 0;

    g_hash_table_iter_init(&iter, contagens);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *code = key;
        int count = *((int *)value);

        if (count > max) {
            max = count;
            melhor = code;
        } 
        else if (count == max && melhor != NULL) {
            if (strcmp(code, melhor) < 0) {
                melhor = code;
            }
        }
    }

    // ---------------------------
    // PASSO 3 – Output
    // ---------------------------
    if (!melhor || max == 0) {
        fprintf(out, "\n");
    } else {
        Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, melhor);
        if (a) {
            fprintf(out, "%s,%s,%s,%s,%d\n",
                a->code_IATA, a->name, a->city, a->country, max);
        } else {
            fprintf(out, "\n");
        }
    }

    g_hash_table_destroy(contagens);
}
