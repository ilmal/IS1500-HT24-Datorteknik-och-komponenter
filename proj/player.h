#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

// struct player
struct player
{
  int positionX;
  int positionY;
  int food;
  int oxygen;
  enum COLLECTIBLETYPE inventory[INVENTORY_SIZE];
  int water;
};

// create player func
struct player create_player();

#endif // PLAYER_H
