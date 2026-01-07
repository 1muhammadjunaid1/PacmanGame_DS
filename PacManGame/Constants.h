#pragma once

// ==========================================
// Game Constants
// ==========================================
constexpr unsigned char CELL_SIZE = 16;
constexpr unsigned char FONT_HEIGHT = 16;
constexpr unsigned char GHOST_1_CHASE = 2;
constexpr unsigned char GHOST_2_CHASE = 1;
constexpr unsigned char GHOST_3_CHASE = 4;
constexpr unsigned char GHOST_ANIMATION_FRAMES = 6;
constexpr unsigned char GHOST_ANIMATION_SPEED = 4;
constexpr unsigned char GHOST_ESCAPE_SPEED = 4;
constexpr unsigned char GHOST_FRIGHTENED_SPEED = 3;
constexpr unsigned char GHOST_SPEED = 1;

constexpr unsigned char MAP_HEIGHT = 21;
constexpr unsigned char MAP_WIDTH = 21;

constexpr unsigned char PACMAN_ANIMATION_FRAMES = 6;
constexpr unsigned char PACMAN_ANIMATION_SPEED = 4;
constexpr unsigned char PACMAN_DEATH_FRAMES = 12;
constexpr unsigned char PACMAN_SPEED = 2;
constexpr unsigned char SCREEN_RESIZE = 2;

constexpr unsigned short CHASE_DURATION = 1024;
constexpr unsigned short ENERGIZER_DURATION = 512;
constexpr unsigned short FRAME_DURATION = 16667;
constexpr unsigned short GHOST_FLASH_START = 64;
constexpr unsigned short LONG_SCATTER_DURATION = 512;
constexpr unsigned short SHORT_SCATTER_DURATION = 256;

// ==========================================
// Enums
// ==========================================
enum Cell
{
	Door,
	Empty,
	Energizer,
	Pellet,
	Wall
};

// ==========================================
// Position Class (Encapsulated)
// ==========================================
class Position
{
private:
	short x;
	short y;

public:
	// Constructor
	Position(short i_x = 0, short i_y = 0) : x(i_x), y(i_y) {}

	// Getters
	short getX() const { return x; }
	short getY() const { return y; }

	// Setters
	void setX(short i_x) { x = i_x; }
	void setY(short i_y) { y = i_y; }

	// Operator Overloading
	bool operator==(const Position& other) const
	{
		return this->x == other.x && this->y == other.y;
	}
    
    bool operator!=(const Position& other) const
    {
        return !(*this == other);
    }
};
