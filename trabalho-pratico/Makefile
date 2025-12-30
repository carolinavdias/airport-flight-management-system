# ===================================================
# Makefile — Fase 1 (alteração para suportar a estrutura modular )
# ===================================================

CC       = gcc
CFLAGS   = -Wall -Wextra -O2 -D_GNU_SOURCE -Iinclude $(shell pkg-config --cflags glib-2.0)
LDLIBS   = $(shell pkg-config --libs glib-2.0) -lm

SRC_DIR  = src
OBJ_DIR  = obj

# ---------------------------------------------------
# Ficheiros fonte em src/ (TODOS os subdiretorios!)
# ---------------------------------------------------
SRCS_LIB := $(shell find $(SRC_DIR) -name '*.c')
OBJS_LIB := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/src/%.o,$(SRCS_LIB))

# Programa principal
OBJS_MAIN := $(OBJ_DIR)/programas/programa-principal.o $(OBJS_LIB)

# Programa de testes
OBJS_TEST := $(OBJ_DIR)/programas/programa-testes.o $(OBJS_LIB)

# Programa interativo
OBJS_INTER := $(OBJ_DIR)/programas/programa-interativo.o $(OBJS_LIB)

# Executáveis
MAIN_BIN  = programa-principal
TESTS_BIN = programa-testes
INTER_BIN = programa-interativo

# ===================================================
# Alvos
# ===================================================
.PHONY: all testes clean

all: $(MAIN_BIN) $(INTER_BIN)
	echo "✔ Programas compilados com sucesso!"

testes: $(TESTS_BIN)
	echo "✔ Programa de testes compilado com sucesso!"

# ===================================================
# Ligação
# ===================================================
$(MAIN_BIN): $(OBJS_MAIN)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(TESTS_BIN): $(OBJS_TEST)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(INTER_BIN): $(OBJS_INTER)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

# ===================================================
# Compilação
# ===================================================
# Ficheiros em programas/
$(OBJ_DIR)/programas/%.o: programas/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Ficheiros em src/ (incluindo subdiretorios!)
$(OBJ_DIR)/src/%.o: $(SRC_DIR)/%.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ===================================================
# Limpeza
# ===================================================
clean:
	rm -rf $(OBJ_DIR) $(MAIN_BIN) $(TESTS_BIN) $(INTER_BIN) resultados
	echo "✔ Limpeza concluída!"


# ===================================================
# Nota técnica:
# SRCS_LIB usa 'find' recursivo para suportar
# a estrutura modular (parsers/, gestores/, etc.)