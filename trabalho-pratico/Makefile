# ===================================================
# Makefile — Fase 1
# ===================================================

CC       = gcc

# Professor pediu para mudar
 #CFLAGS   = -Wall -Wextra -pedantic -O2 -std=c99 -Iinclude $(shell pkg-config --cflags glib-2.0)
CFLAGS   = -Wall -Wextra -O2 -Iinclude $(shell pkg-config --cflags glib-2.0)
LDLIBS   = $(shell pkg-config --libs glib-2.0) -lm

SRC_DIR  = src
OBJ_DIR  = obj
BIN_DIR  = bin
DATA_DIR = data-fase-1
OUT_DIR  = resultados
EXPECTED = resultados-esperados

# ficheiros .c principais
SRCS_ALL := $(wildcard $(SRC_DIR)/*.c) \
            $(wildcard $(SRC_DIR)/*/*.c)

# programa principal (sem o programa de testes)
SRCS_MAIN := $(filter-out $(SRC_DIR)/programa-testes.c, $(SRCS_ALL))
OBJS_MAIN := $(SRCS_MAIN:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# programa de testes (sem o principal)
SRCS_TEST := $(filter-out $(SRC_DIR)/programa-principal.c, $(SRCS_ALL))
OBJS_TEST := $(SRCS_TEST:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# executáveis
MAIN_BIN  = $(BIN_DIR)/programa-principal
TESTS_BIN = $(BIN_DIR)/programa-testes

# ===================================================
# alvos principais
# ===================================================
all: $(MAIN_BIN)
	@echo "✔ Programa principal compilado com sucesso!"

testes: $(TESTS_BIN)
	@echo "✔ Programa de testes compilado com sucesso!"

# ===================================================
# regras de compilação
# ===================================================
$(MAIN_BIN): $(OBJS_MAIN)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(TESTS_BIN): $(OBJS_TEST)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# ===================================================
# criação dos ficheiros .o
# ===================================================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ===================================================
# comandos rápidos
# ===================================================

# executa o programa principal com o dataset e input padrão
run: $(MAIN_BIN)
	@mkdir -p $(OUT_DIR)
	@echo "A executar o programa principal..."
	@$(MAIN_BIN) $(DATA_DIR) inputs_fase1.txt
	@echo "✔ Execução terminada! Resultados guardados em $(OUT_DIR)/"

# compara os outputs com os resultados esperados
check:
	@echo "A comparar resultados..."
	@diff -qr $(OUT_DIR)/ $(EXPECTED)/ || true
	@echo "✔ Comparação concluída."

# limpeza e debug
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

debug: CFLAGS += -g
debug: all

.PHONY: all clean debug testes run check
