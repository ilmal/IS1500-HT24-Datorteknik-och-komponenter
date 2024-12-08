#include <stdlib.h>
#include "player.h"

// func to create player
struct player create_player()
{
  struct player new_player;

  // Starting position (chambers)
  new_player.positionX = 0;
  new_player.positionY = 3;

  // player starts with 99 in all stats
  new_player.food = 99;
  new_player.oxygen = 99;
  new_player.water = 99;

  // Initialize empty inventory 
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    new_player.inventory[i] = NONE;
  }

  return new_player;
}
