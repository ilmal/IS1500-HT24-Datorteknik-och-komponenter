#ifndef TILE_H
#define TILE_H

#include "game.h"

// Forward declaration of player struct
struct player;

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

// Function prototypes
struct tile *storage();
struct tile *chambers();
struct tile *cockpit();
struct tile *cafeteria();
struct tile *engine_bay();
struct tile *laboratory();
struct tile *airlock();
struct tile *landing_site();
struct tile *wasteland();
struct tile *ct(enum TILETYPE type);

#endif // TILE_H
