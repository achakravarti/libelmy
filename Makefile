build/elmy: src/bin/main.c src/lib/rule.c
	mkdir -p build
	gcc $^ -o $@

bin: build/elmy

clean:
	rm -rf build
