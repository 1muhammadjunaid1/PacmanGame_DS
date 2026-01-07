#pragma once
#include "Ghost.h"
#include "DataStructures.h"
#include "Maze.h"

class RedGhost : public Ghost
{
private:
    // BFS Memory
    Position **parent;
    bool **visited;

public:
    RedGhost();
    ~RedGhost(); // Destructor for cleanup
    void update_target(unsigned char i_pacman_direction, const Position &i_ghost_0_position, const Position &i_pacman_position) override;

    // BFS Helper
    // We override the update function to inject BFS movement logic directly
    // instead of just setting a target and letting the base class decide movement.
    // However, the base class update() handles collision and movement speed.
    // The base class uses 'direction' and 'target' to pick the best move.
    // To implement TRUE BFS, we should probably intercept the direction choice.
    // But since the base class 'update' logic is fixed "Greedy towards Target",
    // we can use BFS to find the *immediate next cell* on the shortest path,
    // and set that cell's center as the "Target".
    // This trick forces the Greedy base logic to pick exactly the BFS path.
    Position calculate_bfs_next_target(const Position &start, const Position &end, Maze &maze);

    // We need to override update because we might need access to the Maze for BFS
    void update(unsigned char i_level, Maze &i_maze, Ghost &i_ghost_0, Pacman &i_pacman);
};
