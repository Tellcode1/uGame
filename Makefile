CC=gcc
CFLAGS=-g -Ibuild/
LDFLAGS=-g -lm -lSDL3 -lGL
GLSRC=src/GL/glmod.c src/GL/gldraw.c src/GL/glutils.c src/GL/glad.c
SRC=src/test.c src/msys.c src/mmsg.c src/time.c src/ufr.c src/winm.c src/camera.c $(GLSRC)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))

GLM_HEADER=glmpch.h
GLM_PCH=build/cglm.gch

all: test build/test.b | update_esl

test: $(OBJ) $(realpath build)/libesl.a | update_esl
	$(CC) $(LDFLAGS) $^ -o $@

build/test.b: test.e update_esl
	build/ec $< -o $@

update_esl:
	make 'CC=$(CC)' 'CFLAGS=$(CFLAGS)' 'LDFLAGS=$(LDFLAGS)' 'BUILD_DIR=$(realpath build)' -C ESL/ all

build:
	mkdir -p build
build/GL:
	mkdir -p build/GL

build/%.o: src/%.c $(GLM_PCH) | build build/GL
	$(CC) $(CFLAGS) -c $< -o $@

$(realpath build)/libesl.a:
	make 'CC=$(CC)' 'CFLAGS=$(CFLAGS)' 'LDFLAGS=$(LDFLAGS)' 'BUILD_DIR=$(realpath build)' -C ESL/ all

clean:
	rm -rf build

$(GLM_PCH): $(GLM_HEADER)
	$(CC) $(CFLAGS) -x c-header -o $@ $<
