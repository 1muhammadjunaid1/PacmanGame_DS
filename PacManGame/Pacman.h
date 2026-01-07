#pragma once

#include "Constants.h"
#include <SFML/Graphics.hpp>

class Pacman
{
private:
	bool animation_over;
	bool dead;
	unsigned char direction;
	unsigned short animation_timer;
	unsigned short energizer_timer;
	unsigned char points_earned; // Points earned in the last update
	Position position;

public:
	Pacman();

	bool get_animation_over() const;
	bool get_dead() const;
	unsigned char get_direction() const;
	unsigned short get_energizer_timer() const;

	void draw(bool i_victory, sf::RenderWindow& i_window);
	void reset();
	void set_animation_timer(unsigned short i_animation_timer);
	void set_dead(bool i_dead);
	void set_position(short i_x, short i_y);
    void setPosition(short x, short y); // Helper for consistency with my plan
    
    // Takes Maze reference to check collisions
	void update(unsigned char i_level, class Maze& i_maze); 

	Position get_position() const;
	unsigned char get_points_earned() const; // Get points earned in last update
};
