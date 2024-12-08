#include "inventory.h"

// Helper funcs imported from codebase
extern void print(const char *);
extern void append_str(char *dest, const char *src);
extern void int_to_str(int num, char *str);
extern void print_hold(char *s);

// Function that adds an item to players personal inventory
void add_to_inventory(struct player *player, enum COLLECTIBLETYPE item)
{
  char buffer[256]; // Buffer to concatenate strings with variables
  buffer[0] = '\0'; // Initialize buffer to an empty string

  // Loop through players inventory 
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    // Add item if the slot in inventory is empty(= NONE)
    if (player->inventory[i] == NONE)
    {
      player->inventory[i] = item; // Add the item to the inventory

      // Create string by appending to buffer
      append_str(buffer, "You have added: ");
      append_str(buffer, COLLECTIBLE_NAMES[item]);
      append_str(buffer, " to your inventory.\n");
      print(buffer);

      return;
    }
  }
  // If no empty slot found
  print("Your inventory is full! Cannot add more items.\n");
}

// Function that removes an item from players personal inventory
void remove_from_inventory(struct player *player, int index)
{
  // Buffer to concatenate strings
  char buffer[256];
  char num_str[12]; // used to store string form of slot number
  buffer[0] = '\0'; // Initialize buffer

  // Check for valid index (0-5)
  if (index >= 0 && index < INVENTORY_SIZE)
  {
    // If there is an item on the slot of chosen index
    if (player->inventory[index] != NONE)
    {
      // Create concatenated string w buffer and print
      append_str(buffer, "Removed ");
      append_str(buffer, COLLECTIBLE_NAMES[player->inventory[index]]);
      append_str(buffer, " from inventory slot ");
      int_to_str(index + 1, num_str);   // Converts index+1 to a string stored in num_str
      append_str(buffer, num_str);      // append num_str to buffer
      append_str(buffer, "\n");
      print(buffer);

      player->inventory[index] = NONE;  // Set slot at chosen index to empty(=NONE)
    }
    else
    {
      // Print that the chosen slot is already empty otherwise
      append_str(buffer, "Inventory slot ");
      int_to_str(index + 1, num_str);
      append_str(buffer, num_str);
      append_str(buffer, " is already empty.\n");
      print(buffer);
    }
  }
  // Print invalit slot nr if index is invalid
  else
  {
    print("Invalid inventory slot.\n");
  }
}

// Function that adds an item to storage
void add_to_storage(struct tile *storage_tile, enum COLLECTIBLETYPE item)
{
  char buffer[256];
  buffer[0] = '\0'; // Initialize buffer

  // Loop through all storage slots to find a empty slot(=NONE)
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (storage_tile->storage[i] == NONE)
    {
      storage_tile->storage[i] = item;

      // Concatenate string
      append_str(buffer, "Added ");
      append_str(buffer, COLLECTIBLE_NAMES[item]);
      append_str(buffer, " to storage.\n");
      print(buffer);

      return;
    }
  }

  print("Storage is full! Cannot add more items.\n");
}

// Function that adds fuel to enginebay storage
void add_fuel_enginebay(struct tile *engine_bay_tile, enum COLLECTIBLETYPE item)
{
  char buffer[256];
  buffer[0] = '\0';

  // Checks first slot of storage at engine bay (the only one that matters)
  if (engine_bay_tile->storage[0] == NONE)
  {
    // Add item(=fuel) to slot if empty
    engine_bay_tile->storage[0] = item;

    append_str(buffer, "Added ");
    append_str(buffer, COLLECTIBLE_NAMES[item]);
    append_str(buffer, " to engine bay.\n");
    print(buffer);

    return;
  }
  // If first slot has anything, write that it already has fuel
  print("Engine already has fuel, no need to add more.");
}

// Function to remove items from storage tile
void remove_from_storage(struct tile *storage_tile, int index)
{
  // Buffer to concatenate strings
  char buffer[256];
  char num_str[12];
  buffer[0] = '\0'; // Initialize buffer

  // check for valid index to remove from (0-5)
  if (index >= 0 && index < STORAGE_SIZE)
  {
    // If there is an item on the chosen index
    if (storage_tile->storage[index] != NONE)
    {
      append_str(buffer, "Removed ");
      append_str(buffer, COLLECTIBLE_NAMES[storage_tile->storage[index]]);
      append_str(buffer, " from storage slot ");
      int_to_str(index + 1, num_str);   // sets int index+1 as string in num_str
      append_str(buffer, num_str);      // appends num_str to buffer
      append_str(buffer, "\n");
      print(buffer);

      // Set slot at chosen index to NONE
      storage_tile->storage[index] = NONE;
    }
    else
    {
      // Print that the slot is alr empty at chosen index
      append_str(buffer, "Storage slot ");
      int_to_str(index + 1, num_str);
      append_str(buffer, num_str);
      append_str(buffer, " is already empty.\n");
      print(buffer);
    }
  }
  // Print invalid if index out of bounds
  else
  {
    print("Invalid storage slot.\n");
  }
}

// Function that checks content of storage in storage tile
void check_storage(struct player *player, struct tile *storage_tile)
{
  char buffer[256];
  char num_str[12];

  // Print interaction text
  buffer[0] = '\0';
  append_str(buffer, storage_tile->interaction_text);   // retrieve interaction text from stuct
  append_str(buffer, "\n");
  print(buffer);

  print_hold("You have stored the following items: \n");

  // Looop through every slot in storage
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    // If tile is not empty and is not out of bounds (only essentials)
    if (storage_tile->storage[i] != NONE && storage_tile->storage[i] >= FOOD && storage_tile->storage[i] <= CLOTHING)
    {
      buffer[0] = '\0';
      append_str(buffer, "Slot ");
      int_to_str(i + 1, num_str);   // set i+1 as string in num_str
      append_str(buffer, num_str);  // append num_str
      append_str(buffer, ": ");
      append_str(buffer, COLLECTIBLE_NAMES[storage_tile->storage[i]]);  // use str list to retrieve name of item in storage
      append_str(buffer, "\n");
      print(buffer);
    }
  }
}
