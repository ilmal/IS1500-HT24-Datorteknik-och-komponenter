# Mars Survival Game

## Background

The **Mars Survival Game** is a text-based adventure designed for the **DTEK-V board**, utilizing its hardware capabilities for an immersive and interactive gaming experience. In this game, players explore a Martian environment, manage their survival resources (**oxygen**, **water**, and **food**), and interact with various locations on a map. The objective is to survive the harsh conditions, gather essential resources, and potentially return to Earth.

The game integrates the DTEK-V board’s toggles, push buttons, LEDs, and seven-segment displays, offering players an engaging interface to navigate the Martian landscape. With a map containing diverse terrains and a variety of interactive tiles, each session challenges players to strategize their movements and resource management.

## Story

Stranded on the unforgiving terrain of Mars after a catastrophic mission failure, you must face the ultimate test of survival. Your spacecraft lies in ruins, your resources are critically low, and the hostile environment offers no mercy. As each moment ticks by, you must explore, scavenge, and strategize to gather vital supplies, repair your ship, and defy the odds to return to Earth. Will you rise to the challenge, or will the Red Planet become your final resting place?

## Game Mechanics

### Movement

Use the DTEK-V board’s toggle switches to navigate the map:

- **East**: Move right
- **West**: Move left
- **South**: Move down
- **North**: Move up

The game ensures movement is restricted to valid tiles. Obstacles like mountains or the ship's structure block movement, and players receive feedback on invalid moves.

### Interaction

Interact with the environment using the DTEK-V board:

- **Tile Interaction**: Engage with the current tile to collect resources, discover items, manage the ship
- **Inventory Management**: Manage items using board switches to transfer, consume, or craft resources.

### Resource Management

Manage vital resources to survive:

- **Oxygen, Water, and Food**: Vital resources that deplete over time and must be replenished to survive.
  - **Food**: Found in the cafeteria.
  - **Water**: Can be synthesized in the laboratory.
  - **Oxygen**: Automatically replenished inside the spacecraft but depletes outdoors.
- **Seven-Segment Display**: Tracks your current resource levels in real time.

### Tile Types and Effects

Each tile offers unique interactions and challenges:

- **Pond**: Discover life forms and collect tardigrades.
- **Cave**: Unearth alien bones.
- **Wasteland**: Search for critical items like medkits, maps, and spare parts.
- **Storage**: Store and retrieve items.
- **Cockpit**: Attempt to end the game and return to Earth.
- **Laboratory**: Create water or fuel.
- **Airlock**: Step outside the spacecraft.
- **Landing Site**: Discover tools to repair the ship.

### Challenges

- **Obstacles**: Mountains and rocket walls block movement, while hazards like sharp rocks can deplete oxygen.
- **Survival Stats**: Balance exploration and resource gathering with maintaining vital resources.

## Winning the Game

To win, players must:

1. **Gather essential resources.**
2. **Fuel the spacecraft in the engine bay.**
3. **Navigate to the cockpit and attempt to return to Earth.**

Success depends on gathered resources and the ship's condition.

## Losing the Game

The game ends if any vital resource (**oxygen**, **water**, or **food**) depletes completely.

## Controls

### Movement

- **East**: Toggle switch 1
- **West**: Toggle switch 2
- **South**: Toggle switch 3
- **North**: Toggle switch 4

### Interactions

- **Interact**: Toggle switch 9
- **Back**: Toggle switch 8
- **Drink**: Toggle switch 6
- **Eat**: Toggle switch 7

### Inventory Slots

- **Slot 1**: Toggle switch 1
- **Slot 2**: Toggle switch 2
- **Slot 3**: Toggle switch 3
- **Slot 4**: Toggle switch 4
- **Slot 5**: Toggle switch 5
- **Slot 5**: Toggle switch 6

## Getting Started

1. **Compile** the game code using the RISC-V toolchain.
2. **Send to dtek** the binary onto the DTEK-V board (riscv-run).
4. **Use the switches** to navigate, interact, and manage resources.
5. **TRY NOT TO DIE! :)**

## Conclusion

The **Mars Survival Game** is a challenging and immersive experience that combines resource management, strategic decision-making, and engaging interactions with the DTEK-V board’s hardware. Will you conquer the Martian wilderness and return to Earth, or will the Red Planet claim another victim? The choice is yours!
