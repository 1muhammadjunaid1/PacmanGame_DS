#pragma once

#include <chrono>
#include <SFML/Graphics.hpp>

#include "Constants.h"
#include "Pacman.h"
#include "GhostManager.h"
#include "Maze.h"
#include "HighScores.h"

class Game
{
private:
    bool game_won;
    unsigned char level;
    unsigned int lag;
    unsigned int score;
    unsigned int level_start_score;
    std::chrono::time_point<std::chrono::steady_clock> previous_time;
    sf::RenderWindow window;

    Maze maze;
    Pacman pacman;
    GhostManager ghost_manager;

    MyVector<Position> ghost_positions; // To store initial positions from Maze

    // High Scores System
    HighScores high_scores;

    // Game State Management
    enum GameState
    {
        GameState_MainMenu,
        GameState_Playing,
        GameState_Paused,
        GameState_GameOver
    };
    GameState current_state;

    // Title Screen Resources
    sf::Texture title_texture;
    sf::Sprite title_sprite;

    // --- Audio System ---
 

    // Lives System
    unsigned char lives;
    bool is_death_paused;
    std::chrono::time_point<std::chrono::steady_clock> death_time;

    // Life Icon Resources
    sf::Texture life_texture;
    sf::Sprite life_sprite;
    
    // Game Over Screen Resources
    sf::Texture game_over_texture;
    sf::Sprite game_over_sprite;

    void adjustView();                                    // Handle window resize with aspect ratio preservation
    std::string getLevelFilename(unsigned char levelNum); // Get level filename with fallback

public:
    Game();
    void run();
};
