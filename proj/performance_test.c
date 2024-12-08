#include <stddef.h>
#include "game.h"
#include "tile.h"
#include "player.h"
#include "inventory.h"
#include "board_io.h"

extern void print(const char *);
extern void print_dec(int);
extern void start_game();

void clear_counters(){
    print("Clearing performance counters...");
    asm volatile("csrw mcycle, x0");        // clear register that counts the number of clock cycles that elapsed
    asm volatile("csrw minstret, x0");      // clear register that counts the number of instructions that have been retired
    asm volatile("csrw mhpmcounter3, x0");      // clear register that counts the number of memory instructions that have been retired
    asm volatile("csrw mhpmcounter4, x0");      // clear register that counts the number of times an instruction-fetch resulted in a I-cache miss
    asm volatile("csrw mhpmcounter5, x0");      // clear register that counts the number of times an memory operation resulted in a D-cache miss
    asm volatile("csrw mhpmcounter6, x0");      // clear register that counts the number of stalls the CPU experienced due to I-cache misses
    asm volatile("csrw mhpmcounter7, x0");      // clear register that counts the number of stalls the CPU experienced due to D-cache misses
    asm volatile("csrw mhpmcounter8, x0");      // clear register that counts the number of stalls that the CPU experienced due to data hazards that could not be solved by forwarding
    asm volatile("csrw mhpmcounter9, x0");      // clear register that counts the number of stalls that the CPU experienced due to expensive ALU operations
    print("Performance counters cleared.\n");
}

void read_counters(unsigned int* game_time) {
    // print("Reading performance counters...\n");
    // asm volatile("csrr mcycle, x0");        // reads register that counts the number of clock cycles that elapsed
    // asm volatile("csrr minstret, x0");      // reads register that counts the number of instructions that have been retired
    // asm volatile("csrr mhpmcounter3, x0");      // reads register that counts the number of memory instructions that have been retired
    // asm volatile("csrr mhpmcounter4, x0");      // reads register that counts the number of times an instruction-fetch resulted in a I-cache miss
    // asm volatile("csrr mhpmcounter5, x0");      // reads register that counts the number of times an memory operation resulted in a D-cache miss
    // asm volatile("csrr mhpmcounter6, x0");      // reads register that counts the number of stalls the CPU experienced due to I-cache misses
    // asm volatile("csrr mhpmcounter7, x0");      // reads register that counts the number of stalls the CPU experienced due to D-cache misses
    // asm volatile("csrr mhpmcounter8, x0");      // reads register that counts the number of stalls that the CPU experienced due to data hazards that could not be solved by forwarding
    // asm volatile("csrr mhpmcounter9, x0");      // reads register that counts the number of stalls that the CPU experienced due to expensive ALU operations

    // Read clock cycle register
    asm("csrr %0, mcycle" : "=r"(*game_time));
    print("mcycle (Clock cycle counter): ");
    print_dec(*game_time);
    print(" clock cycles.\n");

    // Read retired instruction register
    asm("csrr %0, minstret" : "=r"(*game_time));
    print("minstret (Retired instruction counter): ");
    print_dec(*game_time);
    print(" instructions.\n");

    // Read memory instruction counter register
    asm("csrr %0, mhpmcounter3" : "=r"(*game_time));
    print("mhpmcounter3 (Memory instruction counter): ");
    print_dec(*game_time);
    print(" memory instructions.\n");

    // Read I-cache miss counter
    asm("csrr %0, mhpmcounter4" : "=r"(*game_time));
    print("mhpmcounter4 (I-cache miss counter): ");
    print_dec(*game_time);
    print(" I-cache misses.\n");

    // Read D-cache miss counter
    asm("csrr %0, mhpmcounter5" : "=r"(*game_time));
    print("mhpmcounter5 (D-cache miss counter): ");
    print_dec(*game_time);
    print(" D-cache misses.\n");

    // Read I-cache stall counter due to I-cache misses
    asm("csrr %0, mhpmcounter6" : "=r"(*game_time));
    print("mhpmcounter6 (I-cache stall counter due to I-cache misses): ");
    print_dec(*game_time);
    print(" I-cache stalls.\n");

    // Read D-cache stall counter due to D-cache misses
    asm("csrr %0, mhpmcounter7" : "=r"(*game_time));
    print("mhpmcounter7 (D-cache stall counter due to D-cache misses): ");
    print_dec(*game_time);
    print(" D-cache stalls.\n");

    // Read stall counter due to unsolved data hazards
    asm("csrr %0, mhpmcounter8" : "=r"(*game_time));
    print("mhpmcounter8 (Stall counter due to data hazards): ");
    print_dec(*game_time);
    print(" data hazard stalls.\n");

    // Read ALU operation stall counter
    asm("csrr %0, mhpmcounter9" : "=r"(*game_time));
    print("mhpmcounter9 (Stall counter due to expensive ALU operations): ");
    print_dec(*game_time);
    print(" ALU stalls.\n");
}

int main()
{
    // print("HELLO!!!");
    unsigned int game_time; // Create a variable called foo_time

    clear_counters();

    // Call the foo function
    start_game();

    read_counters(&game_time);

    // // Print out the value of foo_time (requires print_dec in time4riscv.zip)
    // print("\nTime for start_game() was: ");
    // print_dec(game_time);
    // print("\n");
}