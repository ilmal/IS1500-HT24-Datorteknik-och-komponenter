#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

#include "game.h"
#include "tile.h"
#include "player.h"
#include "inventory.h"

extern void print(const char *);

// Function prototype
void interact_with_tile(struct player player, struct tile current_tile, struct tile map[10][10]);
void display_frame(struct player player, struct tile current_tile, struct tile map[10][10]);

// dropping stats
void drop_stats(struct player player, struct tile current_tile)
{
  if (current_tile.outside_rocket == 1)
  {
    player.oxygen -= 1;
  }
  else if (current_tile.outside_rocket == 0)
  {
    player.oxygen = 100;
  }
  player.water -= 1;
  player.food -= 1;

  // Check if any stat reaches 0
  if (player.oxygen <= 0)
  {
    printf("You have run out of oxygen! Game over. \n");
    exit(0);
  }
  if (player.water <= 0)
  {
    printf("You have run out of water! Game over. \n");
    exit(0);
  }
  if (player.food <= 0)
  {
    printf("You have run out of food! Game over. \n");
    exit(0);
  }
}

int main()
{
  print("IM IN DISPLAY FRAME \n");

  ct(POND);
  ct(STORAGE);
  ct(CHAMBERS);
  ct(COCKPIT);
  ct(CAFETERIA);
  print("3");
  ct(ENGINE_BAY);
  print("4");
  ct(LABORATORY);
  ct(AIRLOCK);
  print("1");
  ct(LANDING_SITE);
  ct(WASTELAND);
  ct(LOOSE_SOIL);
  ct(POND);
  ct(SHARP_ROCKS);
  ct(CAVE);
  ct(CRATER);
  ct(CANYON);
  ct(MOUNTAIN);

  print("POND CREATED");
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

  print("IM IN DISPLAY FRAME");

  struct player player = create_player();

  int running = 1;
  while (running)
  {
    // Get the current tile based on player's position
    struct tile current_tile = map[player.positionY][player.positionX];

    print("IM IN LOOPc");


    drop_stats(player, map[player.positionY][player.positionX]);

    // Display the frame
    display_frame(player, current_tile, map);

    // Handle user input
    printf("What would you like to do? (write a number)\n");
    printf("1. Move\n");
    printf("2. Interact\n");
    printf("3. Quit\n");
    printf("Enter your choice: ");

    int choice;
    scanf("%d", &choice);
    while (getchar() != '\n')
      ;

    if (choice == 1)
    {
      // Movement code...
      printf("Which direction would you like to move?\n");
      printf("1. North\n");
      printf("2. East\n");
      printf("3. South\n");
      printf("4. West\n");
      printf("Enter direction: ");
      int direction;
      scanf("%d", &direction);
      while (getchar() != '\n');

      int new_positionX = player.positionX;
      int new_positionY = player.positionY;

      // Move the player
      if (direction == 1)
      {
        new_positionY -= 1;
      }
      else if (direction == 2)
      {
        new_positionX += 1;
      }
      else if (direction == 3)
      {
        new_positionY += 1;
      }
      else if (direction == 4)
      {
        new_positionX -= 1;
      }
      
      // Check if the new position is within bounds
      if (new_positionX >= 0 && new_positionX < 10 && new_positionY >= 0 && new_positionY < 10 && map[new_positionY][new_positionX].type != EMPTY)
      {
        player.positionX = new_positionX;
        player.positionY = new_positionY;
      }
      else
      {
        printf("You cannot move in that direction.\n");
      }

    }
    else if (choice == 2)
    {
      // Interaction code...
      interact_with_tile(player, current_tile, map);
    }
    else if (choice == 3)
    {
      printf("Are you sure you want to quit the game? yes/no\n");
      running = 0; // temp for dev
      char confirmation[4];
      scanf("%3s", confirmation);
      if (strcmp(confirmation, "yes") == 0)
      {
        printf("Quitting game...\n");
        running = 0;
      }
      else
      {
        printf("Continuing the game...\n");
      }
    }
    else
    {
      printf("Invalid choice\n");
    }
  }
}

void display_frame(struct player player, struct tile current_tile, struct tile map[10][10])
{
  // Clear the screen using ANSI escape code
  printf("\033[2J");

  // Move the cursor to the top-left corner
  printf("\033[H");

  // Draw the frame
  printf("+------------------------------------------------+ \n");
  printf("|            MARS SURVIVAL GAME                  | \n");
  printf("+------------------------------------------------+ \n");
  printf("| Position: (%d, %d)                               \n", player.positionX, player.positionY);
  printf("| Location: %s                                     \n", TILETYPE_NAMES[current_tile.type]);
  printf("+------------------------------------------------+ \n");
  printf("| Map:                                             \n");
  for (int y = 0; y < 10; y++)
  {
    printf("| ");
    for (int x = 0; x < 10; x++)
    {
      if (player.positionX == x && player.positionY == y)
      {
        printf("[🫵 ] ");
      }
      else
      {
        printf("[%s] ", TILETYPE_MAP_NAMES[map[y][x].type]);
      }
    }
    printf("\n");
  }
  printf("| Inventory:                                     | \n");
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    printf("|   Slot %d: %s                                    \n", i + 1,
           player.inventory[i] != NONE ? COLLECTIBLE_NAMES[player.inventory[i]] : "Empty");
  }
  printf("+------------------------------------------------+\n");
  printf("| Items in current tile:                         |\n");
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (current_tile.storage[i] != NONE) // Only print non-empty slots
    {
      printf("|   Slot %d: %s                                    \n", i + 1,
             COLLECTIBLE_NAMES[current_tile.storage[i]]);
    }
  }
  printf("+------------------------------------------------+\n");
  printf("| %s\n", current_tile.interaction_text ? current_tile.interaction_text : "");
  printf("+------------------------------------------------+\n");
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

      printf("Storage Menu:\n");
      printf("1. View storage items\n");
      printf("2. Transfer item from inventory to storage\n");
      printf("3. Retrieve item from storage to inventory\n");
      printf("4. Exit storage\n");
      printf("Enter your choice: ");
      scanf("%d", &storage_choice);
      while (getchar() != '\n')
        ;

      if (storage_choice == 1)
      {
        // View storage items
        check_storage(player, storage_tile);
        printf("Press Enter to continue...");
        getchar();
      }
      else if (storage_choice == 2)
      {
        // Transfer item from inventory to storage
        printf("Your inventory:\n");
        for (int i = 0; i < INVENTORY_SIZE; i++)
        {
          printf("Slot %d: %s\n", i + 1,
                 player.inventory[i] != NONE ? COLLECTIBLE_NAMES[player.inventory[i]] : "Empty");
        }
        printf("Enter the inventory slot number to transfer to storage (or 0 to cancel): ");
        int inv_slot;
        scanf("%d", &inv_slot);
        while (getchar() != '\n')
          ;
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
            printf("Inventory slot %d is empty.\n", inv_slot);
          }
        }
        else
        {
          printf("Cancelled.\n");
        }
      }
      else if (storage_choice == 3)
      {
        // Transfer item from storage to inventory
        printf("Storage items:\n");
        for (int i = 0; i < STORAGE_SIZE; i++)
        {
          if (storage_tile.storage[i] != NONE)
          {
            printf("Slot %d: %s\n", i + 1, COLLECTIBLE_NAMES[storage_tile.storage[i]]);
          }
        }
        printf("Enter the storage slot number to transfer to inventory (or 0 to cancel): ");
        int storage_slot;
        scanf("%d", &storage_slot);
        while (getchar() != '\n')
          ;
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
            printf("Storage slot %d is empty.\n", storage_slot);
          }
        }
        else
        {
          printf("Cancelled.\n");
        }
      }
      else if (storage_choice == 4)
      {
        printf("Exiting storage.\n");
      }
      else
      {
        printf("Invalid choice.\n");
      }
    }
  }
  // Handle other tile interactions similarly
  else if (currentTileType == COCKPIT)
  {
    char end_game;
    printf("Do you want to end the game and return to Earth? (y/n)\n");
    scanf(" %c", &end_game);
    if (end_game == 'y' || end_game == 'Y')
    {
      printf("You have chosen to end the game. Goodbye!\n");
      // do a end screen or something
      exit(0);
    }
    else
    {
      printf("Continuing the game...\n");
    }
  }
  else if (currentTileType == CAFETERIA)
  {
    printf("Grab some food? (y/n)\n");
    char grab_food;
    scanf(" %c", &grab_food);
    if (grab_food == 'y' || grab_food == 'Y')
    {
      add_to_inventory(player, FOOD);
    }
    else
    {
      printf("You did not grab any food.\n");
    }
  }
  else if (currentTileType == ENGINE_BAY)
  {
    printf("Add fuel to the engine bay? (y/n)\n");
    char add_fuel;
    scanf("%c", &add_fuel);

    if (add_fuel == 'y' || add_fuel == 'Y')
    {
      for (int i = 0; i < 5; i++)
      {
        if (player.inventory[i] == FUEL)
        {
          add_to_storage(current_tile, FUEL);
          check_storage(player, current_tile);
          remove_from_inventory(player, i);

          printf("You have now added fuel to the engine bay.\n");
          // TODO: add code to light up one LED on board
        }
        else
        {
          printf("You don't have any fuel in your inventory.\n");
        }
      }
    }
    else if (currentTileType == LABORATORY)
    {
      printf("You can interact with laboratory\n");
      // add logic for creating water/fuel
    }
    else if (currentTileType == AIRLOCK)
    {
      printf("You are about to leave the ship, remember to keep an eye on the oxygen.\n");
    }
    else if (currentTileType == LANDING_SITE)
    {
      printf("You are right outside the ship now. \n");
    }
    else if (currentTileType == WASTELAND)
    {
      printf("You are in the harsh martian wasteland.\n");
    }
    else if (currentTileType == LOOSE_SOIL)
    {
      printf("Uh oh, it took a lot of energy to get out of the loose soil.\n");
    }
    else if (currentTileType == POND)
    {
      char discover_pond;
      printf("You see a large ice pond, maybe there is life? Do you want to discover? (y/n) \n");
      scanf("%c", &discover_pond);
      if (discover_pond == 'y' || discover_pond == 'Y')
      {
        printf("You look around and find some tardigrades! You collect them.\n");
        add_to_inventory(player, TARDIGRADES);
        remove_from_storage(current_tile, 1);
      }
    }
  }
  else if (currentTileType == SHARP_ROCKS)
  {
    printf("Oh no, the sharp rocks cracked up one of your oxygen tanks, you only have half of the oxygen left!\n");
  }
  else if (currentTileType == CAVE)
  {
    printf("You enter a dark cave, do you want to discover more? (y/n)\n");
    char discover_cave;
    scanf("%c", &discover_cave);
    if (discover_cave == 'y' || discover_cave == 'Y')
    {
      printf("You found some alien bones and collected them. \n");
      add_to_inventory(player, ALIEN_BONES);
      remove_from_storage(current_tile, 1);
    }
  }
  else if (currentTileType == CRATER)
  {
    char discover_crater;
    printf("You are in a crater, want to discover some more? (y/n) \n");
    scanf("%c", &discover_crater);
    if (discover_crater == 'y' || discover_crater == 'Y')
    {
      printf("You found found interesting sediments and collected some samples. \n");
      add_to_inventory(player, SEDIMENTARY_LAYERS);
      remove_from_storage(current_tile, 1);
    }
  }
  else if (currentTileType == CANYON)
  {
    char discover_canyon;
    printf("You are in a large canyon, want to see what lies here? (y/n) \n");
    scanf("%c", &discover_canyon);
    if (discover_canyon == 'y' || discover_canyon == 'Y')
    {
      printf("You find interesting RSL trails, you collect some images. \n");
      add_to_inventory(player, RSL_IMAGES);
      remove_from_storage(current_tile, 1);
    }
  }
  else if (currentTileType == MOUNTAIN)
  {
    char discover_mountain;
    printf("You are on top a big mountain, want to discover? (y/n) \n");
    scanf("%c", &discover_mountain);
    if (discover_mountain == 'y' || discover_mountain == 'Y')
    {
      printf("You find some ice samples and collect them. \n");
      add_to_inventory(player, ICE);
      remove_from_storage(current_tile, 1);
    }
  }
  else
  {
    printf("There is nothing to interact with here.\n");
  }
}


void handle_interrupt(void) {
    // Implement your interrupt handling logic here
    printf("Interrupt handled\n");
}
