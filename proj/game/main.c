#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // For usleep
#include <string.h>

#include "game.h"
#include "tile.h"
#include "player.h"
#include "inventory.h"

// Function prototype
void display_frame(struct player *player, struct tile *current_tile, struct tile *map[10][10]);

// dropping stats
void drop_stats(struct player *player, struct tile *current_tile)
{
  if (current_tile->outside_rocket == 1)
  {
    player->oxygen -= 1;
  }
  else if (current_tile->outside_rocket == 0)
  {
    player->oxygen = 100;
  }
  player->water -= 1;
  player->food -= 1;

  // Check if any stat reaches 0
  if (player->oxygen <= 0)
  {
    printf("You have run out of oxygen! Game over. \n");
    exit(0);
  }
  if (player->water <= 0)
  {
    printf("You have run out of water! Game over. \n");
    exit(0);
  }
  if (player->food <= 0)
  {
    printf("You have run out of food! Game over. \n");
    exit(0);
  }
}

int main()
{
  // clang-format off
  struct tile *map[10][10] = {
      {ct(WASTELAND), ct(WASTELAND),  ct(WASTELAND),  ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(WASTELAND), ct(WASTELAND),  ct(WASTELAND),  ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(EMPTY),     ct(EMPTY),      ct(EMPTY),      ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(CHAMBERS),  ct(COCKPIT),    ct(EMPTY),      ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(CAFETERIA), ct(STORAGE),    ct(AIRLOCK),    ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(ENGINE_BAY),ct(LABORATORY), ct(EMPTY),      ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(EMPTY),     ct(EMPTY),      ct(EMPTY),      ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(WASTELAND), ct(WASTELAND),  ct(WASTELAND),  ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(WASTELAND), ct(WASTELAND),  ct(WASTELAND),  ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
      {ct(WASTELAND), ct(WASTELAND),  ct(WASTELAND),  ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND), ct(WASTELAND)},
  };

  struct player *player = create_player();

  int running = 1;
  while (running)
  {
    // Get the current tile based on player's position
    struct tile *current_tile = map[player->positionY][player->positionX];

    drop_stats(player, map[player->positionY][player->positionX]);

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

      int new_positionX = player->positionX;
      int new_positionY = player->positionY;

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
      if (new_positionX >= 0 && new_positionX < 10 && new_positionY >= 0 && new_positionY < 10 && map[new_positionY][new_positionX]->type != EMPTY)
      {
        player->positionX = new_positionX;
        player->positionY = new_positionY;
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

void display_frame(struct player *player, struct tile *current_tile, struct tile *map[10][10])
{
  // Clear the screen using ANSI escape code
  printf("\033[2J");

  // Move the cursor to the top-left corner
  printf("\033[H");

  // Draw the frame
  printf("+------------------------------------------------+ \n");
  printf("|            MARS SURVIVAL GAME                  | \n");
  printf("+------------------------------------------------+ \n");
  printf("| Position: (%d, %d)                               \n", player->positionX, player->positionY);
  printf("| Location: %s                                     \n", TILETYPE_NAMES[current_tile->type]);
  printf("+------------------------------------------------+ \n");
  printf("| Map:                                             \n");
  for (int y = 0; y < 10; y++)
  {
    printf("| ");
    for (int x = 0; x < 10; x++)
    {
      if (player->positionX == x && player->positionY == y)
      {
        printf("[PL] ");
      }
      else
      {
        printf("[%s] ", TILETYPE_MAP_NAMES[map[y][x]->type]);
      }
    }
    printf("\n");
  }
  printf("| Inventory:                                     | \n");
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    printf("|   Slot %d: %s                                    \n", i + 1,
           player->inventory[i] != NONE ? COLLECTIBLE_NAMES[player->inventory[i]] : "Empty");
  }
  printf("+------------------------------------------------+\n");
  printf("| %s\n", current_tile->interaction_text ? current_tile->interaction_text : "");
  printf("+------------------------------------------------+\n");
}

void interact_with_tile(struct player *player, struct tile *current_tile, struct tile *map[10][10])
{
  enum TILETYPE currentTileType = current_tile->type;

  if (currentTileType == STORAGE)
  {
    struct tile *storage_tile = current_tile;
    int storage_choice = 0;
    while (storage_choice != 4)
    {
      // Clear screen and display frame
      display_frame(player, current_tile, map);

      printf("Storage Menu:\n");
      printf("1. View storage items\n");
      printf("2. Transfer item from inventory to storage\n");
      printf("3. Transfer item from storage to inventory\n");
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
                 player->inventory[i] != NONE ? COLLECTIBLE_NAMES[player->inventory[i]] : "Empty");
        }
        printf("Enter the inventory slot number to transfer to storage (or 0 to cancel): ");
        int inv_slot;
        scanf("%d", &inv_slot);
        while (getchar() != '\n')
          ;
        if (inv_slot > 0 && inv_slot <= INVENTORY_SIZE)
        {
          enum COLLECTIBLETYPE item = player->inventory[inv_slot - 1];
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
          if (storage_tile->storage[i] != NONE)
          {
            printf("Slot %d: %s\n", i + 1, COLLECTIBLE_NAMES[storage_tile->storage[i]]);
          }
        }
        printf("Enter the storage slot number to transfer to inventory (or 0 to cancel): ");
        int storage_slot;
        scanf("%d", &storage_slot);
        while (getchar() != '\n')
          ;
        if (storage_slot > 0 && storage_slot <= STORAGE_SIZE)
        {
          enum COLLECTIBLETYPE item = storage_tile->storage[storage_slot - 1];
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
  else
  {
    printf("There is nothing to interact with here.\n");
  }
}
