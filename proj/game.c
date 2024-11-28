#include <stdio.h>
#include <stdlib.h>
#include <string.h>  

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
enum COLLECTIBLETYPE {
    TARDIGRADES,        // found in pond
    SEDIMENTARY_LAYERS, // found in crater
    RSL_IMAGES,         // found in canyon
    ALIEN_BONES,        // found in cave
    OLD_ROVER_PARTS,    // found in wasteland (low chance)
    ICE,                // found in somewhere
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

struct tile
{
    enum TILETYPE type;   
    // interactions
    int interactable;
    char* interaction_text;

    // storage
    enum COLLECTIBLETYPE storage[50];      // storage can store more than enough

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
    int inventory[5];       // player can only collect upto 5 items at hand
    int water;
};

struct player* create_player() {
    struct player* new_player = malloc(sizeof(struct player));

    // start position
    new_player->positionX = 0;
    new_player->positionY = 0;

    // start health stats
    new_player->food = 100;
    new_player->oxygen = 100;
    new_player->water = 100;

    // start inventory
    for (int i = 0; i < 5; i++) {
        new_player->inventory[i] = 0;
    }
    return new_player;
}

// storage struct
struct tile* storage(){
    struct tile* storage_tile = malloc(sizeof(struct tile));
    storage_tile->type = STORAGE;   // set type
    storage_tile->interactable = 1;
    storage_tile->interaction_text = "You are in the storage room which holds your food and scientific samples - you can store your inventory here.";
    memset(storage_tile->storage, 0, sizeof(storage_tile->storage));

    // default items in the storage:
    storage_tile->storage[0] = FOOD;
    storage_tile->storage[1] = BOTTLE_OF_WATER;

    return storage_tile;
}

// chambers struct
struct tile* chambers() {
    struct tile* chambers_tile = malloc(sizeof(struct tile));
    chambers_tile->type = CHAMBERS;     // set type
    chambers_tile->interaction_text = "You are in the personal chambers, this is the starting point.";
    return chambers_tile;
}

// cockpit struct
struct tile* cockpit() {
    struct tile* cockpit_tile = malloc(sizeof(struct tile));
    cockpit_tile->type = COCKPIT;       // set type
    cockpit_tile->interactable = 1;
    cockpit_tile->interaction_text = "You have entered the cockpit, you can now choose to end the game if you want.";
    
    return cockpit_tile;
}

// cafeteria struct
struct tile* cafeteria() {
    struct tile* cafeteria_tile = malloc(sizeof(struct tile));
    cafeteria_tile->type = CAFETERIA;
    cafeteria_tile->interaction_text = "You have entered the cafeteria, collect food here.";
    cafeteria_tile->collectibles = 1;

    return cafeteria_tile;
}
// engine bay struct
struct tile* engine_bay() {
    struct tile* engine_bay_tile = malloc(sizeof(struct tile));
    engine_bay_tile->type = ENGINE_BAY;
    // TODO: power up map minigame stuff

    return engine_bay_tile;
}

// laboratory struct
struct tile* laboratory() {
    struct tile* laboratory_tile = malloc(sizeof(struct tile));
    laboratory_tile->type = LABORATORY;
    // TODO: create fuel/water logic

    return laboratory_tile;
}

// airlock struct
struct tile* airlock() {
    struct tile* airlock_tile = malloc(sizeof(struct tile));
    airlock_tile->type = AIRLOCK;
    // TODO: refill oxygen logic

    return airlock_tile;
}

// landing site struct
struct tile* landing_site() {
    struct tile* landing_site_tile = malloc(sizeof(struct tile));
    landing_site_tile->type = LANDING_SITE;
    return landing_site_tile;
}

// wasteland struct
struct tile* wasteland() {
    struct tile* wasteland_tile = malloc(sizeof(struct tile));
    wasteland_tile->type = WASTELAND;
    return wasteland_tile;
}

// Constructor function to create a new tile (ct = create_tile)
struct tile* ct(enum TILETYPE type) {
    switch (type)
    {
    case 0:
        return storage();
    case 1:
        return chambers();
    case 2:
        return cockpit();
    case 3: 
        return cafeteria();
    case 4: 
        return engine_bay();
    case 5: 
        return laboratory();
    case 6: 
        return airlock();
    case 7:
        return landing_site();
    case 8:
        return wasteland();
    default:
        return wasteland(); 
    }
}

void add_to_inventory(struct player* player, enum COLLECTIBLETYPE item) {
    for (int i = 0; i < 5; i++) {
        if (player->inventory[i] == 0) {  // Check if the slot is empty
            player->inventory[i] = item;  // Add the item to the inventory
            printf("You have added: %s to your inventory.\n", COLLECTIBLE_NAMES[item]);
            return;  
        }
    }
    // If there is no empty slot
    printf("Your inventory is full! Cannot add more items.\n");
}

// helper func to see storage
void check_storage(struct player* player, struct tile* storage_tile) {
    // printf("%s\n", storage_tile->interaction_text);
    printf("You have stored the following items: \n");
    for(int i = 0; i < 5; i++) {
        if(storage_tile->storage[i] != 0) {
            printf("%s\n", COLLECTIBLE_NAMES[storage_tile->storage[i]]);
        }
    }
}

int main(){
    struct tile* map[3][3] = {
        {ct(STORAGE), ct(CHAMBERS), ct(COCKPIT)},
        {ct(CAFETERIA), ct(ENGINE_BAY), ct(LABORATORY)},
        {ct(AIRLOCK), ct(LANDING_SITE), ct(WASTELAND)}
    };    
    // debugging
    for (int y = 0; y < 3; y++) {
        for (int x = 0; x < 3; x++) {
            if (map[y][x] == NULL) {
                printf("Error: map[%d][%d] is not initialized\n", y, x);
            } else {
                printf("map[%d][%d] is initialized: %s\n", y, x, TILETYPE_NAMES[map[y][x]->type]);
            }
        }
    }

    struct player* player = create_player();

    printf("Welcome to the game!\n");
    printf("You are stranded on Mars and need to find a way to survive.\n");
    // TODO: add game description text

    while (1) {
        printf("You are at position (%d, %d)\n", player->positionX, player->positionY);
        printf("%d\n", map[player->positionY][player->positionX]->type);
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

            printf("Current position: (%d, %d)\n", player->positionX, player->positionY);

            // Move the player
            if (direction == 1 && player->positionY > 0) {
                player->positionY -= 1;
            } else if (direction == 2 && player->positionX < 2) {
                player->positionX += 1;
            } else if (direction == 3 && player->positionY < 2) {
                player->positionY += 1;
            } else if (direction == 4 && player->positionX > 0) {
                player->positionX -= 1;
            } else {
                printf("Invalid direction or out of bounds\n");
                continue; // Skip the movement if invalid
            }

            printf("New position: (%d, %d)\n", player->positionX, player->positionY);
            printf("Tile type at new position: %s\n", TILETYPE_NAMES[map[player->positionY][player->positionX]->type]);

        } else if (choice == 2) {
            printf("checking interaction...");
            // // storage
            // if (map[player->positionY][player->positionX]->type == STORAGE) {
            //     struct tile* storage_tile = map[player->positionY][player->positionX];
            //     check_storage(player, storage_tile);
            //     // TODO: storage logic
            // }
            // // cockpit
            // else if(map[player->positionY][player->positionX]->type == COCKPIT) {
            //     char end_game;
            //     printf("Do you want to end the game and return to Earth? (y/n)\n");
            //     scanf("%c\n", &end_game);
            //     if (end_game == 'y' || end_game == 'Y') {
            //         printf("You have chosen to end the game. Goodbye!\n");
            //         // TODO: win/lose logic
            //         break;  
            //     } else {
            //         printf("Lol, you coward. Keep playing then...\n");  // change text maybe?
            //     }
            // }
            // // cafeteria
            // else if(map[player->positionY][player->positionX]->type == CAFETERIA) {
            //     printf("Grab some food? (y/n)\n");
            //     char grab_food;
            //     scanf("%c\n", &grab_food);
            //     if(grab_food == 'y' || grab_food == 'Y') {
            //         printf("You have grabbed one lunchbox.\n");

            //     }
            // }
            
        } else if (choice == 3) {
            printf("Items in your inventory: \n");
            for(int i = 0; i < 5; i++) {
                if(player->inventory[i]!=0) {
                    printf("%s\n", COLLECTIBLE_NAMES[player->inventory[i]]);
                }
            }
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


