#include "game.h"
#include "tile.h"
#include "player.h"
#include "inventory.h"
#include "board_io.h"

// Function prototypes
void interact_with_tile(struct player player, struct tile current_tile, struct tile map[10][10]);
void display_frame(struct player player, struct tile current_tile, struct tile map[10][10]);
int string_compare(const char *str1, const char *str2);

extern void print(const char *);

// Custom function to convert integer to string
void int_to_str(int num, char *str)
{
    int i = 0;
    int is_negative = 0;
    char temp[12];

    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    if (num < 0)
    {
        is_negative = 1;
        num = -num;
    }

    while (num != 0)
    {
        temp[i++] = (num % 10) + '0';
        num /= 10;
    }

    if (is_negative)
    {
        temp[i++] = '-';
    }

    int j = 0;
    // Reverse the string
    while (i > 0)
    {
        str[j++] = temp[--i];
    }
    str[j] = '\0';
}

// Custom function to concatenate strings
void append_str(char *dest, const char *src)
{
    while (*dest)
        dest++;
    while (*src)
    {
        *dest = *src;
        dest++;
        src++;
    }
    *dest = '\0';
}

// Custom function to get the length of a string
int str_length(const char *str)
{
    int len = 0;
    while (str[len])
        len++;
    return len;
}

// Dropping stats
void drop_stats(struct player *player, struct tile current_tile)
{
    if (current_tile.outside_rocket == 1)
    {
        player->oxygen -= 1;
    }
    else if (current_tile.outside_rocket == 0)
    {
        player->oxygen = 100;
    }
    player->water -= 1;
    player->food -= 1;

    // Check if any stat reaches 0
    if (player->oxygen <= 0)
    {
        print("You have run out of oxygen! Game over.\n");
        // Implement your own exit or game over logic
        while (1)
            ; // Halt execution
    }
    if (player->water <= 0)
    {
        print("You have run out of water! Game over.\n");
        while (1)
            ;
    }
    if (player->food <= 0)
    {
        print("You have run out of food! Game over.\n");
        while (1)
            ;
    }
}

#define SWITCH_EAST (1 << 0)  // Bit 0
#define SWITCH_WEST (1 << 1)  // Bit 1
#define SWITCH_SOUTH (1 << 2) // Bit 2
#define SWITCH_NORTH (1 << 3) // Bit 3

#define SWITCH_INTERACT (1 << 9) // Bit 9

void handle_input(int switch_values, struct player *player, struct tile map[10][10])
{
    int new_positionX = player->positionX;
    int new_positionY = player->positionY;
    int moved = 0; // Flag to check if movement occurred

    // if interact is on, dont move, just interact
    if (switch_values & SWITCH_INTERACT)
    {
        interact_with_tile(*player, map[player->positionY][player->positionX], map);
        return;
    }

    // Move East
    if (switch_values & SWITCH_EAST)
    {
        new_positionX += 1;
        moved = 1;
    }

    // Move West
    if (switch_values & SWITCH_WEST)
    {
        new_positionX -= 1;
        moved = 1;
    }

    // Move South
    if (switch_values & SWITCH_SOUTH)
    {
        new_positionY += 1;
        moved = 1;
    }

    // Move North
    if (switch_values & SWITCH_NORTH)
    {
        new_positionY -= 1;
        moved = 1;
    }

    // If any movement occurred, update the player's position
    if (moved)
    {
        // Check if the new position is within bounds and valid
        if (new_positionX >= 0 && new_positionX < 10 &&
            new_positionY >= 0 && new_positionY < 10 &&
            map[new_positionY][new_positionX].type != EMPTY &&
            map[new_positionX][new_positionY].type != MOUNTAIN)
        {
            player->positionX = new_positionX;
            player->positionY = new_positionY;
        }
        else
        {
            print("You cannot move in that direction.\n");
        }
    }
}

int current_display = 0;

int main()
{
    labinit();
    // clang-format off
    struct tile map[10][10] = {
        {ct(POND), ct(LOOSE_SOIL),  ct(WASTELAND),  ct(WASTELAND), ct(WASTELAND), ct(POND), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(POND)},
        {ct(SHARP_ROCKS), ct(WASTELAND),  ct(WASTELAND),  ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(POND), ct(WASTELAND), ct(MOUNTAIN), ct(CANYON)},
        {ct(EMPTY),     ct(EMPTY),      ct(EMPTY),      ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(CANYON)},
        {ct(CHAMBERS),  ct(COCKPIT),    ct(EMPTY),      ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(SHARP_ROCKS), ct(POND), ct(MOUNTAIN), ct(WASTELAND)},
        {ct(CAFETERIA), ct(STORAGE),    ct(AIRLOCK),    ct(LANDING_SITE), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(LOOSE_SOIL)},
        {ct(ENGINE_BAY),ct(LABORATORY), ct(EMPTY),      ct(WASTELAND), ct(WASTELAND), ct(LOOSE_SOIL), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(WASTELAND)},
        {ct(EMPTY),     ct(EMPTY),      ct(EMPTY),      ct(SHARP_ROCKS), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(SHARP_ROCKS), ct(WASTELAND)},
        {ct(WASTELAND), ct(WASTELAND),  ct(LOOSE_SOIL),  ct(WASTELAND), ct(WASTELAND), ct(CANYON), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(WASTELAND)},
        {ct(CRATER), ct(WASTELAND),  ct(WASTELAND),  ct(MOUNTAIN), ct(MOUNTAIN), ct(WASTELAND), ct(MOUNTAIN), ct(MOUNTAIN), ct(MOUNTAIN), ct(POND)},
        {ct(CAVE), ct(WASTELAND),  ct(WASTELAND),  ct(MOUNTAIN), ct(POND), ct(WASTELAND), ct(WASTELAND), ct(CANYON), ct(MOUNTAIN), ct(POND)},
    };
    // clang-format on

    struct player player = create_player();

    // Initial display
    struct tile current_tile = map[player.positionY][player.positionX];
    display_frame(player, current_tile, map);

    while (1)
    {
        // Process Switch Events
        if (switch_event)
        {
            switch_event = 0; // Reset the flag

            int switch_values = get_sw();
            handle_input(switch_values, &player, map);

            // Update the current tile after movement
            current_tile = map[player.positionY][player.positionX];

            // Update the game state (e.g., drop stats)
            drop_stats(&player, current_tile);

            // Display the updated game frame
            display_frame(player, current_tile, map);
        }

        // Process Timer Events
        if (timer_event)
        {
            timer_event = 0; // Reset the flag
            drop_stats(&player, current_tile);
        }

        // Process Button Events
        if (button_event)
        {

            if (current_display == 0)
            {
                set_displays(0, 1);

                current_display = 1;
            }
            else if (current_display == 1)
            {
                set_displays(0, 2);

                current_display = 2;
            }
            else if (current_display == 2)
            {
                set_displays(0, 3);

                current_display = 0;
            }

            button_event = 0; // Reset the flag
        }

        if (switch_event)
        {
            switch_event = 0; // Reset the flag

            int switch_values = get_sw();
            handle_input(switch_values, &player, map);

            // Update the current tile after movement
            current_tile = map[player.positionY][player.positionX];

            // Update the game state (e.g., drop stats)
            drop_stats(&player, current_tile);

            // Display the updated game frame
            display_frame(player, current_tile, map);
        }

        // low-power sleep or wait instruction here
        // to reduce CPU usage?
    }
}

void display_frame(struct player player, struct tile current_tile, struct tile map[10][10])
{
    set_displays(0, 0);

    print("\x1B[2J"); // Clear screen
    print("\x1B[H");  // Move cursor to top-left

    set_displays(0, 1);

    // Draw the frame
    print("+------------------------------------------------+ \n");
    print("|            MARS SURVIVAL GAME                  | \n");
    print("+------------------------------------------------+ \n");

    char buffer[256];
    buffer[0] = '\0';

    append_str(buffer, "| Position: (");
    char num_str[12];
    int_to_str(player.positionX, num_str);
    append_str(buffer, num_str);
    append_str(buffer, ", ");
    int_to_str(player.positionY, num_str);
    append_str(buffer, num_str);
    append_str(buffer, ")                               \n");
    print(buffer);

    buffer[0] = '\0';
    append_str(buffer, "| Location: ");
    append_str(buffer, TILETYPE_NAMES[current_tile.type]);
    append_str(buffer, "                                     \n");
    print(buffer);

    print("+------------------------------------------------+ \n");
    print("| Map:                                             \n");
    for (int y = 0; y < 10; y++)
    {
        char line_buffer[1024];
        line_buffer[0] = '\0';
        append_str(line_buffer, "| ");
        for (int x = 0; x < 10; x++)
        {
            if (player.positionX == x && player.positionY == y)
            {
                append_str(line_buffer, "[🫵 ] ");
            }
            else
            {
                append_str(line_buffer, "[");
                append_str(line_buffer, TILETYPE_MAP_NAMES[map[y][x].type]);
                append_str(line_buffer, "] ");
            }
        }
        append_str(line_buffer, "\n");
        print(line_buffer);
    }
    print("| Inventory:                                     | \n");
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        buffer[0] = '\0';
        append_str(buffer, "|   Slot ");
        int_to_str(i + 1, num_str);
        append_str(buffer, num_str);
        append_str(buffer, ": ");
        if (player.inventory[i] != NONE)
        {
            append_str(buffer, COLLECTIBLE_NAMES[player.inventory[i]]);
        }
        else
        {
            append_str(buffer, "Empty");
        }
        append_str(buffer, "                                    \n");
        print(buffer);
    }
    // print("+------------------------------------------------+\n");
    // print("| Items in current tile:                         |\n");
    // for (int i = 0; i < STORAGE_SIZE; i++)
    // {
    //     if (current_tile.storage[i] != NONE) // Only print non-empty slots
    //     {
    //         buffer[0] = '\0';
    //         append_str(buffer, "|   Slot ");
    //         int_to_str(i + 1, num_str);
    //         append_str(buffer, num_str);
    //         append_str(buffer, ": ");
    //         // append_str(buffer, COLLECTIBLE_NAMES[current_tile.storage[i]]);
    //         append_str(buffer, "                                    \n");
    //         print(buffer);
    //     }
    // }
    print("+------------------------------------------------+\n");
    if (current_tile.interaction_text)
    {
        buffer[0] = '\0';
        append_str(buffer, "| ");
        append_str(buffer, current_tile.interaction_text);
        append_str(buffer, "\n");
        print(buffer);
    }
    print("+------------------------------------------------+\n");
}

// Custom string compare function
int string_compare(const char *str1, const char *str2)
{
    while (*str1 && *str2)
    {
        if (*str1 != *str2)
            return (*str1 - *str2);
        str1++;
        str2++;
    }
    return (*str1 - *str2);
}

void interact_with_tile(struct player player, struct tile current_tile, struct tile map[10][10])
{
    enum TILETYPE currentTileType = current_tile.type;

    if (currentTileType == STORAGE)
    {
        struct tile storage_tile = current_tile;
        int storage_choice = 0;
        while (storage_choice != 4)
        {

            // Clear screen and display frame
            display_frame(player, current_tile, map);

            print("Storage Menu:\n");
            print("1. View storage items\n");
            print("2. Transfer item from inventory to storage\n");
            print("3. Retrieve item from storage to inventory\n");
            print("4. Exit storage\n");
            print("Enter your choice: ");
            // storage_choice = your_input_function();
            storage_choice = 4; // Default choice for testing

            if (storage_choice == 1)
            {
                // View storage items
                check_storage(player, storage_tile);
                print("Press Enter to continue...");
                // Wait for input
            }
            else if (storage_choice == 2)
            {
                // Transfer item from inventory to storage
                print("Your inventory:\n");
                for (int i = 0; i < INVENTORY_SIZE; i++)
                {
                    char buffer[256];
                    buffer[0] = '\0';
                    append_str(buffer, "Slot ");
                    char num_str[12];
                    int_to_str(i + 1, num_str);
                    append_str(buffer, num_str);
                    append_str(buffer, ": ");
                    if (player.inventory[i] != NONE)
                    {
                        append_str(buffer, COLLECTIBLE_NAMES[player.inventory[i]]);
                    }
                    else
                    {
                        append_str(buffer, "Empty");
                    }
                    append_str(buffer, "\n");
                    print(buffer);
                }
                print("Enter the inventory slot number to transfer to storage (or 0 to cancel): ");
                int inv_slot = 0;
                // inv_slot = your_input_function();
                inv_slot = 0; // Default for testing

                if (inv_slot > 0 && inv_slot <= INVENTORY_SIZE)
                {
                    enum COLLECTIBLETYPE item = player.inventory[inv_slot - 1];
                    if (item != NONE)
                    {
                        add_to_storage(storage_tile, item);
                        remove_from_inventory(player, inv_slot - 1);
                    }
                    else
                    {
                        print("Inventory slot is empty.\n");
                    }
                }
                else
                {
                    print("Cancelled.\n");
                }
            }
            else if (storage_choice == 3)
            {
                // Transfer item from storage to inventory
                print("Storage items:\n");
                for (int i = 0; i < STORAGE_SIZE; i++)
                {
                    if (storage_tile.storage[i] != NONE)
                    {
                        char buffer[256];
                        buffer[0] = '\0';
                        append_str(buffer, "Slot ");
                        char num_str[12];
                        int_to_str(i + 1, num_str);
                        append_str(buffer, num_str);
                        append_str(buffer, ": ");
                        append_str(buffer, COLLECTIBLE_NAMES[storage_tile.storage[i]]);
                        append_str(buffer, "\n");
                        print(buffer);
                    }
                }
                print("Enter the storage slot number to transfer to inventory (or 0 to cancel): ");
                int storage_slot = 0;
                // storage_slot = your_input_function();
                storage_slot = 0; // Default for testing

                if (storage_slot > 0 && storage_slot <= STORAGE_SIZE)
                {
                    enum COLLECTIBLETYPE item = storage_tile.storage[storage_slot - 1];
                    if (item != NONE)
                    {
                        add_to_inventory(player, item);
                        remove_from_storage(storage_tile, storage_slot - 1);
                    }
                    else
                    {
                        print("Storage slot is empty.\n");
                    }
                }
                else
                {
                    print("Cancelled.\n");
                }
            }
            else if (storage_choice == 4)
            {
                print("Exiting storage.\n");
            }
            else
            {
                print("Invalid choice.\n");
            }
        }
    }
    // Handle other tile interactions similarly
    else if (currentTileType == COCKPIT)
    {
        char end_game;
        print("Do you want to end the game and return to Earth? (y/n)\n");
        // end_game = your_input_function_char();
        end_game = 'n'; // Default for testing
        if (end_game == 'y' || end_game == 'Y')
        {
            print("You have chosen to end the game. Goodbye!\n");
            // exit(0);
            return;
        }
        else
        {
            print("Continuing the game...\n");
        }
    }
    else if (currentTileType == CAFETERIA)
    {
        print("Grab some food? (y/n)\n");
        char grab_food;
        // grab_food = your_input_function_char();
        grab_food = 'n'; // Default for testing
        if (grab_food == 'y' || grab_food == 'Y')
        {
            add_to_inventory(player, FOOD);
        }
        else
        {
            print("You did not grab any food.\n");
        }
    }
    else if (currentTileType == ENGINE_BAY)
    {
        print("Add fuel to the engine bay? (y/n)\n");
        char add_fuel;
        // add_fuel = your_input_function_char();
        add_fuel = 'n'; // Default for testing

        if (add_fuel == 'y' || add_fuel == 'Y')
        {
            int has_fuel = 0;
            for (int i = 0; i < INVENTORY_SIZE; i++)
            {
                if (player.inventory[i] == FUEL)
                {
                    add_to_storage(current_tile, FUEL);
                    check_storage(player, current_tile);
                    remove_from_inventory(player, i);

                    print("You have now added fuel to the engine bay.\n");
                    has_fuel = 1;
                    // TODO: add code to light up one LED on board
                    break;
                }
            }
            if (!has_fuel)
            {
                print("You don't have any fuel in your inventory.\n");
            }
        }
    }
    else if (currentTileType == LABORATORY)
    {
        print("You can interact with laboratory\n");
        // Add logic for creating water/fuel
    }
    else if (currentTileType == AIRLOCK)
    {
        print("You are about to leave the ship, remember to keep an eye on the oxygen.\n");
    }
    else if (currentTileType == LANDING_SITE)
    {
        print("You are right outside the ship now.\n");
    }
    else if (currentTileType == WASTELAND)
    {
        print("You are in the harsh Martian wasteland.\n");
    }
    else if (currentTileType == LOOSE_SOIL)
    {
        print("Uh oh, it took a lot of energy to get out of the loose soil.\n");
    }
    else if (currentTileType == POND)
    {
        char discover_pond;
        print("You see a large ice pond, maybe there is life? Do you want to discover? (y/n)\n");
        // discover_pond = your_input_function_char();
        discover_pond = 'n'; // Default for testing
        if (discover_pond == 'y' || discover_pond == 'Y')
        {
            print("You look around and find some tardigrades! You collect them.\n");
            add_to_inventory(player, TARDIGRADES);
            remove_from_storage(current_tile, 1);
        }
    }
    else if (currentTileType == SHARP_ROCKS)
    {
        print("Oh no, the sharp rocks cracked one of your oxygen tanks, you only have half of the oxygen left!\n");
    }
    else if (currentTileType == CAVE)
    {
        print("You enter a dark cave, do you want to discover more? (y/n)\n");
        char discover_cave;
        // discover_cave = your_input_function_char();
        discover_cave = 'n'; // Default for testing
        if (discover_cave == 'y' || discover_cave == 'Y')
        {
            print("You found some alien bones and collected them.\n");
            add_to_inventory(player, ALIEN_BONES);
            remove_from_storage(current_tile, 1);
        }
    }
    else if (currentTileType == CRATER)
    {
        char discover_crater;
        print("You are in a crater, want to discover some more? (y/n)\n");
        // discover_crater = your_input_function_char();
        discover_crater = 'n'; // Default for testing
        if (discover_crater == 'y' || discover_crater == 'Y')
        {
            print("You found interesting sediments and collected some samples.\n");
            add_to_inventory(player, SEDIMENTARY_LAYERS);
            remove_from_storage(current_tile, 1);
        }
    }
    else if (currentTileType == CANYON)
    {
        char discover_canyon;
        print("You are in a large canyon, want to see what lies here? (y/n)\n");
        // discover_canyon = your_input_function_char();
        discover_canyon = 'n'; // Default for testing
        if (discover_canyon == 'y' || discover_canyon == 'Y')
        {
            print("You find interesting RSL trails, you collect some images.\n");
            add_to_inventory(player, RSL_IMAGES);
            remove_from_storage(current_tile, 1);
        }
    }
    else if (currentTileType == MOUNTAIN)
    {
        char discover_mountain;
        print("You are on top of a big mountain, want to discover? (y/n)\n");
        // discover_mountain = your_input_function_char();
        discover_mountain = 'n'; // Default for testing
        if (discover_mountain == 'y' || discover_mountain == 'Y')
        {
            print("You find some ice samples and collect them.\n");
            add_to_inventory(player, ICE);
            remove_from_storage(current_tile, 1);
        }
    }
    else
    {
        print("There is nothing to interact with here.\n");
    }
}
