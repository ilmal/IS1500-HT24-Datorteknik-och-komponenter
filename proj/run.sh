rm -r game_binary
gcc -Wall -Wextra -o game_binary main.c tile.c player.c inventory.c game.c
./game_binary
