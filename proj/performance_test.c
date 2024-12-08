#include <stddef.h>
#include "game.h"
#include "tile.h"
#include "player.h"
#include "inventory.h"
#include "board_io.h"

extern void print(const char *);
extern void print_dec(int);
extern void start_game();

int main()
{
    print("HELLO!!!");
    unsigned int game_time; // Create a variable called foo_time

    // Clear the mcycle CSR by writing 0 to it
    asm volatile("csrw mcycle, x0");

    // Call the foo function
    start_game();

    // Read the mcycle value into foo_time
    asm("csrr %0, mcycle" : "=r"(game_time));

    // Print out the value of foo_time (requires print_dec in time4riscv.zip)
    print("\nTime for start_game() was: ");
    print_dec(game_time);
    print("\n");
}