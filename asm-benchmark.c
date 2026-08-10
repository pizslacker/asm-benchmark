#include <stdio.h> 
#include <stdlib.h> 
#include <time.h> 
#include <pthread.h> 
#include <unistd.h> 

#define ARRAY_SIZE 1000000000 // 1 Billion elements (Requires ~8 GB RAM)

// Atomic flag to control the spinner loop 
volatile int keep_spinning = 1; 

// --------------------------------------------------------- 
// Spinner Thread Function 
// --------------------------------------------------------- 
void *spinner_thread(void *arg) { 
    const char spinner[] = "|/-\\"; 
    int i = 0; 
    while (keep_spinning) { 
        // Print current character and backspace the cursor 
        printf("%c\b", spinner[i++ % 4]); 
        fflush(stdout); 
        usleep(100000); // 100ms delay 
    } 
    // Clear the spinner character when stopping 
    printf(" \b"); 
    return NULL; 
} 

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
    __asm__ __volatile__ ( 
        "test %[size], %[size]\n\t" 
        "jz 2f\n" 
        "1:\n\t" 
        "add (%[arr]), %[sum]\n\t" 
        "add $8, %[arr]\n\t" 
        "dec %[size]\n\t" 
        "jnz 1b\n" 
        "2:" 
        : [sum] "+r" (sum), [arr] "+r" (arr), [size] "+r" (size) 
        : 
        : "cc", "memory" // Added "memory" clobber to inform compiler of memory reads
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
    // FIXED: Changed %d to %zu to support size_t and prevent integer overflow
    printf("Allocating array of %zu elements...\n", (size_t)ARRAY_SIZE); 
    
    long long *arr = malloc(ARRAY_SIZE * sizeof(long long)); 
    
    // SAFETY CHECK: Gracefully handle allocation failures
    if (!arr) { 
        printf("Memory allocation failed! System does not have enough free RAM (~8 GB needed).\n"); 
        return 1; 
    } 

    for (size_t i = 0; i < ARRAY_SIZE; i++) { 
        arr[i] = 1; 
    } 

    struct timespec start, end; 
    long long result_c, result_asm; 
    double time_c, time_asm; 

    // Modified to keep the cursor on the same line for the spinner 
    printf("Running benchmarks... "); 
    fflush(stdout); 

    // Start the spinner in a background thread 
    pthread_t spinner; 
    pthread_create(&spinner, NULL, spinner_thread, NULL); 

    // --- Benchmark C Version --- 
    clock_gettime(CLOCK_MONOTONIC, &start); 
    result_c = sum_c(arr, ARRAY_SIZE); 
    clock_gettime(CLOCK_MONOTONIC, &end); 
    time_c = get_time_diff(start, end); 

    // --- Benchmark Assembly Version --- 
    clock_gettime(CLOCK_MONOTONIC, &start); 
    result_asm = sum_asm(arr, ARRAY_SIZE); 
    clock_gettime(CLOCK_MONOTONIC, &end); 
    time_asm = get_time_diff(start, end); 

    // Stop the spinner thread 
    keep_spinning = 0; 
    pthread_join(spinner, NULL); 

    printf("\n\n"); // Print the newlines originally requested 

    printf("[C Version] Result: %lld | Time: %.6f seconds\n", result_c, time_c); 
    printf("[Assembly Version] Result: %lld | Time: %.6f seconds\n", result_asm, time_asm); 

    printf("\nDifference: Assembly was %.2f%% %s than C.\n", 
           (time_c > time_asm) ? ((time_c / time_asm - 1) * 100) : ((time_asm / time_c - 1) * 100), 
           (time_c > time_asm) ? "faster" : "slower"); 

    free(arr); 
    return 0; 
}