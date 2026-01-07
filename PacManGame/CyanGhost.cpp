#include "CyanGhost.h"

CyanGhost::CyanGhost() : Ghost(2)
{
}

void CyanGhost::update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position)
{
    if (use_door)
	{
		if (position == target)
		{
			if (home_exit == target)
			{
				use_door = 0;
			}
			else if (home == target)
			{
				frightened_mode = 0;
				target = home_exit;
			}
		}
	}
	else
    {
        if (0 == movement_mode)
        {
            target = Position(CELL_SIZE * (MAP_WIDTH - 1), CELL_SIZE * (MAP_HEIGHT - 1));
        }
        else
        {
            // Inky (Cyan Ghost) Target Logic:
            // ----------------------------------------------------
            // Inky uses a complex targeting scheme involving both Pacman and the Red Ghost (Blinky).
            // 1. Find the point 2 tiles ahead of Pacman.
            // 2. Draw a vector from Red Ghost's position to this point.
            // 3. Double the length of this vector to find the final target.
            // This makes Inky's movement hard to predict as it depends on Red Ghost's location.

            target = i_pacman_position;
            
            short tx = target.getX();
            short ty = target.getY();

            // Step 1: Offset to 2 cells in front of Pacman
            switch (i_pacman_direction)
            {
                case 0: tx += CELL_SIZE * GHOST_2_CHASE; break;
                case 1: ty -= CELL_SIZE * GHOST_2_CHASE; break;
                case 2: tx -= CELL_SIZE * GHOST_2_CHASE; break;
                case 3: ty += CELL_SIZE * GHOST_2_CHASE;
            }

            // Step 2 & 3: Vector calculation using Red Ghost (ghost_0) position
            // Vector V = (PointAhead - RedGhostPos)
            // Target = PointAhead + V
            short g0x = i_ghost_0_position.getX();
            short g0y = i_ghost_0_position.getY();
            
            tx += (tx - g0x);
            ty += (ty - g0y);
            
            target.setX(tx);
            target.setY(ty);
        }
    }
}
