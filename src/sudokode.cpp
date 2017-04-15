#include <array>
#include <iostream>
#include <chrono>

using SudokuGrid = std::array<std::array<int,9>, 9>;
using Clock      = std::chrono::high_resolution_clock;

// checkers
//-------------------------------------------------------
bool 
checkRowForMatch(SudokuGrid const & grid, int row, int value) {
    for (int column = 0; column < 9; column++) {
        if (grid[row][column] == value) {
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------
bool 
checkColumnForMatch(SudokuGrid const & grid, int column, int value) {
    for (int row = 0; row < 9; row++) {
        if (grid[row][column] == value) {
            return true;
        }
    }
    return false;
}

//-------------------------------------------------------
bool
checkSquareForMatch(SudokuGrid const & grid, int startRow, int startColumn, int value) {
    for (int row = 0; row < 3 ; row++) {
        for (int column = 0; column < 3; column++) {
            if (grid[row + startRow][column + startColumn] == value) {
                return true;
            }
        }
    }
    return false;
}

//-------------------------------------------------------
bool
isSafe(SudokuGrid const & grid, int row, int column, int value) {
    return !checkRowForMatch(grid, row, value)       &&
           !checkColumnForMatch(grid, column, value) &&
           !checkSquareForMatch(grid, row - row % 3 , column - column % 3, value);
}

//-------------------------------------------------------
bool 
findUnpopulatedCell(SudokuGrid const & grid, int & row, int & column) {
    for (row = 0; row < 9; row++) {
        for (column = 0; column < 9; column++) {
            if(grid[row][column] == 0) {
                return true;
            }
        }
    }
    return false;
}

// solvers
//-------------------------------------------------------
bool 
solve_BackTrace(SudokuGrid & grid) {
    int row, column;

    if (!findUnpopulatedCell(grid, row, column)) {
        return true;
    }

    for (int num = 1; num <= 9; num++) {

        if (isSafe(grid, row, column, num)) {

            grid[row][column] = num;
 
            if (solve_BackTrace(grid)) {
                return true;
            }
 
            // failure, unmake & try again
            grid[row][column] = 0;
        }
    }
    return false; // this triggers backtrackin
}

// other
//-------------------------------------------------------
void
printGrid(SudokuGrid const & grid) {
    std::cout << "\n-------------------------\n";
    for (int i = 0; i < 9; i++) {
        std::cout << "| ";
        for (int j = 0; j < 9; j++) {
            std::cout << grid[i][j] << " ";
            if(j == 2 || j == 5) {
                std::cout << "| ";
            }
        }   
        std::cout << "|\n";
        if(i == 2 || i == 5) {
            std::cout << "|-----------------------|\n";
        } 
    }
    std::cout << "-------------------------\n";
}
//-------------------------------------------------------
int main() {

    SudokuGrid input{{
                      {0, 0, 3,     2, 8, 0,     1, 0, 0},
                      {5, 0, 0,     7, 0, 3,     9, 0, 8},
                      {0, 8, 4,     0, 0, 0,     0, 3, 0},

                      {0, 2, 0,     1, 0, 8,     0, 0, 9},
                      {0, 0, 0,     9, 5, 2,     0, 0, 0},
                      {8, 0, 0,     3, 0, 4,     0, 6, 0},

                      {0, 3, 0,     0, 0, 0,     6, 1, 0},
                      {6, 0, 2,     8, 0, 7,     0, 0, 5},
                      {0, 0, 9,     0, 2, 1,     3, 0, 0}
                      }};

    std::cout << "starting grid";
    printGrid(input);

    std::cout << "col0:    " << std::boolalpha << checkColumnForMatch(input, 0, 5)  << "\n";

    std::cout << "row2:    " << std::boolalpha << checkRowForMatch(input, 2, 7) << "\n";

    std::cout << "square0: " << std::boolalpha << checkSquareForMatch(input, 0, 0, 1) << "\n";

    auto start  = Clock::now();
    bool solved = solve_BackTrace(input);
    auto end    = Clock::now();

    auto solveTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    if(solved) {
        std::cout << "\nSoved grid\n";
        printGrid(input);
        std::cout << "solve time: " << solveTime.count() << " microseconds \n";
    } else {
        std::cout << "no solution found" << std::endl;
    }
    
    return 0;
}