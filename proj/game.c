#include <stdio.h>
#include <stdlib.h>
#include <string.h>

<<<<<<< HEAD
#define INVENTORY_SIZE 5
#define STORAGE_SIZE 50
=======
// for timer
#include <unistd.h>
#include <time.h>
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

// Type of "room" the player can be in
enum TILETYPE
{
<<<<<<< HEAD
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
=======
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
    MOUNTAIN,     // 15
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
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
<<<<<<< HEAD
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
=======
    "MOUNTAIN"};
enum COLLECTIBLETYPE
{
    TARDIGRADES,        // found in pond
    SEDIMENTARY_LAYERS, // found in crater
    RSL_IMAGES,         // found in canyon
    ALIEN_BONES,        // found in cave
    OLD_ROVER_PARTS,    // found in wasteland (low chance)
    ICE,                // found in somewhere
    FOOD,               // found in cafeteria
    BOTTLE_OF_WATER,    // found in cafeteria
};

const char *COLLECTIBLE_NAMES[] = {
    "Tardigrades",
    "Sedimentary Layers",
    "RSL Images",
    "Alien Bones",
    "Old Rover Parts",
    "Ice",
    "Food",
    "Bottle of Water"};

struct tile
{
    enum TILETYPE type;
    // interactions
    int interactable;
    char *interaction_text;

    // storage
    enum COLLECTIBLETYPE storage[50]; // storage can store more than enough

    // collectibles
    // int collectibles;
    enum COLLECTIBLETYPE collectible;

    // drop oxygen only if outside
    int outside_rocket;
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
};

struct player
{
<<<<<<< HEAD
  int positionX;
  int positionY;
  int food;
  int oxygen;
  enum COLLECTIBLETYPE inventory[INVENTORY_SIZE]; // Adjusted to use enum type
  int water;
=======
    int positionX;
    int positionY;
    int food;
    int oxygen;
    int inventory[5]; // player can only collect upto 5 items at hand
    int water;
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
};

struct player *create_player()
{
<<<<<<< HEAD
  struct player *new_player = malloc(sizeof(struct player));
=======
    struct player *new_player = malloc(sizeof(struct player));
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

  // start position
  new_player->positionX = 0;
  new_player->positionY = 0;

  // start health stats
  new_player->food = 100;
  new_player->oxygen = 100;
  new_player->water = 100;

<<<<<<< HEAD
  // start inventory
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    new_player->inventory[i] = NONE;
  }
  return new_player;
=======
    // start inventory
    for (int i = 0; i < 5; i++)
    {
        new_player->inventory[i] = 0;
    }
    return new_player;
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
}

// storage struct
struct tile *storage()
{
<<<<<<< HEAD
  struct tile *storage_tile = malloc(sizeof(struct tile));
  storage_tile->type = STORAGE; // set type
  storage_tile->interactable = 1;
  storage_tile->interaction_text = "You are in the storage room which holds your food and scientific samples - you can store your inventory here.";
  memset(storage_tile->storage, 0, sizeof(storage_tile->storage));
=======
    struct tile *storage_tile = malloc(sizeof(struct tile));
    storage_tile->type = STORAGE; // set type
    storage_tile->interactable = 1;
    storage_tile->interaction_text = "You are in the storage room which holds your food and scientific samples - you can store your inventory here.";
    storage_tile->outside_rocket = 0;
    memset(storage_tile->storage, 0, sizeof(storage_tile->storage));
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

  // default items in the storage:
  storage_tile->storage[0] = FOOD;
  storage_tile->storage[1] = BOTTLE_OF_WATER;

  return storage_tile;
}

// chambers struct
struct tile *chambers()
{
<<<<<<< HEAD
  struct tile *chambers_tile = malloc(sizeof(struct tile));
  chambers_tile->type = CHAMBERS; // set type
  chambers_tile->interaction_text = "You are in the personal chambers, this is the starting point.";
  return chambers_tile;
=======
    struct tile *chambers_tile = malloc(sizeof(struct tile));
    chambers_tile->type = CHAMBERS; // set type
    chambers_tile->outside_rocket = 0;
    chambers_tile->interaction_text = "You are in the personal chambers, this is the starting point.";
    return chambers_tile;
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
}

// cockpit struct
struct tile *cockpit()
{
<<<<<<< HEAD
  struct tile *cockpit_tile = malloc(sizeof(struct tile));
  cockpit_tile->type = COCKPIT; // set type
  cockpit_tile->interactable = 1;
  cockpit_tile->interaction_text = "You have entered the cockpit, you can now choose to end the game if you want.";

  return cockpit_tile;
=======
    struct tile *cockpit_tile = malloc(sizeof(struct tile));
    cockpit_tile->type = COCKPIT; // set type
    cockpit_tile->interactable = 1;
    cockpit_tile->outside_rocket = 0;
    cockpit_tile->interaction_text = "You have entered the cockpit, you can now choose to end the game if you want.";

    return cockpit_tile;
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
}

// cafeteria struct
struct tile *cafeteria()
{
<<<<<<< HEAD
  struct tile *cafeteria_tile = malloc(sizeof(struct tile));
  cafeteria_tile->type = CAFETERIA;
  cafeteria_tile->interaction_text = "You have entered the cafeteria, collect food here.";
  cafeteria_tile->collectibles = 1;
=======
    struct tile *cafeteria_tile = malloc(sizeof(struct tile));
    cafeteria_tile->type = CAFETERIA;
    cafeteria_tile->outside_rocket = 0;
    cafeteria_tile->interaction_text = "You have entered the cafeteria, collect food here.";
    cafeteria_tile->collectible = FOOD; // what to be collected here
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

  return cafeteria_tile;
}
// engine bay struct
struct tile *engine_bay()
{
<<<<<<< HEAD
  struct tile *engine_bay_tile = malloc(sizeof(struct tile));
  engine_bay_tile->type = ENGINE_BAY;
  // TODO: power up map minigame stuff
=======
    struct tile *engine_bay_tile = malloc(sizeof(struct tile));
    engine_bay_tile->type = ENGINE_BAY;
    engine_bay_tile->outside_rocket = 0;
    // TODO: power up map minigame stuff
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

  return engine_bay_tile;
}

// laboratory struct
struct tile *laboratory()
{
<<<<<<< HEAD
  struct tile *laboratory_tile = malloc(sizeof(struct tile));
  laboratory_tile->type = LABORATORY;
  // TODO: create fuel/water logic
=======
    struct tile *laboratory_tile = malloc(sizeof(struct tile));
    laboratory_tile->type = LABORATORY;
    laboratory_tile->outside_rocket = 0;
    // TODO: create fuel/water logic
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

  return laboratory_tile;
}

// airlock struct
struct tile *airlock()
{
<<<<<<< HEAD
  struct tile *airlock_tile = malloc(sizeof(struct tile));
  airlock_tile->type = AIRLOCK;
  // TODO: refill oxygen logic
=======
    struct tile *airlock_tile = malloc(sizeof(struct tile));
    airlock_tile->type = AIRLOCK;
    airlock_tile->outside_rocket = 0;
    // TODO: refill oxygen logic
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

  return airlock_tile;
}

// landing site struct
struct tile *landing_site()
{
<<<<<<< HEAD
  struct tile *landing_site_tile = malloc(sizeof(struct tile));
  landing_site_tile->type = LANDING_SITE;
  return landing_site_tile;
=======
    struct tile *landing_site_tile = malloc(sizeof(struct tile));
    landing_site_tile->type = LANDING_SITE;
    landing_site_tile->outside_rocket = 1;
    return landing_site_tile;
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
}

// wasteland struct
struct tile *wasteland()
{
<<<<<<< HEAD
  struct tile *wasteland_tile = malloc(sizeof(struct tile));
  wasteland_tile->type = WASTELAND;
  return wasteland_tile;
=======
    struct tile *wasteland_tile = malloc(sizeof(struct tile));
    wasteland_tile->type = WASTELAND;
    wasteland_tile->outside_rocket = 1;
    return wasteland_tile;
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
}

// loose soil struct
struct tile *loose_soil()
{
    struct tile *loose_soil_tile = malloc(sizeof(struct tile));
    loose_soil_tile->type = LOOSE_SOIL;
    loose_soil_tile->outside_rocket = 1;

    // TODO: logic to decrease food *0.5

    return loose_soil_tile;
}

// pond struct
struct tile *pond()
{
    struct tile *pond_tile = malloc(sizeof(struct tile));
    pond_tile->type = POND;
    pond_tile->outside_rocket = 1;
    pond_tile->collectible = TARDIGRADES;

    return pond_tile;
}

struct tile *sharp_rocks()
{
    struct tile *sharp_rocks_tile = malloc(sizeof(struct tile));
    sharp_rocks_tile->type = SHARP_ROCKS;
    sharp_rocks_tile->outside_rocket = 1;
    // TODO: logic to decrease oxygen with *0.5

    return sharp_rocks_tile;
}

// cave struct
struct tile *cave()
{
    struct tile *cave_tile = malloc(sizeof(struct tile));
    cave_tile->type = CAVE;
    cave_tile->outside_rocket = 1;

    return cave_tile;
}

// crater struct
struct tile *crater()
{
    struct tile *crater_tile = malloc(sizeof(struct tile));
    crater_tile->type = CRATER;
    crater_tile->outside_rocket = 1;
    crater_tile->collectible = SEDIMENTARY_LAYERS;

    return crater_tile;
}
// canyon struct
struct tile *canyon()
{
    struct tile *canyon_tile = malloc(sizeof(struct tile));
    canyon_tile->type = CANYON;
    canyon_tile->outside_rocket = 1;
    canyon_tile->collectible = RSL_IMAGES;

    return canyon_tile;
}

// mountain struct
struct tile *mountain()
{
    struct tile *mountain_tile = malloc(sizeof(struct tile));
    mountain_tile->type = MOUNTAIN;
    mountain_tile->outside_rocket = 1;

    return mountain_tile;
}

// Constructor function to create a new tile (ct = create_tile)
struct tile *ct(enum TILETYPE type)
{
<<<<<<< HEAD
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
=======
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
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
}

void add_to_inventory(struct player *player, enum COLLECTIBLETYPE item)
{
<<<<<<< HEAD
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    if (player->inventory[i] == NONE)
    {                              // Check if the slot is empty
      player->inventory[i] = item; // Add the item to the inventory
      printf("You have added: %s to your inventory.\n", COLLECTIBLE_NAMES[item]);
      return;
=======
    for (int i = 0; i < 5; i++)
    {
        if (player->inventory[i] == 0)
        {                                // Check if the slot is empty
            player->inventory[i] = item; // Add the item to the inventory
            printf("You have added: %s to your inventory.\n", COLLECTIBLE_NAMES[item]);
            return;
        }
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
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

<<<<<<< HEAD
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
=======
void add_to_storage(struct tile *storage_tile, enum COLLECTIBLETYPE item)
{
    for (int i = 0; i < 50; i++)
    {
        if (storage_tile->storage[i] == 0)
        {
            storage_tile->storage[i] = item;
            printf("You have added %s to the storage.\n", COLLECTIBLE_NAMES[item]);
            return;
        }
        printf("The storage room is full! Cannot add more items.\n");
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
    }
  }
}

<<<<<<< HEAD
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
=======
// helper func to see storage
void check_storage(struct player *player, struct tile *storage_tile)
{
    printf("%s\n", storage_tile->interaction_text);
    printf("You have stored the following items: \n");
    for (int i = 0; i < 4; i++)
    {
        if (storage_tile->storage[i] != 0)
        {
            printf("%s\n", COLLECTIBLE_NAMES[storage_tile->storage[i]]);
        }
    }
    char choice_storage;
    printf("\n Do you want to store your current inventory? (y/n)");
    scanf("%c\n", &choice_storage);

    // TODO: store stuff logic
    // if (choice_storage == 'y' || choice_storage == 'Y')
    // {

    // }
}

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
    struct tile *map[3][3] = {
        {ct(STORAGE), ct(CHAMBERS), ct(COCKPIT)},
        {ct(CAFETERIA), ct(ENGINE_BAY), ct(LABORATORY)},
        {ct(AIRLOCK), ct(LANDING_SITE), ct(WASTELAND)}};

    struct player *player = create_player();
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

  printf("Welcome to the game!\n");
  printf("You are stranded on Mars and need to find a way to survive.\n");
  // TODO: add game description text

<<<<<<< HEAD
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
=======
    // dropping stats every 5 secs
    int interval = 5;
    time_t last_time = time(NULL);

    while (1)
    {
        // dropping stats logic
        time_t current_time = time(NULL);
        if (difftime(current_time, last_time) >= interval)
        {
            drop_stats(player, map[player->positionY][player->positionX]);
            last_time = current_time;
        }

        printf("You are at position (%d, %d)\n", player->positionX, player->positionY);
        printf("%d\n", map[player->positionY][player->positionX]->type);
        printf("The tile is: %s\n", TILETYPE_NAMES[map[player->positionY][player->positionX]->type]);
        printf("What would you like to do? (write a number)\n");
        printf("1. Move\n");
        printf("2. Interact\n");
        printf("3. Check inventory\n");
        printf("4. Check vitals\n");
        printf("5. Quit\n");
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);

<<<<<<< HEAD
    // Clear input buffer
    while (getchar() != '\n')
      ;
=======
        if (choice == 1)
        {

            printf("Which direction would you like to move?\n");
            printf("1. North\n");
            printf("2. East\n");
            printf("3. South\n");
            printf("4. West\n");
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f

    if (choice == 1)
    {

      printf("Which direction would you like to move?\n");
      printf("1. North\n");
      printf("2. East\n");
      printf("3. South\n");
      printf("4. West\n");

<<<<<<< HEAD
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
=======
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
            // storage
            if (map[player->positionY][player->positionX]->type == STORAGE)
            {
                check_storage(player, map[player->positionY][player->positionX]);
                // TODO: storage logic
            }
            // cockpit
            else if (map[player->positionY][player->positionX]->type == COCKPIT)
            {
                char end_game;
                printf("Do you want to end the game and return to Earth? (y/n)\n");
                scanf("%c\n", &end_game);
                if (end_game == 'y' || end_game == 'Y')
                {
                    printf("You have chosen to end the game. Goodbye!\n");
                    // TODO: win/lose logic
                    break;
                }
                else
                {
                    printf("Lol, you coward. Keep playing then...\n"); // change text maybe? naaah
                }
            }
            // cafeteria
            else if (map[player->positionY][player->positionX]->type == CAFETERIA)
            {
                printf("Grab some food? (y/n)\n");
                char grab_food;
                scanf("%c\n", &grab_food);
                if (grab_food == 'y' || grab_food == 'Y')
                {
                    printf("You have grabbed one lunchbox.\n");
                }
            }
        }
        else if (choice == 3)
        {
            printf("+------------------ INVENTORY ------------------+\n");
            for (int i = 0; i < 4; i++)
            {
                if (player->inventory[i] != 0)
                {
                    printf("%s\n", COLLECTIBLE_NAMES[player->inventory[i]]);
                }
            }
            printf("+------------------ --------- ------------------+\n");
        }
        else if (choice == 4)
        {
            printf("Food left: %d\n", player->food);
            printf("Water left: %d\n", player->water);
            printf("Oxygen left: %d\n", player->oxygen);
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
>>>>>>> f2e1423e890a3c12e74409f21c6fe5eb8ede9b9f
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
