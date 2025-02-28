# 🔦 C++ Raycasting Engine

A simple but powerful raycasting engine for the Windows console, inspired by classic FPS games like Wolfenstein 3D.

## 🖼️ Preview

https://github.com/user-attachments/assets/91ef15a9-6258-465b-aec7-84b0c932c858

## 📜 About
This project is a console-based 3D raycasting engine that simulates first-person movement and rendering using ASCII characters. It dynamically casts rays into a 16×16 grid world, detects walls, and creates a pseudo-3D environment.

### 💡 Key Features:
- ✅ Raycasting Algorithm - Simulates 3D in a 2D grid-based world.
- ✅ Smooth Player Movement & Rotation - Move with WASD, turn with A/D.
- ✅ Dynamic Wall Shading - Walls fade based on distance.
- ✅ Frame-Rate Independent - Smooth gameplay using chrono.
- ✅ Custom Map Design - Easily modify the world layout.

## 🕹️ Controls
| Key | Action |
|-----|--------|
| W   | Move forward |
| S   | Move backward |
| A   | Rotate left |
| D   | Rotate right |

## 🚀 Getting Started

### 📥 Installation
1. Clone this repository

2. Compile the program (Using g++)

3. Run the executable
(Windows users can compile using MinGW or Visual Studio.)

## 🔍 How It Works
This raycasting engine traces rays from the player's position to detect walls and render them using ASCII shading.

### 🛠️ Core Components
- Raycasting Algorithm - Casts rays at angles based on the player's FOV.
- 2D Tile Map - Defines the world layout (# for walls, . for open space).
- Frame Rendering - Uses a console screen buffer to display ASCII graphics.
- Player Physics - Moves using trigonometric functions (sinf & cosf).

## 🎮 Customize Your World!
Want to change the map?
Edit this section in main.cpp:

wstring map;
map += L"################";
map += L"#..............#";
map += L"#..............#";
map += L"#..........#...#";
map += L"#..........#...#";
map += L"#......#########";
map += L"#..............#";
map += L"################";

Replace # with walls and . with open space to create custom levels! 🏗️

## 💡 Future Improvements
- 🚀 Add sprite rendering (for enemies & objects).
- 🔊 Implement sound effects.
- 🌍 Generate random maps.

## 👾 Credits & Inspiration
🎮 Inspired by Wolfenstein 3D and OneLoneCoder's Raycasting Tutorial.

🚀 Built with C++, Windows API, and trigonometry magic!
