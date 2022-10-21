MAIN := src/gemm.c
CC := clang
CC_ALT := /opt/homebrew/opt/llvm/bin/clang
LDFLAGS := ''
CXXFLAGS := -Wall -Wextra -Ofast -mcpu=apple-m1
MPI_N := 4

.PHONY: all run build* clean demo

all: build run

pre:
	mkdir -p target

run: test_case.in.txt
	@for file in target/*; do \
	    if [[ -f $${file} ]]; then \
	        printf "\n[PROGRAM $${file} START]\n"; \
	        time $${file}; \
	    fi \
	done
	@for file in target/mpi/*; do \
	    printf "\n[PROGRAM $${file} START]\n"; \
	    time mpirun -n $(MPI_N) $${file}; \
	done

build: clean build-omp build-user build-ablas build-mpi build-multi

build-dbg: pre
	$(CC) $(MAIN) $(CXXFLAGS) -g -framework Accelerate -DDBG -o target/gemm_dbg

build-ablas: pre
	$(CC) $(MAIN) $(CXXFLAGS) -framework Accelerate -DABLAS -o target/gemm_ablas

build-omp: pre
	$(CC_ALT) $(MAIN) $(CXXFLAGS) -fopenmp -DOMP -o target/gemm_omp

build-user: pre
	$(CC) $(MAIN) $(CXXFLAGS) -DUSER -o target/gemm_user

build-multi: pre
	$(CC) $(MAIN) $(CXXFLAGS) -pthread -DMULTI -o target/gemm_multi

build-mpi: pre
	mkdir -p target/mpi
	mpicc $(MAIN) $(CXXFLAGS) -DMPI -o target/mpi/gemm_mpi

test_case.in.txt:
	python3 gen_test_case.py > test_case.in.txt

clean:
	rm -r target || true