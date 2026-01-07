#include <fstream>
#include <cmath>
#include <iostream>
#include "Maze.h"
#include "Pacman.h"

Maze::Maze() {
    map = new Cell*[MAP_WIDTH];
    for (int i = 0; i < MAP_WIDTH; ++i) {
        map[i] = new Cell[MAP_HEIGHT];
        for (int j = 0; j < MAP_HEIGHT; ++j) {
            map[i][j] = Cell::Empty;
        }
    }
}

Maze::~Maze() {
    for (int i = 0; i < MAP_WIDTH; ++i) {
        delete[] map[i];
    }
    delete[] map;
}

void Maze::loadLevel(const std::string& filename, MyVector<Position>& ghostPositions, Pacman& pacman) {
    std::ifstream file(filename);
    
    // Ensure ghost positions are ready
    if (ghostPositions.size() < 4) {
        for (int i = ghostPositions.size(); i < 4; i++) {
            ghostPositions.push_back(Position(0,0));
        }
    }

    if (!file.is_open()) {
        std::cerr << "Failed to open " << filename << std::endl;
        return;
    }

    char tile;
    int x = 0;
    int y = 0;

    // We assume the file is formatted correctly (rows and cols)
    while (file.get(tile)) {
        // Skip newline but increment y, reset x
        if (tile == '\n') {
            y++;
            x = 0;
            continue;
        }
        
        // Safety check
        if (x >= MAP_WIDTH || y >= MAP_HEIGHT) {
            continue; 
        }

        switch (tile) {
            case '0': map[x][y] = Cell::Wall; break;
            case '1': map[x][y] = Cell::Pellet; break;
            case '2': map[x][y] = Cell::Empty; break;
            case '3': map[x][y] = Cell::Energizer; break;
            case '4': map[x][y] = Cell::Door; break;
            
            case '5': 
                pacman.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                map[x][y] = Cell::Empty;
                break;

            case '6': // Red Ghost
                ghostPositions[0] = Position(x * CELL_SIZE, y * CELL_SIZE);
                map[x][y] = Cell::Empty;
                break;
            case '7': // Pink Ghost
                ghostPositions[1] = Position(x * CELL_SIZE, y * CELL_SIZE);
                map[x][y] = Cell::Empty;
                break;
            case '8': // Cyan Ghost
                ghostPositions[2] = Position(x * CELL_SIZE, y * CELL_SIZE);
                map[x][y] = Cell::Empty;
                break;
            case '9': // Orange Ghost
                ghostPositions[3] = Position(x * CELL_SIZE, y * CELL_SIZE);
                map[x][y] = Cell::Empty;
                break;
                
            default: // Handle spaces or other chars if any
                map[x][y] = Cell::Empty; 
                break;
        }
        
        // Move to next cell
        if (tile != ' ' && tile != '\n' && tile != '\r')
             x++;
    }
    file.close();
}

unsigned char Maze::checkCollision(bool collectPellets, bool useDoor, short x, short y) {
    unsigned char score = 0;

	//Getting the exact position.
	float cell_x = x / static_cast<float>(CELL_SIZE);
	float cell_y = y / static_cast<float>(CELL_SIZE);

	//A ghost/Pacman can intersect 4 cells at most.
	for (unsigned char a = 0; a < 4; a++)
	{
		short check_x = 0;
		short check_y = 0;

		switch (a)
		{
			case 0: //Top left cell
			{
				check_x = static_cast<short>(floor(cell_x));
				check_y = static_cast<short>(floor(cell_y));

				break;
			}
			case 1: //Top right cell
			{
				check_x = static_cast<short>(ceil(cell_x));
				check_y = static_cast<short>(floor(cell_y));

				break;
			}
			case 2: //Bottom left cell
			{
				check_x = static_cast<short>(floor(cell_x));
				check_y = static_cast<short>(ceil(cell_y));

				break;
			}
			case 3: //Bottom right cell
			{
				check_x = static_cast<short>(ceil(cell_x));
				check_y = static_cast<short>(ceil(cell_y));
			}
		}

		//Making sure that the position is inside the map.
		if (0 <= check_x && 0 <= check_y && MAP_HEIGHT > check_y && MAP_WIDTH > check_x)
		{
			if (0 == collectPellets) //Here we only care about the walls.
			{
				if (Cell::Wall == map[check_x][check_y])
				{
					score = 1; // Still return 1 for wall collision (non-zero means collision)
				}
				else if (0 == useDoor && Cell::Door == map[check_x][check_y])
				{
					score = 1; // Still return 1 for door collision
				}
			}
			else //Here we only care about the collectables.
			{
				if (Cell::Energizer == map[check_x][check_y])
				{
					score = 50; // Power pellet worth 50 points

					map[check_x][check_y] = Cell::Empty;
				}
				else if (Cell::Pellet == map[check_x][check_y])
				{
					score = 10; // Regular pellet worth 10 points
					map[check_x][check_y] = Cell::Empty;
				}
			}
		}
	}

	return score;
}

void Maze::draw(sf::RenderWindow& window, unsigned char level) {
	// Static timer for blinking effect (persists between frames)
	static int blink_timer = 0;
	blink_timer = (blink_timer + 1) % 40; // Cycle loop 0-39
	bool visible = blink_timer < 20;      // Visible for first half of cycle

	// Define color themes for levels 2+ (Level 1 keeps original colors)
	sf::Color levelColors[] = {
		sf::Color(255, 100, 100),  // Level 2: Soft Red
		sf::Color(100, 255, 150),  // Level 3: Mint Green
		sf::Color(255, 150, 255),  // Level 4: Pink
		sf::Color(150, 255, 255),  // Level 5: Cyan
		sf::Color(255, 200, 100),  // Level 6: Orange
		sf::Color(200, 100, 255),  // Level 7: Purple
		sf::Color(255, 255, 100)   // Level 8: Yellow
	};
	
	// Level 1 keeps original appearance 
	// Levels 2+ cycle through the color array
	sf::Color wallColor = (level == 0) ? sf::Color::White : levelColors[(level - 1) % 7];

    sf::Sprite sprite;

	sf::Texture texture;
	texture.loadFromFile("Images/Map.png");

	sprite.setTexture(texture);

	for (unsigned char a = 0; a < MAP_WIDTH; a++)
	{
		for (unsigned char b = 0; b < MAP_HEIGHT; b++)
		{
			sprite.setPosition(static_cast<float>(CELL_SIZE * a), static_cast<float>(CELL_SIZE * b));

			//We just crop out what we need from the texture.
			switch (map[a][b])
			{
				case Cell::Door:
				{
					sprite.setTextureRect(sf::IntRect(2 * CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));

					window.draw(sprite);

					break;
				}
				case Cell::Energizer:
				{
					// Only draw if within the visible phase of the blink cycle
					if (visible)
					{
						sprite.setTextureRect(sf::IntRect(CELL_SIZE, CELL_SIZE, CELL_SIZE, CELL_SIZE));
						window.draw(sprite);
					}

					break;
				}
				case Cell::Pellet:
				{
					sprite.setTextureRect(sf::IntRect(0, CELL_SIZE, CELL_SIZE, CELL_SIZE));

					window.draw(sprite);

					break;
				}
				case Cell::Wall:
				{
					bool down = 0;
					bool left = 0;
					bool right = 0;
					bool up = 0;

					if (b < MAP_HEIGHT - 1)
					{
						if (Cell::Wall == map[a][1 + b])
						{
							down = 1;
						}
					}

					//Since we have warp tunnels, we need to draw them as if they're connected.
					if (0 < a)
					{
						if (Cell::Wall == map[a - 1][b])
						{
							left = 1;
						}
					}
					else
					{
						left = 1;
					}

					if (a < MAP_WIDTH - 1)
					{
						if (Cell::Wall == map[1 + a][b])
						{
							right = 1;
						}
					}
					else
					{
						right = 1;
					}

					if (0 < b)
					{
						if (Cell::Wall == map[a][b - 1])
						{
							up = 1;
						}
					}


					//--------------------------------------------<         DISTRIBUTIVE PROPERTY!         >----------------------------
					sprite.setTextureRect(sf::IntRect(CELL_SIZE * (down + 2 * (left + 2 * (right + 2 * up))), 0, CELL_SIZE, CELL_SIZE));

					// Apply level-based color theme to walls
					sprite.setColor(wallColor);
					window.draw(sprite);
				}
			}
		}
	}
}
