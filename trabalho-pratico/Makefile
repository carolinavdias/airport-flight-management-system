# ===================================================
# Makefile — Fase 1 (CORRIGIDO FINAL)
# ===================================================

CC       = gcc
CFLAGS   = -Wall -Wextra -O2 -Iinclude $(shell pkg-config --cflags glib-2.0)
LDLIBS   = $(shell pkg-config --libs glib-2.0) -lm

SRC_DIR  = src
OBJ_DIR  = obj

# ---------------------------------------------------
# Ficheiros fonte em src/
# ---------------------------------------------------
SRCS_LIB := $(wildcard $(SRC_DIR)/*.c)
OBJS_LIB := $(SRCS_LIB:$(SRC_DIR)/%.c=$(OBJ_DIR)/src/%.o)

# Programa principal
OBJS_MAIN := $(OBJ_DIR)/programa-principal.o $(OBJS_LIB)

# Programa de testes
OBJS_TEST := $(OBJ_DIR)/programa-testes.o $(OBJS_LIB)

# Executáveis
MAIN_BIN  = programa-principal
TESTS_BIN = programa-testes

# ===================================================
# Alvos
# ===================================================
.PHONY: all testes clean

all: $(MAIN_BIN)
	@echo "✔ Programa principal compilado com sucesso!"

testes: $(TESTS_BIN)
	@echo "✔ Programa de testes compilado com sucesso!"

# ===================================================
# Ligação
# ===================================================
$(MAIN_BIN): $(OBJS_MAIN)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(TESTS_BIN): $(OBJS_TEST)
	@$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# ===================================================
# Compilação
# ===================================================
# Ficheiros na raiz
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Ficheiros em src/
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# ===================================================
# Limpeza
# ===================================================
clean:
	@rm -rf $(OBJ_DIR) $(MAIN_BIN) $(TESTS_BIN) resultados
	@echo "✔ Limpeza concluída!"