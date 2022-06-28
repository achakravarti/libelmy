CC=clang
CFLAGS=-Wall -Wextra -I `pg_config --includedir`
LDFLAGS=-L`pg_config --libdir` -L/usr/local/lib -Wl,-R/usr/local/lib -lchrysalid -lpq -lpcre2-8

build/elmy: src/bin/main.c src/lib/rule.c
	mkdir -p build
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

bin: build/elmy

clean:
	rm -rf build
