asm-benchmark: asm-benchmark.c
	gcc -O3 -o asm-benchmark asm-benchmark.c
	strip asm-benchmark

clean:
	rm -f asm-benchmark
