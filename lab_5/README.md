# Lab 5

## Overview

Similar to Lab 2.

# Set up on macOS

```bash
brew install llvm libomp
source ./envrc
```

## Benchmark

C benchmark include a naive algorithm, Apple's implementation of BLAS, and a boosted naive algorithm utilizing OpenMP.

You might need to modify `./Makefile` to tweak the C toolchain, targetting platform and so on (especially when not on a Mac with M1 chip). The `Makefile` is pretty self-explained so I won't go into details here.

A possible output (us is a shorthand of microsecond):

```
[PROGRAM target/gemm_ablas START]

[DURATION] 421 us

[DURATION] 386 us

[DURATION] 3314 us

[DURATION] 11663 us

[DURATION] 28569 us

real    0m0.110s
user    0m0.105s
sys     0m0.004s

[PROGRAM target/gemm_omp START]

[DURATION] 5901 us

[DURATION] 6385 us

[DURATION] 4908 us

[DURATION] 55715 us

[DURATION] 135693 us

real    0m0.304s
user    0m1.722s
sys     0m0.113s

[PROGRAM target/gemm_user START]

[DURATION] 387 us

[DURATION] 1320 us

[DURATION] 1747 us

[DURATION] 236609 us

[DURATION] 649523 us

real    0m1.156s
user    0m1.148s
sys     0m0.007s
```

The barplot of above results:

![Benchmark Barplot](assets/benchmark_barplot.png)
