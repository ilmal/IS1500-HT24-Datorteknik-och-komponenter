#include "inventory.h"

// Assuming these helper functions exist in your codebase:
extern void print(const char *);
extern void append_str(char *dest, const char *src);
extern void int_to_str(int num, char *str);

void add_to_inventory(struct player *player, enum COLLECTIBLETYPE item)
{
  char buffer[256];
  buffer[0] = '\0'; // Initialize buffer to an empty string

  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    if (player->inventory[i] == NONE)
    {
      player->inventory[i] = item; // Add the item to the inventory

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

void remove_from_inventory(struct player *player, int index)
{
  char buffer[256];
  char num_str[12];
  buffer[0] = '\0'; // Initialize buffer

  if (index >= 0 && index < INVENTORY_SIZE)
  {
    if (player->inventory[index] != NONE)
    {
      append_str(buffer, "Removed ");
      append_str(buffer, COLLECTIBLE_NAMES[player->inventory[index]]);
      append_str(buffer, " from inventory slot ");
      int_to_str(index + 1, num_str);
      append_str(buffer, num_str);
      append_str(buffer, "\n");
      print(buffer);

      player->inventory[index] = NONE;
    }
    else
    {
      append_str(buffer, "Inventory slot ");
      int_to_str(index + 1, num_str);
      append_str(buffer, num_str);
      append_str(buffer, " is already empty.\n");
      print(buffer);
    }
  }
  else
  {
    print("Invalid inventory slot.\n");
  }
}

void add_to_storage(struct tile *storage_tile, enum COLLECTIBLETYPE item)
{
  char buffer[256];
  buffer[0] = '\0'; // Initialize buffer

  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (storage_tile->storage[i] == NONE)
    {
      storage_tile->storage[i] = item;

      append_str(buffer, "Added ");
      append_str(buffer, COLLECTIBLE_NAMES[item]);
      append_str(buffer, " to storage.\n");
      print(buffer);

      return;
    }
  }

  print("Storage is full! Cannot add more items.\n");
}

void add_fuel_enginebay(struct tile *engine_bay_tile, enum COLLECTIBLETYPE item)
{
  char buffer[256];
  buffer[0] = '\0';

  if (engine_bay_tile->storage[0] == NONE)
  {
    engine_bay_tile->storage[0] = item;

    append_str(buffer, "Added ");
    append_str(buffer, COLLECTIBLE_NAMES[item]);
    append_str(buffer, " to engine bay.\n");
    print(buffer);

    return;
  }
  print("Engine already has fuel, no need to add more.");
}

void remove_from_storage(struct tile *storage_tile, int index)
{
  char buffer[256];
  char num_str[12];
  buffer[0] = '\0'; // Initialize buffer

  if (index >= 0 && index < STORAGE_SIZE)
  {
    if (storage_tile->storage[index] != NONE)
    {
      append_str(buffer, "Removed ");
      append_str(buffer, COLLECTIBLE_NAMES[storage_tile->storage[index]]);
      append_str(buffer, " from storage slot ");
      int_to_str(index + 1, num_str);
      append_str(buffer, num_str);
      append_str(buffer, "\n");
      print(buffer);

      storage_tile->storage[index] = NONE;
    }
    else
    {
      append_str(buffer, "Storage slot ");
      int_to_str(index + 1, num_str);
      append_str(buffer, num_str);
      append_str(buffer, " is already empty.\n");
      print(buffer);
    }
  }
  else
  {
    print("Invalid storage slot.\n");
  }
}

void check_storage(struct player *player, struct tile *storage_tile)
{
  char buffer[256];
  char num_str[12];

  // Print interaction text
  buffer[0] = '\0';
  append_str(buffer, storage_tile->interaction_text);
  append_str(buffer, "\n");
  print(buffer);

  print_hold("You have stored the following items: \n");
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (storage_tile->storage[i] != NONE && storage_tile->storage[i] >= FOOD && storage_tile->storage[i] <= CLOTHING)
    {
      buffer[0] = '\0';
      append_str(buffer, "Slot ");
      int_to_str(i + 1, num_str);
      append_str(buffer, num_str);
      append_str(buffer, ": ");
      append_str(buffer, COLLECTIBLE_NAMES[storage_tile->storage[i]]);
      append_str(buffer, "\n");
      print(buffer);
    }
  }
}
