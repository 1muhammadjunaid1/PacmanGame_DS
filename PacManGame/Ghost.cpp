#include "Ghost.h"
#include <iostream>

Ghost::Ghost(unsigned char i_id) : id(i_id),
								   movement_mode(0),
								   use_door(0),
								   direction(0),
								   frightened_mode(0),
								   frightened_speed_timer(0),
								   animation_timer(0),
								   home(0, 0),
								   home_exit(0, 0),
								   position(0, 0),
								   target(0, 0)
{
}

bool Ghost::pacman_collision(const Position &i_pacman_position)
{
	if (position.getX() > i_pacman_position.getX() - CELL_SIZE && position.getX() < CELL_SIZE + i_pacman_position.getX())
	{
		if (position.getY() > i_pacman_position.getY() - CELL_SIZE && position.getY() < CELL_SIZE + i_pacman_position.getY())
		{
			return 1;
		}
	}

	return 0;
}

float Ghost::get_target_distance(unsigned char i_direction)
{
	short x = position.getX();
	short y = position.getY();

	switch (i_direction)
	{
	case 0:
	{
		x += GHOST_SPEED;
		break;
	}
	case 1:
	{
		y -= GHOST_SPEED;
		break;
	}
	case 2:
	{
		x -= GHOST_SPEED;
		break;
	}
	case 3:
	{
		y += GHOST_SPEED;
	}
	}

	return static_cast<float>(sqrt(pow(x - target.getX(), 2) + pow(y - target.getY(), 2)));
}

void Ghost::draw(bool i_flash, sf::RenderWindow &i_window)
{
	unsigned char body_frame = static_cast<unsigned char>(floor(animation_timer / static_cast<float>(GHOST_ANIMATION_SPEED)));

	sf::Sprite body;
	sf::Sprite face;

	sf::Texture texture;
	texture.loadFromFile("Images/Ghost.png");

	body.setTexture(texture);
	body.setPosition(static_cast<float>(position.getX()), static_cast<float>(position.getY()));
	body.setTextureRect(sf::IntRect(CELL_SIZE * body_frame, 0, CELL_SIZE, CELL_SIZE));

	face.setTexture(texture);
	face.setPosition(static_cast<float>(position.getX()), static_cast<float>(position.getY()));

	if (0 == frightened_mode)
	{
		switch (id)
		{
		case 0:
		{
			body.setColor(sf::Color(255, 0, 0));
			break;
		}
		case 1:
		{
			body.setColor(sf::Color(255, 182, 255));
			break;
		}
		case 2:
		{
			body.setColor(sf::Color(0, 255, 255));
			break;
		}
		case 3:
		{
			body.setColor(sf::Color(255, 182, 85));
		}
		}

		face.setTextureRect(sf::IntRect(CELL_SIZE * direction, CELL_SIZE, CELL_SIZE, CELL_SIZE));
		i_window.draw(body);
	}
	else if (1 == frightened_mode)
	{
		body.setColor(sf::Color(36, 36, 255));
		face.setTextureRect(sf::IntRect(4 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

		if (1 == i_flash && 0 == body_frame % 2)
		{
			body.setColor(sf::Color(255, 255, 255));
			face.setColor(sf::Color(255, 0, 0));
		}
		else
		{
			body.setColor(sf::Color(36, 36, 255));
			face.setColor(sf::Color(255, 255, 255));
		}

		i_window.draw(body);
	}
	else
	{
		face.setTextureRect(sf::IntRect(CELL_SIZE * direction, 2 * CELL_SIZE, CELL_SIZE, CELL_SIZE));
	}

	i_window.draw(face);

	animation_timer = (1 + animation_timer) % (GHOST_ANIMATION_FRAMES * GHOST_ANIMATION_SPEED);
}

void Ghost::reset(const Position &i_home, const Position &i_home_exit)
{
	movement_mode = 0;
	use_door = 0 < id;

	direction = 0;
	frightened_mode = 0;
	frightened_speed_timer = 0;

	animation_timer = 0;

	home = i_home;
	home_exit = i_home_exit;
	target = i_home_exit;
}

void Ghost::set_position(short i_x, short i_y)
{
	position.setX(i_x);
	position.setY(i_y);
}

void Ghost::switch_mode()
{
	movement_mode = 1 - movement_mode;
}

void Ghost::update(unsigned char i_level, Maze &i_maze, Ghost &i_ghost_0, Pacman &i_pacman)
{
	bool move = 0;
	unsigned char available_ways = 0;
	unsigned char speed = GHOST_SPEED;

	bool walls[4] = {0, 0, 0, 0};

	if (0 == frightened_mode && i_pacman.get_energizer_timer() == ENERGIZER_DURATION - (i_level * 50))
	{
		frightened_speed_timer = GHOST_FRIGHTENED_SPEED;
		frightened_mode = 1;
	}
	else if (0 == i_pacman.get_energizer_timer() && 1 == frightened_mode)
	{
		frightened_mode = 0;
	}

	if (2 == frightened_mode && 0 == position.getX() % GHOST_ESCAPE_SPEED && 0 == position.getY() % GHOST_ESCAPE_SPEED)
	{
		speed = GHOST_ESCAPE_SPEED;
	}

	update_target(i_pacman.get_direction(), i_ghost_0.get_position(), i_pacman.get_position());

	walls[0] = i_maze.checkCollision(0, use_door, speed + position.getX(), position.getY());
	walls[1] = i_maze.checkCollision(0, use_door, position.getX(), position.getY() - speed);
	walls[2] = i_maze.checkCollision(0, use_door, position.getX() - speed, position.getY());
	walls[3] = i_maze.checkCollision(0, use_door, position.getX(), speed + position.getY());

	// greedy-Best-First Search (Greedy BFS) Strategy
	// ==========================================
	// If the ghost is not frightened, it uses a Greedy algorithm to move.
	// 1. Identify all valid moves at the current intersection (not walls, not reversing direction).
	// 2. For each valid move, calculate the straight-line (Euclidean) distance to the 'target'.
	// 3. Choose the direction that minimizes this distance.
	// This is "Greedy" because it makes the locally optimal choice at each step,
	// ignoring walls or future obstacles that might block the path later.
	if (1 != frightened_mode)
	{
		unsigned char optimal_direction = 4; // 4 signifies 'no valid direction found yet'
		move = 1;

		for (unsigned char a = 0; a < 4; a++)
		{
			// Prevent reversing: Don't go back the way we came (unless dead end)
			if (a == (2 + direction) % 4)
			{
				continue;
			}
			// Check if the direction is free (no wall)
			else if (0 == walls[a])
			{
				// If this is the first valid direction found, pick it tentatively
				if (4 == optimal_direction)
				{
					optimal_direction = a;
				}

				available_ways++;

				// Greedy Choice: Compare distances
				// If direction 'a' brings us closer to target than current 'optimal_direction', pick 'a'.
				if (get_target_distance(a) < get_target_distance(optimal_direction))
				{
					optimal_direction = a;
				}
			}
		}

		// Update direction if we found a valid move
		if (1 < available_ways)
		{
			direction = optimal_direction;
		}
		else
		{
			// Dead end or corner: Forced move
			if (4 == optimal_direction)
			{
				// If no forward moves, must reverse (U-turn)
				direction = (2 + direction) % 4;
			}
			else
			{
				direction = optimal_direction;
			}
		}
	}
	else
	{
		unsigned char random_direction = rand() % 4;

		if (0 == frightened_speed_timer)
		{
			move = 1;
			frightened_speed_timer = GHOST_FRIGHTENED_SPEED;

			// Identify all valid directions (not walls)
			MyVector<unsigned char> valid_moves;
			for (unsigned char a = 0; a < 4; a++)
			{
				if (0 == walls[a])
				{
					valid_moves.push_back(a);
				}
			}

			// Filter out reverse direction if we have multiple choices (prevent U-turns if possible)
			if (valid_moves.size() > 1)
			{
				MyVector<unsigned char> forward_moves;
				for (size_t i = 0; i < valid_moves.size(); i++)
				{
					if (valid_moves[i] != (2 + direction) % 4)
					{
						forward_moves.push_back(valid_moves[i]);
					}
				}

				// Pick random from safe forward moves
				if (forward_moves.size() > 0)
				{
					direction = forward_moves[rand() % forward_moves.size()];
				}
				else
				{
					// Should technically not reach here if logic holds, but fallback:
					direction = valid_moves[rand() % valid_moves.size()];
				}
			}
			else if (valid_moves.size() == 1)
			{
				// Dead end or strict corridor (only 1 way to go)
				direction = valid_moves[0];
			}
			// else: Stuck in wall? (0 valid moves), keep current direction or don't change
		}
		else
		{
			frightened_speed_timer--;
		}
	}

	if (1 == move)
	{
		switch (direction)
		{
		case 0:
		{
			position.setX(position.getX() + speed);
			break;
		}
		case 1:
		{
			position.setY(position.getY() - speed);
			break;
		}
		case 2:
		{
			position.setX(position.getX() - speed);
			break;
		}
		case 3:
		{
			position.setY(position.getY() + speed);
		}
		}

		// Warp Tunnels (Horizontal - Left/Right)
		if (-CELL_SIZE >= position.getX())
		{
			position.setX(CELL_SIZE * MAP_WIDTH - speed);
		}
		else if (position.getX() >= CELL_SIZE * MAP_WIDTH)
		{
			position.setX(speed - CELL_SIZE);
		}
		// Warp Tunnels (Vertical - Top/Bottom) <--- ADD THIS BLOCK
		if (-CELL_SIZE >= position.getY())
		{
			position.setY(CELL_SIZE * MAP_HEIGHT - speed);
		}
		else if (position.getY() >= CELL_SIZE * MAP_HEIGHT)
		{
			position.setY(speed - CELL_SIZE);
		}
	}

	if (1 == pacman_collision(i_pacman.get_position()))
	{
		if (0 == frightened_mode)
		{
			i_pacman.set_dead(1);
		}
		else
		{
			use_door = 1;
			frightened_mode = 2;
			target = home;
		}
	}
}

Position Ghost::get_position()
{
	return position;
}
