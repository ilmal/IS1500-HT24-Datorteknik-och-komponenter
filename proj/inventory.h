#ifndef INVENTORY_H
#define INVENTORY_H

#include "player.h"
#include "tile.h"

// Function prototypes
void add_to_inventory(struct player player, enum COLLECTIBLETYPE item);
void remove_from_inventory(struct player player, int index);
void add_to_storage(struct tile storage_tile, enum COLLECTIBLETYPE item);
void remove_from_storage(struct tile storage_tile, int index);
void check_storage(struct player player, struct tile storage_tile);

#endif // INVENTORY_H
