CC := clang++
# CPPFLAGS := -Iinclude --std=c++11 -Wall
CPPFLAGS := -Iinclude --std=c++11 -Wall -Ofast -ffast-math -mcpu=apple-m1 -mtune=native -g
SLIC := src/SLIC_mod.cpp

.PHONY: default main SLIC

default: SLIC main
	$(CC) $(CPPFLAGS) main.o src/SLIC.o -o main -framework Accelerate -L/opt/homebrew/opt/libomp/lib -lomp

run: default
	./main

SLIC: src/SLIC.cpp src/SLIC_mod.cpp include/SLIC.h
	$(CC) $(CPPFLAGS) -c $(SLIC) -o src/SLIC.o

main: main.cpp
	$(CC) $(CPPFLAGS) -c main.cpp -o main.o

clean:
	rm *.o
	rm src/*.o
	rm main