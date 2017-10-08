#include "Minesweeper.h"

int main() {
    try {
        bool validInput = false;
        size_t width, height;
        do{
            width = height = 0;
            std::string in;
            std::cout << "Enter the size of the minefield (width height)" << std::endl;
            getline(std::cin, in);
            auto ss = std::stringstream(in);
            ss >> width >> height ;
            if (ss.bad() || ss.fail()
                || width <= 0 || width > 50
                || height <= 0 || height > 50) {
                std::cout << "Something is amiss." << std::endl;
            } else validInput = true;
        }while(!validInput);
        Minesweeper ms(width, height);
        ms.countNeighbours();
        ms.playGame();
    } catch (const std::bad_alloc& e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}