#include "Minesweeper.h"

namespace {
    enum Color {
        FG_ONE = 27,
        FG_TWO = 28,
        FG_THREE = 196,
        FG_FOUR = 19,
        FG_FIVE = 88,
        FG_SIX = 31,
        FG_SEVEN = 16,
        FG_EIGHT = 241,
        FG_FLAG = 201,
        BG_FLAG = 245,
        BG_HIDDEN = 240,
        BG_FAIL = 160,
        FG_FAIL = 0,
        BG_BASIC = 251
    };

    void resetConsole() {
        std::cout << "\x1b[0m";
        std::cout.flush();
    }

    void clearConsole() {
        std::cout << "\x1b[2J\33[0;0H";
        std::cout.flush();
    }

    void setConsoleColor(Color c, bool bg = false) {
        std::cout << "\x1b[" << (bg ? "48" : "38") << ";5;" << c << "m";
        std::cout.flush();
    }

    void setConsoleColorForNumber(char c) {
        switch (c) {
            case '0':
            case '1':
                setConsoleColor(FG_ONE);
                break;
            case '2':
                setConsoleColor(FG_TWO);
                break;
            case '3':
                setConsoleColor(FG_THREE);
                break;
            case '4':
                setConsoleColor(FG_FOUR);
                break;
            case '5':
                setConsoleColor(FG_FIVE);
                break;
            case '6':
                setConsoleColor(FG_SIX);
                break;
            case '7':
                setConsoleColor(FG_SEVEN);
                break;
            case '8':
                setConsoleColor(FG_EIGHT);
                break;
            default:
                throw "This should never happen";
        }
    }
}

Minesweeper::Minesweeper(const size_t width, const size_t height)
        : width(width), height(height),
          table(new char[width * height]),
          realFlagged(0) {
    fillTable();
}

Minesweeper::~Minesweeper() {
    delete[] table;
}

void Minesweeper::countNeighbours() {
    for (auto i = 0; i < height; ++i) {
        for (auto j = 0; j < width; ++j) {
            auto index = i * width + j;
            if (table[index] == '.') {
                table[index] = getNeighbourCount(i, j);
            }
        }
    }
}

void Minesweeper::printTable() const {
    resetConsole();
    std::cout << "   ";
    for (auto i = 0; i < width; ++i) {
        std::cout << i % 10 << " ";
    }
    std::cout << std::endl;
    std::cout << "---";
    for (auto i = 0; i < width; ++i) {
        std::cout << "--";
    }
    std::cout << std::endl;
    for (auto i = 0; i < height; ++i) {
        resetConsole();
        std::cout << i % 10 << "| ";
        for (auto j = 0; j < width; ++j) {
            resetConsole();
            auto index = i * width + j;
            if (isFlagged(index)) {
                setConsoleColor(BG_FLAG, true);
                setConsoleColor(FG_FLAG);
                std::cout << "P";
            } else if (isHidden(index)) {
                setConsoleColor(BG_HIDDEN, true);
                std::cout << " ";
            } else {
                if (table[index] == '*') {
                    setConsoleColor(BG_FAIL, true);
                    setConsoleColor(FG_FAIL);
                    std::cout << table[index];
                } else {
                    setConsoleColor(BG_BASIC, true);
                    setConsoleColorForNumber(table[index]);
                    if (table[index] == '0') std::cout << " ";
                    else std::cout << table[index];
                }
            }
            std::cout << " ";
        }
        resetConsole();
        std::cout << std::endl;
    }
    resetConsole();
}

void Minesweeper::fillTable() {
    std::random_device rng;
    bombCount = 0;
    for (auto i = 0; i < width * height; ++i) {
        table[i] = rng() % 100 < 15 ? '*' : '.';
        if (table[i] == '*') bombCount++;
    }
}

char Minesweeper::getNeighbourCount(const int y, const int x) const {
    auto cnt = 0;
    for (auto i = -1; i <= 1; ++i) {
        auto cy = y + i;
        if (cy < 0 || cy >= height) continue;
        for (auto j = -1; j <= 1; ++j) {
            if (i == 0 && j == 0) continue;
            auto cx = x + j;
            if (cx < 0 || cx >= width) continue;
            if (table[cy * width + cx] == '*') cnt++;
        }
    }
    return (char) ('0' + cnt);
}

void Minesweeper::playGame() {
    hideTable();
    auto running = true;
    auto mode = REVEAL;
    auto gamestate = GameState(PLAYING);
    auto bombsRemaining = bombCount;
    while (running) {
        clearConsole();
        int row, col;
        row = col = -1;
        printTable();
        printPrompt(mode, bombsRemaining);
        std::string in;
        getline(std::cin, in);
        if (in == "!") {
            if (mode == FLAG) mode = REVEAL;
            else mode = FLAG;
        } else if (in == "exit") running = false;
        else {
            auto ss = std::stringstream(in);
            ss >> row >> col;
            if (ss.bad() || ss.fail()
                || row < 0 || row >= height
                || col < 0 || col >= width) {
                std::cout << "Something is amiss." << std::endl;
                continue;
            }
            if (mode == REVEAL) gamestate = reveal(col, row);
            else gamestate = flag(col, row, &bombsRemaining);
            if (gamestate == WON || gamestate == BANG) running = false;
        }
    }
    if (gamestate == BANG) {
        clearConsole();
        printTable();
        std::cout << "Bad luck!" << std::endl;
    }
    if (gamestate == WON) {
        clearConsole();
        printTable();
        std::cout << "You won!" << std::endl;
    }
}

void Minesweeper::printPrompt(const Mode mode, const int bombCount) const {
    std::cout << bombCount << " bombs remain" << std::endl;
    std::cout << "(type '!' to switch modes or 'exit' to quit)" << std::endl;
    std::cout << (mode == REVEAL ? REVEAL_MODE_PROMPT_PREFIX : FLAG_MODE_PROMPT_PREFIX);
    std::cout << " Enter the row and column you want to play" << std::endl;
}

GameState Minesweeper::reveal(int x, int y) {
    auto index = y * width + x;
    if (!isHidden(index) || isFlagged(index)) return PLAYING;
    show(x, y);
    if (table[index] == '*') return BANG;
    if (table[index] == '0') {
        auto states = new GameState[8];
        auto finalState = GameState(PLAYING);
        auto idx = 0;
        for (auto i = -1; i <= 1; ++i) {
            auto cy = y + i;
            if (cy < 0 || cy >= height) continue;
            for (auto j = -1; j <= 1; ++j) {
                if (i == 0 && j == 0)continue;
                auto cx = x + j;
                if (cx < 0 || cx >= width)continue;
                states[idx++] = reveal(cx, cy);
            }
        }
        for (auto i = 0; i < 8; ++i) {
            if (states[i] == BANG || states[i] == WON) {
                finalState = states[i];
                break;
            }
        }
        if (finalState == PLAYING) finalState = checkTableForWin();
        delete[] states;
        return finalState;
    }
    return checkTableForWin();
}

GameState Minesweeper::flag(int x, int y, int * const bombsRemaining) {
    auto index = y * width + x;
    if (isHidden(index)) {
        if (!isFlagged(index)) {
            table[index] -= FLAG_OFFSET;
            (*bombsRemaining)--;
            if (table[index] - FLAG_OFFSET == '*') realFlagged++;
        } else {
            table[index] += FLAG_OFFSET;
            (*bombsRemaining)++;
            if (table[index] - HIDDEN_OFFSET == '*') realFlagged--;
        }
    }
    return checkTableForWin();
}

bool Minesweeper::isHidden(size_t idx) const {
    return isValidFieldValue(table[idx] - FLAG_OFFSET) ||
           isValidFieldValue(table[idx] - HIDDEN_OFFSET);
}

bool Minesweeper::isFlagged(size_t idx) const {
    return isValidFieldValue(table[idx] - FLAG_OFFSET);
}

bool Minesweeper::isValidFieldValue(char field) const {
    return field == '*' || (field >= '0' && field <= '9');
}

void Minesweeper::show(int x, int y) {
    auto index = y * width + x;
    if (isFlagged(index)) table[index] -= FLAG_OFFSET;
    else table[index] -= HIDDEN_OFFSET;
}

void Minesweeper::hideTable() const {
    for (auto i = 0; i < height; ++i) {
        for (auto j = 0; j < width; ++j) table[i * width + j] += HIDDEN_OFFSET;
    }
}

GameState Minesweeper::checkTableForWin() const {
    if(realFlagged == bombCount) return WON;
    auto hidden = 0;
    for (auto i = size_t(0); i < height * width; ++i) {
        if (isFlagged(i) && table[i] - FLAG_OFFSET == '*') continue;
        if (isHidden(i)) hidden++;
    }
    if(hidden == bombCount-realFlagged) return WON;
    return PLAYING;
}
