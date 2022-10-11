SERVER_BIN ?= socket_server_demo
SRC_DIR ?= ./src
TARGET_DIR ?= ./target
CXXFLAGS ?= -Wall -Ofast
CC := clang
FLAG ?=

.PHONY: all pre build run kill

all: build run

pre:
	mkdir -p $(TARGET_DIR)

build: $(SRC_DIR) pre
	for file in `ls $<` ; do \
	    $(CC) $(CXXFLAGS) $(FLAG) $(SRC_DIR)/$${file} -o $(TARGET_DIR)/`echo $${file} | sed "s/.*\///g; s/\.c//g;"` || 1 ; \
	done

run:
	@echo
	@./target/$(SERVER_BIN)

clean:
	rm -r ./target