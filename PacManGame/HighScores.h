#pragma once

#include "DataStructures.h"
#include <fstream>
#include <string>

// HighScores Class
// Manages the top 3 highest scores using MyLinkedList.
// Provides file I/O for persistent storage.
class HighScores
{
private:
    MyLinkedList<unsigned int> scores; // Sorted linked list (highest to lowest)
    const unsigned char MAX_SCORES = 3;
    std::string filename;

public:
    HighScores(const std::string &file = "HighScores.txt") : filename(file)
    {
        loadFromFile();
    }

    // Adds a score to the list if it qualifies for top 3
    // Returns true if the score was added, false otherwise
    bool addScore(unsigned int score)
    {
        // Don't add zero scores
        if (score == 0)
            return false;

        // If list is not full, or score beats the lowest (3rd place)
        if (scores.size() < MAX_SCORES || (scores.size() == MAX_SCORES && score > scores.getAt(MAX_SCORES - 1)))
        {
            // Insert in sorted order (descending)
            scores.insert_sorted_descending(score);

            // If we now have more than MAX_SCORES, remove the last one
            if (scores.size() > MAX_SCORES)
            {
                scores.removeAt(MAX_SCORES);
            }

            saveToFile();
            return true;
        }

        return false;
    }

    // Checks if a score would qualify for top 3 (without adding it)
    bool qualifiesForHighScore(unsigned int score) const
    {
        if (score == 0)
            return false;

        // List has less than 3 scores
        if (scores.size() < MAX_SCORES)
            return true;

        // Check if score beats the lowest (3rd) score
        return (score > scores.getAt(MAX_SCORES - 1));
    }

    // Loads scores from file
    void loadFromFile()
    {
        scores.clear();

        std::ifstream file(filename);
        if (!file.is_open())
        {
            // File doesn't exist yet, that's okay
            return;
        }

        unsigned int score;
        while (file >> score)
        {
            scores.push_back(score); // Load in order from file
        }

        file.close();
    }

    // Saves scores to file
    void saveToFile() const
    {
        std::ofstream file(filename);
        if (!file.is_open())
        {
            return; // Failed to open file
        }

        // Iterate through the linked list and write each score
        Node<unsigned int> *current = scores.getHead();
        while (current != nullptr)
        {
            file << current->getData() << "\n";
            current = current->getNext();
        }

        file.close();
    }

    // Gets a pointer to the head for iteration/display
    Node<unsigned int> *getHead() const
    {
        return scores.getHead();
    }

    // Gets score at a specific position (0 = 1st place, 1 = 2nd, 2 = 3rd)
    // Returns 0 if position doesn't exist
    unsigned int getScoreAt(unsigned char position) const
    {
        if (position >= MAX_SCORES || position >= scores.size())
            return 0;

        return scores.getAt(position);
    }

    // Get the number of scores currently stored
    size_t getCount() const
    {
        return scores.size();
    }
};
