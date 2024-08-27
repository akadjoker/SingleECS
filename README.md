
# High-Performance ECS  using Raylib

## Overview

This project is a high-performance 2D/3D game engine built using [Raylib](https://www.raylib.com/) and designed with a single component system. It is optimized to handle a large number of sprites efficiently and is intended to be easily integrated with scripting languages.

### Key Features

- **Entity-Component System (ECS)**: Efficient management of game entities and components.
- **Single Component Design**: Simplified ECS design for performance testing and easy integration.
- **High Performance**: Capable of rendering up to 125,000 sprites at a constant 60 FPS without layers and 90,000 sprites with layers enabled.
- **Raylib Integration**: Utilizes Raylib for rendering, providing a simple and powerful interface for graphics.
- **Collision Detection**: Includes basic collision detection functions for polygons, circles, and AABBs.
- **Timer Management**: Handles time-based events and animations with precision.
- **Asset Management**: Efficiently loads, manages, and renders textures and graphical assets.

## Performance

In performance tests, the engine has demonstrated the ability to maintain a steady 60 FPS under heavy load:

- **Without Layers**: 125,000 sprites
- **With Layers**: 90,000 sprites

These results were achieved on a system with the following specifications:

- **CPU**: AMD ryzen 5
- **GPU**: radeon 920
- **RAM**: 20

Please note that performance may vary depending on the hardware and specific configurations used.

## Getting Started

### Prerequisites

To compile and run the engine, you will need:

- [Raylib](https://www.raylib.com/) (v4.0 or later)
- A C++ compiler (e.g., GCC, Clang, MSVC)
- CMake (optional, for cross-platform build)

### Installation

1. **Clone the repository**:
    \`\`\`bash
    git clone https://github.com/akadjoker/SingleECS
    cd SingleECS
    \`\`\`

2. **Build the project**:
    \`\`\`bash
    mkdir build
    cd build
    cmake ..
    make
    \`\`\`

3. **Run the executable**:
    \`\`\`bash
    ./your_executable_name
    \`\`\`

### Usage

- **Entity-Component System**: Use the `ECS` class to create and manage entities. Each entity can be associated with a `Component`, which defines its properties and behavior.
- **Rendering**: Use the `Render` struct to draw textures, polygons, and other shapes to the screen.
- **Collision Detection**: Utilize the `Collider` struct for detecting collisions between entities.
- **Timing**: The `Timer` class allows you to manage time-dependent events, ensuring smooth animations and game logic.

### Example

\`\`\`cpp
#include "Core.hpp"

int main() {

    InitWindow(800, 600, "High-Performance Game Engine");

    ECS ecs;


    // Create entities and components here
    Entity entity = ecs.create();
    Component* component = ecs.get(entity.id);
    component->set_box(50, 50);

    // Main game loop
    while (!WindowShouldClose()) {
        ecs.update(GetFrameTime());
        BeginDrawing();
        ClearBackground(RAYWHITE);
        ecs.render();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
\`\`\`
