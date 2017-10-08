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
    GameState flag(int x, int y, int * bombsRemaining);

private:
    const size_t width, height;
    char * table;
    int bombCount;
    int realFlagged;

    void fillTable();
    char getNeighbourCount(int y, int x) const;
    void printPrompt(Mode mode, int bombCount) const;
    bool isHidden(size_t idx) const;
    bool isFlagged(size_t idx) const;
    bool isValidFieldValue(char field) const;
    void show(int x, int y);
    void hideTable() const;
    GameState checkTableForWin() const;
};

#endif //MINESWEEPER_MINESWEEPER_H
