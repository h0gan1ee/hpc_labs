SRC_DIR ?= ./src
CXXFLAGS ?= -Wall -O2
CC := clang

all: build

build: $(SRC_DIR)/*
	for file in $^ ; do \
	    $(CC) $(CXXFLAGS) $(FLAG) $${file} -o ./target/`echo $${file} | sed "s/.*\///g; s/\.c//g;"` || 1 ; \
	done