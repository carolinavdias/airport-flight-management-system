#define _XOPEN_SOURCE 700

#include "queries/q3.h"
#include "validacoes/validacoes_flights.h"
#include "entidades/flights.h"
#include "entidades/airports.h"

typedef struct contagens2 {
    char *code;
    int count;
} Contagens2;

// =====================================================
// ARRAY GLOBAL DE VOOS ORDENADO POR DATA
// =====================================================
static Voo **voos_ordenados = NULL;
static int num_voos = 0;

// =====================================================
// FUNÇÃO DE COMPARAÇÃO PARA QSORT
// =====================================================
static int compara_voos_por_data(const void *a, const void *b) {
    Voo *v1 = *(Voo **)a;
    Voo *v2 = *(Voo **)b;
    
    long long t1 = voo_get_actual_departure(v1);
    long long t2 = voo_get_actual_departure(v2);
    
    if (t1 < t2) return -1;
    if (t1 > t2) return 1;
    return 0;
}

// =====================================================
// INÍCIO DO INTERVALO - encontra primeiro voo >= data
// =====================================================
static int intervalo_inicio(long long t_inicio) {
    int esq = 0, dir = num_voos - 1;
    int resultado = num_voos; //se não encontrar, retorna fim
    
    while (esq <= dir) {
        int meio = esq + (dir - esq) / 2;
        long long t = voo_get_actual_departure(voos_ordenados[meio]);
        
        if (t >= t_inicio) {
            resultado = meio;
            dir = meio - 1; //continua a procurar à esquerda
        } else {
            esq = meio + 1;
        }
    }
    
    return resultado;
}

// =====================================================
// INICIALIZAÇÃO (chamar uma vez no início)
// =====================================================
void query3_init(GHashTable *tabelaVoos) {
    if (voos_ordenados != NULL) return; //já inicializada
    
    //conta voos válidos (não cancelados)
    num_voos = 0;
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, tabelaVoos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;
        if (voo_get_status(v) != ESTADO_CANCELLED && voo_get_code_origin(v)) {
            num_voos++;
        }
    }
    
    //aloca array
    voos_ordenados = malloc(num_voos * sizeof(Voo *));
    if (!voos_ordenados) return;
    
    //preenche array
    int idx = 0;
    g_hash_table_iter_init(&iter, tabelaVoos);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Voo *v = value;
        if (voo_get_status(v) != ESTADO_CANCELLED && voo_get_code_origin(v)) {
            voos_ordenados[idx++] = v;
        }
    }
    
    //ordena por data de partida uma única vez
    qsort(voos_ordenados, num_voos, sizeof(Voo *), compara_voos_por_data);
}

// =====================================================
// LIMPEZA (chamar no final do programa)
// =====================================================
void query3_cleanup(void) {
    if (voos_ordenados != NULL) {
        free(voos_ordenados);
        voos_ordenados = NULL;
        num_voos = 0;
    }
}

// =====================================================
// QUERY 3 OTIMIZADA
// =====================================================
char *query3(const char *data_inicio, const char *data_fim, GHashTable *tabelaVoos, GHashTable *tabelaAeroportos) {

    //garante inicialização
    if (voos_ordenados == NULL) {
        query3_init(tabelaVoos);
    }
    
    if (!data_inicio || !data_fim || !tabelaAeroportos || num_voos == 0) {
        return strdup("\n");
    }
    
    //valida datas
    if (!valida_DataH(data_inicio) || !valida_DataH(data_fim)) {
        return strdup("\n");
    }
    
    long long t_inicio = converte_dataH(data_inicio);
    long long t_fim = converte_dataH(data_fim);
    
    // OTIMIZAÇÃO: Procura para encontrar início
    int inicio = intervalo_inicio(t_inicio);
    
    //hash table para contar (só para voos no intervalo)
    GHashTable *contagens = g_hash_table_new_full(
        g_str_hash, g_str_equal, NULL, g_free
    );
    
    //itera apenas voos no intervalo
    for (int i = inicio; i < num_voos; i++) {
        Voo *v = voos_ordenados[i];
        long long t_partida = voo_get_actual_departure(v);
        
        //se passou do fim, pode parar (array ordenado)
        if (t_partida > t_fim) break;
        
        const char *origem = voo_get_code_origin(v);
        if (!origem) continue;
        
        //incrementa contagem
        int *ptr = g_hash_table_lookup(contagens, origem);
        if (!ptr) {
            int *novo = g_new(int, 1);
            *novo = 1;
            g_hash_table_insert(contagens, (gpointer)origem, novo);
        } else {
            (*ptr)++;
        }
    }
    
    //encontra máximo
    const char *melhor = NULL;
    int max = 0;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, contagens);
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        char *code = key;
        int count = *((int *)value);
        
        if (count > max || (count == max && melhor && strcmp(code, melhor) < 0)) {
            max = count;
            melhor = code;
        }
    }
    
    //constrói resultado
    char *resultado = NULL;
    
    if (!melhor || max == 0) {
        resultado = strdup("\n");
    } else {
        Aeroporto *a = g_hash_table_lookup(tabelaAeroportos, melhor);
        if (a) {
            //asprintf -> aloca automaticamente memória suficiente para o resultado
            int len = asprintf(&resultado, "%s, %s, %s, %s, %d\n",
                    airport_get_code_IATA(a), airport_get_name(a),
                    airport_get_city(a), airport_get_country(a), max);
            if (len == -1) {
                g_hash_table_destroy(contagens);
                return strdup("\n");
            }
        } else {
            resultado = strdup("\n");
        }
    }
    
    g_hash_table_destroy(contagens);
    
    return resultado ? resultado : strdup("\n");
}