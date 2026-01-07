#include "OrangeGhost.h"
#include <cmath>

OrangeGhost::OrangeGhost() : Ghost(3)
{
}

void OrangeGhost::update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position)
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
            target = Position(0, CELL_SIZE * (MAP_HEIGHT - 1));
        }
        else
        {
            // Clyde (Orange Ghost) Target Logic: "Feigned Ignorance"
            // ----------------------------------------------------
            // Clyde behaves like the Red Ghost (chasing Pacman directly) ONLY when he is far away.
            // If he gets too close (within 8 tiles), he "scatters" back to his home corner (bottom-left).
            // This makes him seem to approach and then shy away.

            // Calculate distance to Pacman
            short px = position.getX();
            short py = position.getY();
            short pacx = i_pacman_position.getX();
            short pacy = i_pacman_position.getY();
            
            float distance = static_cast<float>(sqrt(pow(px - pacx, 2) + pow(py - pacy, 2)));
            
            if (distance >= CELL_SIZE * GHOST_3_CHASE)
            {
                // Far away: Chase Pacman
                target = i_pacman_position;
            }
            else
            {
                // Too close: Retreat to scatter target
                target = Position(0, CELL_SIZE * (MAP_HEIGHT - 1));
            }
        }
    }
}
