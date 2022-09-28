OBLAS_DIR ?= /opt/homebrew/Cellar/openblas/0.3.21
TARGET_DIR ?= ./target
MAIN ?= ./src/main.c
CXXFLAGS ?= -Wall -Ofast -mcpu=apple-m1
CC := clang

.PHONY: all debug pre build* run clean

all: build-user build-oblas build-ablas run

debug: build-dbg run

pre:
	mkdir -p $(TARGET_DIR)

build-dbg: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DDBG -DUSER -o $(TARGET_DIR)/bin_dbg

build-user: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DUSER -o $(TARGET_DIR)/bin_user

build-oblas: pre
	$(CC) $(MAIN) $(CXXFLAGS) -I $(OBLAS_DIR)/include -L $(OBLAS_DIR)/lib -lopenblas -DOBLAS -o $(TARGET_DIR)/bin_oblas

build-ablas: pre
	$(CC) $(MAIN) $(CXXFLAGS) -framework Accelerate -DABLAS -o $(TARGET_DIR)/bin_ablas

run: test_case.in.txt
	@for file in $(TARGET_DIR)/*; do \
	    printf "\n[PROGRAM $${file} START]\n"; \
	    time $${file}; \
	done

test_case.in.txt:
	python3 gen_test_case.py > test_case.in.txt

clean:
	rm -r ./target/**