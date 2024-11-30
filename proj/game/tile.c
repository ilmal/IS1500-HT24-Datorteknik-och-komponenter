#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tile.h"

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

// the ct (create_tile) function
struct tile *ct(enum TILETYPE type)
{
  switch (type)
  {
  case STORAGE:
    return storage();
  case CHAMBERS:
    return chambers();
  case COCKPIT:
    return cockpit();
  case CAFETERIA:
    return cafeteria();
  case ENGINE_BAY:
    return engine_bay();
  case LABORATORY:
    return laboratory();
  case AIRLOCK:
    return airlock();
  case LANDING_SITE:
    return landing_site();
  case WASTELAND:
    return wasteland();
  default:
    return wasteland();
  }
}
