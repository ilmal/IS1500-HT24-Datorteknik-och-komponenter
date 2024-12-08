#include <stdlib.h>
#include "player.h"

struct player create_player()
{
  struct player new_player;

  // Initialize player attributes
  new_player.positionX = 0;
  new_player.positionY = 3;
  new_player.food = 99;
  new_player.oxygen = 99;
  new_player.water = 99;

  // Initialize inventory
  for (int i = 0; i < INVENTORY_SIZE; i++)
  {
    new_player.inventory[i] = NONE;
  }

  return new_player;
}
