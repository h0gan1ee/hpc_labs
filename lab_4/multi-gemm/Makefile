OBLAS_DIR ?= /opt/homebrew/Cellar/openblas/0.3.21
TARGET_DIR ?= ./target
MAIN ?= ./src/main.c
CXXFLAGS ?= -Wall -Wextra -Ofast -mcpu=apple-m1
CC := clang

.PHONY: all debug pre build* run clean

all: build-user build-multi build-ablas run

debug: build-dbg run

pre:
	mkdir -p $(TARGET_DIR)

build-dbg: pre
	$(CC) $(MAIN) $(CXXFLAGS) -g -framework Accelerate -pthread -DDBG -o $(TARGET_DIR)/bin_dbg

build-user: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DUSER -o $(TARGET_DIR)/bin_user

build-multi: pre
	$(CC) $(MAIN) $(CXXFLAGS) -pthread -DMULTI -o $(TARGET_DIR)/bin_multi

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