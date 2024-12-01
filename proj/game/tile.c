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
  storage_tile->outside_rocket = 0;
  memset(storage_tile->storage, 0, sizeof(storage_tile->storage)); // fyller ut storage med nollor

  // default items in the storage:
  storage_tile->storage[0] = FOOD;
  storage_tile->storage[1] = FUEL;

  return storage_tile;
}

// chambers struct
struct tile *chambers()
{
  struct tile *chambers_tile = malloc(sizeof(struct tile));
  chambers_tile->type = CHAMBERS; // set type
  chambers_tile->outside_rocket = 0;
  chambers_tile->interaction_text = "You are in the personal chambers, this is the starting point.";
  return chambers_tile;
}

// cockpit struct
struct tile *cockpit()
{
  struct tile *cockpit_tile = malloc(sizeof(struct tile));
  cockpit_tile->type = COCKPIT; // set type
  cockpit_tile->interactable = 1;
  cockpit_tile->outside_rocket = 0;
  cockpit_tile->interaction_text = "You have entered the cockpit, you can now choose to end the game if you want.";

  return cockpit_tile;
}

// cafeteria struct
struct tile *cafeteria()
{
  struct tile *cafeteria_tile = malloc(sizeof(struct tile));
  cafeteria_tile->type = CAFETERIA;
  cafeteria_tile->outside_rocket = 0;
  cafeteria_tile->interaction_text = "You have entered the cafeteria, collect food here.";
  cafeteria_tile->collectible = FOOD; // what to be collected here

  return cafeteria_tile;
}
// engine bay struct
struct tile *engine_bay()
{
  struct tile *engine_bay_tile = malloc(sizeof(struct tile));
  engine_bay_tile->type = ENGINE_BAY;
  engine_bay_tile->outside_rocket = 0;

  // fyller ut storage i engine bay med nollor
  memset(engine_bay_tile->storage, 0, sizeof(engine_bay_tile->storage));

  return engine_bay_tile;
}

// laboratory struct
struct tile *laboratory()
{
  struct tile *laboratory_tile = malloc(sizeof(struct tile));
  laboratory_tile->type = LABORATORY;
  laboratory_tile->outside_rocket = 0;
  // TODO: create fuel/water logic

  return laboratory_tile;
}

// airlock struct
struct tile *airlock()
{
  struct tile *airlock_tile = malloc(sizeof(struct tile));
  airlock_tile->type = AIRLOCK;
  airlock_tile->outside_rocket = 0;

  return airlock_tile;
}

// landing site struct
struct tile *landing_site()
{
  struct tile *landing_site_tile = malloc(sizeof(struct tile));
  landing_site_tile->type = LANDING_SITE;
  landing_site_tile->outside_rocket = 1;
  return landing_site_tile;
}

// wasteland struct
struct tile *wasteland()
{
  struct tile *wasteland_tile = malloc(sizeof(struct tile));
  wasteland_tile->type = WASTELAND;
  wasteland_tile->outside_rocket = 1;
  return wasteland_tile;
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

  pond_tile->storage[0] = TARDIGRADES;

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
  cave_tile->collectible = ALIEN_BONES;
  cave_tile->storage[0] = ALIEN_BONES;

  return cave_tile;
}

// crater struct
struct tile *crater()
{
  struct tile *crater_tile = malloc(sizeof(struct tile));
  crater_tile->type = CRATER;
  crater_tile->outside_rocket = 1;
  crater_tile->collectible = SEDIMENTARY_LAYERS;
  crater_tile->storage[0] = SEDIMENTARY_LAYERS;

  return crater_tile;
}
// canyon struct
struct tile *canyon()
{
  struct tile *canyon_tile = malloc(sizeof(struct tile));
  canyon_tile->type = CANYON;
  canyon_tile->outside_rocket = 1;
  canyon_tile->collectible = RSL_IMAGES;
  canyon_tile->storage[0] = RSL_IMAGES;

  return canyon_tile;
}

// mountain struct
struct tile *mountain()
{
  struct tile *mountain_tile = malloc(sizeof(struct tile));
  mountain_tile->type = MOUNTAIN;
  mountain_tile->outside_rocket = 1;
  mountain_tile->collectible = ICE;
  mountain_tile->storage[0] = ICE;

  return mountain_tile;
}

// empty struct
struct tile *empty()
{
  struct tile *empty_tile = malloc(sizeof(struct tile));
  empty_tile->type = EMPTY;
  empty_tile->outside_rocket = 0;

  return empty_tile;
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
  case EMPTY:
    return empty();
  default:
    return wasteland();
  }
}
