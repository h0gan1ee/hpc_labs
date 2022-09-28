CC = clang++
CPPFLAGS = -Iinclude --std=c++11 -Ofast -ffast-math -mcpu=apple-m1 -mtune=native -g

.PHONY: default main SLIC

default: SLIC main
	$(CC) $(CPPFLAGS) main.o src/SLIC.o -o main

run: default
	./main

SLIC: src/SLIC.cpp include/SLIC.h
	$(CC) $(CPPFLAGS) -c src/SLIC.cpp -o src/SLIC.o

main: main.cpp
	$(CC) $(CPPFLAGS) -c main.cpp -o main.o

clean:
	rm *.o
	rm src/*.o
	rm main