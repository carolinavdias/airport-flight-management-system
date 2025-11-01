# makefile base - fase 1

CC = gcc
CFLAGS = -Wall -Wextra -pedantic -O2 -Iinclude $(shell pkg-config --cflags glib-2.0)
LDLIBS = $(shell pkg-config --libs glib-2.0)

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# todos os ficheiros fonte exceto o programa-testes
SRCS_MAIN = $(filter-out $(SRC_DIR)/programa-testes.c, $(wildcard $(SRC_DIR)/*.c))
OBJS_MAIN = $(SRCS_MAIN:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# todos os ficheiros fonte exceto o main
SRCS_TEST = $(filter-out $(SRC_DIR)/main.c $(SRC_DIR)/read.c, $(wildcard $(SRC_DIR)/*.c))
OBJS_TEST = $(SRCS_TEST:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

MAIN = $(BIN_DIR)/programa-principal
TESTS = $(BIN_DIR)/programa-testes

all: $(MAIN)

$(MAIN): $(OBJS_MAIN)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(TESTS): $(OBJS_TEST)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@ $(LDLIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

testes: $(TESTS)
	@echo "Programa de testes compilado em bin/programa-testes"

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

debug: CFLAGS += -g
debug: all

.PHONY: all clean debug testes
