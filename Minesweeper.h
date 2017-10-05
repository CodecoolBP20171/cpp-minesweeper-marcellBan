#ifndef MINESWEEPER_MINESWEEPER_H
#define MINESWEEPER_MINESWEEPER_H

#include <iostream>
#include <random>
#include <sstream>

namespace {
    const std::string REVEAL_MODE_PROMPT_PREFIX = "(reveal mode)";
    const std::string FLAG_MODE_PROMPT_PREFIX = "(flag mode)";
    const char FLAG_OFFSET = 30;
    const char HIDDEN_OFFSET = FLAG_OFFSET * 2;
    enum Mode {
        REVEAL, FLAG
    };
    enum GameState {
        PLAYING, WON, BANG
    };
}

class Minesweeper {
public:
    Minesweeper(size_t width, size_t height);
    virtual ~Minesweeper();
    void countNeighbours();
    void printTable() const;
    void playGame();
    GameState reveal(int x, int y);
    void flag(int x, int y);

private:
    const size_t width, height;
    char * table;

    void fillTable();
    char getNeighbourCount(int y, int x) const;
    void printPrompt(const Mode& mode) const;
    bool isHidden(int x, int y) const;
    bool isFlagged(int x, int y) const;
    bool isValidFieldValue(char field) const;
    void show(int x, int y);
    void hideTable() const;
};

#endif //MINESWEEPER_MINESWEEPER_H
