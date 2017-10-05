#include "Minesweeper.h"

int main() {
    try {
        Minesweeper ms(10, 10);
        ms.countNeighbours();
        ms.playGame();
    } catch (const std::bad_alloc& e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}