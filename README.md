# Pac-Man Game C++ Project 
---

##  Project Summary
This project is a fully functional Pac-Man game using using **C++ SFML and Data Structures**. It includes a complete game loop, user input handling, collision detection, and a rendering system. To demonstrate a deep understanding of computer science fundamentals, I built the internal logic using my own custom versions of common data tools.

##  Custom Data Structures
Instead of using built-in C++ containers, I created `DataStructures.h` from scratch to manage memory and performance:

* **MyVector:** A dynamic array that grows automatically to store lists of items.
* **MyQueue:** A circular queue used for the BFS pathfinding logic.
* **MyStack:** A LIFO (Last-In-First-Out) structure used for DFS movement and state management.
* **MyLinkedList:** A flexible chain of data used to manage and display the high score list.

##  Graphics & Assets
* **Sprite Sheets:** The game uses a single image file for all characters and walls to save memory. The engine crops specific rectangles to show animations.
* **Font Rendering:** Uses a custom system (`DrawText.cpp`) to draw text like "Score" and "Game Over" using a custom image-based font.
* **Map Parsing:** Levels are loaded from data arrays that define where walls, pellets, and power-ups are placed.

##  How to Run
1. Clone this repository.
2. Open the project in your preferred C++ IDE (e.g., Visual Studio, CLion).
3. Ensure your graphics library (e.g., SFML or SDL) is correctly linked.
4. Build and run the project.
