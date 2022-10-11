# Lab 2

## Overview

- `./gen_test_case.py`: generate test case input file `./test_case.in.txt` for benchmark. (Required numpy)
- `./gemm_c/`: C implementations of GEMM and benchmark wrapper.

## Benchmark

C benchmark include a naive algorithm, OpenBLAS and Apple's implementation of BLAS.

You might need to modify `./gemm_c/Makefile` to set the path of OpenBLAS, targetting platform and so on (especially when not on a Mac with M1 chip). The `Makefile` is pretty self-explained so I won't go into details here.

A possible output (us is a shorthand of microsecond):

```
[PROGRAM ./target/bin_ablas START]

[DURATION] 382 us

[DURATION] 3421 us

[DURATION] 45666 us

real    0m0.160s
user    0m0.112s
sys     0m0.004s

[PROGRAM ./target/bin_oblas START]

[DURATION] 395 us

[DURATION] 25405 us

[DURATION] 290245 us

real    0m0.544s
user    0m2.728s
sys     0m0.032s

[PROGRAM ./target/bin_user START]

[DURATION] 548 us

[DURATION] 96280 us

[DURATION] 1127444 us

real    0m1.640s
user    0m1.580s
sys     0m0.008s
```

The barplot of above results:

![Benchmark Barplot](assets/benchmark_barplot.png)
