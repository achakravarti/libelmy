build/elmy: src/bin/main.c src/lib/rule.c
	mkdir -p build
	clang $^ -L/usr/local/lib -Wl,-R/usr/local/lib -lchrysalid -lpcre2-8 -o $@

bin: build/elmy

clean:
	rm -rf build
