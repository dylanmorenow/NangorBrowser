BUILD_DIR := build
TARGET    := $(BUILD_DIR)/alprog1
CC        ?= gcc
CFLAGS    ?= -std=c99 -Wall -Wextra -Wpedantic -Iheader

# Semua .c di src/ KECUALI src/test/
SRC := $(shell find src -maxdepth 1 -name "*.c")

# Semua .c untuk test (src/test/ + semua ADT, tanpa main.c)
TEST_SRC := $(shell find src/test -name "*.c") \
            $(filter-out src/main.c, $(SRC))

.PHONY: all build run clean test

all: build

build:
	@echo "build dengan gcc..."
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@echo "build selesai: $(TARGET)"

run: build
	./$(TARGET)

test:
	@echo "Compiling tests..."
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(TEST_SRC) -o $(BUILD_DIR)/test_adt
	@echo "Running tests..."
	@./$(BUILD_DIR)/test_adt

clean:
	rm -rf $(BUILD_DIR)