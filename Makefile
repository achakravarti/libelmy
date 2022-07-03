CC=gcc
CFLAGS=-Wall -Wextra -I `pg_config --includedir`
LDFLAGS=-L`pg_config --libdir` -L/usr/local/lib -Wl,-R/usr/local/lib -lchrysalid -lpq -lpcre2-8

build/elmy: src/bin/main.c src/lib/error.c src/lib/rule.c src/lib/log.c src/lib/logs.c src/lib/db.c
	mkdir -p build
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@

bin: build/elmy

clean:
	rm -rf build
