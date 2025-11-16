#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <glib.h>
#include <ctype.h>
#include <unistd.h>
#include <stdint.h>
#include "read.h"
#include "validators.h"
#include "csv.h"
#include "q1.h"
#include "q2.h"
#include "q3.h"

#define MAX_LINHA 20000
gchar buffer[MAX_LINHA];

// =====================================================
// FUNÇÕES DE LIBERTAÇÃO DE MEMÓRIA
// =====================================================

void libertaAeronave(void *data) {
    Aeronave *a = data;
    if (!a) return;
    g_free(a->identifier);
    g_free(a->manufacturer);
    g_free(a->model);
    g_free(a);
}

void libertaAeroporto(void *data) {
    Aeroporto *a = data;
    if (!a) return;
    g_free(a->code_IATA);
    g_free(a->name);
    g_free(a->city);
    g_free(a->country);
    g_free(a->code_ICAO);
    g_free(a);
}

void libertaVoo(void *data) {
    Voo *v = data;
    if (!v) return;
    g_free(v->flight_id);
    g_free(v->gate);
    g_free(v->code_origin);
    g_free(v->code_destination);
    g_free(v->id_aircraft);
    g_free(v->airline);
    g_free(v->tracking_url);
    g_free(v);
}

void libertaPassageiro(void *data) {
    Passageiros *a = data;
    if (!a) return;
    g_free(a->primeiro_nome);
    g_free(a->ultimo_nome);
    g_free(a->nacionalidade);
    g_free(a->email_passageiro);
    g_free(a->telefone_passageiro);
    g_free(a->morada_passageiro);
    g_free(a->fotografia_passageiro);
    g_free(a);
}

void libertaReserva(void *data) {
    Reservas *a = data;
    if (!a) return;

    g_free(a->id_reserva);

    if (a->reserva_lista.lista_voos_reservados) {
        for (int i = 0; i < a->reserva_lista.n_voos; i++) {
            g_free(a->reserva_lista.lista_voos_reservados[i]);
        }
        free(a->reserva_lista.lista_voos_reservados);
    }

    g_free(a->lugar_reservado);
    g_free(a->qr_code);
    g_free(a);
}

// =====================================================
// FUNÇÕES AUXILIARES
// =====================================================

FILE *abrir_ficheiro(Contexto *ctx, const char *nome_ficheiro, const char *modo) {
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", ctx->dataset_dir, nome_ficheiro);
    FILE *ficheiro = fopen(path, modo);
    if (ficheiro == NULL) {
        perror("Erro ao abrir o ficheiro.\n");
        return NULL;
    }
    return ficheiro;
}

// =====================================================
// FUNÇÃO PRINCIPAL DE LEITURA
// =====================================================

int le_tabela(int opcao_inserida, Contexto ctx, GHashTable *tabela1, GHashTable *tabela2, GHashTable *tabela3, GHashTable *tabela4, GHashTable *tabela5) {

    if (opcao_inserida == 1) {
        FILE *ficheiro = abrir_ficheiro(&ctx, "flights.csv", "r");
        if (ficheiro == NULL) return 0;

        int linhas_totais = 0;
        int linhas_com_sucesso = 0;
        int no_header = 1;
        char header[MAX_LINHA];

        if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
        else {
            buffer[strcspn(buffer,"\n")] = '\0';
            strcpy(header, buffer);
        }

        FILE *ficheiro_erros = NULL;
        int header_escrito = 0;

        while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
            Voo *voo_atual = malloc(sizeof(Voo));
            linhas_totais++;
            int linha_valida = 1;
            int e_maybe = -1;
            buffer[strcspn(buffer, "\n")] = '\0';

            char **campos = NULL;
            size_t n_campos = 0;
            if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

            if (linha_valida && !valida_id_voo(campos[0], &voo_atual->flight_id)) linha_valida = 0;
            if (linha_valida && !valida_DataH(campos[1], &voo_atual->departure)) linha_valida = 0;
            if (linha_valida) {
                if (strcmp(campos[2], "N/A") == 0) {
                    e_maybe = 2;
                    voo_atual->actual_departure = -2;
                } else if (!valida_DataH(campos[2], &voo_atual->actual_departure)) linha_valida = 0;
            }
            if (linha_valida && !valida_DataH(campos[3], &voo_atual->arrival)) linha_valida = 0;
            if (linha_valida) {
                if (e_maybe == 2) {
                    if (strcmp(campos[4], "N/A") != 0) linha_valida = 0;
                    else voo_atual->actual_arrival = -2;
                } else if (!valida_DataH(campos[4], &voo_atual->actual_arrival)) linha_valida = 0;
            }
            if (linha_valida) voo_atual->gate = g_strdup(campos[5]);
            if (linha_valida) {
                if (!valida_Estado(campos[6], &voo_atual->status)) linha_valida = 0;
                if (voo_atual->status == 2 && e_maybe != 2) linha_valida = 0;
            }
            if (linha_valida && !valida_codigoIATA(campos[7], &voo_atual->code_origin)) linha_valida = 0;
            if (linha_valida && !valida_codigoIATA(campos[8], &voo_atual->code_destination)) linha_valida = 0;
            if (linha_valida) voo_atual->id_aircraft = g_strdup(campos[9]);
            if (linha_valida) voo_atual->airline = g_strdup(campos[10]);
            if (linha_valida) voo_atual->tracking_url = g_strdup(campos[11]);

            // Validação lógica
            if (linha_valida && !valida_VOO(*voo_atual, tabela3)) linha_valida = 0;

            if (!linha_valida) {
                if (!ficheiro_erros) {
                    ficheiro_erros = fopen("resultados/flights_errors.csv", "w");
                    if (ficheiro_erros && !header_escrito) {
                        fputs(header, ficheiro_erros);
                        fputc('\n', ficheiro_erros);
                        header_escrito = 1;
                    }
                }
                if (ficheiro_erros) {
                    fputs(buffer, ficheiro_erros);
                    fputc('\n', ficheiro_erros);
                }
                free(voo_atual);
            } else {
                linhas_com_sucesso++;
                g_hash_table_insert(tabela1, g_strdup(voo_atual->flight_id), voo_atual);
            }
            if (campos) csv_free_fields(campos, n_campos);
        }

        if (ficheiro_erros) fclose(ficheiro_erros);
        fclose(ficheiro);
        return 1;

    } else if (opcao_inserida == 2) {
        FILE *ficheiro = abrir_ficheiro(&ctx, "airports.csv", "r");
        if (ficheiro == NULL) return 0;
        char header[MAX_LINHA];

        int linhas_totais = 0, linhas_com_sucesso = 0, no_header = 1;
        if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
        else {
            buffer[strcspn(buffer,"\n")] = '\0';
            strcpy(header, buffer);
        }

        FILE *ficheiro_erros = NULL;
        int header_escrito = 0;

        while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
            Aeroporto *aeroporto_atual = malloc(sizeof(Aeroporto));
            aeroporto_atual->code_IATA = NULL;
            aeroporto_atual->name = NULL;
            aeroporto_atual->city = NULL;
            aeroporto_atual->country = NULL;
            aeroporto_atual->latitude = 0.0;
            aeroporto_atual->longitude = 0.0;
            aeroporto_atual->code_ICAO = NULL;
            aeroporto_atual->type = TIPO_ERROR;

            linhas_totais++;
            int linha_valida = 1;
            buffer[strcspn(buffer, "\n")] = '\0';

            char **campos = NULL;
            size_t n_campos = 0;
            if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

            if (linha_valida && !valida_codigoIATA(campos[0], &aeroporto_atual->code_IATA)) linha_valida = 0;
            if (linha_valida) aeroporto_atual->name = g_strdup(campos[1]);
            if (linha_valida) aeroporto_atual->city = g_strdup(campos[2]);
            if (linha_valida) aeroporto_atual->country = g_strdup(campos[3]);
            if (linha_valida && !valida_coordenadas(campos[4],1,&aeroporto_atual->latitude)) linha_valida = 0;
            if (linha_valida && !valida_coordenadas(campos[5],2,&aeroporto_atual->longitude)) linha_valida = 0;
            if (linha_valida) aeroporto_atual->code_ICAO = g_strdup(campos[6]);
            if (linha_valida && !valida_tipo_aer(campos[7], &aeroporto_atual->type)) linha_valida = 0;

            if (!linha_valida) {
                if (!ficheiro_erros) {
                    ficheiro_erros = fopen("resultados/airports_errors.csv", "w");
                    if (ficheiro_erros && !header_escrito) {
                        fputs(header, ficheiro_erros);
                        fputc('\n', ficheiro_erros);
                        header_escrito = 1;
                    }
                }
                if (ficheiro_erros) {
                    fputs(buffer, ficheiro_erros);
                    fputc('\n', ficheiro_erros);
                }
                free(aeroporto_atual);
            } else {
                linhas_com_sucesso++;
                g_hash_table_insert(tabela2, g_strdup(aeroporto_atual->code_IATA), aeroporto_atual);
            }
            if (campos) csv_free_fields(campos, n_campos);
        }
        if (ficheiro_erros) fclose(ficheiro_erros);
        fclose(ficheiro);
        return 1;

    } else if (opcao_inserida == 3) {
        FILE *ficheiro = abrir_ficheiro(&ctx, "aircrafts.csv", "r");
        if (ficheiro == NULL) return 0;
        char header[MAX_LINHA];

        int linhas_totais = 0, linhas_com_sucesso = 0, no_header = 1;
        if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
        else {
            buffer[strcspn(buffer,"\n")] = '\0';
            strcpy(header, buffer);
        }

        FILE *ficheiro_erros = NULL;
        int header_escrito = 0;

        while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
            Aeronave *aeronave_atual = malloc(sizeof(Aeronave));
            aeronave_atual->identifier = NULL;
            aeronave_atual->manufacturer = NULL;
            aeronave_atual->model = NULL;
            aeronave_atual->year = -1;
            aeronave_atual->capacity = -1;
            aeronave_atual->range = -1;

            linhas_totais++;
            int linha_valida = 1;
            buffer[strcspn(buffer, "\n")] = '\0';

            char **campos = NULL;
            size_t n_campos = 0;
            if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

            if (linha_valida) aeronave_atual->identifier = g_strdup(campos[0]);
            if (linha_valida) aeronave_atual->manufacturer = g_strdup(campos[1]);
            if (linha_valida) aeronave_atual->model = g_strdup(campos[2]);
            if (linha_valida && !valida_year(campos[3], &aeronave_atual->year)) linha_valida = 0;
            if (linha_valida) aeronave_atual->capacity = atoi(campos[4]);
            if (linha_valida) aeronave_atual->range = atoi(campos[5]);

            if (!linha_valida) {
                if (!ficheiro_erros) {
                    ficheiro_erros = fopen("resultados/aircrafts_errors.csv", "w");
                    if (ficheiro_erros && !header_escrito) {
                        fputs(header, ficheiro_erros);
                        fputc('\n', ficheiro_erros);
                        header_escrito = 1;
                    }
                }
                if (ficheiro_erros) {
                    fputs(buffer, ficheiro_erros);
                    fputc('\n', ficheiro_erros);
                }
                free(aeronave_atual);
            } else {
                linhas_com_sucesso++;
                g_hash_table_insert(tabela3, g_strdup(aeronave_atual->identifier), aeronave_atual);
            }
            if (campos) csv_free_fields(campos, n_campos);
        }

        if (ficheiro_erros) fclose(ficheiro_erros);
        fclose(ficheiro);
        return 1;

    } else if (opcao_inserida == 4) {
        FILE *ficheiro = abrir_ficheiro(&ctx, "passengers.csv", "r");
        if (ficheiro == NULL) return 0;
        char header[MAX_LINHA];

        int linhas_totais = 0, linhas_com_sucesso = 0, no_header = 1;
        if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
        else {
            buffer[strcspn(buffer,"\n")] = '\0';
            strcpy(header, buffer);
        }

        FILE *ficheiro_erros = NULL;
        int header_escrito = 0;

        while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
            Passageiros *passageiro_atual = malloc(sizeof(Passageiros));
            passageiro_atual->id_passageiro = -1;
            passageiro_atual->primeiro_nome = NULL;
            passageiro_atual->ultimo_nome = NULL;
            passageiro_atual->nacionalidade = NULL;
            passageiro_atual->email_passageiro = NULL;
            passageiro_atual->telefone_passageiro = NULL;
            passageiro_atual->morada_passageiro = NULL;
            passageiro_atual->fotografia_passageiro = NULL;

            linhas_totais++;
            int linha_valida = 1;
            buffer[strcspn(buffer, "\n")] = '\0';

            char **campos = NULL;
            size_t n_campos = 0;
            if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

            if (linha_valida && !valida_id_passageiro(campos[0], &passageiro_atual->id_passageiro)) linha_valida = 0;
            if (linha_valida) passageiro_atual->primeiro_nome = g_strdup(campos[1]);
            if (linha_valida) passageiro_atual->ultimo_nome = g_strdup(campos[2]);
            if (linha_valida && !valida_Data(campos[3], &passageiro_atual->data_nascimento)) linha_valida = 0;
            if (linha_valida) passageiro_atual->nacionalidade = g_strdup(campos[4]);
            if (linha_valida && !valida_genero(campos[5], &passageiro_atual->genero_passageiro)) linha_valida = 0;
            if (linha_valida && !valida_email(campos[6], &passageiro_atual->email_passageiro)) linha_valida = 0;
            if (linha_valida) passageiro_atual->telefone_passageiro = g_strdup(campos[7]);
            if (linha_valida) passageiro_atual->morada_passageiro = g_strdup(campos[8]);
            if (linha_valida) passageiro_atual->fotografia_passageiro = g_strdup(campos[9]);

            if (!linha_valida) {
                if (!ficheiro_erros) {
                    ficheiro_erros = fopen("resultados/passengers_errors.csv", "w");
                    if (ficheiro_erros && !header_escrito) {
                        fputs(header, ficheiro_erros);
                        fputc('\n', ficheiro_erros);
                        header_escrito = 1;
                    }
                }
                if (ficheiro_erros) {
                    fputs(buffer, ficheiro_erros);
                    fputc('\n', ficheiro_erros);
                }
                free(passageiro_atual);
            } else {
                linhas_com_sucesso++;
                g_hash_table_insert(tabela4, GINT_TO_POINTER(passageiro_atual->id_passageiro), passageiro_atual);
            }
            if (campos) csv_free_fields(campos, n_campos);

        }
        if (ficheiro_erros) fclose(ficheiro_erros);
        fclose(ficheiro);
        return 1;

    } else if (opcao_inserida == 5) {
        FILE *ficheiro = abrir_ficheiro(&ctx, "reservations.csv", "r");
        if (ficheiro == NULL) return 0;
        char header[MAX_LINHA];

        int linhas_totais = 0, linhas_com_sucesso = 0, no_header = 1;
        if (fgets(buffer, sizeof(buffer), ficheiro) == NULL) no_header = 0;
        else {
            buffer[strcspn(buffer,"\n")] = '\0';
            strcpy(header, buffer);
        }

        FILE *ficheiro_erros = NULL;
        int header_escrito = 0;

        while (fgets(buffer, sizeof(buffer), ficheiro) && no_header) {
            Reservas *reserva_atual = malloc(sizeof(Reservas));
            reserva_atual->id_reserva = NULL;
            reserva_atual->lugar_reservado = NULL;
            reserva_atual->preco_reserva = -1.0;
            reserva_atual->qr_code = NULL;

            linhas_totais++;
            int linha_valida = 1;
            buffer[strcspn(buffer, "\n")] = '\0';

            char **campos = NULL;
            size_t n_campos = 0;
            if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

            if (linha_valida && !valida_id_reserva(campos[0], &reserva_atual->id_reserva)) linha_valida = 0;
            if (linha_valida && !valida_voos_reservados(campos[1], &reserva_atual->reserva_lista)) linha_valida = 0;
            if (linha_valida) reserva_atual->id_pessoa_reservou = atoi(campos[2]);
            if (linha_valida) reserva_atual->lugar_reservado = g_strdup(campos[3]);
            if (linha_valida) reserva_atual->preco_reserva = atof(campos[4]);
            if (linha_valida && !valida_bool(campos[5], &reserva_atual->bagagem_extra)) linha_valida = 0;
            if (linha_valida && !valida_bool(campos[6], &reserva_atual->prioridade)) linha_valida = 0;
            if (linha_valida) reserva_atual->qr_code = g_strdup(campos[7]);
            if (linha_valida && !valida_RESERVA(*reserva_atual, tabela1, tabela4)) linha_valida = 0;

            if (!linha_valida) {
                if (!ficheiro_erros) {
                    ficheiro_erros = fopen("resultados/reservations_errors.csv", "w");
                    if (ficheiro_erros && !header_escrito) {
                        fputs(header, ficheiro_erros);
                        fputc('\n', ficheiro_erros);
                        header_escrito = 1;
                    }
                }
                if (ficheiro_erros) {
                    fputs(buffer, ficheiro_erros);
                    fputc('\n', ficheiro_erros);
                }
                free(reserva_atual);
            } else {
                linhas_com_sucesso++;
                g_hash_table_insert(tabela5, g_strdup(reserva_atual->id_reserva), reserva_atual);
            }
            if (campos) csv_free_fields(campos, n_campos);

        }
        if (ficheiro_erros) fclose(ficheiro_erros);
        fclose(ficheiro);
        return 1;
    }

    return 0;
}