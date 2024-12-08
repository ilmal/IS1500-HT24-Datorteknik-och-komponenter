#include <string.h>
#include <stdlib.h>
#include "tile.h"

extern void print(const char *);

// every tiletype will be inserted with custom values for diff attributes

// storage struct
struct tile storage()
{
  struct tile storage_tile;
  storage_tile.type = STORAGE; // set type
  storage_tile.interactable = 1;
  storage_tile.interaction_text = "You are in the storage room which holds your food and scientific samples - you can store your inventory here.";
  storage_tile.outside_rocket = 0;

  // fyller ut storage med nollor (memset fast coolare)
  for (int i = 0; i < 10; i++)
  {
    storage_tile.storage[i] = 0;
  }

  // default items in the storage:
  storage_tile.storage[0] = FOOD;
  storage_tile.storage[1] = FUEL;

  return storage_tile;
}

// chambers struct
struct tile chambers()
{
  struct tile chambers_tile;
  chambers_tile.type = CHAMBERS; // set type
  chambers_tile.outside_rocket = 0;
  chambers_tile.interaction_text = "You are in the personal chambers, this is the starting point.";
  return chambers_tile;
}

// cockpit struct
struct tile cockpit()
{
  struct tile cockpit_tile;
  cockpit_tile.type = COCKPIT; // set type
  cockpit_tile.interactable = 1;
  cockpit_tile.outside_rocket = 0;
  cockpit_tile.interaction_text = "You have entered the cockpit, you can now choose to end the game if you want.";

  return cockpit_tile;
}

// cafeteria struct
struct tile cafeteria()
{
  struct tile cafeteria_tile;
  cafeteria_tile.type = CAFETERIA;
  cafeteria_tile.outside_rocket = 0;
  cafeteria_tile.interaction_text = "You have entered the cafeteria, collect food here.";
  cafeteria_tile.collectible = FOOD; // what to be collected here

  return cafeteria_tile;
}
// engine bay struct
struct tile engine_bay()
{
  struct tile engine_bay_tile;
  engine_bay_tile.type = ENGINE_BAY;
  engine_bay_tile.outside_rocket = 0;
  engine_bay_tile.interaction_text = "You are in the engine bay, you must fill the engine with fuel.";

  // fyller ut engine_bay_tile storage med nollor (memset fast coolare)
  for (int i = 0; i < 10; i++)
  {
    engine_bay_tile.storage[i] = 0;
  }

  return engine_bay_tile;
}

// laboratory struct
struct tile laboratory()
{
  struct tile laboratory_tile;
  laboratory_tile.type = LABORATORY;
  laboratory_tile.outside_rocket = 0;
  laboratory_tile.interaction_text = "You are at the laboratory, you can experiment here.";

  return laboratory_tile;
}

// airlock struct
struct tile airlock()
{
  struct tile airlock_tile;
  airlock_tile.type = AIRLOCK;
  airlock_tile.outside_rocket = 0;
  airlock_tile.interaction_text = "You entered the airlock, make sure to keep an eye on the oxygen when exiting.";

  return airlock_tile;
}

// landing site struct
struct tile landing_site()
{
  struct tile landing_site_tile;
  landing_site_tile.type = LANDING_SITE;
  landing_site_tile.outside_rocket = 1;
  landing_site_tile.interaction_text = "You are at the landing site, you can now explore the planet.";
  landing_site_tile.interactable = 1;
  landing_site_tile.storage[0] = TOOLBOX;

  return landing_site_tile;
}

// wasteland struct
struct tile wasteland()
{
  struct tile wasteland_tile;
  wasteland_tile.type = WASTELAND;
  wasteland_tile.outside_rocket = 1;
  wasteland_tile.interaction_text = "You are in the Martian wasteland.";

  return wasteland_tile;
}

// loose soil struct
struct tile loose_soil()
{
  struct tile loose_soil_tile;
  loose_soil_tile.type = LOOSE_SOIL;
  loose_soil_tile.outside_rocket = 1;
  loose_soil_tile.interaction_text = "Uh oh, it took a lot of energy to get out of the loose soil...";

  return loose_soil_tile;
}

// pond struct
struct tile pond()
{

  struct tile pond_tile;
  pond_tile.type = POND;
  pond_tile.outside_rocket = 1;
  pond_tile.collectible = TARDIGRADES;

  pond_tile.storage[0] = TARDIGRADES;
  pond_tile.interaction_text = "At the pond, you may find life.";

  return pond_tile;
}

struct tile sharp_rocks()
{
  struct tile sharp_rocks_tile;
  sharp_rocks_tile.type = SHARP_ROCKS;
  sharp_rocks_tile.outside_rocket = 1;
  sharp_rocks_tile.interaction_text = "Be careful! The sharp rocks destroyed one oxygen tank and now you lost half of your oxygen.";

  return sharp_rocks_tile;
}

// cave struct
struct tile cave()
{
  struct tile cave_tile;
  cave_tile.type = CAVE;
  cave_tile.outside_rocket = 1;
  cave_tile.collectible = ALIEN_BONES;
  cave_tile.storage[0] = ALIEN_BONES;
  cave_tile.interaction_text = "Darkness engulf you, you have approached a cave.";

  return cave_tile;
}

// crater struct
struct tile crater()
{
  struct tile crater_tile;
  crater_tile.type = CRATER;
  crater_tile.outside_rocket = 1;
  crater_tile.collectible = SEDIMENTARY_LAYERS;
  crater_tile.storage[0] = SEDIMENTARY_LAYERS;
  crater_tile.interaction_text = "You are now in a crater on Mars.";

  return crater_tile;
}
// canyon struct
struct tile canyon()
{
  struct tile canyon_tile;
  canyon_tile.type = CANYON;
  canyon_tile.outside_rocket = 1;
  canyon_tile.collectible = RSL_IMAGES;
  canyon_tile.storage[0] = RSL_IMAGES;
  canyon_tile.interaction_text = "A canyon has many secrets, you may find one?";

  return canyon_tile;
}

// mountain struct
struct tile mountain()
{
  struct tile mountain_tile;
  mountain_tile.type = MOUNTAIN;
  mountain_tile.outside_rocket = 1;
  mountain_tile.interaction_text = "You cannot pass a mountain, you must go around it.";

  return mountain_tile;
}

// empty struct
struct tile empty()
{
  struct tile empty_tile;
  empty_tile.type = EMPTY;
  empty_tile.outside_rocket = 0;
  empty_tile.interaction_text = "What are you even doing on an empty tile??";

  return empty_tile;
}

// the ct (create_tile) function
struct tile ct(enum TILETYPE type)
{
  {
    // depending on input enum TILETYPE, return tile from a certain type
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
    case LOOSE_SOIL:
      return loose_soil();
    case POND:
      return pond();
    case SHARP_ROCKS:
      return sharp_rocks();
    case CAVE:
      return cave();
    case CRATER:
      return crater();
    case CANYON:
      return canyon();
    case MOUNTAIN:
      return mountain();
    case EMPTY:
      return empty();
    default:
      return wasteland();
    }
  }
}
