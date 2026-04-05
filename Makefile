CC=gcc
CFLAGS=-g -Ibuild/
LDFLAGS=-g -lm -lSDL3 -lGL
GLSRC=src/GL/glmod.c src/GL/gldraw.c src/GL/glutils.c src/GL/glad.c
SRC=src/test.c src/msys.c src/mmsg.c src/time.c src/ufr.c src/winm.c src/camera.c $(GLSRC)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))

GLM_HEADER=glmpch.h
GLM_PCH=build/cglm.gch

test: $(OBJ) 
	$(CC) $(LDFLAGS) $^ -o $@

build:
	mkdir -p build
build/GL:
	mkdir -p build/GL

build/%.o: src/%.c $(GLM_PCH) | build build/GL
	$(CC) $(CFLAGS) -c $< -o $@

$(GLM_PCH): $(GLM_HEADER)
	$(CC) $(CFLAGS) -x c-header -o $@ $<
