#include <stdio.h>
#include <stdlib.h>

enum TYPE
{
    // in the rocket
    STORAGE,
    CHAMBERS,
    COCKPIT,
    CAFETERIA,
    ENGINE_BAY,
    LABORATORY,
    AIRLOCK,

    // on the planet
    LANDING_SITE,
    WASTELAND,
    LOOSE_SOIL,
    POND,
    SHARP_ROCKS,
    CAVE,
    CRATER,
    CANYON,
    MOUNTAIN,
};

struct tile
{
    enum TYPE type;   
    int interactions;
    int collectibles;
};

struct player {
    int positionX;
    int positionY;
    int food;
    int oxygen;
    int inventory[10];
    int water;
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

// Constructor function to create a new tile (ct = create_tile)
struct tile* ct(enum TYPE type) {
    struct tile* new_tile = malloc(sizeof(struct tile));
    new_tile->type = type;
    return new_tile;
}


int main(){
    struct tile* map[3][3] = {
        {ct(STORAGE), ct(CHAMBERS), ct(COCKPIT)},
        {ct(CAFETERIA), ct(ENGINE_BAY), ct(LABORATORY)},
        {ct(AIRLOCK), ct(LANDING_SITE), ct(WASTELAND)}
    };    

    struct player* player = create_player();

    printf("Welcome to the game!\n");
    printf("You are stranded on an Mars and need to find a way to survive.\n");

    while (1) {
        printf("You are at position (%d, %d)\n", player->positionX, player->positionY);
        printf("What would you like to do?\n");
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


