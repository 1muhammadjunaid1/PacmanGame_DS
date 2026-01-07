#include <cmath>
#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "DrawText.h"

void draw_text(bool i_center, unsigned short i_x, unsigned short i_y, const std::string& i_text, sf::RenderWindow& i_window)
{
	short character_x = i_x;
	short character_y = i_y;

	unsigned char character_width;

	sf::Sprite sprite;

	sf::Texture texture;
	texture.loadFromFile("Images/Font.png");

	sprite.setTexture(texture);

	if (1 == i_center)
	{
		//I calculated these values in the Paint.NET.
		//So trust me, it works.
		character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - 8 * i_text.length())));
		character_y = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_HEIGHT - FONT_HEIGHT)));
	}

	for (std::string::size_type a = 0; a < i_text.length(); a++)
	{
		if ('\n' == i_text[a])
		{
			if (1 == i_center)
			{
				character_x = static_cast<short>(round(0.5f * (CELL_SIZE * MAP_WIDTH - 8 * i_text.substr(1 + a).length())));
			}
			else
			{
				character_x = i_x;
			}

			character_y += FONT_HEIGHT;

			continue;
		}

		character_width = 8;

		sprite.setPosition(character_x, character_y);

		//The font texture starts with a space.
		//And the ascii code for space is 32.
		//So we subtract 32 to get the index of the character in the texture.
		sprite.setTextureRect(sf::IntRect(8 * (i_text[a] - 32), 0, character_width, FONT_HEIGHT));

		character_x += character_width;

		i_window.draw(sprite);
	}
}