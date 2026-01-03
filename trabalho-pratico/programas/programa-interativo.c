#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>

#include "queries/q1.h"
#include "queries/q2.h"
#include "queries/q3.h"
#include "queries/q4.h"
#include "queries/q5.h"
#include "queries/q6.h"

#include "parsers/parser.h"
#include "parsers/parser_flights.h"
#include "parsers/parser_airports.h"
#include "parsers/parser_aircrafts.h"
#include "parsers/parser_passengers.h"
#include "parsers/parser_reservations.h"

#include "gestor_entidades/gestor_flights.h"
#include "gestor_entidades/gestor_airports.h"
#include "gestor_entidades/gestor_aircrafts.h"
#include "gestor_entidades/gestor_passengers.h"
#include "gestor_entidades/gestor_reservations.h"
#include "gestor_entidades/gestor.h"

#include "utils/utils.h"

#include "IO/processador_input_interativo.h"

/**
 * Cores para o terminal.
 */

#define COLOR_RESET   "\x1b[0m"
#define COLOR_GREEN   "\x1b[32m"
#define COLOR_YELLOW  "\x1b[33m"
#define COLOR_BLUE    "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN    "\x1b[36m"
#define COLOR_RED     "\x1b[31m"

/**
 * Estado global dos gestores.
 */

typedef struct {
    GestorAirports *gestorAeroportos;
    GestorFlights *gestorVoos;
    GestorAircrafts *gestorAeronaves;
    GestorPassengers *gestorPassageiros;
    GestorReservations *gestorReservas;
    int dados_carregados;
} EstadoPrograma;

/**
 * Limpa buffer de entrada.
 */

static void limpa_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
 * Lê linha de forma segura.
 */

static char *le_linha() {
    char *linha = NULL;
    size_t tam = 0;
    ssize_t len = getline(&linha, &tam, stdin);
    
    if (len > 0 && linha[len-1] == '\n') {
        linha[len-1] = '\0';
    }
    
    return linha;
}

/**
 * Mostra banner inicial.
 */

static void mostra_banner() {
    printf("\n");
    printf(COLOR_CYAN "╔═══════════════════════════════════════════╗\n" COLOR_RESET);
    printf(COLOR_CYAN "║" COLOR_RESET "     " COLOR_MAGENTA "Sistema de Gestão de Aeroportos" COLOR_RESET "       " COLOR_CYAN "║\n" COLOR_RESET);
    printf(COLOR_CYAN "║" COLOR_RESET "               " COLOR_YELLOW "Projeto LI3" COLOR_RESET "                 " COLOR_CYAN "║\n" COLOR_RESET);
    printf(COLOR_CYAN "╚═══════════════════════════════════════════╝\n" COLOR_RESET);
    printf("\n");
}

/**
 * Mostra menu principal.
 */

static void mostra_menu() {
    printf("\n" COLOR_BLUE "═══════════════════ MENU ═══════════════════\n" COLOR_RESET);
    printf(COLOR_GREEN "  1" COLOR_RESET " - Query 1: Resumo de aeroporto\n");
    printf(COLOR_GREEN "  2" COLOR_RESET " - Query 2: Top N aeronaves\n");
    printf(COLOR_GREEN "  3" COLOR_RESET " - Query 3: Aeroporto com mais partidas\n");
    printf(COLOR_GREEN "  4" COLOR_RESET " - Query 4: Passageiro que esteve mais tempo no Top 10 dos que gastaram mais em reservas num período de tempo\n");
    printf(COLOR_GREEN "  5" COLOR_RESET " - Query 5: Top N companhias aéreas\n");
    printf(COLOR_GREEN "  6" COLOR_RESET " - Query 6: Destino mais comum por nacionalidade\n");
    printf(COLOR_YELLOW "  r" COLOR_RESET " - Recarregar dataset\n");
    printf(COLOR_RED "  q" COLOR_RESET " - Sair\n");
    printf(COLOR_BLUE "═══════════════════════════════════════════════\n" COLOR_RESET);
    printf("\n" COLOR_CYAN "Escolha uma opção: " COLOR_RESET);
}

/**
 * Carrega dataset.
 */

static int carrega_dataset(EstadoPrograma *estado, const char *caminho) {
    printf(COLOR_YELLOW "\n⏳ A carregar dados de: %s\n" COLOR_RESET, caminho);
    
    Contexto *ctx = cria_contexto();
    if (!ctx) {
        printf(COLOR_RED "❌ Erro ao criar contexto!\n" COLOR_RESET);
        return 0;
    }
    
    set_contexto(ctx, caminho);
    errors_begin();
    
    //cria gestores
    estado->gestorAeronaves = gestor_aircrafts_cria();
    estado->gestorVoos = gestor_flights_novo();
    estado->gestorAeroportos = gestor_airports_cria();
    estado->gestorPassageiros = gestor_passengers_novo();
    estado->gestorReservas = gestor_reservations_cria();
    
    if (!estado->gestorAeroportos || !estado->gestorAeronaves || 
        !estado->gestorVoos || !estado->gestorPassageiros || !estado->gestorReservas) {
        printf(COLOR_RED "❌ Erro ao criar gestores!\n" COLOR_RESET);
        errors_end();
        return 0;
    }
    
    //carrega dados
    read_csv(ctx, estado->gestorVoos, estado->gestorAeroportos, 
          estado->gestorAeronaves, estado->gestorPassageiros, estado->gestorReservas);
    
    errors_end();
    free(ctx);
    
    printf(COLOR_GREEN "✓ Dados carregados com sucesso!\n" COLOR_RESET);
    estado->dados_carregados = 1;
    return 1;
}

/**
 * Executa Query 1.
 */

static void executa_query1(EstadoPrograma *estado) {
    printf(COLOR_CYAN "\n╔═══ Query 1: Resumo de Aeroporto ═══╗\n" COLOR_RESET);
    printf("Digite o código IATA do aeroporto: ");
    
    char *codigo = le_linha();
    if (!codigo || codigo[0] == '\0') {
        printf(COLOR_RED "⚠ Código inválido!\n" COLOR_RESET);
        free(codigo);
        return;
    }
    
    char *resultado = query1(codigo, estado->gestorAeroportos, 
                            estado->gestorVoos, estado->gestorReservas);
    
    if (resultado && resultado[0] != '\n') {
        printf(COLOR_GREEN "\n✓ Resultado:\n" COLOR_RESET);
        printf("%s", resultado);
    } else {
        printf(COLOR_YELLOW "\n⚠ Aeroporto não encontrado ou sem dados.\n" COLOR_RESET);
    }
    
    free(resultado);
    free(codigo);
}

/**
 * Executa Query 2.
 */

static void executa_query2(EstadoPrograma *estado) {
    printf(COLOR_CYAN "\n╔═══ Query 2: Top N Aeronaves ═══╗\n" COLOR_RESET);
    printf("Digite N (número de aeronaves): ");
    
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf(COLOR_RED "⚠ Número inválido!\n" COLOR_RESET);
        limpa_buffer();
        return;
    }
    limpa_buffer();
    
    printf("Fabricante (opcional, Enter para todos): ");
    char *fabricante = le_linha();
    
    char comando[512];
    if (fabricante && fabricante[0] != '\0') {
        snprintf(comando, sizeof(comando), "%d %s", n, fabricante);
    } else {
        snprintf(comando, sizeof(comando), "%d", n);
    }
    
    char *resultado = query2(comando, estado->gestorAeronaves, estado->gestorVoos);
    
    if (resultado && resultado[0] != '\n') {
        printf(COLOR_GREEN "\n✓ Resultado:\n" COLOR_RESET);
        printf("%s", resultado);
    } else {
        printf(COLOR_YELLOW "\n⚠ Nenhum resultado encontrado.\n" COLOR_RESET);
    }
    
    free(resultado);
    free(fabricante);
}

/**
 * Executa Query 3.
 */

static void executa_query3(EstadoPrograma *estado) {
    printf(COLOR_CYAN "\n╔═══ Query 3: Aeroporto com Mais Partidas ═══╗\n" COLOR_RESET);
    printf("Data início (YYYY-MM-DD): ");
    
    char *data_inicio = le_linha();
    if (!data_inicio || data_inicio[0] == '\0') {
        printf(COLOR_RED "⚠ Data inválida!\n" COLOR_RESET);
        free(data_inicio);
        return;
    }
    
    printf("Data fim (YYYY-MM-DD): ");
    char *data_fim = le_linha();
    if (!data_fim || data_fim[0] == '\0') {
        printf(COLOR_RED "⚠ Data inválida!\n" COLOR_RESET);
        free(data_inicio);
        free(data_fim);
        return;
    }
    
    //adiciona hora
    char inicio_completo[64], fim_completo[64];
    snprintf(inicio_completo, sizeof(inicio_completo), "%s 00:00", data_inicio);
    snprintf(fim_completo, sizeof(fim_completo), "%s 23:59", data_fim);
    
    char *resultado = query3(inicio_completo, fim_completo, 
                            estado->gestorVoos, estado->gestorAeroportos);
    
    if (resultado && resultado[0] != '\n') {
        printf(COLOR_GREEN "\n✓ Resultado:\n" COLOR_RESET);
        printf("%s", resultado);
    } else {
        printf(COLOR_YELLOW "\n⚠ Nenhum voo encontrado nesse período.\n" COLOR_RESET);
    }
    
    free(resultado);
    free(data_inicio);
    free(data_fim);
}

/**
 * /Executa Query 4.
 */

static void executa_query4(EstadoPrograma *estado) {
    printf(COLOR_CYAN "\n╔═══ Query 4: Passageiro que esteve mais tempo no Top 10 dos que gastaram mais em reservas num período de tempo\n" COLOR_RESET);

    printf("Data início (YYYY-MM-DD)\n(opcional, Enter para todos): ");
    
    char *data_inicio = le_linha();
    //limpa_buffer();
/*
    if (!data_inicio || data_inicio[0] == '\0') {
        printf(COLOR_RED "⚠ Data inválida!\n" COLOR_RESET);
        free(data_inicio);
        return; */

    char *data_fim = NULL;
    char comando[512];

    if (data_inicio && data_inicio[0] != '\0') {
	printf("Data fim (YYYY-MM-DD): ");
    	char *data_fim = le_linha();
        if (!data_fim || data_fim[0] == '\0') {
            printf(COLOR_RED "⚠ Data inválida!\n" COLOR_RESET);
            free(data_inicio);
            return;
        }
	else {
	    snprintf(comando, sizeof(comando), "%s %s", data_inicio, data_fim);
   	}
    }
/*
	 &&
	data_fim    && data_fim[0]    != '\0') {

    	snprintf(comando, sizeof(comando), "%s %s", data_inicio, data_fim);
    } else {
	//snprintf(comando, sizeof(comando), "");
    }
  */  
    char *resultado = query4(comando, estado->gestorPassageiros, estado->gestorVoos, estado->gestorReservas);
    
    if (resultado && resultado[0] != '\n') {
        printf(COLOR_GREEN "\n✓ Resultado:\n" COLOR_RESET);
        printf("%s", resultado);
    } else {
        printf(COLOR_YELLOW "\n⚠ Nenhum resultado encontrado.\n" COLOR_RESET);
    }
    
    free(resultado);
    if (data_inicio) free(data_inicio);
    if (data_fim) free(data_fim);
}

/**
 * Executa Query 5.
 */

static void executa_query5(EstadoPrograma *estado) {
    printf(COLOR_CYAN "\n╔═══ Query 5: Top N Companhias Aéreas ═══╗\n" COLOR_RESET);
    printf("Digite N (número de companhias aéreas): ");
    
    int n;
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf(COLOR_RED "⚠ Número inválido!\n" COLOR_RESET);
        limpa_buffer();
        return;
    }
    
    char comando[512];
    snprintf(comando, sizeof(comando), "%d", n);

    char *resultado = query5(comando, estado->gestorVoos);
    
    if (resultado && resultado[0] != '\n') {
        printf(COLOR_GREEN "\n✓ Resultado:\n" COLOR_RESET);
        printf("%s", resultado);
    } else {
        printf(COLOR_YELLOW "\n⚠ Nenhum resultado encontrado.\n" COLOR_RESET);
    }
    
    free(resultado);
}

/**
 * Executa Query 6.
 */

static void executa_query6(EstadoPrograma *estado) {
    printf(COLOR_CYAN "\n╔═══ Query 6: Destino Mais Comum ═══╗\n" COLOR_RESET);
    printf("Digite a nacionalidade: ");
    
    char *nacionalidade = le_linha();
    if (!nacionalidade || nacionalidade[0] == '\0') {
        printf(COLOR_RED "⚠ Nacionalidade inválida!\n" COLOR_RESET);
        free(nacionalidade);
        return;
    }
    
    char *resultado = query6(nacionalidade, estado->gestorPassageiros, estado->gestorVoos, estado->gestorReservas);
    
    if (resultado && resultado[0] != '\n') {
        printf(COLOR_GREEN "\n✓ Resultado:\n" COLOR_RESET);
        printf("%s", resultado);
    } else {
        printf(COLOR_YELLOW "\n⚠ Não foi encontrado qualquer passageiro encontrado com essa nacionalidade.\n" COLOR_RESET);
    }
    
    free(resultado);
    free(nacionalidade);
}

/**
 * Liberta recursos
 */

static void liberta_recursos(EstadoPrograma *estado) {
    if (!estado->dados_carregados) return;
    
    if (estado->gestorAeronaves) gestor_aircrafts_liberta(estado->gestorAeronaves);
    if (estado->gestorVoos) gestor_flights_destroy(estado->gestorVoos);
    if (estado->gestorAeroportos) gestor_airports_liberta(estado->gestorAeroportos);
    if (estado->gestorPassageiros) gestor_passengers_destroy(estado->gestorPassageiros);
    if (estado->gestorReservas) gestor_reservations_liberta(estado->gestorReservas);
    
    estado->dados_carregados = 0;
}

/**
 * Limpa o terminal após execução de cada query.
 */

static void limpa_terminal() {
    printf("\033[2J\033[H");
    fflush(stdout);
}

/**
 * Função main
 */

int main() {
    EstadoPrograma estado = {0};
    
    mostra_banner();
    
    //pede caminho do dataset
    printf(COLOR_CYAN "Digite o caminho do dataset " COLOR_RESET);
    printf(COLOR_YELLOW "(Enter para './dataset'): " COLOR_RESET);
    
    char *caminho = le_linha();
    if (!caminho || caminho[0] == '\0') {
        free(caminho);
        caminho = strdup("./dataset");
        printf(COLOR_GREEN "✓ A usar caminho padrão: %s\n" COLOR_RESET, caminho);
    }
    
    //carrega dados
    if (!carrega_dataset(&estado, caminho)) {
        printf(COLOR_RED "\n❌ Falha ao carregar dataset. A encerrar...\n" COLOR_RESET);
        free(caminho);
        return EXIT_FAILURE;
    }
    
    free(caminho);
    
    //loop principal
    int continuar = 1;
    while (continuar) {
        mostra_menu();
        
        char opcao[10];
        if (!fgets(opcao, sizeof(opcao), stdin)) break;
        
        //remove newline
        opcao[strcspn(opcao, "\n")] = '\0';
        
        if (strlen(opcao) == 0) continue;
        
        switch (opcao[0]) {
            case '1':
                executa_query1(&estado);
                break;
                
            case '2':
                executa_query2(&estado);
                break;
                
            case '3':
                executa_query3(&estado);
                break;

            case '4':
                executa_query4(&estado);
                break;

            case '5':
                executa_query5(&estado);
                break;

            case '6':
                executa_query6(&estado);
                break;
                
            case 'r':
            case 'R':
                printf(COLOR_CYAN "\nDigite o novo caminho do dataset: " COLOR_RESET);
                caminho = le_linha();
                if (caminho && caminho[0] != '\0') {
                    liberta_recursos(&estado);
                    carrega_dataset(&estado, caminho);
                }
                free(caminho);
                break;
                
            case 'q':
            case 'Q':
                continuar = 0;
                break;
                
            default:
                printf(COLOR_RED "\n⚠ Opção inválida! Tente novamente.\n" COLOR_RESET);
                break;
        }
        
        if (continuar) {
            printf(COLOR_YELLOW "\n\nPressione Enter para continuar..." COLOR_RESET);
            getchar();
            limpa_terminal();
        }
    }
    
    //limpa
    printf(COLOR_YELLOW "\n👋 A encerrar programa...\n" COLOR_RESET);
    liberta_recursos(&estado);
    printf(COLOR_GREEN "✓ Recursos libertados. Até à próxima!\n" COLOR_RESET);
    
    return EXIT_SUCCESS;
}
