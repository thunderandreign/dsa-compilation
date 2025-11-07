/**
 * Backtracking Strategy: N-Queens Problem
 * Core Idea: Place N queens on an N×N chessboard such that no two queens attack each other
 * Time Complexity: O(N!) - exponential as we try all possible placements
 * Space Complexity: O(N) for recursion stack and board representation
 */

import java.util.*;

public class NQueens {
    
    /**
     * Solve N-Queens problem and return all possible solutions
     * @param n Size of the chessboard (n x n) and number of queens
     * @return List of all valid solutions, each represented as board configuration
     */
    public static List<List<String>> solveNQueens(int n) {
        List<List<String>> solutions = new ArrayList<>();
        int[] queens = new int[n]; // queens[i] = column position of queen in row i
        Arrays.fill(queens, -1);
        
        backtrack(queens, 0, n, solutions);
        return solutions;
    }
    
    /**
     * Backtracking helper function
     * @param queens Array representing queen positions (index = row, value = column)
     * @param row Current row to place a queen
     * @param n Board size
     * @param solutions List to store all valid solutions
     */
    private static void backtrack(int[] queens, int row, int n, List<List<String>> solutions) {
        // Base case: all queens placed successfully
        if (row == n) {
            solutions.add(constructBoard(queens, n));
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
     * Check if placing a queen at (row, col) is valid
     * @param queens Current queen positions
     * @param row Row to check
     * @param col Column to check
     * @return true if position is safe, false otherwise
     */
    private static boolean isValid(int[] queens, int row, int col) {
        for (int i = 0; i < row; i++) {
            int placedCol = queens[i];
            
            // Check column conflict
            if (placedCol == col) {
                return false;
            }
            
            // Check diagonal conflicts
            // Two queens are on the same diagonal if |row1-row2| = |col1-col2|
            if (Math.abs(i - row) == Math.abs(placedCol - col)) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * Construct board representation from queen positions
     * @param queens Array of queen positions
     * @param n Board size
     * @return Board as list of strings
     */
    private static List<String> constructBoard(int[] queens, int n) {
        List<String> board = new ArrayList<>();
        for (int i = 0; i < n; i++) {
            StringBuilder row = new StringBuilder();
            for (int j = 0; j < n; j++) {
                if (queens[i] == j) {
                    row.append('Q');
                } else {
                    row.append('.');
                }
            }
            board.add(row.toString());
        }
        return board;
    }
    
    /**
     * Print a single solution
     * @param solution Board configuration
     * @param solutionNumber Solution index
     */
    private static void printSolution(List<String> solution, int solutionNumber) {
        System.out.println("Solution " + solutionNumber + ":");
        for (String row : solution) {
            System.out.println(row);
        }
        System.out.println();
    }
    
    /**
     * Count total number of solutions without storing them (memory efficient)
     * @param n Board size
     * @return Number of valid solutions
     */
    public static int countNQueens(int n) {
        int[] queens = new int[n];
        Arrays.fill(queens, -1);
        return countBacktrack(queens, 0, n);
    }
    
    /**
     * Backtracking helper for counting solutions
     */
    private static int countBacktrack(int[] queens, int row, int n) {
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
     * Optimized N-Queens using bit manipulation for faster conflict checking
     * @param n Board size
     * @return Number of solutions
     */
    public static int solveNQueensBitwise(int n) {
        return backtrackBitwise(0, 0, 0, 0, n);
    }
    
    /**
     * Bitwise backtracking implementation
     * @param row Current row
     * @param cols Bitmask for occupied columns
     * @param diag1 Bitmask for occupied main diagonals
     * @param diag2 Bitmask for occupied anti-diagonals
     * @param n Board size
     * @return Number of solutions
     */
    private static int backtrackBitwise(int row, int cols, int diag1, int diag2, int n) {
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
     * Demonstrate step-by-step solution construction
     * @param n Board size
     */
    public static void demonstrateSteps(int n) {
        System.out.println("Step-by-step N-Queens solution for " + n + "x" + n + " board:");
        int[] queens = new int[n];
        Arrays.fill(queens, -1);
        
        demonstrateBacktrack(queens, 0, n, 1);
    }
    
    /**
     * Helper method to show backtracking steps
     */
    private static boolean demonstrateBacktrack(int[] queens, int row, int n, int step) {
        System.out.println("Step " + step + ": Trying to place queen in row " + row);
        
        if (row == n) {
            System.out.println("✓ All queens placed successfully!");
            printBoard(queens, n);
            return true;
        }
        
        for (int col = 0; col < n; col++) {
            System.out.println("  Trying column " + col + "...");
            
            if (isValid(queens, row, col)) {
                System.out.println("  ✓ Position (" + row + ", " + col + ") is safe");
                queens[row] = col;
                printBoard(queens, n);
                
                if (demonstrateBacktrack(queens, row + 1, n, step + 1)) {
                    return true; // Solution found
                }
                
                System.out.println("  ✗ Backtracking from (" + row + ", " + col + ")");
                queens[row] = -1;
            } else {
                System.out.println("  ✗ Position (" + row + ", " + col + ") conflicts with existing queens");
            }
        }
        
        return false;
    }
    
    /**
     * Print current board state
     */
    private static void printBoard(int[] queens, int n) {
        System.out.println("Current board:");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (queens[i] == j) {
                    System.out.print("Q ");
                } else if (queens[i] == -1) {
                    System.out.print(". ");
                } else {
                    System.out.print(". ");
                }
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Analyze conflict patterns
     */
    public static void analyzeConflicts(int n) {
        System.out.println("Conflict Analysis for " + n + "-Queens:");
        
        // Show what positions conflict with a queen at (0, 0)
        System.out.println("Positions that conflict with queen at (0, 0):");
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                boolean conflict = false;
                
                // Same row
                if (i == 0) conflict = true;
                
                // Same column  
                if (j == 0) conflict = true;
                
                // Main diagonal (row - col = constant)
                if (i - j == 0 - 0) conflict = true;
                
                // Anti-diagonal (row + col = constant)
                if (i + j == 0 + 0) conflict = true;
                
                System.out.print(conflict ? "X " : ". ");
            }
            System.out.println();
        }
        System.out.println();
    }
    
    public static void main(String[] args) {
        System.out.println("=== N-Queens Problem - Backtracking ===\n");
        
        // Test Case 1: Small board (4x4)
        System.out.println("Test Case 1: 4-Queens");
        List<List<String>> solutions4 = solveNQueens(4);
        System.out.println("Number of solutions: " + solutions4.size());
        
        for (int i = 0; i < solutions4.size(); i++) {
            printSolution(solutions4.get(i), i + 1);
        }
        
        // Test Case 2: Classic 8-Queens
        System.out.println("Test Case 2: 8-Queens");
        int count8 = countNQueens(8);
        System.out.println("Number of solutions for 8-Queens: " + count8);
        
        // Show first solution only (to avoid too much output)
        List<List<String>> solutions8 = solveNQueens(8);
        if (!solutions8.isEmpty()) {
            System.out.println("First solution:");
            printSolution(solutions8.get(0), 1);
        }
        
        // Test Case 3: Step-by-step demonstration for small board
        System.out.println("Test Case 3: Step-by-step solution");
        demonstrateSteps(4);
        
        // Test Case 4: Performance comparison
        System.out.println("Test Case 4: Performance comparison");
        
        for (int n = 1; n <= 12; n++) {
            long startTime = System.currentTimeMillis();
            int count = countNQueens(n);
            long endTime = System.currentTimeMillis();
            
            long startTimeBit = System.currentTimeMillis();
            int countBit = solveNQueensBitwise(n);
            long endTimeBit = System.currentTimeMillis();
            
            System.out.printf("%d-Queens: %d solutions (Standard: %dms, Bitwise: %dms) [Verification: %s]\n", 
                            n, count, endTime - startTime, endTimeBit - startTimeBit, 
                            count == countBit ? "✓" : "✗");
        }
        System.out.println();
        
        // Test Case 5: Conflict analysis
        System.out.println("Test Case 5: Understanding conflicts");
        analyzeConflicts(5);
        
        // Test Case 6: Edge cases
        System.out.println("Test Case 6: Edge cases");
        
        // 1x1 board
        List<List<String>> solutions1 = solveNQueens(1);
        System.out.println("1-Queens: " + solutions1.size() + " solution");
        if (!solutions1.isEmpty()) {
            printSolution(solutions1.get(0), 1);
        }
        
        // 2x2 and 3x3 boards (no solutions)
        System.out.println("2-Queens: " + countNQueens(2) + " solutions (impossible)");
        System.out.println("3-Queens: " + countNQueens(3) + " solutions (impossible)");
        
        // Test Case 7: Mathematical insights
        System.out.println("Test Case 7: Mathematical insights");
        System.out.println("N-Queens solutions count follows no simple formula.");
        System.out.println("Known values:");
        int[] knownCounts = {1, 0, 0, 2, 10, 4, 40, 92, 352, 724, 2680, 14200, 73712};
        for (int i = 1; i < knownCounts.length; i++) {
            System.out.println("N=" + i + ": " + knownCounts[i] + " solutions");
        }
        
        System.out.println("\nComplexity Analysis:");
        System.out.println("- Time: O(N!) in worst case - we try all possible arrangements");
        System.out.println("- Space: O(N) for recursion stack and queen positions");
        System.out.println("- Optimizations: bitwise operations, constraint propagation");
        System.out.println("- Pruning: early rejection of invalid partial solutions");
    }
}