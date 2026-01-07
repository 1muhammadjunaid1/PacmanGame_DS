#pragma once
#include "Ghost.h"

class CyanGhost : public Ghost
{
public:
	CyanGhost();
	void update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position) override;
};
