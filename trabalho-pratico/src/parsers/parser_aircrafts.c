#define _POSIX_C_SOURCE 200809L
#include "parsers/parser_aircrafts.h"

#include "entidades/aircrafts.h"
#include "validacoes/validacoes_aircrafts.h"
#include "csv.h"

#include <string.h>
#include <glib.h>

/*
typedef struct parsecontext {
    GestorAircrafts *gestor;
    ErrorWriter *error_writer;
    char *header;
    char original_line[20000];
} ParseContext;
*/

int le_tabela_Aeronaves(Contexto *ctx, GestorAircrafts *AC) {

    int MAX_LINHA = 2000;
    gchar buffer[MAX_LINHA];


    FILE *ficheiro = abrir_ficheiro(ctx, "aircrafts.csv", "r");
    if (ficheiro == NULL) return 0;

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
    	Aeronave *aeronave_atual = criaAeronave();

       	int linha_valida = 1;
        buffer[strcspn(buffer, "\n")] = '\0';

        char **campos = NULL;
        size_t n_campos = 0;
        if (csv_split(buffer, &campos, &n_campos) != 0) linha_valida = 0;

        if (linha_valida) aircraft_set_id(aeronave_atual,campos[0]);
        if (linha_valida) aircraft_set_manuf(aeronave_atual,campos[1]);
        if (linha_valida) aircraft_set_model_(aeronave_atual,campos[2]);

        if (linha_valida && valida_year(campos[3])) aircraft_set_year(aeronave_atual,campos[3]);
	else linha_valida = 0;

        if (linha_valida) aircraft_set_cap(aeronave_atual,campos[4]);
        if (linha_valida) aircraft_set_range_(aeronave_atual,campos[5]);

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
            libertaAeronave(aeronave_atual);
        } else {
            gestor_aircrafts_insere(AC, aeronave_atual);
        }
        if (campos) csv_free_fields(campos, n_campos);
    }

    if (ficheiro_erros) fclose(ficheiro_erros);
    fclose(ficheiro);
    return 1;
}

/*
static void processar_linha_aircraft(char **campos, size_t n_campos, void *user_data) {
    ParseContext *ctx = (ParseContext*)user_data;
    
    Aeronave *aeronave = calloc(1, sizeof(Aeronave));
    int linha_valida = 1;
    
    //valida e preenche
    if (linha_valida) 
        aeronave->identifier = g_strdup(campos[0]);
    if (linha_valida) 
        aeronave->manufacturer = g_strdup(campos[1]);
    if (linha_valida) 
        aeronave->model = g_strdup(campos[2]);
    if (linha_valida && !valida_year(campos[3], &aeronave->year)) 
        linha_valida = 0;
    if (linha_valida) 
        aeronave->capacity = atoi(campos[4]);
    if (linha_valida) 
        aeronave->range = atoi(campos[5]);
    
    //trata resultado
    if (!linha_valida) {
        //cria error writer no primeiro erro
        if (!ctx->error_writer) {
            ctx->error_writer = processador_output_create_error_file("aircrafts", ctx->header);
        }
        
        //reconstrui linha original
        snprintf(ctx->original_line, sizeof(ctx->original_line),
                 "%s;%s;%s;%s;%s;%s",
                 campos[0], campos[1], campos[2], campos[3], campos[4], campos[5]);
        
        processador_output_write_error(ctx->error_writer, ctx->original_line);
        libertaAeronave(aeronave);
    } else {
        gestor_aircrafts_insere(ctx->gestor, aeronave);
    }
}

int le_tabela_Aeronaves(Contexto ctx, GestorAircrafts *AC) {
    //lê header
    char header[20000];
    if (!parser_input_read_header(ctx.dataset_dir, "aircrafts.csv",  // ← ctx.dataset_dir (não &ctx)
                                  header, sizeof(header))) {
        return 0;
    }
    
    //prepara contexto
    ParseContext parse_ctx = {
        .gestor = AC,
        .error_writer = NULL,
        .header = header
    };
    
    //processa CSV
    int result = parser_input_read_csv(
        ctx.dataset_dir,              // ← ctx.dataset_dir (array, já é endereço)
        "aircrafts.csv",
        processar_linha_aircraft,
        &parse_ctx
    );
    
    //limpa
    if (parse_ctx.error_writer) {
        processador_output_close_error_file(parse_ctx.error_writer);
    }
    
    return result;
}
*/