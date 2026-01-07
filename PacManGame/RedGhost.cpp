#include "RedGhost.h"
#include <iostream>

RedGhost::RedGhost() : Ghost(0)
{
    parent = new Position*[MAP_WIDTH];
    visited = new bool*[MAP_WIDTH];
    for(int i=0; i<MAP_WIDTH; i++) {
        parent[i] = new Position[MAP_HEIGHT];
        visited[i] = new bool[MAP_HEIGHT];
    }
}

RedGhost::~RedGhost()
{
    for(int i=0; i<MAP_WIDTH; i++) {
        delete[] parent[i];
        delete[] visited[i];
    }
    delete[] parent;
    delete[] visited;
}

void RedGhost::update_target(unsigned char i_pacman_direction, const Position& i_ghost_0_position, const Position& i_pacman_position)
{
    if (use_door)
    {
        if (position == target)
        {
            if (home_exit == target)
            {
                use_door = 0;
            }
            else if (home == target)
            {
                frightened_mode = 0;
                target = home_exit;
            }
        }
    }
    else
    {
        // Base compatibility: standard target update
        target = i_pacman_position;
    }
}

// Special override for RedGhost to use BFS
void RedGhost::update(unsigned char i_level, Maze& i_maze, Ghost& i_ghost_0, Pacman& i_pacman)
{
    // First, let the base class handle all the timers, frightened mode, etc.
    // If we are Frightened or Scatted, we fallback to standard behavior to avoid breaking game rules.
    if (frightened_mode > 0 || movement_mode == 0) // frightened or scatter
    {
        Ghost::update(i_level, i_maze, i_ghost_0, i_pacman);
        return;
    }
    
    // Chase Mode: Breadth-First Search (BFS)
    // ----------------------------------------------------
    // The Red Ghost uses BFS to find the shortest path to Pacman.
    // BFS is ideal here because the maze is an unweighted grid (every step costs 1).
    // It guarantees the shortest path in terms of number of tile steps.
    
    // 1. Calculate the intended target (Pacman's exact position)
    update_target(i_pacman.get_direction(), i_ghost_0.get_position(), i_pacman.get_position());
    
    // 2. Perform BFS to determine the optimal next move.
    // We only recalculate the path when the ghost is centered on a tile (not between tiles).
    // This prevents jittery movement and ensures grid-aligned decision making.
    if (position.getX() % CELL_SIZE == 0 && position.getY() % CELL_SIZE == 0)
    {
        Position nextStep = calculate_bfs_next_target(position, target, i_maze);
        
        // The BFS function returns the immediate next tile coordinate we should move to.
        // We set this 'one-step-ahead' position as our temporary target.
        // The base class's Greedy algorithm will then naturally steer us towards this immediate neighbor.
        target = nextStep;
    }
    
    // 3. Call base update to execute the move
    Ghost::update(i_level, i_maze, i_ghost_0, i_pacman);
}

Position RedGhost::calculate_bfs_next_target(const Position& start, const Position& end, Maze& maze)
{
    // ==========================================
    // Breadth-First Search (BFS) Algorithm
    // ==========================================
    // Goal: Find the shortest path from 'start' to 'end' in the maze.
    // Data Structures Used:
    // 1. Queue (MyQueue): To explore nodes level-by-level (FIFO).
    // 2. Visited Array (bool**): To keep track of visited nodes and avoid cycles.
    // 3. Parent Array (Position**): To reconstruct the path backwards from destination to source.

    // Step 1: Convert pixel positions to grid coordinates (Indices)
    int startX = static_cast<int>(round(start.getX() / (double)CELL_SIZE));
    int startY = static_cast<int>(round(start.getY() / (double)CELL_SIZE));
    int targetX = static_cast<int>(round(end.getX() / (double)CELL_SIZE));
    int targetY = static_cast<int>(round(end.getY() / (double)CELL_SIZE));
    
    // Bounds check to prevent out-of-range errors
    if (startX < 0 || startX >= MAP_WIDTH || startY < 0 || startY >= MAP_HEIGHT) return start;
    
    // Initialize the Queue for BFS
    MyQueue<Position> queue;
    
    // Step 2: Reset the 'visited' tracking array for a fresh search
    for(int i=0; i<MAP_WIDTH; i++) {
        for(int j=0; j<MAP_HEIGHT; j++) {
            visited[i][j] = false;
        }
    }
    
    // Start BFS from the current position
    queue.enqueue(Position(startX, startY));
    visited[startX][startY] = true;
    
    bool found = false;
    
    // Step 3: Main BFS Loop
    // Continue until the queue is empty or we reached the target
    while(!queue.isEmpty())
    {
        Position current = queue.front();
        queue.dequeue();
        
        int cx = current.getX();
        int cy = current.getY();
        
        // Check if we reached the target
        if (cx == targetX && cy == targetY)
        {
            found = true;
            break;
        }
        
        // Define neighbor offsets (Up, Down, Left, Right)
        int dirs[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };
        
        // Explore all 4 adjacent neighbors
        for(int k=0; k<4; k++)
        {
            int nx = cx + dirs[k][0];
            int ny = cy + dirs[k][1];
            
            // Check boundaries
            if (nx >= 0 && nx < MAP_WIDTH && ny >= 0 && ny < MAP_HEIGHT)
            {
                // Check if the neighbor is valid:
                // 1. Not visited yet
                // 2. Not a wall
                if (!visited[nx][ny] && maze.getCell(nx, ny) != Cell::Wall)
                {
                    visited[nx][ny] = true;         // Mark as visited
                    parent[nx][ny] = current;       // Store 'current' as the parent of 'neighbor'
                    queue.enqueue(Position(nx, ny)); // Add neighbor to queue for later exploration
                }
            }
        }
    }
    
    // Step 4: Path Reconstruction (Backtracking)
    // We need the *first* step after the start position. 
    // We trace back from 'end' to 'start' using the parent pointers.
    Position nextMove = start; // Default stays put if no path found
    
    if (found)
    {
        Position curr(targetX, targetY);
        // Backtrack until we find the node that is a direct child of start
        while(curr.getX() != startX || curr.getY() != startY)
        {
            nextMove = curr; // 'nextMove' updates to the node we are currently at (closer to start)
            Position p = parent[curr.getX()][curr.getY()];
            
            // Safety check against infinite loops (though BFS shouldn't have them)
            if (p.getX() == curr.getX() && p.getY() == curr.getY()) break; 
            
            curr = p; // Move one step back towards start
        }
        
        // Convert the grid coordinate back to pixel position for the game engine
        nextMove.setX(nextMove.getX() * CELL_SIZE);
        nextMove.setY(nextMove.getY() * CELL_SIZE);
    }
    else
    {
        // If path blocked (target inside wall?), just point towards target to avoid freezing
        nextMove = end; 
    }
    
    return nextMove;
}
