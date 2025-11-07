#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/**
 * Backtracking Strategy: N-Queens Problem
 * Core Idea: Place N queens on an N×N chessboard such that no two queens attack each other
 * Time Complexity: O(N!) - exponential as we try all possible placements
 * Space Complexity: O(N) for recursion stack and board representation
 */

#define MAX_N 20

/**
 * Structure to store a single solution
 */
typedef struct {
    int queens[MAX_N]; // queens[i] = column position of queen in row i
    int size;
} Solution;

/**
 * Structure to store all solutions
 */
typedef struct {
    Solution solutions[1000]; // Adjust size as needed
    int count;
} SolutionSet;

/**
 * Check if placing a queen at (row, col) is valid
 * @param queens Current queen positions
 * @param row Row to check
 * @param col Column to check
 * @return true if position is safe, false otherwise
 */
bool isValid(int queens[], int row, int col) {
    for (int i = 0; i < row; i++) {
        int placedCol = queens[i];
        
        // Check column conflict
        if (placedCol == col) {
            return false;
        }
        
        // Check diagonal conflicts
        // Two queens are on the same diagonal if |row1-row2| = |col1-col2|
        if (abs(i - row) == abs(placedCol - col)) {
            return false;
        }
    }
    return true;
}

/**
 * Backtracking function to solve N-Queens
 * @param queens Array representing queen positions
 * @param row Current row to place a queen
 * @param n Board size
 * @param solutions Structure to store all solutions
 */
void backtrack(int queens[], int row, int n, SolutionSet* solutions) {
    // Base case: all queens placed successfully
    if (row == n) {
        // Store this solution
        if (solutions->count < 1000) {
            for (int i = 0; i < n; i++) {
                solutions->solutions[solutions->count].queens[i] = queens[i];
            }
            solutions->solutions[solutions->count].size = n;
            solutions->count++;
        }
        return;
    }
    
    // Try placing queen in each column of current row
    for (int col = 0; col < n; col++) {
        if (isValid(queens, row, col)) {
            // Place queen
            queens[row] = col;
            
            // Recurse to next row
            backtrack(queens, row + 1, n, solutions);
            
            // Backtrack: remove queen (implicit - we'll overwrite)
            queens[row] = -1;
        }
    }
}

/**
 * Solve N-Queens problem and return all solutions
 * @param n Size of the chessboard
 * @param solutions Structure to store solutions
 */
void solveNQueens(int n, SolutionSet* solutions) {
    int queens[MAX_N];
    for (int i = 0; i < n; i++) {
        queens[i] = -1;
    }
    
    solutions->count = 0;
    backtrack(queens, 0, n, solutions);
}

/**
 * Count solutions without storing them (memory efficient)
 * @param queens Array representing queen positions
 * @param row Current row
 * @param n Board size
 * @return Number of valid solutions
 */
int countBacktrack(int queens[], int row, int n) {
    if (row == n) {
        return 1; // Found a valid solution
    }
    
    int count = 0;
    for (int col = 0; col < n; col++) {
        if (isValid(queens, row, col)) {
            queens[row] = col;
            count += countBacktrack(queens, row + 1, n);
            queens[row] = -1;
        }
    }
    return count;
}

/**
 * Count total number of solutions
 * @param n Board size
 * @return Number of solutions
 */
int countNQueens(int n) {
    int queens[MAX_N];
    for (int i = 0; i < n; i++) {
        queens[i] = -1;
    }
    return countBacktrack(queens, 0, n);
}

/**
 * Print a single solution
 * @param solution Solution to print
 * @param solutionNumber Solution index
 */
void printSolution(Solution* solution, int solutionNumber) {
    printf("Solution %d:\n", solutionNumber);
    for (int i = 0; i < solution->size; i++) {
        for (int j = 0; j < solution->size; j++) {
            if (solution->queens[i] == j) {
                printf("Q ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Print current board state during demonstration
 * @param queens Queen positions
 * @param n Board size
 */
void printBoard(int queens[], int n) {
    printf("Current board:\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (queens[i] == j) {
                printf("Q ");
            } else {
                printf(". ");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Demonstrate step-by-step solution construction
 * @param queens Queen positions
 * @param row Current row
 * @param n Board size
 * @param step Step counter
 * @return true if solution found, false otherwise
 */
bool demonstrateBacktrack(int queens[], int row, int n, int* step) {
    printf("Step %d: Trying to place queen in row %d\n", *step, row);
    (*step)++;
    
    if (row == n) {
        printf("✓ All queens placed successfully!\n");
        printBoard(queens, n);
        return true;
    }
    
    for (int col = 0; col < n; col++) {
        printf("  Trying column %d...\n", col);
        
        if (isValid(queens, row, col)) {
            printf("  ✓ Position (%d, %d) is safe\n", row, col);
            queens[row] = col;
            printBoard(queens, n);
            
            if (demonstrateBacktrack(queens, row + 1, n, step)) {
                return true; // Solution found
            }
            
            printf("  ✗ Backtracking from (%d, %d)\n", row, col);
            queens[row] = -1;
        } else {
            printf("  ✗ Position (%d, %d) conflicts with existing queens\n", row, col);
        }
    }
    
    return false;
}

/**
 * Demonstrate step-by-step solution
 * @param n Board size
 */
void demonstrateSteps(int n) {
    printf("Step-by-step N-Queens solution for %dx%d board:\n", n, n);
    int queens[MAX_N];
    for (int i = 0; i < n; i++) {
        queens[i] = -1;
    }
    
    int step = 1;
    demonstrateBacktrack(queens, 0, n, &step);
}

/**
 * Analyze conflict patterns
 * @param n Board size
 */
void analyzeConflicts(int n) {
    printf("Conflict Analysis for %d-Queens:\n", n);
    
    // Show what positions conflict with a queen at (0, 0)
    printf("Positions that conflict with queen at (0, 0):\n");
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            bool conflict = false;
            
            // Same row
            if (i == 0) conflict = true;
            
            // Same column  
            if (j == 0) conflict = true;
            
            // Main diagonal (row - col = constant)
            if (i - j == 0 - 0) conflict = true;
            
            // Anti-diagonal (row + col = constant)
            if (i + j == 0 + 0) conflict = true;
            
            printf("%s ", conflict ? "X" : ".");
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Optimized N-Queens using bit manipulation
 * @param row Current row
 * @param cols Bitmask for occupied columns
 * @param diag1 Bitmask for occupied main diagonals
 * @param diag2 Bitmask for occupied anti-diagonals
 * @param n Board size
 * @return Number of solutions
 */
int backtrackBitwise(int row, int cols, int diag1, int diag2, int n) {
    if (row == n) {
        return 1;
    }
    
    int count = 0;
    int availablePositions = ((1 << n) - 1) & ~(cols | diag1 | diag2);
    
    while (availablePositions != 0) {
        int position = availablePositions & -availablePositions; // Get rightmost set bit
        availablePositions &= availablePositions - 1; // Remove rightmost set bit
        
        count += backtrackBitwise(row + 1, 
                                 cols | position,
                                 (diag1 | position) << 1,
                                 (diag2 | position) >> 1,
                                 n);
    }
    return count;
}

/**
 * Solve N-Queens using bitwise optimization
 * @param n Board size
 * @return Number of solutions
 */
int solveNQueensBitwise(int n) {
    return backtrackBitwise(0, 0, 0, 0, n);
}

/**
 * Get current time in milliseconds
 */
long long getCurrentTimeMillis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

int main() {
    printf("=== N-Queens Problem - Backtracking ===\n\n");
    
    // Test Case 1: Small board (4x4)
    printf("Test Case 1: 4-Queens\n");
    SolutionSet solutions4;
    solveNQueens(4, &solutions4);
    printf("Number of solutions: %d\n\n", solutions4.count);
    
    for (int i = 0; i < solutions4.count; i++) {
        printSolution(&solutions4.solutions[i], i + 1);
    }
    
    // Test Case 2: Classic 8-Queens
    printf("Test Case 2: 8-Queens\n");
    int count8 = countNQueens(8);
    printf("Number of solutions for 8-Queens: %d\n", count8);
    
    // Show first solution only
    SolutionSet solutions8;
    solveNQueens(8, &solutions8);
    if (solutions8.count > 0) {
        printf("First solution:\n");
        printSolution(&solutions8.solutions[0], 1);
    }
    
    // Test Case 3: Step-by-step demonstration for small board
    printf("Test Case 3: Step-by-step solution\n");
    demonstrateSteps(4);
    
    // Test Case 4: Performance comparison
    printf("Test Case 4: Performance comparison\n");
    
    for (int n = 1; n <= 12; n++) {
        long long startTime = getCurrentTimeMillis();
        int count = countNQueens(n);
        long long endTime = getCurrentTimeMillis();
        
        long long startTimeBit = getCurrentTimeMillis();
        int countBit = solveNQueensBitwise(n);
        long long endTimeBit = getCurrentTimeMillis();
        
        printf("%d-Queens: %d solutions (Standard: %lldms, Bitwise: %lldms) [Verification: %s]\n", 
               n, count, endTime - startTime, endTimeBit - startTimeBit,
               count == countBit ? "✓" : "✗");
    }
    printf("\n");
    
    // Test Case 5: Conflict analysis
    printf("Test Case 5: Understanding conflicts\n");
    analyzeConflicts(5);
    
    // Test Case 6: Edge cases
    printf("Test Case 6: Edge cases\n");
    
    // 1x1 board
    SolutionSet solutions1;
    solveNQueens(1, &solutions1);
    printf("1-Queens: %d solution\n", solutions1.count);
    if (solutions1.count > 0) {
        printSolution(&solutions1.solutions[0], 1);
    }
    
    // 2x2 and 3x3 boards (no solutions)
    printf("2-Queens: %d solutions (impossible)\n", countNQueens(2));
    printf("3-Queens: %d solutions (impossible)\n", countNQueens(3));
    printf("\n");
    
    // Test Case 7: Mathematical insights
    printf("Test Case 7: Mathematical insights\n");
    printf("N-Queens solutions count follows no simple formula.\n");
    printf("Known values:\n");
    int knownCounts[] = {1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712};
    int knownSize = sizeof(knownCounts) / sizeof(knownCounts[0]);
    
    for (int i = 1; i < knownSize; i++) {
        printf("N=%d: %d solutions\n", i, knownCounts[i]);
    }
    
    printf("\nComplexity Analysis:\n");
    printf("- Time: O(N!) in worst case - we try all possible arrangements\n");
    printf("- Space: O(N) for recursion stack and queen positions\n");
    printf("- Optimizations: bitwise operations, constraint propagation\n");
    printf("- Pruning: early rejection of invalid partial solutions\n");
    
    // Test Case 8: Larger boards (if time permits)
    printf("\nTest Case 8: Larger boards\n");
    for (int n = 13; n <= 15; n++) {
        printf("Computing %d-Queens...\n", n);
        long long startTime = getCurrentTimeMillis();
        int count = solveNQueensBitwise(n); // Use faster bitwise version
        long long endTime = getCurrentTimeMillis();
        printf("%d-Queens: %d solutions (Time: %lldms)\n", n, count, endTime - startTime);
    }
    
    return 0;
}