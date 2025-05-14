MAKEFLAGS = --no-print-directory

CC = gcc

CFLAGS = -Wall -Wextra
LDFLAGS =

DBFLAGS = -Og -g
RLFLAGS = -O3 -march=native -mtune=native -DNDEBUG

FILES = src/*.c
TARGET = primegen

FILES_WASM = src/primegen.c src/bmap.c
TARGET_WASM_JS = build-wasm/primegen.js
TARGET_WASM_FILE = build-wasm/primegen.wasm

debug:
	@make clean
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) $(FILES)

release:
	@make clean
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(RLFLAGS) $(FILES)

wasm:
	rm -rf $(TARGET_WASM_JS) $(TARGET_WASM_FILE)
	emcc -o $(TARGET_WASM_JS) \
		-O3 -s ALLOW_MEMORY_GROWTH=1 \
		-s EXPORTED_FUNCTIONS='["_generate_primes", "_free"]' \
		-s EXPORTED_RUNTIME_METHODS='["cwrap"]' \
		$(FILES_WASM)

clean:
	@rm -rf $(TARGET)* primes.txt

run:
	@make debug
	@echo "-------------------------"
	@./$(TARGET)
	@echo "-------------------------"
	@make clean
