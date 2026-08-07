#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define ARRAY_SIZE 100000000 // 100 million elements

// ---------------------------------------------------------
// 1. Standard C Implementation
// ---------------------------------------------------------
long long sum_c(const long long *arr, size_t size) {
    long long sum = 0;
    for (size_t i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

// ---------------------------------------------------------
// 2. Inline x86_64 Assembly Implementation
// ---------------------------------------------------------
long long sum_asm(long long *arr, size_t size) {
    long long sum = 0;
    
    // __volatile__ prevents the compiler from optimizing the assembly block away
    __asm__ __volatile__ (
        "test %[size], %[size]\n\t"  // Check if size is 0
        "jz 2f\n"                    // If zero, jump forward to label '2'
        "1:\n\t"                     // Loop start (label '1')
        "add (%[arr]), %[sum]\n\t"   // Add the value at pointer [arr] to [sum]
        "add $8, %[arr]\n\t"         // Move pointer forward by 8 bytes (sizeof long long)
        "dec %[size]\n\t"            // Decrement the size counter
        "jnz 1b\n"                   // If size is not zero, jump back to label '1'
        "2:"                         // End label
        
        // Output operands (using '+' because we read and write to them)
        : [sum] "+r" (sum), [arr] "+r" (arr), [size] "+r" (size)
        // Input operands (none, since we used '+' above)
        : 
        // Clobbered registers ('cc' means we modify the condition codes/flags)
        : "cc"
    );
    
    return sum;
}

// ---------------------------------------------------------
// Helper function to calculate time difference in seconds
// ---------------------------------------------------------
double get_time_diff(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;
}

int main() {
    printf("Allocating array of %d elements...\n", ARRAY_SIZE);
    long long *arr = malloc(ARRAY_SIZE * sizeof(long long));
    if (!arr) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    // Populate array with dummy data
    for (size_t i = 0; i < ARRAY_SIZE; i++) {
        arr[i] = 1; // Keeping it simple to prevent overflow
    }

    struct timespec start, end;
    long long result_c, result_asm;
    double time_c, time_asm;

    printf("Running benchmarks...\n\n");

    // --- Benchmark C Version ---
    clock_gettime(CLOCK_MONOTONIC, &start);
    result_c = sum_c(arr, ARRAY_SIZE);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    time_c = get_time_diff(start, end);
    printf("[C Version]       Result: %lld | Time: %.6f seconds\n", result_c, time_c);

    // --- Benchmark Assembly Version ---
    clock_gettime(CLOCK_MONOTONIC, &start);
    result_asm = sum_asm(arr, ARRAY_SIZE);
    clock_gettime(CLOCK_MONOTONIC, &end);
    
    time_asm = get_time_diff(start, end);
    printf("[Assembly Version] Result: %lld | Time: %.6f seconds\n", result_asm, time_asm);

    // --- Comparison ---
    printf("\nDifference: Assembly was %.2f%% %s than C.\n", 
           (time_c > time_asm) ? ((time_c / time_asm - 1) * 100) : ((time_asm / time_c - 1) * 100),
           (time_c > time_asm) ? "faster" : "slower");

    free(arr);
    return 0;
}