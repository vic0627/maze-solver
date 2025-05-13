# ----------------------------------------
# 變數定義
# ----------------------------------------
CC       ?= gcc
CFLAGS   ?= -Iinclude -Wall -Wextra -fPIC
LDFLAGS  ?= -ldl

SRC_DIR      := src
PLUGIN_SRC   := plugins_src
OBJ_DIR      := .build/obj
PLUGIN_DIR   := .build/plugins
BIN          := .build/maze_solver

# maze gen
MAZE_GEN_SRC := maze_src/main.c
MAZE_GEN_BIN := .build/maze_gen

# 自動搜集原始檔與對應物件檔
SRCS      := $(wildcard $(SRC_DIR)/*.c)
OBJS      := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# 插件清單（依 plugins_src 檔名自動偵測）
PLUGINS   := $(notdir $(basename $(wildcard $(PLUGIN_SRC)/*.c)))
PLUGIN_SOS:= $(patsubst %,$(PLUGIN_DIR)/lib%.so,$(PLUGINS))

# Phony 目標
.PHONY: all clean

# ----------------------------------------
# 預設目標：編譯主程式與所有插件
# ----------------------------------------
all: $(BIN) $(PLUGIN_SOS) $(MAZE_GEN_BIN)

# ----------------------------------------
# 主程式：鏈結所有物件檔
# ----------------------------------------
$(BIN): $(OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ----------------------------------------
# 物件檔規則：.c → .o
# ----------------------------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# ----------------------------------------
# 共用庫規則：.c → .so
# ----------------------------------------
$(PLUGIN_DIR)/lib%.so: $(PLUGIN_SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -shared $< -o $@

# ----------------------------------------
# 迷宮生成器
# ----------------------------------------
$(MAZE_GEN_BIN): $(MAZE_GEN_SRC)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# ----------------------------------------
# 清理：移除 .build 目錄
# ----------------------------------------
clean:
	rm -rf .build