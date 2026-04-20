BUILD_DIR := build
TARGET := $(BUILD_DIR)/alprog1
CC ?= gcc
CFLAGS ?= -std=c99 -Wall -Wextra -Wpedantic -Iheader
SRC := $(shell find src -name "*.c")

.PHONY: all build run clean

all: build

build:
	@echo "build dengan gcc..."
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CFLAGS) $(SRC) -o $(TARGET)
	@echo "build selesai: $(TARGET)"

run: build
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
