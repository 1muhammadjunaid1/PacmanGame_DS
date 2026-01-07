#pragma once
#include "Ghost.h"
#include "DataStructures.h"
#include "Maze.h"

class PinkGhost : public Ghost
{
public:
	PinkGhost();
	void update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position) override;
    

    
    void update(unsigned char i_level, Maze& i_maze, Ghost& i_ghost_0, Pacman& i_pacman);
};
