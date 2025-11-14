# ===================================================
# Makefile — Fase 1 (arrumado para o AutoTester)
# ===================================================

CC       = gcc

# flags de compilação (sem -std=c99 porque o prof pediu para tirar)
CFLAGS   = -Wall -Wextra -O2 -Iinclude $(shell pkg-config --cflags glib-2.0)
LDLIBS   = $(shell pkg-config --libs glib-2.0) -lm

SRC_DIR  = src
OBJ_DIR  = obj
DATA_DIR = data-fase-1
OUT_DIR  = resultados
EXPECTED = resultados-esperados

# ---------------------------------------------------
# ficheiros .c principais
# ---------------------------------------------------
SRCS_ALL := $(wildcard $(SRC_DIR)/*.c) \
            $(wildcard $(SRC_DIR)/*/*.c)

# programa principal (sem o programa de testes)
SRCS_MAIN := programa-principal.c $(filter-out $(SRC_DIR)/programa-testes.c, $(SRCS_ALL))
OBJS_MAIN := $(SRCS_MAIN:%.c=$(OBJ_DIR)/%.o)

# programa de testes (sem o principal)
SRCS_TEST := programa-testes.c $(filter-out $(SRC_DIR)/programa-principal.c, $(SRCS_ALL))
OBJS_TEST := $(SRCS_TEST:%.c=$(OBJ_DIR)/%.o)

# executáveis (na pasta atual, como o AutoTester espera)
MAIN_BIN  = programa-principal
TESTS_BIN = programa-testes

# ===================================================
# alvos principais
# ===================================================
all: $(MAIN_BIN)
	@echo "✔ Programa principal compilado com sucesso!"

testes: $(TESTS_BIN)
	@echo "✔ Programa de testes compilado com sucesso!"

# ===================================================
# regras de ligação
# ===================================================
$(MAIN_BIN): $(OBJS_MAIN)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(TESTS_BIN): $(OBJS_TEST)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# ===================================================
# criação dos ficheiros .o
# ===================================================
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ===================================================
# comandos rápidos
# ===================================================

# executa o programa principal com o dataset e input padrão
run: $(MAIN_BIN)
	@mkdir -p $(OUT_DIR)
	@echo "A executar o programa principal..."
	@./$(MAIN_BIN) dataset inputs_fase1.txt
	@echo "✔ Execução terminada! Resultados guardados em $(OUT_DIR)/"

# compara os outputs com os resultados esperados
check:
	@echo "A comparar resultados..."
	@diff -qr $(OUT_DIR)/ $(EXPECTED)/ || true
	@echo "✔ Comparação concluída."

# limpeza e debug
clean:
	rm -rf $(OBJ_DIR) $(MAIN_BIN) $(TESTS_BIN) $(OUT_DIR)

debug: CFLAGS += -g
debug: all

.PHONY: all clean debug testes run check
