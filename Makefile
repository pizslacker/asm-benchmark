assy-benchmark: assy-benchmark.c
	gcc -O3 -o assy-benchmark assy-benchmark.c
	strip assy-benchmark

clean:
	rm -f assy-benchmark