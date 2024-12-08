#include "game.h"

// List of names as string for each tiletype
const char *TILETYPE_NAMES[] = {
    "STORAGE",
    "CHAMBERS",
    "COCKPIT",
    "CAFETERIA",
    "ENGINE_BAY",
    "LABORATORY",
    "AIRLOCK",
    "LANDING SITE",
    "WASTELAND",
    "LOOSE SOIL",
    "POND",
    "SHARP ROCKS",
    "CAVE",
    "CRATER",
    "CANYON",
    "MOUNTAIN",
    ""};

// List of names as string with emojis for each tiletype -> these are a bit unintuitive
const char *TILETYPE_MAP_NAMES[] = {
    "📦",   // Storage
    "🛏️ ",  // Chambers
    "🚀",   // Cockpit
    "🥡",   // Cafeteria
    "💨",   // Engine bay
    "🧪",   // Laboratory
    "🚪",   // Airlock
    "📍",   // Landing site
    "🏜️ ",  // Wasteland
    "⏳",   // Loose soil
    "🧊",   // Pond -> extremely unintuitive
    "🪨 ",  // Sharp rocks
    "🦇",   // Cave
    "🕳️ ",  // Crater
    "🧗",   // Canyon
    "⛰️ ",  // Mountain
    "  "};

// String list for name of each possible collectible item
const char *COLLECTIBLE_NAMES[] = {
    "Empty Slot",
    "Tardigrades",
    "Sedimentary Layers",
    "RSL Images",
    "Alien Bones",
    "Old Rover Parts",
    "Ice",

    "Food",
    "Bottle of Water",
    "Fuel",
    "Medical kit",
    "Toolbox",
    "Map",
    "Spare parts",
    "Teddy bear",
    "Blanket",
    "Clothing"};
