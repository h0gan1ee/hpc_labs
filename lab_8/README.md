# Lab 8

## Overview

Perform MP LINKPACK benchmark on x86_64 Ubuntu computer.

## Hardware specifications

The original CPU report is at `./assets/cpuinfo.txt`.

```
CPU Cores:      6
Model Name:     Intel(R) Core(TM) i7-8700 CPU @ 3.20GHz
Frequency:      800.055 MHz
Cache Size:     12288 KB
```

## Setup HPL

### Installation guide

The installation process replicate <https://www.mgaillard.fr/2022/08/27/benchmark-with-hpl.html>.

### Dependencies

- GCC 5.4.0
- OpenMPI 3.2
- ATLAS 3.10.2
- GFortran 5.4.0

### Building artifacts

The version of HPL source code is 2.3. The modified Makefile is at `./assets/Make.linux`.

## Performance

Theoretically, the CPU could run at 38.40264 GFLOPS.

The first report of MPL benchmark is at `./assets/xhpl.txt`, and the corresponding parameters are described in `./assets/HPL.dat`

```
======================================================
T/V             N   NB  P     Q     Time        Gflops
------------------------------------------------------
WR11C2R4     5040  128  1     1     5.34    1.5997e+01
```