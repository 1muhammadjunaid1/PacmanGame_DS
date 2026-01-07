# Pac-Man Game C++ Project Details
We created a pacman game using C++ SFML and Data Structures.

# Group Members
Muhammad Junaid - 241443
Mehshan Zafar   - 241399

A custom implementation of the classic arcade game built from scratch in C++. This project focuses on efficient pathfinding algorithms and custom-built data structures to handle game logic without relying on the standard C++ library (STL).

---

## 🕹️ Project Summary
This project is a fully functional Pac-Man game. It includes a complete game loop, user input handling, collision detection, and a rendering system. To demonstrate a deep understanding of computer science fundamentals, I built the internal logic using my own custom versions of common data tools.

## 👻 Ghost AI (How They Think)
Each ghost has a unique "personality" governed by different algorithms:

* **Red Ghost (Blinky) - The Aggressive Tracker**
    * **Logic:** Uses **Breadth-First Search (BFS)** with a custom Queue.
    * **Behavior:** It always finds the absolute shortest path to Pac-Man. This forces the player to stay alert and keep moving.
* **Pink Ghost (Pinky) - The Unpredictable Wanderer**
    * **Logic:** Uses **Depth-First Search (DFS)** with a custom Stack.
    * **Behavior:** It explores paths as deeply as possible before backtracking. This creates winding, unexpected movement patterns.
* **Cyan Ghost (Inky) - The Flanker**
    * **Logic:** **Cooperative Vector Strategy**.
    * **Behavior:** It calculates its target based on both Pac-Man's position and the Red Ghost's location. This often results in Inky "sandwiching" the player.
* **Orange Ghost (Clyde) - The Shy Chaser**
    * **Logic:** **Proximity-Based Switching**.
    * **Behavior:** It chases Pac-Man when far away but "scatters" back to its home corner if it gets too close (within 8 tiles).

## 🛠️ Custom Data Structures
Instead of using built-in C++ containers, I created `DataStructures.h` from scratch to manage memory and performance:

* **MyVector:** A dynamic array that grows automatically to store lists of items.
* **MyQueue:** A circular queue used for the BFS pathfinding logic.
* **MyStack:** A LIFO (Last-In-First-Out) structure used for DFS movement and state management.
* **MyLinkedList:** A flexible chain of data used to manage and display the high score list.

## 🎮 Game Systems
* **Grid Movement:** The map is divided into a 16x16 grid to ensure smooth turning and perfect wall collisions.
* **Frightened Mode:** When Pac-Man eats a Power Pellet, ghosts turn blue and move away from the player.
* **Wrap-Around Portals:** Logic that allows players and ghosts to teleport from one side of the screen to the other.
* **High Score System:** Keeps track of player achievements across different game sessions.

## 🎨 Graphics & Assets
* **Sprite Sheets:** The game uses a single image file for all characters and walls to save memory. The engine crops specific rectangles to show animations.
* **Font Rendering:** Uses a custom system (`DrawText.cpp`) to draw text like "Score" and "Game Over" using a custom image-based font.
* **Map Parsing:** Levels are loaded from data arrays that define where walls, pellets, and power-ups are placed.

## 🚀 How to Run
1. Clone this repository.
2. Open the project in your preferred C++ IDE (e.g., Visual Studio, CLion).
3. Ensure your graphics library (e.g., SFML or SDL) is correctly linked.
4. Build and run the project.
