/**
 * Backtracking Strategy: Sudoku Solver
 * Core Idea: Fill empty cells with digits 1-9 such that each row, column, and 3x3 box contains each digit exactly once
 * Time Complexity: O(9^(n*n)) where n is the number of empty cells (worst case)
 * Space Complexity: O(n*n) for the board and recursion stack
 */

import java.util.*;

public class SudokuSolver {
    
    private static final int SIZE = 9;
    private static final int EMPTY = 0;
    private static final int BOX_SIZE = 3;
    
    /**
     * Solve a Sudoku puzzle using backtracking
     * @param board 9x9 Sudoku grid (0 represents empty cells)
     * @return true if puzzle is solvable, false otherwise
     */
    public static boolean solveSudoku(int[][] board) {
        // Find next empty cell
        int[] emptyCell = findEmptyCell(board);
        if (emptyCell == null) {
            return true; // No empty cells, puzzle solved
        }
        
        int row = emptyCell[0];
        int col = emptyCell[1];
        
        // Try digits 1-9
        for (int digit = 1; digit <= 9; digit++) {
            if (isValidMove(board, row, col, digit)) {
                // Place digit
                board[row][col] = digit;
                
                // Recursively solve
                if (solveSudoku(board)) {
                    return true;
                }
                
                // Backtrack: remove digit
                board[row][col] = EMPTY;
            }
        }
        
        return false; // No solution found
    }
    
    /**
     * Find the next empty cell in the board
     * @param board Sudoku grid
     * @return [row, col] of empty cell, or null if no empty cells
     */
    private static int[] findEmptyCell(int[][] board) {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (board[row][col] == EMPTY) {
                    return new int[]{row, col};
                }
            }
        }
        return null;
    }
    
    /**
     * Check if placing digit at (row, col) is valid
     * @param board Sudoku grid
     * @param row Row index
     * @param col Column index
     * @param digit Digit to place (1-9)
     * @return true if move is valid, false otherwise
     */
    private static boolean isValidMove(int[][] board, int row, int col, int digit) {
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
     * Check if the current board state is valid
     * @param board Sudoku grid
     * @return true if valid, false otherwise
     */
    public static boolean isValidBoard(int[][] board) {
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
     * Print the Sudoku board
     * @param board Sudoku grid
     */
    public static void printBoard(int[][] board) {
        for (int row = 0; row < SIZE; row++) {
            if (row % 3 == 0 && row != 0) {
                System.out.println("------+-------+------");
            }
            
            for (int col = 0; col < SIZE; col++) {
                if (col % 3 == 0 && col != 0) {
                    System.out.print("| ");
                }
                
                if (board[row][col] == EMPTY) {
                    System.out.print(". ");
                } else {
                    System.out.print(board[row][col] + " ");
                }
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Create a deep copy of the board
     * @param original Original board
     * @return Copy of the board
     */
    private static int[][] copyBoard(int[][] original) {
        int[][] copy = new int[SIZE][SIZE];
        for (int i = 0; i < SIZE; i++) {
            System.arraycopy(original[i], 0, copy[i], 0, SIZE);
        }
        return copy;
    }
    
    /**
     * Enhanced solver with step tracking
     * @param board Sudoku grid
     * @param steps List to track solution steps
     * @return true if solvable, false otherwise
     */
    public static boolean solveSudokuWithSteps(int[][] board, List<String> steps) {
        int[] emptyCell = findEmptyCell(board);
        if (emptyCell == null) {
            steps.add("✓ Puzzle solved successfully!");
            return true;
        }
        
        int row = emptyCell[0];
        int col = emptyCell[1];
        
        steps.add(String.format("Trying to fill cell (%d, %d)", row + 1, col + 1));
        
        for (int digit = 1; digit <= 9; digit++) {
            if (isValidMove(board, row, col, digit)) {
                steps.add(String.format("  Trying digit %d at (%d, %d) - Valid!", digit, row + 1, col + 1));
                board[row][col] = digit;
                
                if (solveSudokuWithSteps(board, steps)) {
                    return true;
                }
                
                steps.add(String.format("  Backtracking from (%d, %d), removing %d", row + 1, col + 1, digit));
                board[row][col] = EMPTY;
            } else {
                steps.add(String.format("  Digit %d at (%d, %d) - Invalid (conflicts detected)", digit, row + 1, col + 1));
            }
        }
        
        return false;
    }
    
    /**
     * Find all possible values for a given cell
     * @param board Sudoku grid
     * @param row Row index
     * @param col Column index
     * @return Set of possible values
     */
    public static Set<Integer> getPossibleValues(int[][] board, int row, int col) {
        Set<Integer> possible = new HashSet<>();
        
        if (board[row][col] != EMPTY) {
            return possible; // Cell already filled
        }
        
        for (int digit = 1; digit <= 9; digit++) {
            if (isValidMove(board, row, col, digit)) {
                possible.add(digit);
            }
        }
        
        return possible;
    }
    
    /**
     * Enhanced solver using Most Constrained Variable (MCV) heuristic
     * @param board Sudoku grid
     * @return true if solvable, false otherwise
     */
    public static boolean solveSudokuMCV(int[][] board) {
        // Find empty cell with minimum possible values (Most Constrained Variable)
        int bestRow = -1, bestCol = -1;
        int minPossible = 10;
        
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (board[row][col] == EMPTY) {
                    Set<Integer> possible = getPossibleValues(board, row, col);
                    
                    if (possible.isEmpty()) {
                        return false; // No valid moves, backtrack
                    }
                    
                    if (possible.size() < minPossible) {
                        minPossible = possible.size();
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
        Set<Integer> possibleValues = getPossibleValues(board, bestRow, bestCol);
        for (int digit : possibleValues) {
            board[bestRow][bestCol] = digit;
            
            if (solveSudokuMCV(board)) {
                return true;
            }
            
            board[bestRow][bestCol] = EMPTY;
        }
        
        return false;
    }
    
    /**
     * Count the number of solutions for a given puzzle
     * @param board Sudoku grid
     * @return Number of solutions
     */
    public static int countSolutions(int[][] board) {
        return countSolutionsHelper(copyBoard(board));
    }
    
    /**
     * Helper method to count solutions
     */
    private static int countSolutionsHelper(int[][] board) {
        int[] emptyCell = findEmptyCell(board);
        if (emptyCell == null) {
            return 1; // Found a solution
        }
        
        int row = emptyCell[0];
        int col = emptyCell[1];
        int count = 0;
        
        for (int digit = 1; digit <= 9; digit++) {
            if (isValidMove(board, row, col, digit)) {
                board[row][col] = digit;
                count += countSolutionsHelper(board);
                board[row][col] = EMPTY;
            }
        }
        
        return count;
    }
    
    /**
     * Generate a random valid Sudoku puzzle
     * @param difficulty Number of cells to remove (higher = harder)
     * @return Generated puzzle
     */
    public static int[][] generatePuzzle(int difficulty) {
        int[][] board = new int[SIZE][SIZE];
        
        // Fill diagonal 3x3 boxes first (they don't interfere with each other)
        fillDiagonalBoxes(board);
        
        // Fill remaining cells
        solveSudoku(board);
        
        // Remove cells to create puzzle
        Random random = new Random();
        int cellsToRemove = Math.min(difficulty, SIZE * SIZE - 17); // At least 17 clues needed
        
        for (int i = 0; i < cellsToRemove; i++) {
            int row, col;
            do {
                row = random.nextInt(SIZE);
                col = random.nextInt(SIZE);
            } while (board[row][col] == EMPTY);
            
            int backup = board[row][col];
            board[row][col] = EMPTY;
            
            // Ensure puzzle still has unique solution
            if (countSolutions(board) != 1) {
                board[row][col] = backup; // Restore if multiple solutions
            }
        }
        
        return board;
    }
    
    /**
     * Fill diagonal 3x3 boxes with random valid numbers
     */
    private static void fillDiagonalBoxes(int[][] board) {
        for (int box = 0; box < SIZE; box += BOX_SIZE) {
            fillBox(board, box, box);
        }
    }
    
    /**
     * Fill a 3x3 box with random numbers
     */
    private static void fillBox(int[][] board, int startRow, int startCol) {
        List<Integer> numbers = new ArrayList<>();
        for (int i = 1; i <= 9; i++) {
            numbers.add(i);
        }
        Collections.shuffle(numbers);
        
        int index = 0;
        for (int row = startRow; row < startRow + BOX_SIZE; row++) {
            for (int col = startCol; col < startCol + BOX_SIZE; col++) {
                board[row][col] = numbers.get(index++);
            }
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Sudoku Solver - Backtracking ===\n");
        
        // Test Case 1: Easy puzzle
        System.out.println("Test Case 1: Easy Sudoku");
        int[][] easyPuzzle = {
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
        
        System.out.println("Original puzzle:");
        printBoard(easyPuzzle);
        
        int[][] easyCopy = copyBoard(easyPuzzle);
        boolean solved = solveSudoku(easyCopy);
        
        if (solved) {
            System.out.println("Solution:");
            printBoard(easyCopy);
        } else {
            System.out.println("No solution found.");
        }
        
        // Test Case 2: Hard puzzle
        System.out.println("Test Case 2: Hard Sudoku");
        int[][] hardPuzzle = {
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
        
        System.out.println("Original hard puzzle:");
        printBoard(hardPuzzle);
        
        int[][] hardCopy = copyBoard(hardPuzzle);
        long startTime = System.currentTimeMillis();
        boolean hardSolved = solveSudoku(hardCopy);
        long endTime = System.currentTimeMillis();
        
        if (hardSolved) {
            System.out.println("Solution (solved in " + (endTime - startTime) + "ms):");
            printBoard(hardCopy);
        }
        
        // Test Case 3: Step-by-step solving
        System.out.println("Test Case 3: Step-by-step solving (simple puzzle)");
        int[][] simplePuzzle = {
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
        
        List<String> steps = new ArrayList<>();
        int[][] stepCopy = copyBoard(simplePuzzle);
        solveSudokuWithSteps(stepCopy, steps);
        
        System.out.println("First 20 steps:");
        for (int i = 0; i < Math.min(20, steps.size()); i++) {
            System.out.println(steps.get(i));
        }
        if (steps.size() > 20) {
            System.out.println("... (" + (steps.size() - 20) + " more steps)");
        }
        System.out.println();
        
        // Test Case 4: Performance comparison
        System.out.println("Test Case 4: Performance comparison (Basic vs MCV)");
        
        // Create multiple copies for fair comparison
        int[][] perfPuzzle = copyBoard(hardPuzzle);
        int[][] mcvPuzzle = copyBoard(hardPuzzle);
        
        startTime = System.currentTimeMillis();
        solveSudoku(perfPuzzle);
        long basicTime = System.currentTimeMillis() - startTime;
        
        startTime = System.currentTimeMillis();
        solveSudokuMCV(mcvPuzzle);
        long mcvTime = System.currentTimeMillis() - startTime;
        
        System.out.println("Basic backtracking: " + basicTime + "ms");
        System.out.println("MCV heuristic: " + mcvTime + "ms");
        System.out.println("Speedup: " + (basicTime > 0 ? String.format("%.2fx", (double)basicTime / mcvTime) : "N/A"));
        System.out.println();
        
        // Test Case 5: Puzzle validation
        System.out.println("Test Case 5: Puzzle validation");
        
        System.out.println("Valid puzzle: " + isValidBoard(easyPuzzle));
        
        // Create invalid puzzle
        int[][] invalidPuzzle = copyBoard(easyPuzzle);
        invalidPuzzle[0][0] = 3; // Conflict with existing 3 in same row
        System.out.println("Invalid puzzle (duplicate in row): " + isValidBoard(invalidPuzzle));
        System.out.println();
        
        // Test Case 6: Multiple solutions
        System.out.println("Test Case 6: Solution counting");
        
        int[][] emptierPuzzle = {
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
        System.out.println("Number of solutions for modified puzzle: " + solutionCount);
        System.out.println();
        
        // Test Case 7: Possible values analysis
        System.out.println("Test Case 7: Constraint analysis");
        System.out.println("Possible values for empty cells in first row:");
        
        for (int col = 0; col < SIZE; col++) {
            if (easyPuzzle[0][col] == EMPTY) {
                Set<Integer> possible = getPossibleValues(easyPuzzle, 0, col);
                System.out.println("Cell (1, " + (col + 1) + "): " + possible);
            }
        }
        System.out.println();
        
        // Test Case 8: Generated puzzle
        System.out.println("Test Case 8: Generated puzzle");
        System.out.println("Generating puzzle with 50 empty cells...");
        
        int[][] generated = generatePuzzle(50);
        System.out.println("Generated puzzle:");
        printBoard(generated);
        
        int[][] generatedCopy = copyBoard(generated);
        boolean generatedSolved = solveSudoku(generatedCopy);
        System.out.println("Generated puzzle solvable: " + generatedSolved);
        
        if (generatedSolved) {
            System.out.println("Solution:");
            printBoard(generatedCopy);
        }
        
        // Test Case 9: Edge cases
        System.out.println("Test Case 9: Edge cases");
        
        // Empty board
        int[][] emptyBoard = new int[SIZE][SIZE];
        int[][] emptyCopy = copyBoard(emptyBoard);
        startTime = System.currentTimeMillis();
        boolean emptySolved = solveSudoku(emptyCopy);
        endTime = System.currentTimeMillis();
        
        System.out.println("Empty board solvable: " + emptySolved + " (Time: " + (endTime - startTime) + "ms)");
        
        // Nearly complete board
        int[][] nearComplete = copyBoard(easyCopy); // Use the solved easy puzzle
        nearComplete[8][8] = EMPTY; // Make one cell empty
        
        startTime = System.currentTimeMillis();
        boolean nearSolved = solveSudoku(nearComplete);
        endTime = System.currentTimeMillis();
        
        System.out.println("Nearly complete board solvable: " + nearSolved + " (Time: " + (endTime - startTime) + "ms)");
        
        System.out.println("\nComplexity Analysis:");
        System.out.println("- Time: O(9^(n*n)) worst case, where n is empty cells");
        System.out.println("- Space: O(n*n) for board and recursion stack");
        System.out.println("- Optimizations: MCV heuristic, constraint propagation");
        System.out.println("- Pruning: early constraint checking prevents invalid branches");
    }
}