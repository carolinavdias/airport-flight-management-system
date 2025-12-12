#define _POSIX_C_SOURCE 200809L
#include "queries/q1.h"

#include <string.h>

#include "entidades/airports.h"
#include "validacoes/validacoes_airports.h"

const char* tipoToString(Tipo_aeroporto t) {
    switch (t) {
        case TIPO_SMALL_AIRPORT: return "small_airport";
        case TIPO_MEDIUM_AIRPORT: return "medium_airport";
        case TIPO_LARGE_AIRPORT: return "large_airport";
        case TIPO_HELIPORT: return "heliport";
        case TIPO_SEAPLANE_BASE: return "seaplane_base";
        default: return "error";
    }
}

//query 1 (dado um código de aeroporto, procura-o na tabela e imprime as suas informações)
void query1(const char *code, GHashTable *tabelaAeroportos, FILE *out) {
    if (!valida_codigoIATA(code)) {
        fprintf(out, "\n");
        return;
    }

    Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, code);
    if (a != NULL) {
        fprintf(out, "%s,%s,%s,%s,%s\n",
                airport_get_code_IATA(a), airport_get_name(a), airport_get_city(a), airport_get_country(a), tipoToString(airport_get_type(a)));
    } else {
        fprintf(out, "\n");
    }
}


/*
//verifica se o código do aeroporto é válido (3 letras maiúsculas)
int codigoValido(const char *codigo) {
    if (!codigo || strlen(codigo) != 3) return 0;
    for (int i = 0; i < 3; i++) {
        if (!isupper((unsigned char)codigo[i]) || !isalpha((unsigned char)codigo[i])) return 0;
    }
    return 1;
}

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

    // getline(&linha, &tamanho, f); // ignore header

    int numeroLinha = 2; //linha real do ficheiro (1 é o cabeçalho)

    // ignora cabeçalho
    ssize_t nread = getline(&linha, &tamanho, f);
    if (nread == -1) {
    // ficheiro vazio ou erro ao ler
        fclose(f);
        free(linha);
        return NULL; // ou trata conforme a tua lógica
    }


    while (getline(&linha, &tamanho, f) != -1) {
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
            //a->type    = g_strdup(type); 

            if (strcmp(type, "small_airport") == 0) a->type = TIPO_SMALL_AIRPORT;
            else if (strcmp(type, "medium_airport") == 0) a->type = TIPO_MEDIUM_AIRPORT;
            else if (strcmp(type, "large_airport") == 0) a->type = TIPO_LARGE_AIRPORT;
            else if (strcmp(type, "heliport") == 0) a->type = TIPO_HELIPORT;
            else if (strcmp(type, "seaplane_base") == 0) a->type = TIPO_SEAPLANE_BASE;
            else if (strcmp(type, "closed_airport") == 0) a->type = TIPO_CLOSED_AIRPORT;
            else a->type = TIPO_ERROR;

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


static int stringVaziaOuEspacos(const char *s) {
    if (!s) return 1;
    while (*s) {
        if (!isspace((unsigned char)*s)) return 0;
        s++;
    }
    return 1;
}
*/

