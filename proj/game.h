#ifndef GAME_H
#define GAME_H

#define INVENTORY_SIZE 6
#define STORAGE_SIZE 5

// Enum for TILETYPE
enum TILETYPE
{
  // in the rocket
  STORAGE,    // 0
  CHAMBERS,   // 1
  COCKPIT,    // 2
  CAFETERIA,  // 3
  ENGINE_BAY, // 4
  LABORATORY, // 5
  AIRLOCK,    // 6

  // on the planet
  LANDING_SITE, // 7
  WASTELAND,    // 8
  LOOSE_SOIL,   // 9
  POND,         // 10
  SHARP_ROCKS,  // 11
  CAVE,         // 12
  CRATER,       // 13
  CANYON,       // 14
  MOUNTAIN,     // 15

  // misc
  EMPTY // 16
};

extern const char *TILETYPE_NAMES[];

// Enum for TILETYPE_MAP
extern const char *TILETYPE_MAP_NAMES[];

// Enum for COLLECTIBLETYPE
enum COLLECTIBLETYPE
{
  NONE,               // represents empty slot
  TARDIGRADES,        // 1
  SEDIMENTARY_LAYERS, // 2
  RSL_IMAGES,         // 3
  ALIEN_BONES,        // 4
  OLD_ROVER_PARTS,    // 5
  ICE,                // 6

  // essential stuff
  FOOD,            // 7
  BOTTLE_OF_WATER, // 8
  FUEL,            // 9
  MEDICAL_KIT,     // 10
  TOOLBOX,         // 11
  MAP,             // 12
  SPARE_PARTS,     // 13
  TEDDY_BEAR,      // 14
  BLANKET,         // 15
  CLOTHING         // 16
};

extern const char *COLLECTIBLE_NAMES[];

#endif // GAME_H
