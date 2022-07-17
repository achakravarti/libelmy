
# Common flags

CC=gcc
CFLAGS=-Wall -Wextra -I `pg_config --includedir`
LDFLAGS=-L`pg_config --libdir` -L/usr/local/lib -Wl,-R/usr/local/lib
LDEPS=-lchrysalid -lpq -lpcre2-8


# Path to generated libelmy.so
LIB_OUT=build/libelmy.so

# Source files required to build libelmy.so
LIB_SRC!=find src/lib/ -type f -name '*.c' | sort

# List of object files corresponding to LIB_SRC
LIB_OBJ=$(LIB_SRC:src/lib/%.c=build/%.o)


BIN_OUT=build/elmy
BIN_SRC!=find src/bin/ -type f -name '*.c' | sort



# Rule to build object files from libelmy source files
build/%.o: src/lib/%.c
	mkdir -p build/lib
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

# Rule to build libelmy.so from object files
$(LIB_OUT): $(LIB_OBJ)
	$(CC) -rdynamic -shared $(LDFLAGS) $(LDEPS) $< -o $@

#build/elmy: src/bin/opt.c src/bin/print.c src/bin/cmd.c src/bin/main.c src/lib/error.c src/lib/page.c src/lib/rule.c src/lib/log.c src/lib/logs.c src/lib/db.c
#$(BIN_OUT): $(BIN_SRC) src/lib/error.c src/lib/page.c src/lib/rule.c src/lib/log.c src/lib/logs.c src/lib/db.c
$(BIN_OUT): $(BIN_SRC) $(LIB_OUT)
	mkdir -p build
	$(CC) $(CFLAGS) $(BIN_SRC) $(LDFLAGS) $(LIB_OUT) $(LDEPS) -o $@

# Target to build libelmy.so
lib: $(LIB_OUT)




bin: build/elmy

clean:
	rm -rf build
	rm -f vgcore*
