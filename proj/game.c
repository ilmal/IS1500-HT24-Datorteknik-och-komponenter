#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INVENTORY_SIZE 5
#define STORAGE_SIZE 50

// Type of "room" the player can be in
enum TILETYPE
{
  // in the rocket
  STORAGE,    // 0
  CHAMBERS,   // 1
  COCKPIT,    // 2
  CAFETERIA,  // 3
  ENGINE_BAY, // 4
  LABORATORY, // 5
  AIRLOCK,    // 6

  // on the planet
  LANDING_SITE, // 7
  WASTELAND,    // 8
  LOOSE_SOIL,   // 9
  POND,         // 10
  SHARP_ROCKS,  // 11
  CAVE,         // 12
  CRATER,       // 13
  CANYON,       // 14
  MOUNTAIN,     // 15,
  DEFAULT
};

const char *TILETYPE_NAMES[] = {
    "STORAGE",
    "CHAMBERS",
    "COCKPIT",
    "CAFETERIA",
    "ENGINE_BAY",
    "LABORATORY",
    "AIRLOCK",
    "LANDING_SITE",
    "WASTELAND",
    "LOOSE_SOIL",
    "POND",
    "SHARP_ROCKS",
    "CAVE",
    "CRATER",
    "CANYON",
    "MOUNTAIN",
    "DEFAULT"};

// Adjusted enum to start from 1 and use 0 as NONE
enum COLLECTIBLETYPE
{
  NONE = 0,           // represents empty slot
  TARDIGRADES,        // 1
  SEDIMENTARY_LAYERS, // 2
  RSL_IMAGES,         // 3
  ALIEN_BONES,        // 4
  OLD_ROVER_PARTS,    // 5
  ICE,                // 6
  FOOD,               // 7
  BOTTLE_OF_WATER     // 8
};

const char *COLLECTIBLE_NAMES[] = {
    "Empty Slot",         // 0
    "Tardigrades",        // 1
    "Sedimentary Layers", // 2
    "RSL Images",         // 3
    "Alien Bones",        // 4
    "Old Rover Parts",    // 5
    "Ice",                // 6
    "Food",               // 7
    "Bottle of Water"     // 8
};

struct tile
{
  enum TILETYPE type;
  // interactions
  int interactable;
  char *interaction_text;

  // storage
  enum COLLECTIBLETYPE storage[STORAGE_SIZE]; // storage can store more than enough

  // collectibles
  int collectibles;

  // stat changes
  int drop_oxygen;
  int drop_food;
  int drop_water;
};

struct player
{
  int positionX;
  int positionY;
  int food;
  int oxygen;
  enum COLLECTIBLETYPE inventory[INVENTORY_SIZE]; // Adjusted to use enum type
  int water;
};

struct player *create_player()
{
  struct player *new_player = malloc(sizeof(struct player));

  // start position
  new_player->positionX = 0;
  new_player->positionY = 0;

  // start health stats
  new_player->food = 100;
  new_player->oxygen = 100;
  new_player->water = 100;

  // start inventory
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    new_player->inventory[i] = NONE;
  }
  return new_player;
}

// storage struct
struct tile *storage()
{
  struct tile *storage_tile = malloc(sizeof(struct tile));
  storage_tile->type = STORAGE; // set type
  storage_tile->interactable = 1;
  storage_tile->interaction_text = "You are in the storage room which holds your food and scientific samples - you can store your inventory here.";
  memset(storage_tile->storage, 0, sizeof(storage_tile->storage));

  // default items in the storage:
  storage_tile->storage[0] = FOOD;
  storage_tile->storage[1] = BOTTLE_OF_WATER;

  return storage_tile;
}

// chambers struct
struct tile *chambers()
{
  struct tile *chambers_tile = malloc(sizeof(struct tile));
  chambers_tile->type = CHAMBERS; // set type
  chambers_tile->interaction_text = "You are in the personal chambers, this is the starting point.";
  return chambers_tile;
}

// cockpit struct
struct tile *cockpit()
{
  struct tile *cockpit_tile = malloc(sizeof(struct tile));
  cockpit_tile->type = COCKPIT; // set type
  cockpit_tile->interactable = 1;
  cockpit_tile->interaction_text = "You have entered the cockpit, you can now choose to end the game if you want.";

  return cockpit_tile;
}

// cafeteria struct
struct tile *cafeteria()
{
  struct tile *cafeteria_tile = malloc(sizeof(struct tile));
  cafeteria_tile->type = CAFETERIA;
  cafeteria_tile->interaction_text = "You have entered the cafeteria, collect food here.";
  cafeteria_tile->collectibles = 1;

  return cafeteria_tile;
}
// engine bay struct
struct tile *engine_bay()
{
  struct tile *engine_bay_tile = malloc(sizeof(struct tile));
  engine_bay_tile->type = ENGINE_BAY;
  // TODO: power up map minigame stuff

  return engine_bay_tile;
}

// laboratory struct
struct tile *laboratory()
{
  struct tile *laboratory_tile = malloc(sizeof(struct tile));
  laboratory_tile->type = LABORATORY;
  // TODO: create fuel/water logic

  return laboratory_tile;
}

// airlock struct
struct tile *airlock()
{
  struct tile *airlock_tile = malloc(sizeof(struct tile));
  airlock_tile->type = AIRLOCK;
  // TODO: refill oxygen logic

  return airlock_tile;
}

// landing site struct
struct tile *landing_site()
{
  struct tile *landing_site_tile = malloc(sizeof(struct tile));
  landing_site_tile->type = LANDING_SITE;
  return landing_site_tile;
}

// wasteland struct
struct tile *wasteland()
{
  struct tile *wasteland_tile = malloc(sizeof(struct tile));
  wasteland_tile->type = WASTELAND;
  return wasteland_tile;
}

// Constructor function to create a new tile (ct = create_tile)
struct tile *ct(enum TILETYPE type)
{
  switch (type)
  {
  case 0:
    return storage();
  case 1:
    return chambers();
  case 2:
    return cockpit();
  case 3:
    return cafeteria();
  case 4:
    return engine_bay();
  case 5:
    return laboratory();
  case 6:
    return airlock();
  case 7:
    return landing_site();
  case 8:
    return wasteland();
  default:
    return wasteland();
  }
}

void add_to_inventory(struct player *player, enum COLLECTIBLETYPE item)
{
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    if (player->inventory[i] == NONE)
    {                              // Check if the slot is empty
      player->inventory[i] = item; // Add the item to the inventory
      printf("You have added: %s to your inventory.\n", COLLECTIBLE_NAMES[item]);
      return;
    }
  }
  // If there is no empty slot
  printf("Your inventory is full! Cannot add more items.\n");
}

void remove_from_inventory(struct player *player, int index)
{
  if (index >= 0 && index < INVENTORY_SIZE)
  {
    if (player->inventory[index] != NONE)
    {
      printf("Removed %s from inventory slot %d\n", COLLECTIBLE_NAMES[player->inventory[index]], index + 1);
      player->inventory[index] = NONE;
    }
    else
    {
      printf("Inventory slot %d is already empty.\n", index + 1);
    }
  }
  else
  {
    printf("Invalid inventory slot.\n");
  }
}

void add_to_storage(struct tile *storage_tile, enum COLLECTIBLETYPE item)
{
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (storage_tile->storage[i] == NONE)
    {
      storage_tile->storage[i] = item;
      printf("Added %s to storage.\n", COLLECTIBLE_NAMES[item]);
      return;
    }
  }
  printf("Storage is full! Cannot add more items.\n");
}

void remove_from_storage(struct tile *storage_tile, int index)
{
  if (index >= 0 && index < STORAGE_SIZE)
  {
    if (storage_tile->storage[index] != NONE)
    {
      printf("Removed %s from storage slot %d\n", COLLECTIBLE_NAMES[storage_tile->storage[index]], index + 1);
      storage_tile->storage[index] = NONE;
    }
    else
    {
      printf("Storage slot %d is already empty.\n", index + 1);
    }
  }
  else
  {
    printf("Invalid storage slot.\n");
  }
}

// helper func to see storage
void check_storage(struct player *player, struct tile *storage_tile)
{
  printf("%s\n", storage_tile->interaction_text);
  printf("You have stored the following items: \n");
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (storage_tile->storage[i] != NONE)
    {
      printf("Slot %d: %s\n", i + 1, COLLECTIBLE_NAMES[storage_tile->storage[i]]);
    }
  }
}

int main()
{
  struct tile *map[3][3] = {
      {ct(STORAGE), ct(CHAMBERS), ct(COCKPIT)},
      {ct(CAFETERIA), ct(ENGINE_BAY), ct(LABORATORY)},
      {ct(AIRLOCK), ct(LANDING_SITE), ct(WASTELAND)}};
  // debugging
  for (int y = 0; y < 3; y++)
  {
    for (int x = 0; x < 3; x++)
    {
      if (map[y][x] == NULL)
      {
        printf("Error: map[%d][%d] is not initialized\n", y, x);
      }
      else
      {
        printf("map[%d][%d] is initialized: %s\n", y, x, TILETYPE_NAMES[map[y][x]->type]);
      }
    }
  }

  struct player *player = create_player();

  printf("Welcome to the game!\n");
  printf("You are stranded on Mars and need to find a way to survive.\n");
  // TODO: add game description text

  while (1)
  {
    printf("You are at position (%d, %d)\n", player->positionX, player->positionY);
    printf("The tile is: %s\n", TILETYPE_NAMES[map[player->positionY][player->positionX]->type]);
    printf("What would you like to do? (write a number)\n");
    printf("1. Move\n");
    printf("2. Interact\n");
    printf("3. Check inventory\n");
    printf("4. Check vitals\n");
    printf("5. Quit\n");

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

    // Clear input buffer
    while (getchar() != '\n')
      ;

    if (choice == 1)
    {

      printf("Which direction would you like to move?\n");
      printf("1. North\n");
      printf("2. East\n");
      printf("3. South\n");
      printf("4. West\n");

      int direction;
      printf("Enter direction: ");
      scanf("%d", &direction);

      // Clear input buffer
      while (getchar() != '\n')
        ;

      // Move the player
      if (direction == 1 && player->positionY > 0)
      {
        player->positionY -= 1;
      }
      else if (direction == 2 && player->positionX < 2)
      {
        player->positionX += 1;
      }
      else if (direction == 3 && player->positionY < 2)
      {
        player->positionY += 1;
      }
      else if (direction == 4 && player->positionX > 0)
      {
        player->positionX -= 1;
      }
      else
      {
        printf("Invalid direction or out of bounds\n");
        continue; // Skip the movement if invalid
      }

      printf("New position: (%d, %d)\n", player->positionX, player->positionY);
      printf("Tile type at new position: %s\n", TILETYPE_NAMES[map[player->positionY][player->positionX]->type]);
    }
    else if (choice == 2)
    {
      printf("Checking interaction... \n");
      enum TILETYPE currentTileType = map[player->positionY][player->positionX]->type;

      if (currentTileType == STORAGE)
      {
        struct tile *storage_tile = map[player->positionY][player->positionX];
        int storage_choice = 0;
        while (storage_choice != 4)
        {
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
          }
          else if (storage_choice == 2)
          {
            // Transfer item from inventory to storage
            printf("Your inventory:\n");
            for (int i = 0; i < INVENTORY_SIZE; i++)
            {
              if (player->inventory[i] != NONE)
              {
                printf("Slot %d: %s\n", i + 1, COLLECTIBLE_NAMES[player->inventory[i]]);
              }
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
      // cockpit
      else if (currentTileType == COCKPIT)
      {
        char end_game;
        printf("Do you want to end the game and return to Earth? (y/n)\n");
        scanf(" %c", &end_game); // Added space before %c to consume any leftover newline
        if (end_game == 'y' || end_game == 'Y')
        {
          printf("You have chosen to end the game. Goodbye!\n");
          // TODO: win/lose logic
          break;
        }
        else
        {
          printf("Continuing the game...\n");
        }
      }
      // cafeteria
      else if (currentTileType == CAFETERIA)
      {
        printf("Grab some food? (y/n)\n");
        char grab_food;
        scanf(" %c", &grab_food); // Added space before %c
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
    else if (choice == 3)
    {
      printf("+------------------ INVENTORY ------------------+\n");
      for (int i = 0; i < INVENTORY_SIZE; i++)
      {
        if (player->inventory[i] != NONE)
        {
          printf("Slot %d: %s\n", i + 1, COLLECTIBLE_NAMES[player->inventory[i]]);
        }
      }
      printf("+-----------------------------------------------+\n");
    }
    else if (choice == 4)
    {
      printf("Checking vitals...\n");
      printf("Food: %d\n", player->food);
      printf("Oxygen: %d\n", player->oxygen);
      printf("Water: %d\n", player->water);
    }
    else if (choice == 5)
    {
      printf("Quitting game...\n");
      break;
    }
    else
    {
      printf("Invalid choice\n");
    }
  }
  return 0;
}
