#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <ctype.h> //para funcoes isupper e isdigit
#include <stdlib.h> //para atoi e atof
#include <glib.h>
#include "q2.h"
#include "errors.h"
#include "read.h"
#include "q3.h"

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


//antiga
//query 2 (top N aeronaves de um determinado fabricante com mais voos realizados)
void query21(const char *linhaComando,
            GHashTable *tabelaAeronaves,
            GHashTable *tabelaVoos,
            FILE *out)
{
    int n;
    char fabricante[100] = "";

    if (sscanf(linhaComando, "%d %99s", &n, fabricante) < 1) {
        fprintf(out, "\n");
        return;
    }

    GList *listaAeronaves = g_hash_table_get_values(tabelaAeronaves);
    GList *listaVoos = g_hash_table_get_values(tabelaVoos);

    GList *filtrada = NULL;

    for (GList *l = listaAeronaves; l != NULL; l = l->next) {

        Aeronave *a = (Aeronave *) l->data;

        if (strlen(fabricante) == 0 ||
            g_strcmp0(a->manufacturer, fabricante) == 0)
        {
            int counter = 0;

            for (GList *v = listaVoos; v != NULL; v = v->next) {
                Voo *voo = (Voo *) v->data;
                if (voo->id_aircraft &&
                    strcmp(voo->id_aircraft, a->identifier) == 0)
                {
                    counter++;
                }
            }

            Contagem *c = g_new(Contagem, 1);
            c->identifier = g_strdup(a->identifier);
            c->manufacturer = g_strdup(a->manufacturer);
            c->model = g_strdup(a->model);
            c->count = counter;
            filtrada = g_list_prepend(filtrada, c);
        }
    }

    g_list_free(listaVoos);
    g_list_free(listaAeronaves);

    filtrada = g_list_sort(filtrada, (GCompareFunc) comparaContagens);

    int printed = 0;
    for (GList *l = filtrada; l != NULL && printed < n; l = l->next, printed++) {
        Contagem *c = l->data;
        fprintf(out, "%s,%s,%s,%d\n",
                c->identifier, c->manufacturer, c->model, c->count);
    }

    for (GList *l = filtrada; l != NULL; l = l->next) {
        Contagem *c = l->data;
        g_free(c->identifier);
        g_free(c->manufacturer);
        g_free(c->model);
        g_free(c);
    }
    g_list_free(filtrada);
}


//nova para merge
void query2(const char *linhaComando,GHashTable *tabelaAeronaves,GHashTable *tabelaVoos,FILE *out) {
    int n;
    char fabricante_raw[200];

    // Lê "N fabricante", permitindo fabricantes com espaços
    if (sscanf(linhaComando, "%d %[^\n]", &n, fabricante_raw) < 1) {
    	fprintf(out, "\n");
    	return;
    }

    // Se fabricante vier vazio, não filtramos
    int filtra_por_fabricante = (strlen(fabricante_raw) > 0);

    // Normalizar para comparação case-insensitive
    gchar *fabricante = g_ascii_strdown(fabricante_raw, -1);

    // ================================================
    // 1) Criar tabela de contagens
    // ================================================
GHashTable *contagens = g_hash_table_new_full(  
        g_str_hash, g_str_equal, g_free, NULL);  

GList *voos = g_hash_table_get_values(tabelaVoos);  

for (GList *l = voos; l != NULL; l = l->next) {  
    Voo *v = l->data;  

    if (v->id_aircraft != NULL) {  
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
}  

g_list_free(voos);  

// ================================================  
// 2) Filtrar aeronaves e montar lista  
// ================================================  
GList *aeronaves = g_hash_table_get_values(tabelaAeronaves);  
GList *lista = NULL;  

for (GList *l = aeronaves; l != NULL; l = l->next) {  
    Aeronave *a = l->data;  

    int aceita = 1; // FLAG — admite incluir por defeito  

    if (filtra_por_fabricante) {  
        gchar *fab_a = g_ascii_strdown(a->manufacturer, -1);  

        if (g_strcmp0(fab_a, fabricante) != 0) {  
            aceita = 0;  
        }  

        g_free(fab_a);  
    }  

    if (aceita) {  
        int count = 0;  
        gpointer val = g_hash_table_lookup(contagens, a->identifier);  
        if (val != NULL) count = GPOINTER_TO_INT(val);  

        Contagem *c = g_new(Contagem, 1);  
        c->identifier   = g_strdup(a->identifier);  
        c->manufacturer = g_strdup(a->manufacturer);  
        c->model        = g_strdup(a->model);  
        c->count        = count;  

        lista = g_list_prepend(lista, c);  
    }  
}  

g_list_free(aeronaves);  
g_free(fabricante);  

// ================================================  
// 3) Ordenar  
// ================================================  
lista = g_list_sort(lista, (GCompareFunc) comparaContagens);  

// ================================================  
// 4) Imprimir top N  
// ================================================  
int printed = 0;  

for (GList *l = lista; l != NULL && printed < n; l = l->next) {  
    Contagem *c = l->data;  
    fprintf(out, "%s,%s,%s,%d\n",  
            c->identifier, c->manufacturer, c->model, c->count);  
    printed++;  
}  

// ================================================  
// 5) Libertar memória  
// ================================================  
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




