#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define COLUMNS 10

// Sieve of Eratosthenes - assignment 3: task 2 (using heap)
void print_sieves(int n) {
    int i; // initialize an "i" so it can be used in each for-loop

    // populate array with n st true stuff
    int *primes = (int *)malloc(n * sizeof(int)); // using the heap instead of stack
    for (i = 0; i < n; i++) {
        primes[i] = 1;
    }

    // set all nonprimes to false with sieve
    for (i = 2; i < sqrt(n); i++) {
        if (primes[i] == 1) {
            for (int k = i * i; k < n; k+=i) {
                primes[k] = 0;
            }
        }
    }

    int count = 0;
    
    // print all numbers set to true in array
    for (i = 2; i < n; i++) {
        if(primes[i]) {
            count++;
            printf("%10d ", i);

            if (count % COLUMNS == 0){
                printf("\n");
            }
        }
    }
    printf("\n");
    free(primes); // free allocated memory from heap
}

int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_sieves(atoi(argv[1]));
    }
    else
    printf("Please state an integer number.\n");
    return 0;
}