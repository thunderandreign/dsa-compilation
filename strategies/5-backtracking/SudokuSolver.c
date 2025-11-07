#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/**
 * Backtracking Strategy: Sudoku Solver
 * Core Idea: Fill empty cells with digits 1-9 such that each row, column, and 3x3 box contains each digit exactly once
 * Time Complexity: O(9^(n*n)) where n is the number of empty cells (worst case)
 * Space Complexity: O(n*n) for the board and recursion stack
 */

#define SIZE 9
#define EMPTY 0
#define BOX_SIZE 3

/**
 * Structure to represent a cell position
 */
typedef struct {
    int row;
    int col;
} Position;

/**
 * Find the next empty cell in the board
 * @param board Sudoku grid
 * @param pos Pointer to store position of empty cell
 * @return true if empty cell found, false otherwise
 */
bool findEmptyCell(int board[SIZE][SIZE], Position* pos) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == EMPTY) {
                pos->row = row;
                pos->col = col;
                return true;
            }
        }
    }
    return false;
}

/**
 * Check if placing digit at (row, col) is valid
 * @param board Sudoku grid
 * @param row Row index
 * @param col Column index
 * @param digit Digit to place (1-9)
 * @return true if move is valid, false otherwise
 */
bool isValidMove(int board[SIZE][SIZE], int row, int col, int digit) {
    // Check row
    for (int c = 0; c < SIZE; c++) {
        if (board[row][c] == digit) {
            return false;
        }
    }
    
    // Check column
    for (int r = 0; r < SIZE; r++) {
        if (board[r][col] == digit) {
            return false;
        }
    }
    
    // Check 3x3 box
    int boxStartRow = (row / BOX_SIZE) * BOX_SIZE;
    int boxStartCol = (col / BOX_SIZE) * BOX_SIZE;
    
    for (int r = boxStartRow; r < boxStartRow + BOX_SIZE; r++) {
        for (int c = boxStartCol; c < boxStartCol + BOX_SIZE; c++) {
            if (board[r][c] == digit) {
                return false;
            }
        }
    }
    
    return true;
}

/**
 * Solve a Sudoku puzzle using backtracking
 * @param board 9x9 Sudoku grid (0 represents empty cells)
 * @return true if puzzle is solvable, false otherwise
 */
bool solveSudoku(int board[SIZE][SIZE]) {
    Position pos;
    
    // Find next empty cell
    if (!findEmptyCell(board, &pos)) {
        return true; // No empty cells, puzzle solved
    }
    
    // Try digits 1-9
    for (int digit = 1; digit <= 9; digit++) {
        if (isValidMove(board, pos.row, pos.col, digit)) {
            // Place digit
            board[pos.row][pos.col] = digit;
            
            // Recursively solve
            if (solveSudoku(board)) {
                return true;
            }
            
            // Backtrack: remove digit
            board[pos.row][pos.col] = EMPTY;
        }
    }
    
    return false; // No solution found
}

/**
 * Print the Sudoku board
 * @param board Sudoku grid
 */
void printBoard(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        if (row % 3 == 0 && row != 0) {
            printf("------+-------+------\n");
        }
        
        for (int col = 0; col < SIZE; col++) {
            if (col % 3 == 0 && col != 0) {
                printf("| ");
            }
            
            if (board[row][col] == EMPTY) {
                printf(". ");
            } else {
                printf("%d ", board[row][col]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Copy a board
 * @param dest Destination board
 * @param src Source board
 */
void copyBoard(int dest[SIZE][SIZE], int src[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            dest[i][j] = src[i][j];
        }
    }
}

/**
 * Check if the current board state is valid
 * @param board Sudoku grid
 * @return true if valid, false otherwise
 */
bool isValidBoard(int board[SIZE][SIZE]) {
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] != EMPTY) {
                int digit = board[row][col];
                board[row][col] = EMPTY; // Temporarily remove to check
                
                if (!isValidMove(board, row, col, digit)) {
                    board[row][col] = digit; // Restore
                    return false;
                }
                
                board[row][col] = digit; // Restore
            }
        }
    }
    return true;
}

/**
 * Count the number of solutions for a given puzzle
 * @param board Sudoku grid
 * @return Number of solutions
 */
int countSolutions(int board[SIZE][SIZE]) {
    Position pos;
    
    if (!findEmptyCell(board, &pos)) {
        return 1; // Found a solution
    }
    
    int count = 0;
    for (int digit = 1; digit <= 9; digit++) {
        if (isValidMove(board, pos.row, pos.col, digit)) {
            board[pos.row][pos.col] = digit;
            count += countSolutions(board);
            board[pos.row][pos.col] = EMPTY;
        }
    }
    
    return count;
}

/**
 * Get possible values for a cell
 * @param board Sudoku grid
 * @param row Row index
 * @param col Column index
 * @param possible Array to store possible values (1-indexed, 0 means not possible)
 * @return Number of possible values
 */
int getPossibleValues(int board[SIZE][SIZE], int row, int col, bool possible[10]) {
    // Initialize
    for (int i = 0; i < 10; i++) {
        possible[i] = false;
    }
    
    if (board[row][col] != EMPTY) {
        return 0; // Cell already filled
    }
    
    int count = 0;
    for (int digit = 1; digit <= 9; digit++) {
        if (isValidMove(board, row, col, digit)) {
            possible[digit] = true;
            count++;
        }
    }
    
    return count;
}

/**
 * Enhanced solver using Most Constrained Variable (MCV) heuristic
 * @param board Sudoku grid
 * @return true if solvable, false otherwise
 */
bool solveSudokuMCV(int board[SIZE][SIZE]) {
    // Find empty cell with minimum possible values
    int bestRow = -1, bestCol = -1;
    int minPossible = 10;
    
    for (int row = 0; row < SIZE; row++) {
        for (int col = 0; col < SIZE; col++) {
            if (board[row][col] == EMPTY) {
                bool possible[10];
                int possibleCount = getPossibleValues(board, row, col, possible);
                
                if (possibleCount == 0) {
                    return false; // No valid moves, backtrack
                }
                
                if (possibleCount < minPossible) {
                    minPossible = possibleCount;
                    bestRow = row;
                    bestCol = col;
                }
            }
        }
    }
    
    if (bestRow == -1) {
        return true; // No empty cells, solved
    }
    
    // Try all possible values for the most constrained cell
    bool possibleValues[10];
    getPossibleValues(board, bestRow, bestCol, possibleValues);
    
    for (int digit = 1; digit <= 9; digit++) {
        if (possibleValues[digit]) {
            board[bestRow][bestCol] = digit;
            
            if (solveSudokuMCV(board)) {
                return true;
            }
            
            board[bestRow][bestCol] = EMPTY;
        }
    }
    
    return false;
}

/**
 * Solve with step tracking (simplified version)
 * @param board Sudoku grid
 * @param stepCount Pointer to step counter
 * @return true if solved, false otherwise
 */
bool solveSudokuWithSteps(int board[SIZE][SIZE], int* stepCount) {
    Position pos;
    
    if (!findEmptyCell(board, &pos)) {
        printf("Step %d: ✓ Puzzle solved successfully!\n", ++(*stepCount));
        return true;
    }
    
    printf("Step %d: Trying to fill cell (%d, %d)\n", ++(*stepCount), pos.row + 1, pos.col + 1);
    
    for (int digit = 1; digit <= 9; digit++) {
        if (isValidMove(board, pos.row, pos.col, digit)) {
            printf("  Trying digit %d at (%d, %d) - Valid!\n", digit, pos.row + 1, pos.col + 1);
            board[pos.row][pos.col] = digit;
            
            if (solveSudokuWithSteps(board, stepCount)) {
                return true;
            }
            
            printf("  Backtracking from (%d, %d), removing %d\n", pos.row + 1, pos.col + 1, digit);
            board[pos.row][pos.col] = EMPTY;
        } else {
            printf("  Digit %d at (%d, %d) - Invalid (conflicts detected)\n", digit, pos.row + 1, pos.col + 1);
        }
        
        // Limit output for demonstration
        if (*stepCount > 50) {
            printf("  ... (stopping step display after 50 steps)\n");
            return solveSudoku(board);
        }
    }
    
    return false;
}

/**
 * Get current time in milliseconds
 */
long long getCurrentTimeMillis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

/**
 * Fill a 3x3 box with numbers 1-9 in order (for testing)
 */
void fillBox(int board[SIZE][SIZE], int startRow, int startCol) {
    int num = 1;
    for (int row = startRow; row < startRow + BOX_SIZE; row++) {
        for (int col = startCol; col < startCol + BOX_SIZE; col++) {
            board[row][col] = num++;
        }
    }
}

/**
 * Generate a simple puzzle by filling diagonal boxes and solving
 */
void generateSimplePuzzle(int board[SIZE][SIZE], int cellsToRemove) {
    // Initialize empty board
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = EMPTY;
        }
    }
    
    // Fill diagonal boxes (they don't interfere with each other)
    for (int box = 0; box < SIZE; box += BOX_SIZE) {
        fillBox(board, box, box);
    }
    
    // Solve the rest
    solveSudoku(board);
    
    // Remove cells randomly
    srand((unsigned int)time(NULL));
    int removed = 0;
    while (removed < cellsToRemove) {
        int row = rand() % SIZE;
        int col = rand() % SIZE;
        
        if (board[row][col] != EMPTY) {
            board[row][col] = EMPTY;
            removed++;
        }
    }
}

int main() {
    printf("=== Sudoku Solver - Backtracking ===\n\n");
    
    // Test Case 1: Easy puzzle
    printf("Test Case 1: Easy Sudoku\n");
    int easyPuzzle[SIZE][SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 7, 9}
    };
    
    printf("Original puzzle:\n");
    printBoard(easyPuzzle);
    
    int easyCopy[SIZE][SIZE];
    copyBoard(easyCopy, easyPuzzle);
    bool solved = solveSudoku(easyCopy);
    
    if (solved) {
        printf("Solution:\n");
        printBoard(easyCopy);
    } else {
        printf("No solution found.\n");
    }
    
    // Test Case 2: Hard puzzle
    printf("Test Case 2: Hard Sudoku\n");
    int hardPuzzle[SIZE][SIZE] = {
        {0, 0, 0, 6, 0, 0, 4, 0, 0},
        {7, 0, 0, 0, 0, 3, 6, 0, 0},
        {0, 0, 0, 0, 9, 1, 0, 8, 0},
        {0, 0, 0, 0, 0, 0, 0, 0, 0},
        {0, 5, 0, 1, 8, 0, 0, 0, 3},
        {0, 0, 0, 3, 0, 6, 0, 4, 5},
        {0, 4, 0, 2, 0, 0, 0, 6, 0},
        {9, 0, 3, 0, 0, 0, 0, 0, 0},
        {0, 2, 0, 0, 0, 0, 1, 0, 0}
    };
    
    printf("Original hard puzzle:\n");
    printBoard(hardPuzzle);
    
    int hardCopy[SIZE][SIZE];
    copyBoard(hardCopy, hardPuzzle);
    long long startTime = getCurrentTimeMillis();
    bool hardSolved = solveSudoku(hardCopy);
    long long endTime = getCurrentTimeMillis();
    
    if (hardSolved) {
        printf("Solution (solved in %lldms):\n", endTime - startTime);
        printBoard(hardCopy);
    }
    
    // Test Case 3: Step-by-step solving
    printf("Test Case 3: Step-by-step solving (first few steps)\n");
    int stepCopy[SIZE][SIZE];
    copyBoard(stepCopy, easyPuzzle);
    
    int stepCount = 0;
    solveSudokuWithSteps(stepCopy, &stepCount);
    printf("\n");
    
    // Test Case 4: Performance comparison
    printf("Test Case 4: Performance comparison (Basic vs MCV)\n");
    
    int perfPuzzle[SIZE][SIZE], mcvPuzzle[SIZE][SIZE];
    copyBoard(perfPuzzle, hardPuzzle);
    copyBoard(mcvPuzzle, hardPuzzle);
    
    startTime = getCurrentTimeMillis();
    solveSudoku(perfPuzzle);
    long long basicTime = getCurrentTimeMillis() - startTime;
    
    startTime = getCurrentTimeMillis();
    solveSudokuMCV(mcvPuzzle);
    long long mcvTime = getCurrentTimeMillis() - startTime;
    
    printf("Basic backtracking: %lldms\n", basicTime);
    printf("MCV heuristic: %lldms\n", mcvTime);
    if (mcvTime > 0) {
        printf("Speedup: %.2fx\n", (double)basicTime / mcvTime);
    }
    printf("\n");
    
    // Test Case 5: Puzzle validation
    printf("Test Case 5: Puzzle validation\n");
    
    printf("Valid puzzle: %s\n", isValidBoard(easyPuzzle) ? "true" : "false");
    
    // Create invalid puzzle
    int invalidPuzzle[SIZE][SIZE];
    copyBoard(invalidPuzzle, easyPuzzle);
    invalidPuzzle[0][0] = 3; // Conflict with existing 3 in same row
    printf("Invalid puzzle (duplicate in row): %s\n", isValidBoard(invalidPuzzle) ? "true" : "false");
    printf("\n");
    
    // Test Case 6: Solution counting
    printf("Test Case 6: Solution counting\n");
    
    int emptierPuzzle[SIZE][SIZE] = {
        {5, 3, 0, 0, 7, 0, 0, 0, 0},
        {6, 0, 0, 1, 9, 5, 0, 0, 0},
        {0, 9, 8, 0, 0, 0, 0, 6, 0},
        {8, 0, 0, 0, 6, 0, 0, 0, 3},
        {4, 0, 0, 8, 0, 3, 0, 0, 1},
        {7, 0, 0, 0, 2, 0, 0, 0, 6},
        {0, 6, 0, 0, 0, 0, 2, 8, 0},
        {0, 0, 0, 4, 1, 9, 0, 0, 5},
        {0, 0, 0, 0, 8, 0, 0, 0, 0}  // Made last cell empty
    };
    
    int solutionCount = countSolutions(emptierPuzzle);
    printf("Number of solutions for modified puzzle: %d\n\n", solutionCount);
    
    // Test Case 7: Constraint analysis
    printf("Test Case 7: Constraint analysis\n");
    printf("Possible values for empty cells in first row:\n");
    
    for (int col = 0; col < SIZE; col++) {
        if (easyPuzzle[0][col] == EMPTY) {
            bool possible[10];
            int count = getPossibleValues(easyPuzzle, 0, col, possible);
            
            printf("Cell (1, %d): {", col + 1);
            bool first = true;
            for (int digit = 1; digit <= 9; digit++) {
                if (possible[digit]) {
                    if (!first) printf(", ");
                    printf("%d", digit);
                    first = false;
                }
            }
            printf("} (%d possible)\n", count);
        }
    }
    printf("\n");
    
    // Test Case 8: Generated puzzle
    printf("Test Case 8: Generated puzzle\n");
    printf("Generating puzzle with 45 empty cells...\n");
    
    int generated[SIZE][SIZE];
    generateSimplePuzzle(generated, 45);
    printf("Generated puzzle:\n");
    printBoard(generated);
    
    int generatedCopy[SIZE][SIZE];
    copyBoard(generatedCopy, generated);
    bool generatedSolved = solveSudoku(generatedCopy);
    printf("Generated puzzle solvable: %s\n", generatedSolved ? "true" : "false");
    
    if (generatedSolved) {
        printf("Solution:\n");
        printBoard(generatedCopy);
    }
    
    // Test Case 9: Edge cases
    printf("Test Case 9: Edge cases\n");
    
    // Empty board
    int emptyBoard[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            emptyBoard[i][j] = EMPTY;
        }
    }
    
    int emptyCopy[SIZE][SIZE];
    copyBoard(emptyCopy, emptyBoard);
    startTime = getCurrentTimeMillis();
    bool emptySolved = solveSudoku(emptyCopy);
    endTime = getCurrentTimeMillis();
    
    printf("Empty board solvable: %s (Time: %lldms)\n", emptySolved ? "true" : "false", endTime - startTime);
    
    // Nearly complete board
    int nearComplete[SIZE][SIZE];
    copyBoard(nearComplete, easyCopy); // Use the solved easy puzzle
    nearComplete[8][8] = EMPTY; // Make one cell empty
    
    startTime = getCurrentTimeMillis();
    bool nearSolved = solveSudoku(nearComplete);
    endTime = getCurrentTimeMillis();
    
    printf("Nearly complete board solvable: %s (Time: %lldms)\n", nearSolved ? "true" : "false", endTime - startTime);
    
    printf("\nComplexity Analysis:\n");
    printf("- Time: O(9^(n*n)) worst case, where n is empty cells\n");
    printf("- Space: O(n*n) for board and recursion stack\n");
    printf("- Optimizations: MCV heuristic, constraint propagation\n");
    printf("- Pruning: early constraint checking prevents invalid branches\n");
    
    return 0;
}