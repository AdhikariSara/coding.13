*
Homework Answers - Chapter 19

1. The `gettimeofday()` function can measure very short intervals,
   but the operation being timed should last at least a few microseconds for accuracy. Repeating the page access in a loop
   helps obtain a more precise measurement by accumulating enough time to measure.

2. `tlb.c` measures the average time taken to access each page by taking the number of pages 
   and the number of trials as input. The program touches each page in a loop to calculate the average access time.

5. To prevent the compiler from optimizing away the loop, we can use the accessed page values in a way that 
   affects the program's output or store them in a variable that is printed later.

6. To ensure the program runs on a single CPU, we can "pin" it to a specific CPU using `sched_setaffinity`. 
   If we don’t do this, the program may switch between CPUs, causing inconsistencies in timing because each CPU has its own TLB.

7. If the array isn’t initialized, the first access might take longer due to system setup costs such as demand zeroing. 
   Initializing the array beforehand avoids this issue, ensuring consistent timing measurements.
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number_of_pages> <number_of_trials>\n", argv[0]);
        return 1;
    }

    int num_pages = atoi(argv[1]);        
    int trials = atoi(argv[2]);           
    int page_size = getpagesize();        
    int *array = malloc(num_pages * page_size);  

    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }

    
    for (int i = 0; i < num_pages * page_size / sizeof(int); i++) {
        array[i] = i;
    }

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    
    long sum = 0;  

    for (int t = 0; t < trials; t++) {
        for (int i = 0; i < num_pages * page_size / sizeof(int); i += page_size / sizeof(int)) {
            sum += array[i];  
        }
    }

    clock_gettime(CLOCK_MONOTONIC, &end);

    
    long elapsed = (end.tv_sec - start.tv_sec) * 1000000000L + (end.tv_nsec - start.tv_nsec);
    printf("Average time per trial: %ld nanoseconds\n", elapsed / trials);

    free(array);  
    return 0;
}

