#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"

struct player
{
  int positionX;
  int positionY;
  int food;
  int oxygen;
  enum COLLECTIBLETYPE inventory[INVENTORY_SIZE];
  int water;
};

struct player *create_player();

#endif // PLAYER_H
