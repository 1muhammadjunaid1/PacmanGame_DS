#include "Game.h"
#include "DrawText.h"
#include <iostream>
#include <fstream>

std::string Game::getLevelFilename(unsigned char levelNum)
{
    std::string filename = "Levels/Level" + std::to_string(levelNum) + ".txt";

    // Check if file exists
    std::ifstream file(filename);
    if (file.good())
    {
        file.close();
        return filename;
    }

    // Fallback to Level1.txt if file doesn't exist
    file.close();
    return "Levels/Level.txt";
}

Game::Game() : game_won(0),
               level(0),
               lag(0),
               score(0),
               level_start_score(0),
               current_state(GameState_MainMenu),
               lives(3),
               is_death_paused(false)
{
    window.create(sf::VideoMode(CELL_SIZE * MAP_WIDTH * SCREEN_RESIZE, (FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT) * SCREEN_RESIZE), "Pac-Man", sf::Style::Close | sf::Style::Resize);
    window.setView(sf::View(sf::FloatRect(0, 0, CELL_SIZE * MAP_WIDTH, FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT)));

    // Load Title Screen
    title_texture.loadFromFile("Images/Title.png");
    title_sprite.setTexture(title_texture);

    // Load Life Icon
    life_texture.loadFromFile("Images/Pacman.png");
    life_sprite.setTexture(life_texture);
    life_sprite.setTextureRect(sf::IntRect(0, 0, CELL_SIZE, CELL_SIZE));

    // Load Game Over Screen
    if (game_over_texture.loadFromFile("Images/GameOver.png"))
    {
        game_over_sprite.setTexture(game_over_texture);
        // Set origin to center for easier positioning
        game_over_sprite.setOrigin(game_over_texture.getSize().x / 2.0f, game_over_texture.getSize().y / 2.0f);
        // Position at center of screen
        game_over_sprite.setPosition(CELL_SIZE * MAP_WIDTH / 2.0f, CELL_SIZE * MAP_HEIGHT / 2.0f);
    }

    srand(static_cast<unsigned>(time(0)));

    // Load Level and positions - starts at level 0, so we load level 1
    maze.loadLevel(getLevelFilename(level + 1), ghost_positions, pacman);

    ghost_manager.reset(level, ghost_positions);

    previous_time = std::chrono::steady_clock::now();
}

void Game::run()
{
    while (window.isOpen())
    {
        unsigned delta_time = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::steady_clock::now() - previous_time).count();
        lag += delta_time;
        previous_time += std::chrono::microseconds(delta_time);

        while (FRAME_DURATION <= lag)
        {
            lag -= FRAME_DURATION;

            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                {
                    window.close();
                }
                else if (event.type == sf::Event::Resized)
                {
                    adjustView();
                }
                else if (event.type == sf::Event::KeyPressed)
                {
                    // State Management Input
                    if (current_state == GameState_MainMenu)
                    {
                        if (event.key.code == sf::Keyboard::Enter)
                        {
                            current_state = GameState_Playing;
                            // Intro logic removed
                        }
                    }
                    else if (current_state == GameState_Playing)
                    {
                        if (event.key.code == sf::Keyboard::P)
                        {
                            current_state = GameState_Paused;
                        }
                    }
                    else if (current_state == GameState_Paused)
                    {
                        if (event.key.code == sf::Keyboard::P)
                        {
                            current_state = GameState_Playing;
                        }
                    }
                    else if (current_state == GameState_GameOver)
                    {
                        if (event.key.code == sf::Keyboard::R)
                        {
                            // Try Again - Restart current level
                            game_won = 0;
                            lives = 3;
                            // Reset score to what it was at start of level
                            score = level_start_score;

                            maze.loadLevel(getLevelFilename(level + 1), ghost_positions, pacman);
                            ghost_manager.reset(level, ghost_positions);
                            pacman.reset();
                            current_state = GameState_Playing;
                        }
                        else if (event.key.code == sf::Keyboard::N)
                        {
                            // Start New - Go to Level 1
                            game_won = 0;
                            level = 0;
                            score = 0;
                            level_start_score = 0;
                            lives = 3;
                            maze.loadLevel(getLevelFilename(level + 1), ghost_positions, pacman);
                            ghost_manager.reset(level, ghost_positions);
                            pacman.reset();
                            current_state = GameState_Playing;
                        }
                    }
                }
            }

            if (current_state == GameState_Playing)
            {
                // Check for death pause timeout
                if (is_death_paused)
                {
                    auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
                                       std::chrono::steady_clock::now() - death_time)
                                       .count();

                    if (elapsed >= 3)
                    {
                        is_death_paused = false;

                        if (lives > 0)
                        {
                            // Still have lives, reset positions
                            pacman.reset();
                            ghost_manager.reset(level, ghost_positions);
                        }
                        else
                        {
                            // No lives left, go to Game Over
                            // Submit score to high scores
                            high_scores.addScore(score);
                            current_state = GameState_GameOver;
                        }
                    }
                }

                // Only update game logic if Playing and not in death pause
                if (current_state == GameState_Playing && !is_death_paused && 0 == game_won && 0 == pacman.get_dead())
                {
                    game_won = 1;

                    pacman.update(level, maze);

                    // Get points earned from Pacman's update
                    unsigned char points = pacman.get_points_earned();
                    if (points >= 10) // 10 for pellet, 50 for energizer
                    {
                        score += points;
                    }

                    ghost_manager.update(level, maze, pacman);

                    // Win Condition Check
                    if (score - level_start_score >= 1000)
                    {
                        game_won = 1; // Trigger level transition
                    }
                    else
                    {
                        game_won = 0; // Carry on playing
                    }
                    if (1 == game_won)
                    {
                        pacman.set_animation_timer(0);
                    }
                }
                // Handle death detection
                else if (current_state == GameState_Playing && !is_death_paused && 1 == pacman.get_dead())
                {
                    if (lives > 0)
                        lives--;
                    is_death_paused = true;
                    death_time = std::chrono::steady_clock::now();

                    // siren stop logic removed
                }
                else if (!is_death_paused && sf::Keyboard::isKeyPressed(sf::Keyboard::Enter))
                {
                    if (1 == game_won)
                    {
                        game_won = 0;
                        level++;
                        lives = 3;
                        // Save score at start of new level
                        level_start_score = score;

                        maze.loadLevel(getLevelFilename(level + 1), ghost_positions, pacman);
                        ghost_manager.reset(level, ghost_positions);
                        pacman.reset();
                    }
                }
            }

            if (FRAME_DURATION > lag)
            {
                window.clear();

                if (current_state == GameState_MainMenu)
                {
                    window.draw(title_sprite);
                }
                else
                {
                    // Draw Game (Playing, Paused, Death Pause, or GAME OVER now too!)
                    // We keep maze visible in background of Game Over or Pause
                    if ((current_state == GameState_Playing || current_state == GameState_Paused || current_state == GameState_GameOver || is_death_paused) && (0 == game_won || is_death_paused || current_state == GameState_GameOver || current_state == GameState_Paused))
                    {
                        maze.draw(window, level);
                        ghost_manager.draw(GHOST_FLASH_START >= pacman.get_energizer_timer(), window);
                        draw_text(0, 0, CELL_SIZE * MAP_HEIGHT, "Level: " + std::to_string(1 + level), window);

                        // Display score on bottom right
                        std::string scoreText = "Score: " + std::to_string(score);
                        short scoreX = CELL_SIZE * MAP_WIDTH - 8 * scoreText.length(); // 8 pixels per character
                        draw_text(0, scoreX, CELL_SIZE * MAP_HEIGHT, scoreText, window);

                        // Draw life icons in center
                        // Don't draw negative lives or weirdness
                        if (lives > 0 && lives <= 3)
                        {
                            short livesStartX = (CELL_SIZE * MAP_WIDTH / 2) - (lives * CELL_SIZE / 2);
                            for (unsigned char i = 0; i < lives; i++)
                            {
                                life_sprite.setPosition(livesStartX + (i * CELL_SIZE), CELL_SIZE * MAP_HEIGHT);
                                window.draw(life_sprite);
                            }
                        }
                    }

                    pacman.draw(game_won, window);

                    if (1 == pacman.get_animation_over())
                    {
                        if (1 == game_won)
                        {
                            draw_text(1, 0, 0, "Next level!", window);
                        }
                        // We strictly use the image below now
                    }

                    if (current_state == GameState_Paused)
                    {
                        draw_text(1, 0, 0, "PAUSED", window);
                    }
                    else if (current_state == GameState_GameOver)
                    {
                        // Draw Game Over Image instead of Text
                        window.draw(game_over_sprite);
                    }
                }

                window.display();
            }
        }
    }
}

void Game::adjustView()
{
    // Original game dimensions
    float gameWidth = CELL_SIZE * MAP_WIDTH;
    float gameHeight = FONT_HEIGHT + CELL_SIZE * MAP_HEIGHT;
    float gameAspectRatio = gameWidth / gameHeight;

    // Current window dimensions
    sf::Vector2u windowSize = window.getSize();
    float windowAspectRatio = static_cast<float>(windowSize.x) / windowSize.y;

    sf::View view(sf::FloatRect(0, 0, gameWidth, gameHeight));

    // Calculate viewport with letterboxing
    if (windowAspectRatio > gameAspectRatio)
    {
        // Window is wider than game - add black bars on left/right
        float viewportWidth = gameAspectRatio / windowAspectRatio;
        view.setViewport(sf::FloatRect((1.0f - viewportWidth) / 2.0f, 0, viewportWidth, 1));
    }
    else
    {
        // Window is taller than game - add black bars on top/bottom
        float viewportHeight = windowAspectRatio / gameAspectRatio;
        view.setViewport(sf::FloatRect(0, (1.0f - viewportHeight) / 2.0f, 1, viewportHeight));
    }

    window.setView(view);
}
