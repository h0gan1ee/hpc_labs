OBLAS_DIR ?= /opt/homebrew/Cellar/openblas/0.3.21
TARGET_DIR ?= ./target
CC ?= clang
MAIN ?= ./src/main.c
CXXFLAGS ?= -Wall -Ofast -mcpu=apple-m1

all: build-user build-oblas run

debug: build-dbg run

pre:
	mkdir -p $(TARGET_DIR)

build-dbg: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DDBG -DUSER -o $(TARGET_DIR)/bin_dbg

build-user: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DUSER -o $(TARGET_DIR)/bin_user

build-oblas: pre
	$(CC) $(MAIN) $(CXXFLAGS) -I $(OBLAS_DIR)/include -L $(OBLAS_DIR)/lib -lopenblas -DOBLAS -o $(TARGET_DIR)/bin_oblas

run: $(TARGET_DIR)/*
	@for file in $^; do \
	    printf "\n[PROGRAM $${file} START]\n"; \
	    time $${file}; \
	done

clean:
	rm -r ./target/**