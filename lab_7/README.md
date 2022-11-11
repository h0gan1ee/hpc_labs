# Lab 7

## Overview

Profile the SLIC algorithm and compare the differences of performance.

## Benchmark

The output of the baseline algorithm is as below:

```
Case 1 Computing time: 13565.6ms
There are 0 points' labels are different from original file.
Case 2 Computing time: 45031.3ms
There are 0 points' labels are different from original file.
Case 3 Computing time: 9814.22ms
There are 0 points' labels are different from original file.
```

The current output of the optimized algorithm is as below:

```
Case 1 Computing time: 1218.35ms
There are 0 points' labels are different from original file.
Case 2 Computing time: 4018.76ms
There are 0 points' labels are different from original file.
Case 3 Computing time: 856.686ms
There are 0 points' labels are different from original file.
```

, which is roughly 10+ times faster than the original.

## Improvements

### Compliler and flags

As usual, `clang++` always outperform `g++` on Apple Silicon Mac, so I switch to `clang++` and add a few compiler flags. The flags are as below:

```Makefile
CPPFLAGS := -Iinclude --std=c++11 -fopenmp -Wall -Ofast -ffast-math -mcpu=apple-m1 -mtune=native -g
```

The debug flag `-g` is for dumping `dSYM` later for profiling using Instruments (a profiler included in Xcode).

## Parallelization

A lot of 'for loops' could be parallelized fearlessly. So instead of managing threads manually, I use compilation directives from OpenMP to make thing easier. The performance improved immediately after parallelizing a few loops.

After further profiling with Instruments, I found some heavy-duty loops inside `PerformSuperpixelSegmentation_VariableSandM` function, which also can be parallelized, but need to be careful not affecting the algorithm's correctness. And of course, the computation time dramatically decreased after the optimization.

## SIMD (Single instruction, multiple data)

In a previous experiment, using SIMD instructions on vector manipulations (via Apple's Accelerate Framework) surprisingly didn't yield a better results. Thus, I give up on this.

