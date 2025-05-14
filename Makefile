MAKEFLAGS = --no-print-directory

CC = gcc

CFLAGS = -Wall -Wextra
LDFLAGS =

DBFLAGS = -Og -g
RLFLAGS = -O3 -march=native -mtune=native -DNDEBUG

FILES = src/*.c
TARGET = primegen

FILES_WASM = src/primegen.c src/bmap.c
TARGET_WASM = build-wasm/primegen.wasm

debug:
	@make clean
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(DBFLAGS) $(FILES)

release:
	@make clean
	$(CC) -o $(TARGET) $(CFLAGS) $(LDFLAGS) $(RLFLAGS) $(FILES)

wasm:
	rm -rf build-wasm/primgen.wasm
	emcc -o $(TARGET_WASM) -O3 -s EXPORTED_FUNCTIONS=_generate_primes $(FILES_WASM)

clean:
	@rm -rf $(TARGET)* primes.txt

run:
	@make debug
	@echo "-------------------------"
	@./$(TARGET)
	@echo "-------------------------"
	@make clean
