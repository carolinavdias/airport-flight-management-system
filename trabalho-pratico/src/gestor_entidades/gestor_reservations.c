#include "gestor_entidades/gestor_reservations.h"
#include <glib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ===================================================
// ESTRUTURA 
// ===================================================

/**
 * Estrutura interna do gestor de reservas.
 *
 * Armazena todas as reservas numa hash table indexada pelo seu ID.
 * A tabela liberta automaticamente as chaves e as reservas armazenadas.
 */

struct gestor_reservations {
    GHashTable *cache_q4;     /**< semana -> (passageiro -> gasto total) */
    GHashTable *cache_top10;  /**< semana -> GSList* de doc_numbers no top10 */
    GHashTable *tabela;       /**< Hash table: id_reserva -> Reservas* */

};

// ============================================
// CRIA GESTOR DE RESERVAS
// ============================================ */

GestorReservations *gestor_reservations_cria(void) {
    GestorReservations *g = malloc(sizeof(struct gestor_reservations));
    if (!g) return NULL;
    g->tabela = g_hash_table_new_full(
        g_str_hash, 
        g_str_equal, 
        g_free,          // liberta chave
        libertaReserva   // liberta Reservas*
    );
    return g;
}

// =================================================== 
// OPERAÇÕES BÁSICAS 
// =================================================== 

void gestor_reservations_insere(GestorReservations *g, Reservas *reserva) {
    if (!g || !reserva) return;
    
    // r_get_id_reserva agora retorna cópia com g_strdup()
    char *id = r_get_id_reserva(reserva);
    if (!id) return;
    
    g_hash_table_insert(g->tabela, id, reserva);  // chave será libertada por g_free
}

int gestor_reservations_existe(GestorReservations *g, const char *id_reserva) {
    return g && id_reserva && g_hash_table_contains(g->tabela, id_reserva);
}

Reservas *gestor_reservations_procura(GestorReservations *g, const char *id_reserva) {
    if (!g || !id_reserva) return NULL;
    return g_hash_table_lookup(g->tabela, id_reserva);
}

// ============================================
// DESTRÓI GESTOR DE RESERVAS
// ============================================ */

void gestor_reservations_liberta(GestorReservations *g) {
    if (!g) return;
    if (g->cache_q4) g_hash_table_destroy(g->cache_q4);
    if (g->cache_top10) g_hash_table_destroy(g->cache_top10);
    if (g->tabela) {
        g_hash_table_destroy(g->tabela);
    }
    free(g);
}

/**
 * Função auxiliar usada na Q6.
 *
 * Percorre todas as reservas e devolve uma GSList* contendo as reservas
 * cujo id_pessoa_reservou coincide com o doc_number fornecido.
 */

GSList *gestor_reservations_get_by_passenger(GestorReservations *gr, const char *doc_number) {
    if (!gr || !doc_number) return NULL;

    GSList *lista = NULL;
    int id_pessoa = atoi(doc_number);  // converter string para int

    GHashTableIter iter;
    gpointer key, value;

    g_hash_table_iter_init(&iter, gr->tabela);

    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Reservas *r = (Reservas *)value;
        
        int id_reserva = r_get_id_pessoa_reservou(r);  // retorna int
        
        if (id_reserva == id_pessoa) {
            lista = g_slist_prepend(lista, r);
        }
    }

    return lista;
}

// ============================================
// NOVA FUNÇÃO PARA FASE 2 (encapsulamento)
// ============================================ 

void gestor_reservations_foreach(GestorReservations *g, ReservationIterFunc f, void *user_data) {
    if (!g || !g->tabela || !f) return;
    
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, g->tabela);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        f((Reservas *)value, user_data);
    }
}
/**
 * Conta quantas reservas incluem um determinado voo.
 *
 * Percorre todas as reservas e verifica se o flight_id aparece
 * na lista de voos reservados.
 */

int gestor_reservations_conta_por_voo(GestorReservations *g, const char *flight_id) {
    if (!g || !g->tabela || !flight_id) return 0;
    
    int count = 0;
    GHashTableIter iter;
    gpointer key, value;
    
    g_hash_table_iter_init(&iter, g->tabela);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        Reservas *r = (Reservas *)value;
        int n_voos = r_get_lista_n_voos(r);
        
        for (int i = 0; i < n_voos; i++) {
            char *voo_id = r_get_voo_por_indice(r, i);
            if (voo_id && strcmp(voo_id, flight_id) == 0) {
                count++;
                g_free(voo_id);
                break;
            }
            g_free(voo_id);
        }
    }
    
    return count;
}

/* ============================================
 * FUNÇÕES PARA CACHE Q4 (OTIMIZAÇÃO)
 * ============================================ */

// Função auxiliar para libertar hash table interna (passageiro -> gasto)
static void liberta_gastos_semana(gpointer data) {
    g_hash_table_destroy((GHashTable *)data);
}

void gestor_reservations_init_cache_q4(GestorReservations *g) {
    if (!g) return;
    // Estrutura: semana (GINT_TO_POINTER) -> hash table (doc_number -> double*)
    g->cache_q4 = g_hash_table_new_full(
        g_direct_hash, g_direct_equal,
        NULL,  // chave é inteiro, não precisa free
        liberta_gastos_semana
    );
}

void gestor_reservations_add_gasto_q4(GestorReservations *g, long id_semana, const char *doc_number, double preco) {
    if (!g || !g->cache_q4 || !doc_number) return;
    
    // Obter ou criar hash de gastos para esta semana
    GHashTable *gastos_semana = g_hash_table_lookup(g->cache_q4, GINT_TO_POINTER(id_semana));
    if (!gastos_semana) {
        gastos_semana = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
        g_hash_table_insert(g->cache_q4, GINT_TO_POINTER(id_semana), gastos_semana);
    }
    
    // Acumular gasto do passageiro
    double *gasto_atual = g_hash_table_lookup(gastos_semana, doc_number);
    if (!gasto_atual) {
        gasto_atual = g_new(double, 1);
        *gasto_atual = 0.0;
        g_hash_table_insert(gastos_semana, g_strdup(doc_number), gasto_atual);
    }
    *gasto_atual += preco;
}

void gestor_reservations_foreach_cache_q4(GestorReservations *g, CacheQ4IterFunc func, void *user_data) {
    if (!g || !g->cache_q4 || !func) return;
    
    GHashTableIter iter_semana;
    gpointer key_semana, value_semana;
    
    g_hash_table_iter_init(&iter_semana, g->cache_q4);
    while (g_hash_table_iter_next(&iter_semana, &key_semana, &value_semana)) {
        long id_semana = GPOINTER_TO_INT(key_semana);
        GHashTable *gastos = (GHashTable *)value_semana;
        
        GHashTableIter iter_pass;
        gpointer key_pass, value_pass;
        
        g_hash_table_iter_init(&iter_pass, gastos);
        while (g_hash_table_iter_next(&iter_pass, &key_pass, &value_pass)) {
            const char *doc_number = (const char *)key_pass;
            double gasto = *(double *)value_pass;
            func(id_semana, doc_number, gasto, user_data);
        }
    }
}

/* Estrutura auxiliar para ordenação */
typedef struct {
    char *doc_number;
    double gasto;
} GastoOrdenado;

static int compara_gastos_desc(const void *a, const void *b) {
    const GastoOrdenado *g1 = a;
    const GastoOrdenado *g2 = b;
    if (g1->gasto > g2->gasto) return -1;
    if (g1->gasto < g2->gasto) return 1;
    return strcmp(g1->doc_number, g2->doc_number);
}

static void liberta_lista_top10(gpointer data) {
    g_slist_free_full((GSList *)data, g_free);
}

void gestor_reservations_finaliza_cache_q4(GestorReservations *g) {
    if (!g || !g->cache_q4) return;
    
    // Criar cache_top10
    g->cache_top10 = g_hash_table_new_full(
        g_direct_hash, g_direct_equal,
        NULL, liberta_lista_top10
    );
    
    GHashTableIter iter_sem;
    gpointer key_sem, val_sem;
    g_hash_table_iter_init(&iter_sem, g->cache_q4);
    
    while (g_hash_table_iter_next(&iter_sem, &key_sem, &val_sem)) {
        long id_semana = GPOINTER_TO_INT(key_sem);
        GHashTable *gastos = (GHashTable *)val_sem;
        guint n = g_hash_table_size(gastos);
        if (n == 0) continue;
        
        // Converter para array
        GastoOrdenado *arr = g_new(GastoOrdenado, n);
        GHashTableIter iter_g;
        gpointer k, v;
        guint i = 0;
        g_hash_table_iter_init(&iter_g, gastos);
        while (g_hash_table_iter_next(&iter_g, &k, &v)) {
            arr[i].doc_number = (char *)k;
            arr[i].gasto = *(double *)v;
            i++;
        }
        
        // Ordenar
        qsort(arr, n, sizeof(GastoOrdenado), compara_gastos_desc);
        
        // Guardar top 10 como lista
        GSList *top10 = NULL;
        int limite = (n < 10) ? (int)n : 10;
        for (int j = limite - 1; j >= 0; j--) {
            top10 = g_slist_prepend(top10, g_strdup(arr[j].doc_number));
        }
        
        g_hash_table_insert(g->cache_top10, GINT_TO_POINTER(id_semana), top10);
        g_free(arr);
    }
}

void gestor_reservations_foreach_top10(GestorReservations *g, Top10IterFunc func, void *user_data) {
    if (!g || !g->cache_top10 || !func) return;
    
    GHashTableIter iter;
    gpointer key, value;
    g_hash_table_iter_init(&iter, g->cache_top10);
    
    while (g_hash_table_iter_next(&iter, &key, &value)) {
        long id_semana = GPOINTER_TO_INT(key);
        GSList *top10 = (GSList *)value;
        
        for (GSList *l = top10; l; l = l->next) {
            func(id_semana, (const char *)l->data, user_data);
        }
    }
}
