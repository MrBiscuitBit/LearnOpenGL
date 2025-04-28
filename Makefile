CC = gcc

CFLAGS = -Wall -std=c99 -g 
LDFLAGS = -lglfw3dll -lopengl32 -lgdi32 

INCLUDES = -Iinclude -Iinclude/glad
LIBS = -Llib -Llib/GLFW

SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=build/%.o)

TARGET = bin/learn_opengl.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(LIBS) $(LDFLAGS) 

build/%.o: src/%.c
	@mkdir -p build
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@rm -rf build $(TARGET)

rebuild: clean all

.PHONY: all clean rebuild	