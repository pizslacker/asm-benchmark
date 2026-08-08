# ASM-benchmark

C benchmark program showcasing how compiler optimizations of today greatly improves C performance, compared to _the olden days_ of handwritten assembly (ASM).

Because GCC is exceptionally good at optimizing C code, the flags you use to compile this will heavily impact the results.

This C program uses GCC inline assembly for `x86_64` architecture. It compares a standard C implementation of an array-summation function against a handwritten assembly version, using `POSIX` high-resolution timers (`clock_gettime`) to measure the performance of both.

### Without C optimization (`-O0`)
```bash
Allocating array of 100000000 elements...
Running benchmarks...

[C Version]       Result: 100000000 | Time: 0.146395 seconds
[Assembly Version] Result: 100000000 | Time: 0.035967 seconds

Difference: Assembly was 307.03% faster than C.
```

### With full C optmization (`-O3`)
```bash
Allocating array of 100000000 elements...
Running benchmarks...

[C Version]       Result: 100000000 | Time: 0.029381 seconds
[Assembly Version] Result: 100000000 | Time: 0.036178 seconds

Difference: Assembly was 23.13% slower than C.
```

At `-O3`, the tables often turn. GCC will "unroll" the C loop and automatically utilize SIMD (Single Instruction, Multiple Data) or AVX vector instructions on modern CPUs. It will process 2, 4, or even 8 integers simultaneously, completely outperforming our naive scalar assembly loop.
