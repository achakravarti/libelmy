
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
LIB_OBJ=$(LIB_SRC:src/lib/%.c=build/lib/%.o)

# Path to generated elmy binary
BIN_OUT=build/elmy

# Source files required to build elmy binary
BIN_SRC!=find src/bin/ -type f -name '*.c' | sort

# Rule to build object files from libelmy source files
build/lib/%.o: src/lib/%.c
	@mkdir -p build/lib
	$(CC) -fPIC $(CFLAGS) -c $< -o $@


# Rule to build object files from libelmy source files
build/bin/%.o: src/bin/%.c
	@mkdir -p build/bin
	$(CC) -fPIC $(CFLAGS) -c $< -o $@

# Rule to build libelmy.so from object files
$(LIB_OUT): $(LIB_OBJ)
	$(CC) -rdynamic -shared $(LDFLAGS) $^ $(LDEPS) -o $@

# Rule to build elmy binary
$(BIN_OUT): $(BIN_SRC) $(LIB_OUT)
	$(CC) $(LDFLAGS) -Lbuild $(BIN_SRC) -lelmy $(LDEPS) -o $@
	@echo "To run elmy binary before install, run the following first:"
	@echo "$$ LD_LIBRARY_PATH=\$$LD_LIBRARY_PATH:build"
	@echo "$$ export LD_LIBRARY_PATH"

# Target to build libelmy.so
lib: $(LIB_OUT)

# Target to build elmy binary
bin: build/elmy

# Target to clean build artefacts
clean:
	rm -rf build
	rm -f vgcore*
