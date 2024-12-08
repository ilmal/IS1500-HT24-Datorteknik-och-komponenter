#include <stddef.h>
#include "game.h"
#include "tile.h"
#include "player.h"
#include "inventory.h"
#include "board_io.h"

#define SWITCH_EAST (1 << 0)  // Bit 0
#define SWITCH_WEST (1 << 1)  // Bit 1
#define SWITCH_SOUTH (1 << 2) // Bit 2
#define SWITCH_NORTH (1 << 3) // Bit 3

#define SWITCH_DRINK (1 << 6)
#define SWITCH_EAT (1 << 7)

#define SWITCH_BACK (1 << 8)     // Bit 8
#define SWITCH_INTERACT (1 << 9) // Bit 9

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

int current_display = 0;
int game_running = 1;

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

// Func to drop stats -> food, water, oxygen (added gameover flag)
void drop_stats(struct player *player, struct tile current_tile)
{
    // Check if tiletype is outside rocket and only drop oxygen if so
    if (current_tile.outside_rocket == 1)
    {
        player->oxygen -= 1;
    }
    else if (current_tile.outside_rocket == 0)
    {
        player->oxygen = 99;
    }
    // drop water and food
    player->water -= 1;
    player->food -= 1;

    // Check if any stat reaches 0
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
    print(s);
    int error_wait = 2;
    while (error_wait)
    {
        if (timer_event)
        {
            timer_event = 0; // Reset the flag
            error_wait--;
        }
    }
}

void handle_input(int switch_values, struct player *player, struct tile map[10][10])
{
    int new_positionX = player->positionX;
    int new_positionY = player->positionY;
    int moved = 0; // Flag to check if movement occurred

    // If interact is on, don't move, just interact
    if (switch_values & SWITCH_INTERACT)
    {
        struct tile *current_tile = &map[player->positionY][player->positionX];
        interact_with_tile(player, current_tile, map, switch_values);
        return;
    }

    // Movement logic
    if (switch_values & SWITCH_EAST)
    {
        new_positionX += 1;
        moved = 1;
    }
    if (switch_values & SWITCH_WEST)
    {
        new_positionX -= 1;
        moved = 1;
    }
    if (switch_values & SWITCH_SOUTH)
    {
        new_positionY += 1;
        moved = 1;
    }
    if (switch_values & SWITCH_BACK)
    {
        new_positionY -= 1;
        moved = 1;
    }

    // If any movement occurred
    if (moved)
    {
        // Check that player moves in correct position
        if (new_positionX >= 0 && new_positionX < 10 &&
            new_positionY >= 0 && new_positionY < 10)
        {
            if (!(map[new_positionY][new_positionX].type == EMPTY))
            {
                if (!(map[new_positionY][new_positionX].type == MOUNTAIN))
                {
                    player->positionX = new_positionX;
                    player->positionY = new_positionY;
                }
                else
                {
                    print_hold("You cant move here, there is a mountain in you way!\n");
                }
            }
            else
            {
                print_hold("You cant move here, there is a rocket in you way! Perhaps you can find a door?\n");
            }
        }
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
                found = i;
                break;
            }
        }

        if (found != -1)
        {
            // Consume the water
            player->inventory[found] = NONE;
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
                found = i;
                break;
            }
        }

        if (found != -1)
        {
            // Consume the food
            player->inventory[found] = NONE;
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

void update_display(struct player player)
{
    if (current_display == 0)
    {
        // Set leftmost displays to Food
        set_displays(5, 10);
        set_displays(4, 11);
        set_displays(3, 11);
        set_displays(2, 12);

        // Set countdown for foodstats
        int food_10 = player.food / 10;
        int food_1 = player.food % 10;
        set_displays(1, food_10);
        set_displays(0, food_1);
        return;
    }
    else if (current_display == 1)
    {
        // Set leftmost displays to H2O
        set_displays(5, 13);
        set_displays(4, 2);
        set_displays(3, 0);
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
        set_displays(5, 0);
        set_displays(4, 2);
        set_displays(3, 100);
        set_displays(2, 100);

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
    int number_of_leds = 0b0000000000;
    struct tile storage_tile = map[4][1];
    struct tile engine_bay = map[5][0];

    // If fuel in engine bay
    if (engine_bay.storage[0] == 9)
    {
        // Shift one step to the left and add one 1bit
        number_of_leds <<= 1;
        number_of_leds += 0b0000000001;
    }
    else
    {
        return;
    }

    // check entire inventory for stuff in enum between 10-16 since these are essentials
    for (int i = 0; i < INVENTORY_SIZE; i++)
    {
        if (player.inventory[i] >= 10 && player.inventory[i] <= 16)
        {
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

    struct player player = create_player();
    print("1");

    struct tile current_tile = map[player.positionY][player.positionX];
    display_frame(player, current_tile, map);

    while (game_running)
    {
        if (switch_event)
        {
            switch_event = 0; // Reset the flag
            int switch_values = get_sw();
            handle_input(switch_values, &player, map);

            current_tile = map[player.positionY][player.positionX];
            display_frame(player, current_tile, map);
        }

        if (timer_event)
        {
            timer_event = 0; // Reset the flag
            drop_stats(&player, current_tile);
            update_display(player);
        }

        if (button_event)
        {
            update_display(player);
            current_display = (current_display + 1) % 3; // always 0, 1, 2
            button_event = 0;                            // Reset the flag
        }

        update_status(player, map);
    }
}

// displays the entire map and info etc
void display_frame(struct player player, struct tile current_tile, struct tile map[10][10])
{
    print("\x1B[2J"); // Clear screen
    print("\x1B[H");  // Move cursor to top-left

    print("+------------------------------------------------+ \n");
    print("|            MARS SURVIVAL GAME                  | \n");
    print("+------------------------------------------------+ \n");

    // Use buffer to concatenete strings w variables
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
                append_str(line_buffer, "[🎅] ");
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
        int sv = get_sw();
        if (sv & SWITCH_EAST) // Yes
            return 1;
        if (sv & SWITCH_WEST) // No
            return 0;
    }
    // Unreachable, but needed for some compilers
    return 0;
}

// Wait for the player to exit by pressing NORTH
void wait_for_exit(const char *msg)
{
    if (!msg)
        msg = "Press BACK to exit.\n";
    print_hold(msg);
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

void interact_with_tile(struct player *player, struct tile *current_tile, struct tile map[10][10], int switch_values)
{
    enum TILETYPE currentTileType = current_tile->type;

    if (currentTileType == STORAGE)
    {
        print("Storage Menu:\n");
        print("EAST: View storage items\n");
        print("WEST: Transfer item from inventory to storage\n");
        print("SOUTH: Transfer item from storage to inventory\n");
        print("NORTH: Exit storage\n");
        print("Press a switch to choose an action.\n");

        while (1)
        {
            switch_values = get_sw();

            if (switch_values & SWITCH_BACK)
            {
                print_hold("Exiting storage.\n");
                return;
            }
            else if (switch_values & SWITCH_EAST)
            {
                // View storage items
                print_hold("Storage items:\n");
                check_storage(player, current_tile);
                break;
            }
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

                int chosen_slot = 0;
                int needs_reset = 1;
                while (1)
                {
                    int current_switch = get_sw();
                    if (!current_switch && needs_reset)
                    {
                        needs_reset = 0;
                    }
                    if (needs_reset)
                        continue;

                    if (current_switch & SWITCH_BACK)
                    {
                        print_hold("Cancelled.\n");
                        break;
                    }

                    int slot_switches = current_switch & (SWITCH_SLOT_1 | SWITCH_SLOT_2 | SWITCH_SLOT_3 | SWITCH_SLOT_4 | SWITCH_SLOT_5);
                    if (slot_switches == 0)
                        continue;

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

                    // choose thing to pluck based on switch value
                    if (chosen_slot > 0 && chosen_slot <= INVENTORY_SIZE)
                    {
                        enum COLLECTIBLETYPE item = player->inventory[chosen_slot - 1];
                        if (item != NONE)
                        {
                            add_to_storage(current_tile, item);
                            remove_from_inventory(player, chosen_slot - 1);
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
            else if (switch_values & SWITCH_SOUTH)
            {
                // Transfer item from storage to inventory
                print_hold("Storage items:\n");
                for (int i = 0; i < STORAGE_SIZE; i++)
                {
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
    else if (currentTileType == COCKPIT)
    {
        print("Do you want to end the game and return to Earth?\n");
        if (wait_for_yes_no())
        {
            print("You have chosen to end the game.!\n");
            int chance_to_survive = 0;

            switch (get_leds())
            {
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
                chance_to_survive = 98;
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

            if (random_value <= chance_to_survive)
            {
                print("You survived the journey back to Earth! Congratulations!\n");
            }
            else
            {
                print("Unfortunately, you did not survive the journey back to Earth.\n");
            }

            game_running = 0;
        }
        else
        {
            print("Continuing the game...\n");
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == CAFETERIA)
    {
        print("Grab some food?\n");
        if (wait_for_yes_no())
        {
            add_to_inventory(player, FOOD);
        }
        else
        {
            print_hold("You did not grab any food.\n");
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == ENGINE_BAY)
    {
        print("Add fuel to the engine bay?\n");
        if (wait_for_yes_no())
        {
            int has_fuel = 0;
            for (int i = 0; i < INVENTORY_SIZE; i++)
            {
                if (player->inventory[i] == FUEL)
                {
                    add_fuel_enginebay(current_tile, FUEL);
                    check_storage(player, current_tile);
                    remove_from_inventory(player, i);
                    print("You have now added fuel to the engine bay.\n");
                    has_fuel = 1;
                    break;
                }
            }
            if (!has_fuel)
            {
                print_hold("You don't have any fuel in your inventory.\n");
            }
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == LABORATORY)
    {
        print("You can interact with the laboratory.\n");
        print("EAST: Attempt to create WATER\n");
        print("WEST: Attempt to create FUEL\n");
        print("NORTH: Exit\n");

        while (1)
        {
            int sv = get_sw();
            if (sv & SWITCH_NORTH)
            {
                break;
            }
            else if (sv & SWITCH_EAST)
            {
                add_to_inventory(player, BOTTLE_OF_WATER);
                print_hold("You created some WATER.\n");
                break;
            }
            else if (sv & SWITCH_WEST)
            {
                add_to_inventory(player, FUEL);
                print_hold("You created some FUEL.\n");
                break;
            }
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == AIRLOCK)
    {
        print_hold("You are about to leave the ship, remember to keep an eye on the oxygen.\n");
        wait_for_exit(NULL);
    }
    else if (currentTileType == LANDING_SITE)
    {
        print_hold("You are right outside the ship now.\n");
        print("Do you want to look around the landing site?\n");
        if (wait_for_yes_no())
        {
            print("After exploring the landing site, you find a toolbox with vital tools for fixing the ship!\n");
            add_to_inventory(player, TOOLBOX);
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == WASTELAND)
    {
        print_hold("You are in the harsh Martian wasteland.\n");

        // Find medkit in wasteland pos. (4, 0)
        if (player->positionY == 0 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a medical kit!\n");
                add_to_inventory(player, MEDICAL_KIT);
            }
        }

        // Find map
        if (player->positionY == 7 && player->positionX == 1)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a MAP!\n");
                add_to_inventory(player, MAP);
            }
        }

        if (player->positionY == 2 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found spare parts!\n");
                add_to_inventory(player, SPARE_PARTS);
            }
        }

        if (player->positionY == 6 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a teddy bear!\n");
                add_to_inventory(player, TEDDY_BEAR);
            }
        }

        if (player->positionY == 5 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found a blanket!\n");
                add_to_inventory(player, BLANKET);
            }
        }

        if (player->positionY == 4 && player->positionX == 4)
        {
            print("Do you want to look around the wasteland?\n");
            if (wait_for_yes_no())
            {
                print("After looking around, you found cozy clothing!\n");
                add_to_inventory(player, CLOTHING);
            }
        }

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
        print_hold("Uh oh, it took a lot of energy to get out of the loose soil.\n");
        player->food /= 2;
        wait_for_exit(NULL);
    }
    else if (currentTileType == POND)
    {
        print("You see a large ice pond, maybe there is life? Discover?\n");
        if (wait_for_yes_no())
        {
            // Collect tardigrades
            print("You look around and find some tardigrades! You collect them.\n");
            add_to_inventory(player, TARDIGRADES);
            // If you need to remove something from storage or do any other action, do it here
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == SHARP_ROCKS)
    {
        print_hold("Oh no, the sharp rocks cracked one of your oxygen tanks, you only have half of the oxygen left!\n");
        player->oxygen /= 2;
        wait_for_exit(NULL);
    }
    else if (currentTileType == CAVE)
    {
        print("You enter a dark cave, discover more?\n");
        if (wait_for_yes_no())
        {
            print_hold("You found some alien bones and collected them.\n");
            add_to_inventory(player, ALIEN_BONES);
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == CRATER)
    {
        print("You are in a crater, want to discover some more?\n");
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
        if (wait_for_yes_no())
        {
            print("You find interesting RSL trails, you collect some images.\n");
            add_to_inventory(player, RSL_IMAGES);
        }
        wait_for_exit(NULL);
    }
    else if (currentTileType == MOUNTAIN)
    {
        print("You are on top of a big mountain, want to discover?\n");
        if (wait_for_yes_no())
        {
            print("You find some ice samples and collect them.\n");
            add_to_inventory(player, ICE);
        }
        wait_for_exit(NULL);
    }
    else
    {
        print_hold("There is nothing to interact with here.\n");
        wait_for_exit(NULL);
    }
}
