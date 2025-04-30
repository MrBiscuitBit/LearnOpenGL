CC = gcc
SRC_DIR = src
OUT_DIR = bin
BUILD_DIR = build
TARGET = learnopengl

INCLUDES = -Iinclude -Iinclude/glad
CFLAGS = -std=c99
LIBS = -Llib -Llib/GLFW
LDFLAGS = -lglfw3dll -lopengl32 -lgdi32

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(SRC:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)

DEBUG_FLAGS = -g -O0 -DDEBUG
RELEASE_FLAGS = -O2 -mwindows -DNDEBUG

.PHONY: all debug release clean rebuild drebuild run drun build_dirs

all: release

debug: FLAGS = $(DEBUG_FLAGS)
debug: build_dirs $(OUT_DIR)/$(TARGET)_debug.exe

release: FLAGS = $(RELEASE_FLAGS)
release: build_dirs $(OUT_DIR)/$(TARGET).exe

$(OUT_DIR)/$(TARGET).exe: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(LIBS) $(LDFLAGS)

$(OUT_DIR)/$(TARGET)_debug.exe: $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(LIBS) $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) $(FLAGS) $(INCLUDES) -c $< -o $@

build_dirs:
	@mkdir -p $(BUILD_DIR)
	@mkdir -p $(OUT_DIR)

clean:
	@rm -rf $(BUILD_DIR)
	@rm -f $(OUT_DIR)/$(TARGET)*.exe

run: release
	$(OUT_DIR)/$(TARGET).exe

drun: debug
	$(OUT_DIR)/$(TARGET)_debug.exe --log

rebuild: clean all
drebuild: clean debug