#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <ctype.h>   // isupper, isdigit, isspace
#include <stdlib.h>  // atoi, atof
#include <glib.h>
#include "q2.h"
#include "errors.h"
#include "read.h"
#include "q3.h"

// =====================================================
//   ESTRUTURAS (assumidas de q2.h / read.h)
// =====================================================
//  Aeronave:
//    char *identifier;
//    char *manufacturer;
//    char *model;
//    int   year;
//    int   capacity;
//    float range;
//
//  Voo:
//    char *flight_id;
//    char *departure;
//    char *actual_departure;
//    char *arrival;
//    char *actual_arrival;
//    char *gate;
//    int   status;          // 0=On Time, 1=Delayed, 2=Cancelled
//    char *code_origin;
//    char *code_destination;
//    char *id_aircraft;
//    char *airline;
//    char *tracking_url;
//
//  Contagem:
//    char *identifier;
//    char *manufacturer;
//    char *model;
//    int   count;
// =====================================================


// -----------------------------------------------------
// Libertar aeronave (para usar em hash tables)
// -----------------------------------------------------
void libertaAeronave(void *data) {
    Aeronave *a = data;
    if (!a) return;
    g_free(a->identifier);
    g_free(a->manufacturer);
    g_free(a->model);
    g_free(a);
}

// -----------------------------------------------------
// Funções auxiliares simples
// -----------------------------------------------------
static int stringVaziaOuEspacos(const char *s) {
    if (!s) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}

static int anoValido(int ano) {
    return (ano >= 1900 && ano <= 2025);
}

static int positivo(float n) {
    return (n > 0);
}

// remove espaços à esquerda e à direita *in-place*
static void trim(char *s) {
    if (!s) return;

    // leading
    char *p = s;
    while (*p && isspace((unsigned char)*p)) p++;
    if (p != s) memmove(s, p, strlen(p) + 1);

    // trailing
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

// -----------------------------------------------------
// Carregar aircrafts.csv para hash table (se for usado)
// chave: identifier, valor: Aeronave*
// -----------------------------------------------------
GHashTable* carregarAeronaves(const char *caminhoFicheiro) {
    FILE *f = fopen(caminhoFicheiro, "r");
    if (!f) {
        perror("Erro ao abrir o ficheiro de aeronaves");
        exit(EXIT_FAILURE);
    }

    GHashTable *tabela =
        g_hash_table_new_full(g_str_hash, g_str_equal, g_free, libertaAeronave);

    char *linha = NULL;
    size_t tamanho = 0;

    // ignora cabeçalho
    __attribute__((unused)) ssize_t nread = getline(&linha, &tamanho, f);

    int numeroLinha = 2; // 1 é cabeçalho

    while (getline(&linha, &tamanho, f) != -1) {
        linha[strcspn(linha, "\n")] = '\0';

        gchar **campos = g_strsplit(linha, ",", 0);

        if (g_strv_length(campos) != 6) {
            errors_add("aircrafts.csv", numeroLinha, "linha incompleta");
            g_strfreev(campos);
            numeroLinha++;
            continue;
        }

        for (int i = 0; campos[i] != NULL; i++) {
            campos[i] = g_strstrip(campos[i]);
            size_t len = strlen(campos[i]);
            if (len > 0 && campos[i][0] == '"')
                memmove(campos[i], campos[i] + 1, len - 1);
            if (len > 1 && campos[i][strlen(campos[i]) - 1] == '"')
                campos[i][strlen(campos[i]) - 1] = '\0';
        }

        const char *identifier   = campos[0];
        const char *manufacturer = campos[1];
        const char *model        = campos[2];
        const char *year_str     = campos[3];
        const char *capacity_str = campos[4];
        const char *range_str    = campos[5];

        int campos_ok = !(stringVaziaOuEspacos(identifier)   ||
                          stringVaziaOuEspacos(manufacturer) ||
                          stringVaziaOuEspacos(model)        ||
                          stringVaziaOuEspacos(year_str)     ||
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
            if (!campos_ok)       errors_add("aircrafts.csv", numeroLinha, "campos vazios");
            else if (!id_ok)      errors_add("aircrafts.csv", numeroLinha, "identifier inválido");
            else if (!ano_ok)     errors_add("aircrafts.csv", numeroLinha, "ano inválido");
            else if (!cap_ok)     errors_add("aircrafts.csv", numeroLinha, "capacity inválida");
            else if (!ran_ok)     errors_add("aircrafts.csv", numeroLinha, "range inválido");
        }

        g_strfreev(campos);
        numeroLinha++;
    }

    free(linha);
    fclose(f);
    return tabela;
}

// -----------------------------------------------------
// Identificador de aeronave: AA-1234
// -----------------------------------------------------
int identificadorValido(const char *id) {
    if (!id || strlen(id) != 7) return 0;
    if (!isupper((unsigned char)id[0]) || !isupper((unsigned char)id[1])) return 0;
    if (id[2] != '-') return 0;
    for (int i = 3; i < 7; i++)
        if (!isdigit((unsigned char)id[i])) return 0;
    return 1;
}

// -----------------------------------------------------
// Comparador de Contagem: primeiro por count desc,
// depois por identifier asc (lexicográfico)
// -----------------------------------------------------
int comparaContagens(const Contagem *a, const Contagem *b) {
    if (a->count != b->count)
        return b->count - a->count; // maior primeiro
    return g_strcmp0(a->identifier, b->identifier);
}

// -----------------------------------------------------
// QUERY 2
// 2 <N> [manufacturer]
// Top N aeronaves com mais voos (não cancelados),
// opcionalmente filtradas por fabricante.
// -----------------------------------------------------
void query25(const char *linhaComando,
            GHashTable *tabelaAeronaves,
            GHashTable *tabelaVoos,
            FILE *out)
{
    int N;
    char fabricante_raw[200] = "";

    // lê "<N> [fabricante com espaços]"
    int arg = sscanf(linhaComando, "%d %[^\n]", &N, fabricante_raw);
    if (arg < 1 || N <= 0) {
        fprintf(out, "\n");
        return;
    }

    // limpar espaços extra do fabricante (leading + trailing)
    if (arg == 2) {
        trim(fabricante_raw);
    } else {
        fabricante_raw[0] = '\0';
    }

    int usar_filtro = (fabricante_raw[0] != '\0');
    gchar *fabricante = usar_filtro ? g_ascii_strdown(fabricante_raw, -1) : NULL;

    // ===================================
    // PASSO 1 — contar voos por aeronave
    // ===================================
    GHashTable *contagens = g_hash_table_new_full(
        g_str_hash, g_str_equal, g_free, NULL);

    GList *listaVoos = g_hash_table_get_values(tabelaVoos);

    for (GList *l = listaVoos; l != NULL; l = l->next) {
        Voo *v = l->data;

        // ignorar cancelados
        if (v->status == 2)
            continue;

        // ignorar voos sem aeronave associada
        if (!v->id_aircraft || strlen(v->id_aircraft) == 0)
            continue;

        gpointer val = g_hash_table_lookup(contagens, v->id_aircraft);

        if (val == NULL) {
            g_hash_table_insert(contagens,
                                g_strdup(v->id_aircraft),
                                GINT_TO_POINTER(1));
        } else {
            int novo = GPOINTER_TO_INT(val) + 1;
            g_hash_table_replace(contagens,
                                 g_strdup(v->id_aircraft),
                                 GINT_TO_POINTER(novo));
        }
    }

    g_list_free(listaVoos);

    // ===================================
    // PASSO 2 — construir lista de Contagem
    // ===================================
    GList *listaAeronaves = g_hash_table_get_values(tabelaAeronaves);
    GList *resultado = NULL;

    for (GList *l = listaAeronaves; l != NULL; l = l->next) {
        Aeronave *a = l->data;

        // aplicar filtro de fabricante (case-insensitive)
        if (usar_filtro) {
            gchar *fab = g_ascii_strdown(a->manufacturer, -1);
            int ok = (g_strcmp0(fab, fabricante) == 0);
            g_free(fab);
            if (!ok) continue;
        }

        int count = 0;
        gpointer val = g_hash_table_lookup(contagens, a->identifier);
        if (val) count = GPOINTER_TO_INT(val);

        Contagem *c = g_new(Contagem, 1);
        c->identifier   = g_strdup(a->identifier);
        c->manufacturer = g_strdup(a->manufacturer);
        c->model        = g_strdup(a->model);
        c->count        = count;

        resultado = g_list_prepend(resultado, c);
    }

    g_list_free(listaAeronaves);
    if (fabricante) g_free(fabricante);

    // ===================================
    // PASSO 3 — ordenar lista
    // ===================================
    resultado = g_list_sort(resultado, (GCompareFunc) comparaContagens);

    // ===================================
    // PASSO 4 — imprimir top N
    // ===================================
    int printed = 0;
    for (GList *l = resultado; l != NULL && printed < N; l = l->next, printed++) {
        Contagem *c = l->data;
        fprintf(out, "%s,%s,%s,%d\n",
                c->identifier, c->manufacturer, c->model, c->count);
    }

    // se não imprimimos nada (N>0 mas não há resultados) → linha vazia
    if (printed == 0) {
        fprintf(out, "\n");
    }

    // ===================================
    // PASSO 5 — libertar memória
    // ===================================
    for (GList *l = resultado; l != NULL; l = l->next) {
        Contagem *c = l->data;
        g_free(c->identifier);
        g_free(c->manufacturer);
        g_free(c->model);
        g_free(c);
    }
    g_list_free(resultado);
    g_hash_table_destroy(contagens);
}



void query2(const char *linhaComando,
            GHashTable *tabelaAeronaves,
            GHashTable *tabelaVoos,
            FILE *out)
{
    int N;
    char fabricante_raw[200] = "";

    // Lê N + fabricante opcional
    int arg = sscanf(linhaComando, "%d %[^\n]", &N, fabricante_raw);
    if (arg < 1 || N <= 0) {
        fprintf(out, "\n");
        return;
    }

    // limpar espaços (apenas se veio fabricante)
    if (arg == 2) trim(fabricante_raw);
    else fabricante_raw[0] = '\0';

    int usar_filtro = (fabricante_raw[0] != '\0');

    // converter só UMA VEZ para lowercase
    gchar *fabricante = usar_filtro ?
        g_ascii_strdown(fabricante_raw, -1) : NULL;

    // ===============================
    // PASSO 1 — contar voos por aeronave
    // ===============================

    GHashTable *contagens = g_hash_table_new(g_str_hash, g_str_equal);

    GHashTableIter itv;
    gpointer keyv, valv;

    g_hash_table_iter_init(&itv, tabelaVoos);
    while (g_hash_table_iter_next(&itv, &keyv, &valv)) {

        Voo *v = valv;

        // ignorar cancelados
        if (v->status == 2) continue;

        // ignorar voos sem aeronave
        if (!v->id_aircraft || v->id_aircraft[0] == '\0') continue;

        gpointer old = g_hash_table_lookup(contagens, v->id_aircraft);
        if (!old) {
            g_hash_table_insert(contagens,
                                (char*)v->id_aircraft,  // sem strdup
                                GINT_TO_POINTER(1));
        } else {
            int novo = GPOINTER_TO_INT(old) + 1;
            g_hash_table_insert(contagens,
                                (char*)v->id_aircraft,
                                GINT_TO_POINTER(novo));
        }
    }


    // ===============================
    // PASSO 2 — criar lista de Contagem
    // ===============================

    GList *resultado = NULL;

    GHashTableIter ita;
    gpointer keya, vala;

    g_hash_table_iter_init(&ita, tabelaAeronaves);
    while (g_hash_table_iter_next(&ita, &keya, &vala)) {

        Aeronave *a = vala;

        // aplicar filtro (agora sem alocações)
        if (usar_filtro) {
            const char *s1 = a->manufacturer;
            const char *s2 = fabricante;
            int ok = 1;

            for (; *s1 && *s2; s1++, s2++) {
                if (g_ascii_tolower(*s1) != *s2) {
                    ok = 0;
                    break;
                }
            }
            if (!(*s1 == '\0' && *s2 == '\0')) ok = 0;

            if (!ok) continue;
        }

        int count = 0;
        gpointer val = g_hash_table_lookup(contagens, a->identifier);
        if (val) count = GPOINTER_TO_INT(val);

        Contagem *c = g_new(Contagem, 1);
        c->identifier   = g_strdup(a->identifier);
        c->manufacturer = g_strdup(a->manufacturer);
        c->model        = g_strdup(a->model);
        c->count        = count;

        resultado = g_list_prepend(resultado, c);
    }

    if (fabricante) g_free(fabricante);

    // ===============================
    // PASSO 3 — ordenar lista
    // ===============================
    resultado = g_list_sort(resultado,
        (GCompareFunc) comparaContagens);

    // ===============================
    // PASSO 4 — imprimir top N
    // ===============================
    int printed = 0;
    for (GList *l = resultado; l != NULL && printed < N; l = l->next, printed++) {
        Contagem *c = l->data;
        fprintf(out, "%s,%s,%s,%d\n",
                c->identifier, c->manufacturer, c->model, c->count);
    }

    if (printed == 0)
        fprintf(out, "\n");

    // ===============================
    // PASSO 5 — libertar memória
    // ===============================
    for (GList *l = resultado; l != NULL; l = l->next) {
        Contagem *c = l->data;
        g_free(c->identifier);
        g_free(c->manufacturer);
        g_free(c->model);
        g_free(c);
    }
    g_list_free(resultado);

    g_hash_table_destroy(contagens);
}
