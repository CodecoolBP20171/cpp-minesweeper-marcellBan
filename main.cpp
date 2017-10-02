#include <iostream>
#include <random>

namespace {
    class Minesweeper {
    public:
        Minesweeper(const size_t width, const size_t height)
                : width(width), height(height),
                  table(new char[width * height]) {
            fillTable();
        }

        /* In a real implementation there would also be a
         * - copy constructor
         * - assignment operator
         * - move constructor
         * - move assignment operator
         * We will learn about them later
         */

        virtual ~Minesweeper() {
            delete[] table;
        }

        void countNeighbours() {
            // step 2 goes here
        }

        void printTable() const {
            // step 3 goes here
        }

    private:
        void fillTable() {
            std::random_device rng;
            for (auto i = 0; i < width * height; ++i) {
                table[i] = rng() % 100 < 30 ? '*' : '.';
            }
        }

        const size_t width, height;
        char * table;
    };
}

int main() {
    try {
        Minesweeper ms(100, 50);
        ms.printTable();
        ms.countNeighbours();
        ms.printTable();
    } catch (const std::bad_alloc& e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}