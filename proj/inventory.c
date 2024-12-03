#include <stdio.h>
#include "inventory.h"

void add_to_inventory(struct player player, enum COLLECTIBLETYPE item)
{
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    if (player.inventory[i] == NONE)
    {                             // Check if the slot is empty
      player.inventory[i] = item; // Add the item to the inventory
      printf("You have added: %s to your inventory.\n", COLLECTIBLE_NAMES[item]);
      return;
    }
  }
  // If there is no empty slot
  printf("Your inventory is full! Cannot add more items.\n");
}

void remove_from_inventory(struct player player, int index)
{
  if (index >= 0 && index < INVENTORY_SIZE)
  {
    if (player.inventory[index] != NONE)
    {
      printf("Removed %s from inventory slot %d\n", COLLECTIBLE_NAMES[player.inventory[index]], index + 1);
      player.inventory[index] = NONE;
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

void add_to_storage(struct tile storage_tile, enum COLLECTIBLETYPE item)
{
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (storage_tile.storage[i] == NONE)
    {
      storage_tile.storage[i] = item;
      printf("Added %s to storage.\n", COLLECTIBLE_NAMES[item]);
      return;
    }
  }
  printf("Storage is full! Cannot add more items.\n");
}

void remove_from_storage(struct tile storage_tile, int index)
{
  if (index >= 0 && index < STORAGE_SIZE)
  {
    if (storage_tile.storage[index] != NONE)
    {
      printf("Removed %s from storage slot %d\n", COLLECTIBLE_NAMES[storage_tile.storage[index]], index + 1);
      storage_tile.storage[index] = NONE;
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

// Helper function to see storage
void check_storage(struct player player, struct tile storage_tile)
{
  printf("%s\n", storage_tile.interaction_text);
  printf("You have stored the following items: \n");
  for (int i = 0; i < STORAGE_SIZE; i++)
  {
    if (storage_tile.storage[i] != NONE)
    {
      printf("Slot %d: %s\n", i + 1, COLLECTIBLE_NAMES[storage_tile.storage[i]]);
    }
  }
}
