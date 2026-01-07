#pragma once

#include <string>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "DataStructures.h"

// Forward declaration to avoid circular dependency
class Pacman;

class Maze {
private:
    Cell** map;

public:
    Maze();
    ~Maze();

    // Loads level from text file
    void loadLevel(const std::string& filename, MyVector<Position>& ghostPositions, Pacman& pacman);

    // Checks collision and returns score points earned (0, 10, or 50)
    unsigned char checkCollision(bool collectPellets, bool useDoor, short x, short y);

    // Renders the maze with level-based color theme
    void draw(sf::RenderWindow& window, unsigned char level);

    // Get raw map for advanced usage if needed
    Cell** getMap() const { return map; }
    
    // For modifying map (eating pellets)
    void setCell(int x, int y, Cell value) {
        if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
            map[x][y] = value;
        }
    }
    
    Cell getCell(int x, int y) const {
         if (x >= 0 && x < MAP_WIDTH && y >= 0 && y < MAP_HEIGHT) {
            return map[x][y];
        }
        return Cell::Empty; // fallback
    }
};
