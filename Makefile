CC=gcc
CFLAGS=-g -DDEBUG
LDFLAGS=-g
LIBS=-lm -lSDL3 -lGL
GLSRC=src/GL/glmod.c src/GL/gldraw.c src/GL/glutils.c src/GL/glad.c
SRC=src/test.c src/msys.c src/mmsg.c src/time.c src/ufr.c src/winm.c src/camera.c src/ESL/uesl.c src/ESL/input.c src/input.c $(GLSRC)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))

GLM_HEADER=glmpch.h
GLM_PCH=build/cglm.gch

all: test build/test.b | build/ec

test: $(OBJ) $(realpath build)/libesl.a | build/ec
	$(CC) $(LDFLAGS) $^ $(LIBS) -o $@

build/test.b: test.e build/ec
	build/ec $< -o $@

build/ec:
	make 'CC=$(CC)' 'CFLAGS=$(CFLAGS)' 'LDFLAGS=$(LDFLAGS) $(LIBS)' 'BUILD_DIR=$(realpath build)' -C ESL/ all

build_directories: build build/GL build/ESL

build:
	mkdir -p build
build/GL:
	mkdir -p build/GL
build/ESL:
	mkdir -p build/ESL

build/%.o: src/%.c $(GLM_PCH) | build_directories
	$(CC) $(CFLAGS) -c $< -o $@

$(realpath build)/libesl.a:
	make 'CC=$(CC)' 'CFLAGS=$(CFLAGS)' 'LDFLAGS=$(LDFLAGS) $(LIBS)' 'BUILD_DIR=$(realpath build)' -C ESL/ all

clean:
	rm -rf build

$(GLM_PCH): $(GLM_HEADER)
	$(CC) $(CFLAGS) -x c-header -o $@ $<
