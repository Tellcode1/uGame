CC=gcc
CFLAGS=-g
LDFLAGS=-g -lm -lSDL3 -lGL
GLSRC=src/GL/glmod.c src/GL/gldraw.c src/GL/glutils.c src/GL/glad.c
SRC=src/test.c src/msys.c src/mmsg.c src/time.c src/ufr.c src/winm.c src/camera.c $(GLSRC)
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))

test: $(OBJ) build/libnvstd.a
	$(CC) $(LDFLAGS) $^ -o $@

build:
	mkdir -p build
build/GL:
	mkdir -p build/GL

build/%.o: src/%.c | build build/GL
	$(CC) $(CFLAGS) -c $^ -o $@

build/libnvstd.a: | build
	make CC='$(CC)' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' -C ../std/ all
	cp ../std/build/libnvstd.a $@