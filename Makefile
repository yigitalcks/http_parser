
SRC_DIR = src
OBJ_DIR = obj
HEADER_DIR = header
BUILD_DIR = build

CC = gcc
CFLAGS = -Wall -Wextra -I$(HEADER_DIR)

SRCS = $(wildcard src/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
EXECUTABLE = $(BUILD_DIR)/app

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CC) $(OBJS) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)

.PHONY: all clean
