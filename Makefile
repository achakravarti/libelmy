CC=clang
CFLAGS=-Wall -Wextra -I `pg_config --includedir`
LDFLAGS=-L/usr/local/lib -Wl,-R/usr/local/lib -lchrysalid -lpcre2-8 -L`pg_config --libdir`

build/elmy: src/bin/main.c src/lib/rule.c
	mkdir -p build
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

bin: build/elmy

clean:
	rm -rf build
