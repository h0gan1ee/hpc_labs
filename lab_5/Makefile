MAIN := src/gemm.c
CC := clang
LDFLAGS := ''
CXXFLAGS := -fopenmp -Wall -Wextra -Ofast -mcpu=apple-m1

.PHONY: all run build* clean demo

all: build run

pre:
	mkdir -p target

run: test_case.in.txt
	@for file in target/*; do \
	    printf "\n[PROGRAM $${file} START]\n"; \
	    time $${file}; \
	done

build: clean build-omp build-user build-ablas

build-dbg: pre
	$(CC) $(MAIN) $(CXXFLAGS) -g -framework Accelerate -DDBG -o target/gemm_dbg

build-ablas: pre
	$(CC) $(MAIN) $(CXXFLAGS) -framework Accelerate -DABLAS -o target/gemm_ablas

build-omp: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DOMP -o target/gemm_omp

build-user: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DUSER -o target/gemm_user

demo: pre
	$(CC) $(CXXFLAGS) $(LDFLAGS) src/hello.c -o target/hello
	./target/hello

test_case.in.txt:
	python3 gen_test_case.py > test_case.in.txt

clean:
	rm -r target || true