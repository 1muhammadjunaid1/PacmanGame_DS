#include "PinkGhost.h"
#include <iostream>

PinkGhost::PinkGhost() : Ghost(1)
{
}

void PinkGhost::update_target(unsigned char i_pacman_direction, const Position &i_ghost_0_position, const Position &i_pacman_position)
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
        // Base compatibility (Classic Ambush Logic)
        // ----------------------------------------------------
        // Pinky (Pink Ghost) tries to ambush Pacman by targeting 4 tiles *ahead* of him.
        // This encourages him to cut Pacman off rather than following directly behind (like Red).
        target = i_pacman_position;

        short tx = target.getX();
        short ty = target.getY();

        // Project target 4 tiles ahead in Pacman's current direction
        switch (i_pacman_direction)
        {
        case 0: // Right
            tx += CELL_SIZE * GHOST_1_CHASE;
            break;
        case 1: // Up
            ty -= CELL_SIZE * GHOST_1_CHASE;
            break;
        case 2: // Left
            tx -= CELL_SIZE * GHOST_1_CHASE;
            break;
        case 3: // Down
            ty += CELL_SIZE * GHOST_1_CHASE;
        }

        target.setX(tx);
        target.setY(ty);
    }
}

void PinkGhost::update(unsigned char i_level, Maze &i_maze, Ghost &i_ghost_0, Pacman &i_pacman)
{
    Ghost::update(i_level, i_maze, i_ghost_0, i_pacman);
}
