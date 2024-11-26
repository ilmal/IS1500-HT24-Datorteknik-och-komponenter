#include <stdio.h>
#include <string.h>

#define MAX_ROOMS 5
#define MAX_INPUT 50

typedef struct Room {
    char description[256];
    struct Room *north;
    struct Room *south;
    struct Room *east;
    struct Room *west;
} Room;

void initializeRooms(Room rooms[]) {
    // Room 0
    strcpy(rooms[0].description, "You are in a forest clearing. Paths lead east and south.");
    rooms[0].east = &rooms[1];
    rooms[0].south = &rooms[2];
    rooms[0].north = NULL;
    rooms[0].west = NULL;

    // Room 1
    strcpy(rooms[1].description, "You are on a narrow path. The forest thickens to the north.");
    rooms[1].west = &rooms[0];
    rooms[1].north = &rooms[3];
    rooms[1].east = NULL;
    rooms[1].south = NULL;

    // Room 2
    strcpy(rooms[2].description, "You are at the edge of a peaceful lake.");
    rooms[2].north = &rooms[0];
    rooms[2].east = NULL;
    rooms[2].south = NULL;
    rooms[2].west = NULL;

    // Room 3
    strcpy(rooms[3].description, "You are in a dark cave. It is cold and damp.");
    rooms[3].south = &rooms[1];
    rooms[3].east = NULL;
    rooms[3].west = NULL;
    rooms[3].north = NULL;

    // Room 4
    strcpy(rooms[4].description, "You have found a hidden treasure! Congratulations!");
    rooms[4].south = &rooms[3];
    rooms[4].east = NULL;
    rooms[4].west = NULL;
    rooms[4].north = NULL;

    // Connecting Room 3 to Room 4
    rooms[3].north = &rooms[4];
}

int main() {
    Room rooms[MAX_ROOMS];
    Room *currentRoom;
    char input[MAX_INPUT];

    initializeRooms(rooms);
    currentRoom = &rooms[0];

    printf("Welcome to the Adventure Game!\n");

    while (1) {
        printf("\n%s\n", currentRoom->description);
        printf("What do you do? (go north, go south, go east, go west, quit): ");
        fgets(input, MAX_INPUT, stdin);

        // Remove newline character from input
        input[strcspn(input, "\n")] = 0;

        if (strcmp(input, "go north") == 0) {
            if (currentRoom->north != NULL) {
                currentRoom = currentRoom->north;
            } else {
                printf("You can't go that way.\n");
            }
        } else if (strcmp(input, "go south") == 0) {
            if (currentRoom->south != NULL) {
                currentRoom = currentRoom->south;
            } else {
                printf("You can't go that way.\n");
            }
        } else if (strcmp(input, "go east") == 0) {
            if (currentRoom->east != NULL) {
                currentRoom = currentRoom->east;
            } else {
                printf("You can't go that way.\n");
            }
        } else if (strcmp(input, "go west") == 0) {
            if (currentRoom->west != NULL) {
                currentRoom = currentRoom->west;
            } else {
                printf("You can't go that way.\n");
            }
        } else if (strcmp(input, "quit") == 0) {
            printf("Thanks for playing!\n");
            break;
        } else {
            printf("I don't understand that command.\n");
        }

        // Check if the player has reached the treasure
        if (currentRoom == &rooms[4]) {
            printf("\n%s\n", currentRoom->description);
            printf("You have completed the game!\n");
            break;
        }
    }

    return 0;
}
