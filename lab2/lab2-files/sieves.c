#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

#define COLUMNS 10

// Sieve of Eratosthenes - assignment 3: task 1 (using stack)
void print_sieves(int n) {
    int i;
    // populate array with n st true stuff
    int primes[n];
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
    int distanceCount = 0; // surprise assignment -- counts amount of times primes have dist 8

    // print all numbers set to true in array
    for (i = 2; i < n; i++) {
        if(primes[i]) {
            count++;
            printf("%10d ", i);

            if (count % COLUMNS == 0){
                printf("\n");
            }

            // check if dist is 8 and increment distanceCount 
            if (primes[i] && primes[i-8] && i>=8) {
                distanceCount++;
            }
        }
    }
    printf("\n");
    // print out amount of times dist is 8
    printf("Amount times there is a distance of 8 between two primes: %10d", distanceCount);
    printf("\n");
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