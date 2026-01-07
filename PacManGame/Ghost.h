#pragma once

#include <cmath>
#include <SFML/Graphics.hpp>
#include "Constants.h"
#include "DataStructures.h"
#include "Maze.h"
#include "Pacman.h" 

// Abstract Base Class
class Ghost
{
protected:
	bool movement_mode;
	bool use_door;
	unsigned char direction;
	unsigned char frightened_mode;
	unsigned char frightened_speed_timer;
	unsigned char id;
	unsigned short animation_timer;

	Position home;
	Position home_exit;
	Position position;
	Position target;

public:
	Ghost(unsigned char i_id);
    virtual ~Ghost() {} // Virtual destructor

	// Pure Virtual Function - Polymorphism
	virtual void update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position) = 0;

	bool pacman_collision(const Position& i_pacman_position);
	float get_target_distance(unsigned char i_direction);

	void draw(bool i_flash, sf::RenderWindow& i_window);
	void reset(const Position& i_home, const Position& i_home_exit);
	void set_position(short i_x, short i_y);
	void switch_mode();
	void update(unsigned char i_level, Maze& i_maze, Ghost& i_ghost_0, Pacman& i_pacman);

	Position get_position();
    unsigned char get_id() const { return id; }
};
