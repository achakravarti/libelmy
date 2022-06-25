build/elmy: src/bin/main.c
	mkdir -p build
	gcc $^ -o $@

bin: build/elmy
