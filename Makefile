CC=gcc
CFLAGS=-g
LDFLAGS=-lm -g

test: build/test.o build/msys.o build/mevents.o build/time.o build/libnvstd.a
	$(CC) $(LDFLAGS) $^ -o $@

build:
	mkdir -p build

build/%.o: src/%.c | build
	$(CC) $(CFLAGS) -c $^ -o $@

build/libnvstd.a: | build
	make CC='$(CC)' CFLAGS='$(CFLAGS)' LDFLAGS='$(LDFLAGS)' -C ../std/ all
	cp ../std/build/libnvstd.a $@