#include "GhostManager.h"
#include "RedGhost.h"
#include "PinkGhost.h"
#include "CyanGhost.h"
#include "OrangeGhost.h"
#include <cmath>

GhostManager::GhostManager() :
	current_wave(0),
	wave_timer(LONG_SCATTER_DURATION)
{
    // Initialize polymorphic ghosts
    ghosts.push_back(new RedGhost());
    ghosts.push_back(new PinkGhost());
    ghosts.push_back(new CyanGhost());
    ghosts.push_back(new OrangeGhost());
}

GhostManager::~GhostManager()
{
    for (size_t i = 0; i < ghosts.size(); i++)
    {
        delete ghosts[i];
    }
    ghosts.clear();
}

void GhostManager::draw(bool i_flash, sf::RenderWindow& i_window)
{
	for (size_t i = 0; i < ghosts.size(); i++)
	{
		ghosts[i]->draw(i_flash, i_window);
	}
}

void GhostManager::reset(unsigned char i_level, const MyVector<Position>& i_ghost_positions)
{
	current_wave = 0;

	//Difficulty locked to Level 1 behavior
	wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, 0));

	for (size_t i = 0; i < ghosts.size(); i++)
	{
        // Safety check index
        if (i < i_ghost_positions.size()) {
		    ghosts[i]->set_position(i_ghost_positions[i].getX(), i_ghost_positions[i].getY());
        }
	}

	for (size_t i = 0; i < ghosts.size(); i++)
	{
		//We use the blue ghost (index 2) to get the location of the house 
        //and the red ghost (index 0) to get the location of the exit.
		ghosts[i]->reset(ghosts[2]->get_position(), ghosts[0]->get_position());
	}
}

void GhostManager::update(unsigned char i_level, Maze& i_maze, Pacman& i_pacman)
{
	if (0 == i_pacman.get_energizer_timer()) //We won't update the wave timer when Pacman is energized.
	{
		if (0 == wave_timer)
		{
			if (7 > current_wave)
			{
				current_wave++;

				for (size_t i = 0; i < ghosts.size(); i++)
				{
					ghosts[i]->switch_mode();
				}
			}

			//I took the rules from the website.
			if (1 == current_wave % 2)
			{
				wave_timer = CHASE_DURATION;
			}
			else if (2 == current_wave)
			{
				// Keep Level 1 timing
				wave_timer = static_cast<unsigned short>(LONG_SCATTER_DURATION / pow(2, 0));
			}
			else
			{
				// Keep Level 1 timing
				wave_timer = static_cast<unsigned short>(SHORT_SCATTER_DURATION / pow(2, 0));
			}
		}
		else
		{
			wave_timer--;
		}
	}

	for (size_t i = 0; i < ghosts.size(); i++)
	{
        // ghosts[0] is passed as reference to Red Ghost
		ghosts[i]->update(i_level, i_maze, *ghosts[0], i_pacman);
	}
}