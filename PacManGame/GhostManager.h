#pragma once

#include "Constants.h"
#include "DataStructures.h"
#include "Ghost.h"
#include "Maze.h"
#include "Pacman.h"

class GhostManager
{
private:
	unsigned char current_wave;
	unsigned short wave_timer;

	MyVector<Ghost*> ghosts;

public:
	GhostManager();
	~GhostManager(); // Destructor to clean up pointers

	void draw(bool i_flash, sf::RenderWindow& i_window);
	void reset(unsigned char i_level, const MyVector<Position>& i_ghost_positions);
	void update(unsigned char i_level, Maze& i_maze, Pacman& i_pacman);
};
