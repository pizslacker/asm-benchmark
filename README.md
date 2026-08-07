# assy-benchmark

Benchmark program showcasing how compiler optimizations of today greatly improves C performance, compared to theolden days of handwriting assembly (ASM).

Because GCC is exceptionally good at optimizing C code, the flags you use to compile this will heavily impact the results.

### Without C optimization (`-O0`)
```bash
Allocating array of 100000000 elements...
Running benchmarks...

[C Version]       Result: 100000000 | Time: 0.146395 seconds
[Assembly Version] Result: 100000000 | Time: 0.035967 seconds

Difference: Assembly was 307.03% faster than C.
```

### With C optmization (`-O3`)
```bash
Allocating array of 100000000 elements...
Running benchmarks...

[C Version]       Result: 100000000 | Time: 0.029381 seconds
[Assembly Version] Result: 100000000 | Time: 0.036178 seconds

Difference: Assembly was 23.13% slower than C.
```
