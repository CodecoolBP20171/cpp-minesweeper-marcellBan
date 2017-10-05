#include "Minesweeper.h"

Minesweeper::Minesweeper(const size_t width, const size_t height)
        : width(width), height(height),
          table(new char[width * height]) {
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
    for (auto i = 0; i < height; ++i) {
        for (auto j = 0; j < width; ++j) {
            auto index = i * width + j;
            if (isFlagged(j, i)) std::cout << "F";
            else if (isHidden(j, i)) std::cout << "H";
            else std::cout << table[index];
            std::cout << " ";
        }
        std::cout << std::endl;
    }
}

void Minesweeper::fillTable() {
    std::random_device rng;
    for (auto i = 0; i < width * height; ++i) {
        table[i] = rng() % 100 < 15 ? '*' : '.';
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
    while (running) {
        int row, col;
        row = col = -1;
        printTable();
        printPrompt(mode);
        std::string in;
        getline(std::cin, in);
        if (in == "!") {
            if (mode == FLAG) mode = REVEAL;
            else mode = FLAG;
        } else if (in == "exit") running = false;
        else {
            auto ss = std::stringstream(in);
            ss >> row >> col;
            if (row == -1 || col == -1 || ss.bad() || ss.fail()) {
                std::cout << "Something is amiss." << std::endl;
                continue;
            }
            if (mode == REVEAL) gamestate = reveal(col, row);
            else flag(col, row);
            if (gamestate == WON || gamestate == BANG) running = false;
        }
    }
    if (gamestate == BANG) {
        std::cout << "Bad luck" << std::endl;
        printTable();
    }
}

void Minesweeper::printPrompt(const Mode& mode) const {
    std::cout << (mode == REVEAL ? REVEAL_MODE_PROMPT_PREFIX : FLAG_MODE_PROMPT_PREFIX);
    std::cout << " Enter the row and column you want to play (or type '!' to switch modes or 'exit' to quit)"
              << std::endl;
}

GameState Minesweeper::reveal(int x, int y) {
    auto index = y * width + x;
    if (!isHidden(x, y)) return PLAYING;
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
        delete[] states;
        return finalState;
    }
}

void Minesweeper::flag(int x, int y) {
    if (isHidden(x, y)) {
        if (!isFlagged(x, y)) table[y * width + x] -= FLAG_OFFSET;
        else table[y * width + x] += FLAG_OFFSET;
    }
}

bool Minesweeper::isHidden(int x, int y) const {
    return isValidFieldValue(table[y * width + x] - FLAG_OFFSET) ||
           isValidFieldValue(table[y * width + x] - HIDDEN_OFFSET);
}

bool Minesweeper::isFlagged(int x, int y) const {
    return isValidFieldValue(table[y * width + x] - FLAG_OFFSET);
}

bool Minesweeper::isValidFieldValue(char field) const {
    return field == '*' || (field >= '0' && field <= '9');
}

void Minesweeper::show(int x, int y) {
    auto index = y * width + x;
    if (isFlagged(x, y)) table[index] -= FLAG_OFFSET;
    else table[index] -= HIDDEN_OFFSET;
}

void Minesweeper::hideTable() const {
    for (auto i = 0; i < height; ++i) {
        for (auto j = 0; j < width; ++j) table[i * width + j] += HIDDEN_OFFSET;
    }
}
