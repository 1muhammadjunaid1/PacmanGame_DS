#include <cmath>
#include <algorithm> // for std::max
#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "Pacman.h"
#include "Maze.h"

// No MapCollision.hpp needed as it's part of Maze now

Pacman::Pacman() :
	animation_over(0),
	dead(0),
	direction(0),
	energizer_timer(0),
	animation_timer(0),
	points_earned(0),
    position(0, 0)
{
}

bool Pacman::get_animation_over() const
{
	return animation_over;
}

bool Pacman::get_dead() const
{
	return dead;
}

unsigned char Pacman::get_direction() const
{
	return direction;
}

unsigned short Pacman::get_energizer_timer() const
{
	return energizer_timer;
}

void Pacman::draw(bool i_victory, sf::RenderWindow& i_window)
{
	unsigned char frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(PACMAN_ANIMATION_SPEED)));

	sf::Sprite sprite;

	sf::Texture texture;

	sprite.setPosition(static_cast<float>(position.getX()), static_cast<float>(position.getY()));

	if (1 == dead || 1 == i_victory)
	{
		if (animation_timer < PACMAN_DEATH_FRAMES * PACMAN_ANIMATION_SPEED)
		{
			animation_timer++;

			texture.loadFromFile("Images/PacmanDeath.png");

			sprite.setTexture(texture);
			sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, 0, CELL_SIZE, CELL_SIZE));

			i_window.draw(sprite);
		}
		else
		{
			//You can only die once.
			animation_over = 1;
		}
	}
	else
	{
		texture.loadFromFile("Images/Pacman.png");

		sprite.setTexture(texture);
		sprite.setTextureRect(sf::IntRect(CELL_SIZE * frame, CELL_SIZE * direction, CELL_SIZE, CELL_SIZE));

		i_window.draw(sprite);

		animation_timer = (1 + animation_timer) % (PACMAN_ANIMATION_FRAMES * PACMAN_ANIMATION_SPEED);
	}
}

void Pacman::reset()
{
	animation_over = 0;
	dead = 0;

	direction = 0;

	animation_timer = 0;
	energizer_timer = 0;
}

void Pacman::set_animation_timer(unsigned short i_animation_timer)
{
	animation_timer = i_animation_timer;
}

void Pacman::set_dead(bool i_dead)
{
	dead = i_dead;

	if (1 == dead)
	{
		//Making sure that the animation starts from the beginning.
		animation_timer = 0;
	}
}

void Pacman::set_position(short i_x, short i_y)
{
	position.setX(i_x);
    position.setY(i_y);
}

void Pacman::setPosition(short x, short y)
{
    set_position(x, y);
}

void Pacman::update(unsigned char i_level, Maze& i_maze)
{
	points_earned = 0; // Reset at start of each update
	bool walls[4] = {0, 0, 0, 0}; // Raw array instead of std::array
    
	walls[0] = i_maze.checkCollision(0, 0, PACMAN_SPEED + position.getX(), position.getY());
	walls[1] = i_maze.checkCollision(0, 0, position.getX(), position.getY() - PACMAN_SPEED);
	walls[2] = i_maze.checkCollision(0, 0, position.getX() - PACMAN_SPEED, position.getY());
	walls[3] = i_maze.checkCollision(0, 0, position.getX(), PACMAN_SPEED + position.getY());

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		if (0 == walls[0]) //You can't turn in this direction if there's a wall there.
		{
			direction = 0;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		if (0 == walls[1])
		{
			direction = 1;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		if (0 == walls[2])
		{
			direction = 2;
		}
	}

	if (1 == sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		if (0 == walls[3])
		{
			direction = 3;
		}
	}

	if (0 == walls[direction])
	{
		switch (direction)
		{
			case 0:
			{
				position.setX(position.getX() + PACMAN_SPEED);
				break;
			}
			case 1:
			{
				position.setY(position.getY() - PACMAN_SPEED);
				break;
			}
			case 2:
			{
				position.setX(position.getX() - PACMAN_SPEED);
				break;
			}
			case 3:
			{
				position.setY(position.getY() + PACMAN_SPEED);
			}
		}
	}

    // Warp Tunnels (Horizontal - Left/Right)
	if (-CELL_SIZE >= position.getX())
	{
		position.setX(CELL_SIZE * MAP_WIDTH - PACMAN_SPEED);
	}
	else if (CELL_SIZE * MAP_WIDTH <= position.getX())
	{
		position.setX(PACMAN_SPEED - CELL_SIZE);
	}
	// Warp Tunnels (Vertical - Top/Bottom) 
    if (-CELL_SIZE >= position.getY())
    {
        position.setY(CELL_SIZE * MAP_HEIGHT - PACMAN_SPEED);
    }
    else if (CELL_SIZE * MAP_HEIGHT <= position.getY())
    {
        position.setY(PACMAN_SPEED - CELL_SIZE);
    }
	// Check for pellet collection and store points
	points_earned = i_maze.checkCollision(1, 0, position.getX(), position.getY());
	
	if (points_earned >= 10) // If something was collected (pellet or energizer)
	{
		if (points_earned == 50) // Energizer
		{
			//He becomes energized!
			energizer_timer = ENERGIZER_DURATION - (i_level * 50);
		}
	}
	else
	{
		energizer_timer = std::max(0, energizer_timer - 1);
	}
}

Position Pacman::get_position() const
{
	return position;
}

unsigned char Pacman::get_points_earned() const
{
	return points_earned;
}