CC=gcc
CFLAGS=-g
LDFLAGS=-lm -lSDL3

SRC=src/test.c src/msys.c src/mmsg.c src/time.c src/ufr.c src/winm.c
OBJ=$(patsubst src/%.c,build/%.o,$(SRC))

test: $(OBJ) build/libnvstd.a
	$(CC) $(LDFLAGS) $^ -o $@

build:
	mkdir -p build

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $^ -o $@

build/libnvstd.a: | build
	make CC='$(CC)' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' -C ../std/ all
	cp ../std/build/libnvstd.a $@