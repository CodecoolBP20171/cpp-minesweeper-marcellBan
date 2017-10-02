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
            for (auto i = 0; i < height; ++i) {
                for (auto j = 0; j < width; ++j) {
                    auto index = i * width + j;
                    if (table[index] == '.') {
                        table[index] = getNeighbourCount(i, j);
                    }
                }
            }
        }

        void printTable() const {
            for (auto i = 0; i < height; ++i) {
                for (auto j = 0; j < width; ++j) {
                    auto index = i * width + j;
                    std::cout << table[index] << " ";
                }
                std::cout << std::endl;
            }
        }

    private:
        void fillTable() {
            std::random_device rng;
            for (auto i = 0; i < width * height; ++i) {
                table[i] = rng() % 100 < 15 ? '*' : '.';
            }
        }

        char getNeighbourCount(const int y, const int x) const {
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

        const size_t width, height;
        char * table;
    };
}

int main() {
    try {
        Minesweeper ms(50, 30);
        ms.printTable();
        ms.countNeighbours();
        ms.printTable();
    } catch (const std::bad_alloc& e) {
        std::cerr << "Couldn't allocate enough memory for minesweeper table" << std::endl;
        return EXIT_FAILURE;
    }

    return 0;
}