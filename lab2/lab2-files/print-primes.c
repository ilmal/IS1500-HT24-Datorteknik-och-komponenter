/*
 print-primes.c
 By David Broman.
 Last modified: 2015-09-15
 This file is in the public domain.
*/


#include <stdio.h>
#include <stdlib.h>

#define COLUMNS 10

int is_prime(int n){
    // try dividing with numbers up to square root of n
    for(int i = 2; i*i <= n; i++){
        // return false(0) if inserted number is divisible with i
        if(n % i == 0) 
            return 0;
    }
    return 1;
}

// prints out n and puts them in columns if called several times
void print_numbers(int n) {
    static int count = 0; // counter to check amount of numbers printed
    printf("%10d ", n); // print number
    count++; // increment counter each time a number is printed
    
    // If count is divisible with amount of columns, print newline
    if (count % COLUMNS == 0){
        printf("\n");
    }
}

void print_primes(int n){
    // Should print out all prime numbers less than 'n'
    // with the following formatting. Note that
    // the number of columns is stated in the define
    // COLUMNS
    // iterate through every number and check if it's a prime
    for (int i=2; i<n; i++){
        if (is_prime(i)){
            print_numbers(i); // call print_numbers if its a prime
        }
    }
    printf("\n"); // newline at the end
}

// 'argc' contains the number of program arguments, and
// 'argv' is an array of char pointers, where each
// char pointer points to a null-terminated string.
int main(int argc, char *argv[]){
    if(argc == 2)
    {
        print_primes(atoi(argv[1]));
    }
    else
    printf("Please state an integer number.\n");
    return 0;
}

 
