#include <stddef.h>
#include "game.h"
#include "tile.h"
#include "player.h"
#include "inventory.h"
#include "board_io.h"

// Predefined masks for which switch that trigger what response
#define SWITCH_EAST (1 << 0)  // Bit 0 -> shifts 0b1 by 0 pos. resulting in 0b0001
#define SWITCH_WEST (1 << 1)  // Bit 1 -> shifts 0b1 by 1 pos. resulting in 0b0010
#define SWITCH_SOUTH (1 << 2) // Bit 2 -> shifts 0b1 by 2 pos. resulting in 0b0100
#define SWITCH_NORTH (1 << 3) // Bit 3 -> shifts 0b1 by 3 pos. resulting in 0b1000

#define SWITCH_DRINK (1 << 6) // Bit 6 -> shifts 0b1 by 6 pos. resulting in 0b01000000
#define SWITCH_EAT (1 << 7)   // bit 7 -> shifts 0b1 by 7 pos. resulting in 0b10000000

#define SWITCH_BACK (1 << 8)     // Bit 8 -> shifts 0b1 by 8 pos. resulting in 0b0000000100000000
#define SWITCH_INTERACT (1 << 9) // Bit 9 -> shifts 0b1 by 9 pos. resulting in 0b0000001000000000

// Slot selection switches
#define SWITCH_SLOT_1 (1 << 0)   
#define SWITCH_SLOT_2 (1 << 1)
#define SWITCH_SLOT_3 (1 << 2)
#define SWITCH_SLOT_4 (1 << 3)
#define SWITCH_SLOT_5 (1 << 4)

extern void print(const char *);
extern void print_dec(int);

// Function Prototypes
void interact_with_tile(struct player *player, struct tile *current_tile, struct tile map[10][10], int switch_values);
void display_frame(struct player player, struct tile current_tile, struct tile map[10][10]);

// flags
int current_display = 0;
int game_running = 1;

// Custom function to convert integer to string
// Parameters:
// - num: The integer to convert.
// - str: The buffer to store the resulting string. Must be large enough to hold the result, including the null terminator.
void int_to_str(int num, char *str)
{
    int i = 0;      // Index for temp array
    int is_negative = 0;    // flag for neg numbers
    char temp[12];  // temp. buffer for holding numbers digits in reverse order

    // Handles special case when num=0
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Check if its negative
    if (num < 0)
    {
        is_negative = 1;    // Set flag for neg.
        num = -num;         // Make number pos.
    }

    // Extract digits and store in temp buffer in reverse order
    while (num != 0)
    {
        temp[i++] = (num % 10) + '0';       // Get last digit, convert to str and store
        num /= 10;
    }

    // add minus sign to temp buffer if its neg. number
    if (is_negative)
    {
        temp[i++] = '-';
    }

    // Index for final res. str.
    int j = 0;

    // Reverse the string
    while (i > 0)
    {
        str[j++] = temp[--i];
    }
    str[j] = '\0';
}

// Custom function to concatenate strings
// Appends the source string (src) to the destination string (dest).
// Parameters:
// - dest: The destination string, which must have enough space to hold the appended result.
// - src: The source string to append to dest.
void append_str(char *dest, const char *src)
{
    // Move the `dest` pointer to the null terminator of the existing string (= end of the string dest).
    while (*dest)
        dest++; // Advance until the end of the current string.
    
    // Copy each character from `src` to the end of `dest`.
    while (*src)
    {
        *dest = *src;   // Copy the current character from `src` to `dest`.
        dest++;         // Move to the next position in `dest`.
        src++;          // Move to the next position in `src`.
    }
    // Add a null terminator to mark the end of the concatenated string.
    *dest = '\0';
}

// Custom function to get the length of a string
int str_length(const char *str)
{
    // Set length to 0 initially
    int len = 0;

    // Increment len until it is out of bounds w str array
    while (str[len])
        len++;
    return len;
}

// Func to drop stats -> food, water, oxygen
void drop_stats(struct player *player, struct tile current_tile)
{
    // Check if tiletype is outside rocket and only drop oxygen if so
    if (current_tile.outside_rocket == 1)
    {
        player->oxygen -= 1;
    }
    // Set oxygen to 99 as long as player is in rocket 
    else if (current_tile.outside_rocket == 0)
    {
        player->oxygen = 99;
    }
    // drop water and food
    player->water -= 1;
    player->food -= 1;

    // Check if any stat reaches 0 and set game_running flag to false(=0)
    if (player->oxygen <= 0)
    {
        print("You have run out of oxygen! Game over.\n");
        game_running = 0;
    }
    if (player->water <= 0)
    {
        print("You have run out of water! Game over.\n");
        game_running = 0;
    }
    if (player->food <= 0)
    {
        print("You have run out of food! Game over.\n");
        game_running = 0;
    }
}

// print for a little while and then disappear
void print_hold(char *s)
{
    print(s);       // print input
    int error_wait = 2;     // initialize counter for timer events

    // wait for 2 timer events
    while (error_wait)
    {
        // Check if event has occurred
        if (timer_event)
        {
            timer_event = 0; // Reset the flag
            error_wait--;   // decrement counter
        }
    }
}

// Func which handles inputs from the board from switches
void handle_input(int switch_values, struct player *player, struct tile map[10][10])
{
    // Initialize startpos.
    int new_positionX = player->positionX;
    int new_positionY = player->positionY;
    int moved = 0; // Flag to check if movement occurred

    // If interact is on, don't move, just interact
    if (switch_values & SWITCH_INTERACT)
    {
        struct tile *current_tile = &map[player->positionY][player->positionX];
        interact_with_tile(player, current_tile, map, switch_values);       // calls func that eval how interaction with each tiletype looks like
        return;
    }

    // Movement logic using AND-operator
    if (switch_values & SWITCH_EAST)
    {
        new_positionX += 1;     // Go right using switch 0
        moved = 1;
    }
    if (switch_values & SWITCH_WEST)
    {
        new_positionX -= 1;     // Go left using switch 1
        moved = 1;
    }
    if (switch_values & SWITCH_SOUTH)
    {
        new_positionY += 1;     // Go down using switch 2
        moved = 1;
    }
    if (switch_values & SWITCH_BACK)
    {
        new_positionY -= 1;     // Go up using switch 4
        moved = 1;
    }

    // If any movement occurred
    if (moved)
    {
        // Check that player moves in correct position (inside the 10x10 map)
        if (new_positionX >= 0 && new_positionX < 10 &&
            new_positionY >= 0 && new_positionY < 10)
        {
            // Check that tile is not EMPTY type (rocket walls)
            if (!(map[new_positionY][new_positionX].type == EMPTY))
            {
                // Check that it is not mountain since they cant be crossed
                if (!(map[new_positionY][new_positionX].type == MOUNTAIN))
                {
                    // Set pos. for player to the new incremented/decremented pos.
                    player->positionX = new_positionX;
                    player->positionY = new_positionY;
                }
                // If it is indeed a mountain
                else
                {
                    print_hold("You cant move here, there is a mountain in you way!\n");
                }
            }
            // If it is rocket walls
            else
            {
                print_hold("You cant move here, there is a rocket in you way! Perhaps you can find a door?\n");
            }
        }
        // If it is outside the map
        else
        {
            print_hold("Don't go that way, beyond the map you will get lost!\n");
        }
    }

    // Handle drinking water
    if (switch_values & SWITCH_DRINK)
    {
        // Look for WATER in the inventory
        int found = -1;
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
            if (player->inventory[i] == BOTTLE_OF_WATER)
            {
                found = i;      // set slot where water was found
                break;
            }
        }

        // If we indeed find water
        if (found != -1)
        {
            // Consume the water
            player->inventory[found] = NONE;        // Clear water from inventory

            // Increase player's water stat by 50 but max at 99
            player->water += 50;
            if (player->water > 99)
                player->water = 99;
            print_hold("You drank water and quenched your thirst!\n");
        }
        else
        {
            print_hold("You have no water to drink.\n");
        }
    }

    // Handle eating food
    if (switch_values & SWITCH_EAT)
    {
        // Look for FOOD in the inventory
        int found = -1;
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
            if (player->inventory[i] == FOOD)
            {
                found = i;      // Set to index where food was found in inventory
                break;
            }
        }

        // If food is found
        if (found != -1)
        {
            // Consume the food
            player->inventory[found] = NONE;        // Set slot to NONE
            // Increase player's food stat by 50 but max at 99
            player->food += 50;
            if (player->food > 99)
                player->food = 99;
            print_hold("You ate some food and feel more energetic!\n");
        }
        else
        {
            print_hold("You have no food to eat.\n");
        }
    }
}

// Func which updates 7seg displays
void update_display(struct player player)
{
    if (current_display == 0)
    {
        // Set leftmost displays to Food
        set_displays(5, 10);    // 10 = F
        set_displays(4, 11);    // 11 = o
        set_displays(3, 11);    // 11 = o
        set_displays(2, 12);    // 12 = d

        // Set countdown for foodstats
        int food_10 = player.food / 10;     // retrieve tiotal
        int food_1 = player.food % 10;      // retrieve ental
        set_displays(1, food_10);
        set_displays(0, food_1);
        return;
    }
    else if (current_display == 1)
    {
        // Set leftmost displays to H2O
        set_displays(5, 13);  // 13 = H
        set_displays(4, 2);   // 2 = 2
        set_displays(3, 0);   // 0 = O
        set_displays(2, 100); // empty

        // Set countdown for water
        int water_10 = player.water / 10;
        int water_1 = player.water % 10;
        set_displays(1, water_10);
        set_displays(0, water_1);
        return;
    }
    else if (current_display == 2)
    {
        // Set leftmost displays to O2
        set_displays(5, 0);     // 0 = O
        set_displays(4, 2);     // 2 = 2
        set_displays(3, 100);   // empty
        set_displays(2, 100);   // empty

        // Set oxygen countdown
        int oxygen_10 = player.oxygen / 10;
        int oxygen_1 = player.oxygen % 10;
        set_displays(1, oxygen_10);
        set_displays(0, oxygen_1);
        return;
    }
}

// To show process using LEDs -> only possible after adding fuel to engine bay
void update_status(struct player player, struct tile map[10][10])
{
    int number_of_leds = 0b0000000000;      // start with no light up leds
    struct tile storage_tile = map[4][1];   
    struct tile engine_bay = map[5][0];

    // If fuel in engine bay -> 9 = enum FUEL for COLLECTIBLETYPE
    if (engine_bay.storage[0] == 9)
    {
        // Shift one step to the left so 0b0000000001 becomes 0b0000000010 and 0b0000000011 becomes 0b0000000110
        number_of_leds <<= 1;
        number_of_leds += 0b0000000001;     // add 1 so lsb LED light up
    }
    else
    {
        return;
    }

    // check entire inventory for stuff in enum between 10-16 since these are essentials
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        // If inventory contain essential items for win rate (10-16)
        if (player.inventory[i] >= 10 && player.inventory[i] <= 16)
        {
            // shift one step left and add 1 
            number_of_leds <<= 1;
            number_of_leds += 0b0000000001;
        }
    }

    // same logic but for storage
    for (int i = 0; i < STORAGE_SIZE; i++)
    {
        if (storage_tile.storage[i] >= 10 && storage_tile.storage[i] <= 16)
        {
            number_of_leds <<= 1;
            number_of_leds += 0b0000000001;
        }
    }
    // use number_of_leds to mask which LEDs to light
    set_leds(number_of_leds);
}

// main renamed temporarily for performance tests
void start_game()
{
    print("1");
    labinit();
    print("2");

    // a map made out of struct tiles using func ct (=create tile)
    struct tile map[10][10] = {
        {ct(POND), ct(LOOSE_SOIL), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(POND), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(POND)},
        {ct(SHARP_ROCKS), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(POND), ct(WASTELAND), ct(MOUNTAIN), ct(CANYON)},
        {ct(EMPTY), ct(EMPTY), ct(EMPTY), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(CANYON)},
        {ct(CHAMBERS), ct(COCKPIT), ct(EMPTY), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(SHARP_ROCKS), ct(POND), ct(MOUNTAIN), ct(WASTELAND)},
        {ct(CAFETERIA), ct(STORAGE), ct(AIRLOCK), ct(LANDING_SITE), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(LOOSE_SOIL)},
        {ct(ENGINE_BAY), ct(LABORATORY), ct(EMPTY), ct(WASTELAND), ct(WASTELAND), ct(LOOSE_SOIL), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(WASTELAND)},
        {ct(EMPTY), ct(EMPTY), ct(EMPTY), ct(SHARP_ROCKS), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(SHARP_ROCKS), ct(WASTELAND)},
        {ct(WASTELAND), ct(WASTELAND), ct(LOOSE_SOIL), ct(WASTELAND), ct(WASTELAND), ct(CANYON), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(WASTELAND)},
        {ct(CRATER), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(MOUNTAIN), ct(WASTELAND), ct(MOUNTAIN), ct(MOUNTAIN), ct(MOUNTAIN), ct(POND)},
        {ct(CAVE), ct(WASTELAND), ct(WASTELAND), ct(MOUNTAIN), ct(POND), ct(WASTELAND), ct(WASTELAND), ct(CANYON), ct(MOUNTAIN), ct(POND)},
    };

    print("3");

    // initialize a player of type struct player
    struct player player = create_player();
    print("1");

    // set current tile to the one that corresponds to playerpos.
    struct tile current_tile = map[player.positionY][player.positionX];
    display_frame(player, current_tile, map);

    // If game_running flag is true
    while (game_running)
    {
        // When switch event is triggered
        if (switch_event)
        {
            switch_event = 0; // Reset the flag
            int switch_values = get_sw();   // retrieve switch values
            handle_input(switch_values, &player, map);  // handle switch values input accordingly

            current_tile = map[player.positionY][player.positionX]; // set position
            display_frame(player, current_tile, map);   // call display frame func with new pos.
        }

        // when timer event happens, reset flag and drop stats for player, 
        if (timer_event)
        {
            timer_event = 0; // Reset the flag
            drop_stats(&player, current_tile);
            update_display(player);             // update 7seg displays 
        }
        // when button event happens, reset flag 
        if (button_event)
        {
            update_display(player);
            current_display = (current_display + 1) % 3; // always 0, 1, 2 -> switch between displaying of food, water, and oxygen
            button_event = 0;                            // Reset the flag
        }
        // update status of LEDS for each cycle
        update_status(player, map);
    }
}

// displays the entire map and info etc
void display_frame(struct player player, struct tile current_tile, struct tile map[10][10])
{
    print("\x1B[2J"); // Clear screen
    print("\x1B[H");  // Move cursor to top-left

    // Print title of game
    print("+------------------------------------------------+ \n");
    print("|            MARS SURVIVAL GAME                  | \n");
    print("+------------------------------------------------+ \n");

    // Use buffer to concatenete strings w variables
    char buffer[256];
    buffer[0] = '\0';

    append_str(buffer, "| Position: (");
    char num_str[12];
    int_to_str(player.positionX, num_str);      // add pos X 
    append_str(buffer, num_str);                // append pos X
    append_str(buffer, ", ");
    int_to_str(player.positionY, num_str);      // add pos Y
    append_str(buffer, num_str);                // append pos Y
    append_str(buffer, ")                               \n");
    print(buffer);

    buffer[0] = '\0';
    append_str(buffer, "| Location: ");
    append_str(buffer, TILETYPE_NAMES[current_tile.type]);  // append location name as string from TILETYPE_NAMES 
    append_str(buffer, "                                     \n");
    print(buffer);

    // Print map
    print("+------------------------------------------------+ \n");
    print("| Map:                                             \n");

    // iterate through each row
    for (int y = 0; y < 10; y++)
    {
        // prepare a buffer for each row
        char line_buffer[1024];
        line_buffer[0] = '\0';
        append_str(line_buffer, "| ");

        // Iterate through each column of each current row
        for (int x = 0; x < 10; x++)
        {
            // If current tile coordinates matches players, add santa emoji (player)
            if (player.positionX == x && player.positionY == y)
            {
                append_str(line_buffer, "[🎅] ");
            }
            // Otherwise, retrieve emoji from TILETYPE_MAP_NAMES which is a list of emojis for each tiletype
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
    // print inventory content
    print("| Inventory:                                     | \n");
    // iterate through the entire inventory
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        buffer[0] = '\0';
        append_str(buffer, "|   Slot ");
        int_to_str(i + 1, num_str);
        append_str(buffer, num_str);
        append_str(buffer, ": ");

        // append str for name of inventory item if its not NONE
        if (player.inventory[i] != NONE)
        {
            append_str(buffer, COLLECTIBLE_NAMES[player.inventory[i]]);
        }
        // otherwise just print empty for current slot
        else
        {
            append_str(buffer, "Empty");
        }
        append_str(buffer, "                                    \n");
        print(buffer);
    }
    // Print interaction text for current tile -> avail in struct for tiletype
    print("+------------------------------------------------+\n");
    if (current_tile.interaction_text)
    {
        buffer[0] = '\0';
        append_str(buffer, "| ");
        append_str(buffer, current_tile.interaction_text); // retrieve interaction text from tiletype
        append_str(buffer, "\n");
        print(buffer);
    }
    print("+------------------------------------------------+\n");
}

// Wait for yes/no input using EAST (yes) and WEST (no)
int wait_for_yes_no(void)
{
    print("Press EAST for Yes, WEST for No.\n");
    while (1)
    {
        int sv = get_sw();      // retrieve switch inputs
        if (sv & SWITCH_EAST) // Yes 
            return 1;
        if (sv & SWITCH_WEST) // No
            return 0;
    }
    return 0;
}

// Wait for the player to exit by pressing NORTH
void wait_for_exit(const char *msg)
{
    if (!msg)
        msg = "Press BACK to exit.\n";
    print_hold(msg);

    // Infinite loop to wait for pressing BACK
    while (1)
    {
        int sv = get_sw();
        if (sv & SWITCH_BACK)
        {
            print_hold("Exiting...\n");
            break;
        }
    }
}

// Function that handles interaction with every tiletype
void interact_with_tile(struct player *player, struct tile *current_tile, struct tile map[10][10], int switch_values)
{
    // retrieves type for the current tile
    enum TILETYPE currentTileType = current_tile->type;

    // if its storage
    if (currentTileType == STORAGE)
    {
        // choose one of following options
        print("Storage Menu:\n");
        print("EAST: View storage items\n");
        print("WEST: Transfer item from inventory to storage\n");
        print("SOUTH: Transfer item from storage to inventory\n");
        print("NORTH: Exit storage\n");
        print("Press a switch to choose an action.\n");

        // infinite loop to wait for user input
        while (1)
        {
            switch_values = get_sw();   // retrieve switch inputs

            // if turned BACK switch on -> exit storage interaction mode
            if (switch_values & SWITCH_BACK)
            {
                print_hold("Exiting storage.\n");
                return;
            }
            // if choose east
            else if (switch_values & SWITCH_EAST)
            {
                // View storage items
                print_hold("Storage items:\n");
                check_storage(player, current_tile);    // call check_storage and display content
                break;
            }
            // if choose west
            else if (switch_values & SWITCH_WEST)
            {
                // Transfer item from inventory to storage
                print_hold("Your inventory:\n");

                // print inventory logic
                for (int i = 0; i < INVENTORY_SIZE; i++)
                {
                    char buffer[256];
                    buffer[0] = '\0';
                    append_str(buffer, "Slot ");
                    char num_str[12];
                    int_to_str(i + 1, num_str);
                    append_str(buffer, num_str);
                    append_str(buffer, ": ");

                    // append inventory item name if it exists
                    if (player->inventory[i] != NONE)
                    {
                        append_str(buffer, COLLECTIBLE_NAMES[player->inventory[i]]);
                    }
                    else
                    {
                        append_str(buffer, "Empty");
                    }
                    append_str(buffer, "\n");
                    print(buffer);
                }

                print("Flip one slot switch (1-5) to choose the slot to transfer (BACK to cancel):\n");

                int chosen_slot = 0;    // This variable will store the index of the slot that the user chooses for the transfer 
                int needs_reset = 1;    // This flag helps manage the flow to ignore invalid inputs at the beginning of the loop.
                
                // start infinite loop for switch detection
                while (1)
                {
                    int current_switch = get_sw();      // retrieve switches
                    if (!current_switch && needs_reset) 
                    {
                        needs_reset = 0;    // ensures that the user can’t make a valid selection until a switch is flipped
                    }
                    if (needs_reset)
                        continue;
                    // Cancel action 
                    if (current_switch & SWITCH_BACK)
                    {
                        print_hold("Cancelled.\n");
                        break;
                    }
                    // detects which slot is switched
                    int slot_switches = current_switch & (SWITCH_SLOT_1 | SWITCH_SLOT_2 | SWITCH_SLOT_3 | SWITCH_SLOT_4 | SWITCH_SLOT_5);
                    if (slot_switches == 0)
                        continue;
                    // Based on which switch is set, modify chosen slot
                    if (slot_switches & SWITCH_SLOT_1)
                        chosen_slot = 1;
                    else if (slot_switches & SWITCH_SLOT_2)
                        chosen_slot = 2;
                    else if (slot_switches & SWITCH_SLOT_3)
                        chosen_slot = 3;
                    else if (slot_switches & SWITCH_SLOT_4)
                        chosen_slot = 4;
                    else if (slot_switches & SWITCH_SLOT_5)
                        chosen_slot = 5;

                    // choose thing to pluck based on switch value (must be valid)
                    if (chosen_slot > 0 && chosen_slot <= INVENTORY_SIZE)
                    {
                        enum COLLECTIBLETYPE item = player->inventory[chosen_slot - 1]; // retrieve item from correct slot in inventory
                        if (item != NONE)
                        {
                            add_to_storage(current_tile, item);     // add the item to storage
                            remove_from_inventory(player, chosen_slot - 1);     // remove the item from inventory
                        }
                        else
                        {
                            print_hold("Inventory slot is empty.\n");
                        }
                    }
                    else
                    {
                        print_hold("Cancelled.\n");
                    }
                    break;
                }
                break;
            }
            // transfer from storage to inventory
            else if (switch_values & SWITCH_SOUTH)
            {
                // Transfer item from storage to inventory
                print_hold("Storage items:\n");

                // iterate through storage 
                for (int i = 0; i < STORAGE_SIZE; i++)
                {
                    // If it is not NONE
                    if (current_tile->storage[i] != NONE)
                    {
                        char buffer[256];
                        buffer[0] = '\0';
                        append_str(buffer, "Slot ");
                        char num_str[12];
                        int_to_str(i + 1, num_str);
                        append_str(buffer, num_str);
                        append_str(buffer, ": ");
                        append_str(buffer, COLLECTIBLE_NAMES[current_tile->storage[i]]);
                        append_str(buffer, "\n");
                        print(buffer);
                    }
                }

                print("Flip one slot switch (1-5) to choose the storage slot to transfer to inventory (BACK to cancel):\n");

                // same logic
                int chosen_slot = 0;
                int needs_reset = 1;
                while (1)
                {
                    int current_switch = get_sw();
                    if (!current_switch && needs_reset)
                        needs_reset = 0;

                    if (needs_reset)
                        continue;

                    if (current_switch & SWITCH_BACK)
                    {
                        print_hold("Cancelled.\n");
                        break;
                    }

                    if (current_switch & SWITCH_SLOT_1)
                        chosen_slot = 1;
                    else if (current_switch & SWITCH_SLOT_2)
                        chosen_slot = 2;
                    else if (current_switch & SWITCH_SLOT_3)
                        chosen_slot = 3;
                    else if (current_switch & SWITCH_SLOT_4)
                        chosen_slot = 4;
                    else if (current_switch & SWITCH_SLOT_5)
                        chosen_slot = 5;

                    if (chosen_slot > 0 && chosen_slot <= STORAGE_SIZE)
                    {
                        enum COLLECTIBLETYPE item = current_tile->storage[chosen_slot - 1];
                        if (item != NONE)
                        {
                            add_to_inventory(player, item);
                            remove_from_storage(current_tile, chosen_slot - 1);
                        }
                        else
                        {
                            print_hold("Storage slot is empty.\n");
                        }
                    }
                    else
                    {
                        continue;
                    }
                    break;
                }
                break;
            }
        }

        wait_for_exit("Press NORTH (or BACK) to exit storage.\n");
        print("Exiting storage.\n");
    }
    // if its cockpit
    else if (currentTileType == COCKPIT)
    {
        print("Do you want to end the game and return to Earth?\n");
        if (wait_for_yes_no())
        {
            print("You have chosen to end the game.!\n");
            int chance_to_survive = 0;      // initial chance to survive

            // retrieve leds -> set chance to survive depending on leds
            switch (get_leds())
            {
            // you cant try to leave unless you atleast have fuel in engine
            case 0b0000000000:
                print_hold("You tried to leave, but the ship doesn't want to start...");
                return;
            case 0b0000000001:
                print("1");
                chance_to_survive = 10;
                break;
            case 0b0000000011:
                print("2");
                chance_to_survive = 20;
                break;
            case 0b0000000111:
                print("3");
                chance_to_survive = 30;
                break;
            case 0b0000001111:
                print("4");
                chance_to_survive = 40;
                break;
            case 0b0000011111:
                print("5");
                chance_to_survive = 50;
                break;
            case 0b0000111111:
                print("6");
                chance_to_survive = 60;
                break;
            case 0b0001111111:
                print("7");
                chance_to_survive = 70;
                break;
            case 0b0011111111:
                print("8");
                chance_to_survive = 80;
                break;
            case 0b0111111111:
                print("9");
                chance_to_survive = 90;
                break;
            case 0b1111111111:
                print("10");
                chance_to_survive = 98;     // just for fun hehe :)
                break;
            default:
                break;
            }

            // math magic (magic = food) to calculate win or loss based on percentage chance
            int random_value = (player->food * player->water * 6942069) % 100;
            print("Your chance to survive was: ");
            print_dec(chance_to_survive);
            print("%\n");

            print("The random value was: ");
            print_dec(random_value);
            print("\n");

            // if random value less than chance to survive -> you win!!! congrats
            if (random_value <= chance_to_survive)
            {
                print("You survived the journey back to Earth! Congratulations!\n");
            }
            // if its greater than survival chance you die -> game over
            else
            {
                print("Unfortunately, you did not survive the journey back to Earth.\n");
            }
            // set game running flag to false
            game_running = 0;
        }
        else
        {
            print("Continuing the game...\n");
        }
        wait_for_exit(NULL);
    }
    // if its cafeteria
    else if (currentTileType == CAFETERIA)
    {
        print("Grab some food?\n");
        if (wait_for_yes_no())
        {
            add_to_inventory(player, FOOD);     // add food to inventory 
        }
        else
        {
            print_hold("You did not grab any food.\n");
        }
        wait_for_exit(NULL);
    }
    // engine bay logic
    else if (currentTileType == ENGINE_BAY)
    {
        print("Add fuel to the engine bay?\n");
        if (wait_for_yes_no())
        {
            int has_fuel = 0;       // flag to check if fuel is added

            // iterate through inventory
            for (int i = 0; i < INVENTORY_SIZE; i++)
            {
                // add fuel if fuel is found in inventory
                if (player->inventory[i] == FUEL)
                {
                    add_fuel_enginebay(current_tile, FUEL);
                    check_storage(player, current_tile);
                    remove_from_inventory(player, i);
                    print("You have now added fuel to the engine bay.\n");
                    has_fuel = 1;       // set flag to true
                    break;
                }
            }
            // if there is no fuel in inventory
            if (!has_fuel)
            {
                print_hold("You don't have any fuel in your inventory.\n");
            }
        }
        wait_for_exit(NULL);
    }
    // lab logic
    else if (currentTileType == LABORATORY)
    {
        print("You can interact with the laboratory.\n");
        print("EAST: Attempt to create WATER\n");
        print("WEST: Attempt to create FUEL\n");
        print("NORTH: Exit\n");

        while (1)
        {
            int sv = get_sw();  // retrieve player choice
            if (sv & SWITCH_NORTH)
            {
                break;
            }
            else if (sv & SWITCH_EAST)
            {
                add_to_inventory(player, BOTTLE_OF_WATER);      // add water to inventory
                print_hold("You created some WATER.\n");
                break;
            }
            else if (sv & SWITCH_WEST)
            {
                add_to_inventory(player, FUEL);                 // add fuel to inventory
                print_hold("You created some FUEL.\n");
                break;
            }
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == AIRLOCK)
    {
        print_hold("You are about to leave the ship, remember to keep an eye on the oxygen.\n");    // reminder
        wait_for_exit(NULL);
    }
    else if (currentTileType == LANDING_SITE)
    {
        print_hold("You are right outside the ship now.\n");
        print("Do you want to look around the landing site?\n");
        if (wait_for_yes_no())
        {
            print("After exploring the landing site, you find a toolbox with vital tools for fixing the ship!\n");
            add_to_inventory(player, TOOLBOX);      // can find toolbox here
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == WASTELAND)
    {
        print_hold("You are in the harsh Martian wasteland.\n");

        // Find medkit in wasteland pos. (4, 0) only
        if (player->positionY == 0 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a medical kit!\n");
                add_to_inventory(player, MEDICAL_KIT);
            }
        }

        // Find map in pos. (1, 7) only
        if (player->positionY == 7 && player->positionX == 1)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a MAP!\n");
                add_to_inventory(player, MAP);
            }
        }
        // Find spare parts in pos. (4, 2) only
        if (player->positionY == 2 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found spare parts!\n");
                add_to_inventory(player, SPARE_PARTS);
            }
        }
        // find teddy bear in pos (4, 6) only
        if (player->positionY == 6 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a teddy bear!\n");
                add_to_inventory(player, TEDDY_BEAR);
            }
        }
        // find blanket in pos (4, 5) only
        if (player->positionY == 5 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a blanket!\n");
                add_to_inventory(player, BLANKET);
            }
        }
        // find clothing in pos (4, 4) onnly
        if (player->positionY == 4 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found cozy clothing!\n");
                add_to_inventory(player, CLOTHING);
            }
        }
        // find old rover parts in pos (4, 3) only
        if (player->positionY == 3 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a cool rover arm!\n");
                add_to_inventory(player, OLD_ROVER_PARTS);
            }
        }

        wait_for_exit(NULL);
    }
    else if (currentTileType == LOOSE_SOIL)
    {
        print_hold("Be careful, don't get stuck here!\n");
        player->food /= 2;      // if interact w loose soil, loose half of food
        wait_for_exit(NULL);
    }
    else if (currentTileType == POND)
    {
        print("You see a large ice pond, maybe there is life? Discover?\n");
        if (wait_for_yes_no())
        {
            // Collect tardigrades
            print("You look around and find some tardigrades! You collect them.\n");
            add_to_inventory(player, TARDIGRADES);      // find life
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == SHARP_ROCKS)
    {
        print_hold("Be careful of such sharp rocks, they may break something.\n");
        player->oxygen /= 2;        // remove half of oxygen if interact with sharp rocks
        wait_for_exit(NULL);
    }
    else if (currentTileType == CAVE)
    {
        print("You enter a dark cave, discover more?\n");
        if (wait_for_yes_no())
        {
            print_hold("You found some alien bones and collected them.\n");
            add_to_inventory(player, ALIEN_BONES);      // posibility to retrieve alien bones in cave
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == CRATER)
    {
        print("You are in a crater, want to discover some more?\n");
        // posibility to retrieve sedimentary layers in crater
        if (wait_for_yes_no())
        {
            print("You found interesting sediments and collected some samples.\n");
            add_to_inventory(player, SEDIMENTARY_LAYERS);
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == CANYON)
    {
        print("You are in a large canyon, want to see what lies here?\n");
        // posibility to retrieve rsl images in canyon
        if (wait_for_yes_no())
        {
            print("You find interesting RSL trails, you collect some images.\n");
            add_to_inventory(player, RSL_IMAGES);
        }
        wait_for_exit(NULL);
    }
    // no conditions met -> print no interaction
    else
    {
        print_hold("There is nothing to interact with here.\n");
        wait_for_exit(NULL);
    }
}
