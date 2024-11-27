#include <stdio.h>
#include <stdlib.h>

// Type of "room" the player can be in
enum TILETYPE
{
    // in the rocket
    STORAGE,        // 0
    CHAMBERS,       // 1
    COCKPIT,        // 2
    CAFETERIA,      // 3
    ENGINE_BAY,     // 4
    LABORATORY,     // 5
    AIRLOCK,        // 6

    // on the planet
    LANDING_SITE,   // 7
    WASTELAND,      // 8
    LOOSE_SOIL,     // 9
    POND,           // 10
    SHARP_ROCKS,    // 11
    CAVE,           // 12
    CRATER,         // 13
    CANYON,         // 14
    MOUNTAIN,       // 15
};

const char* TILETYPE_NAMES[] = {
    "STORAGE",
    "CHAMBERS",
    "COCKPIT",
    "CAFETERIA",
    "ENGINE_BAY",
    "LABORATORY",
    "AIRLOCK",
    "LANDING_SITE",
    "WASTELAND",
    "LOOSE_SOIL",
    "POND",
    "SHARP_ROCKS",
    "CAVE",
    "CRATER",
    "CANYON",
    "MOUNTAIN"
};

struct tile
{
    enum TILETYPE type;   
    // interactions
    int interactable;
    char interaction_text;

    // storage
    enum COLLECTIBLETYPE storage[10];

    // collectibles
    int collectibles;

    // stat changes
    int drop_oxygen;
    int drop_food;
    int drop_water;
};

struct player {
    int positionX;
    int positionY;
    int food;
    int oxygen;
    int inventory[5];
    int water;
};

enum COLLECTIBLETYPE {
    TARDIGRADES,        // found in pond
    SEDIMENTARY_LAYERS, // found in crater
    RSL_IMAGES,         // found in canyon
    ALIEN_BONES,        // found in cave
    OLD_ROVER_PARTS,    // found in wasteland (low chance)
    ICE,                // found in 
    FOOD,               // found in cafeteria
    BOTTLE_OF_WATER,    // found in cafeteria    
};

const char* COLLECTIBLE_NAMES[] = {
    "Tardigrades",
    "Sedimentary Layers",
    "RSL Images"
    "Alien Bones",
    "Old Rover Parts"
    "Ice",
    "Food",
    "Bottle of Water"
};

struct player* create_player() {
    struct player* new_player = malloc(sizeof(struct player));
    new_player->positionX = 0;
    new_player->positionY = 0;
    new_player->food = 100;
    new_player->oxygen = 100;
    new_player->water = 100;
    for (int i = 0; i < 10; i++) {
        new_player->inventory[i] = 0;
    }
    return new_player;
}

struct tile* storage(){
    struct tile* storage_tile = malloc(sizeof(struct tile));
    storage_tile->interactable = 1;
    storage_tile->interaction_text = "You are in the storage room. You can store your science samples here.";
    memset(storage_tile->storage, 0, sizeof(storage_tile->storage));

    // default items in the storage:
    storage_tile->storage[0] = FOOD;
    storage_tile->storage[1] = BOTTLE_OF_WATER;

    return storage_tile;
}


// Constructor function to create a new tile (ct = create_tile)
struct tile* ct(enum TILETYPE type) {
    switch (type)
    {
    case 0:
        return storage();
    default:
        break;
    }
}


int main(){
    struct tile* map[3][3] = {
        {ct(STORAGE), ct(CHAMBERS), ct(COCKPIT)},
        {ct(CAFETERIA), ct(ENGINE_BAY), ct(LABORATORY)},
        {ct(AIRLOCK), ct(LANDING_SITE), ct(WASTELAND)}
    };    

    struct player* player = create_player();

    printf("Welcome to the game!\n");
    printf("You are stranded on Mars and need to find a way to survive.\n");

    while (1) {
        printf("You are at position (%d, %d)\n", player->positionX, player->positionY);
        printf("The tile is: %s\n", TILETYPE_NAMES[map[player->positionY][player->positionX]->type]);
        printf("What would you like to do? (write a number)\n");
        printf("1. Move\n");
        printf("2. Interact\n");
        printf("3. Check inventory\n");
        printf("4. Check vitals\n");
        printf("5. Quit\n");

        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            
            printf("Which direction would you like to move?\n");
            printf("1. North\n");
            printf("2. East\n");
            printf("3. South\n");
            printf("4. West\n");

            int direction;
            scanf("%d", &direction);

            if (direction == 1) {
                player->positionY -= 1;
            } else if (direction == 2) {
                player->positionX += 1;
            } else if (direction == 3) {
                player->positionY += 1;
            } else if (direction == 4) {
                player->positionX -= 1;
            } else {
                printf("Invalid direction\n");
            }
        } else if (choice == 2) {
            printf("Interacting with the environment...\n");
        } else if (choice == 3) {
            printf("Checking inventory...\n");
        } else if (choice == 4) {
            printf("Checking vitals...\n");
        } else if (choice == 5) {
            printf("Quitting game...\n");
            break;
        } else {
            printf("Invalid choice\n");
        }
    }
}


