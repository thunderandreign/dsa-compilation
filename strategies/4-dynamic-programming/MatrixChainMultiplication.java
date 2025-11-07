/**
 * Dynamic Programming Strategy: Matrix Chain Multiplication
 * Core Idea: Find the optimal way to parenthesize matrix multiplication to minimize scalar multiplications
 * Time Complexity: O(n³) where n is the number of matrices
 * Space Complexity: O(n²) for storing optimal costs and split points
 */

import java.util.*;

public class MatrixChainMultiplication {
    
    /**
     * Matrix class to represent matrix dimensions
     */
    static class Matrix {
        int rows;
        int cols;
        String name;
        
        Matrix(int rows, int cols, String name) {
            this.rows = rows;
            this.cols = cols;
            this.name = name;
        }
        
        @Override
        public String toString() {
            return String.format("%s(%dx%d)", name, rows, cols);
        }
    }
    
    /**
     * Find minimum number of scalar multiplications needed to multiply chain of matrices
     * @param dimensions Array where dimensions[i] represents rows of matrix i and cols of matrix i-1
     * @return Minimum number of scalar multiplications
     */
    public static int matrixChainOrder(int[] dimensions) {
        int n = dimensions.length - 1; // Number of matrices
        
        // dp[i][j] = minimum cost to multiply matrices from i to j
        int[][] dp = new int[n][n];
        
        // Cost is zero when multiplying one matrix (diagonal)
        for (int i = 0; i < n; i++) {
            dp[i][i] = 0;
        }
        
        // Fill table for chains of increasing length
        for (int chainLength = 2; chainLength <= n; chainLength++) {
            for (int i = 0; i <= n - chainLength; i++) {
                int j = i + chainLength - 1;
                dp[i][j] = Integer.MAX_VALUE;
                
                // Try all possible split points k
                for (int k = i; k < j; k++) {
                    // Cost = left chain + right chain + cost of multiplying results
                    int cost = dp[i][k] + dp[k + 1][j] + dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                    dp[i][j] = Math.min(dp[i][j], cost);
                }
            }
        }
        
        return dp[0][n - 1];
    }
    
    /**
     * Find optimal parenthesization along with minimum cost
     * @param dimensions Array of matrix dimensions
     * @return Result containing minimum cost and optimal parenthesization
     */
    public static MatrixChainResult matrixChainOrderWithParentheses(int[] dimensions) {
        int n = dimensions.length - 1;
        int[][] dp = new int[n][n];
        int[][] split = new int[n][n]; // To store optimal split points
        
        // Initialize diagonal to 0
        for (int i = 0; i < n; i++) {
            dp[i][i] = 0;
        }
        
        // Fill table for chains of increasing length
        for (int chainLength = 2; chainLength <= n; chainLength++) {
            for (int i = 0; i <= n - chainLength; i++) {
                int j = i + chainLength - 1;
                dp[i][j] = Integer.MAX_VALUE;
                
                for (int k = i; k < j; k++) {
                    int cost = dp[i][k] + dp[k + 1][j] + dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                    if (cost < dp[i][j]) {
                        dp[i][j] = cost;
                        split[i][j] = k; // Remember optimal split point
                    }
                }
            }
        }
        
        String parentheses = getOptimalParentheses(split, 0, n - 1);
        return new MatrixChainResult(dp[0][n - 1], parentheses, dp, split);
    }
    
    /**
     * Recursively construct optimal parenthesization string
     */
    private static String getOptimalParentheses(int[][] split, int i, int j) {
        if (i == j) {
            return "M" + i;
        } else {
            return "(" + getOptimalParentheses(split, i, split[i][j]) + 
                   " × " + getOptimalParentheses(split, split[i][j] + 1, j) + ")";
        }
    }
    
    /**
     * Result class to hold matrix chain multiplication solution
     */
    static class MatrixChainResult {
        int minCost;
        String optimalParentheses;
        int[][] dpTable;
        int[][] splitTable;
        
        MatrixChainResult(int minCost, String optimalParentheses, int[][] dpTable, int[][] splitTable) {
            this.minCost = minCost;
            this.optimalParentheses = optimalParentheses;
            this.dpTable = dpTable;
            this.splitTable = splitTable;
        }
    }
    
    /**
     * Print the DP table for educational purposes
     */
    public static void printDPTable(int[][] dp, int n) {
        System.out.println("DP Table (minimum costs):");
        System.out.print("   ");
        for (int j = 0; j < n; j++) {
            System.out.printf("%8d", j);
        }
        System.out.println();
        
        for (int i = 0; i < n; i++) {
            System.out.printf("%2d:", i);
            for (int j = 0; j < n; j++) {
                if (i <= j) {
                    System.out.printf("%8d", dp[i][j]);
                } else {
                    System.out.print("       -");
                }
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Print the split table
     */
    public static void printSplitTable(int[][] split, int n) {
        System.out.println("Split Table (optimal k values):");
        System.out.print("   ");
        for (int j = 0; j < n; j++) {
            System.out.printf("%4d", j);
        }
        System.out.println();
        
        for (int i = 0; i < n; i++) {
            System.out.printf("%2d:", i);
            for (int j = 0; j < n; j++) {
                if (i < j) {
                    System.out.printf("%4d", split[i][j]);
                } else {
                    System.out.print("   -");
                }
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Demonstrate step-by-step matrix chain multiplication
     */
    public static void demonstrateMatrixChain(int[] dimensions, Matrix[] matrices) {
        int n = dimensions.length - 1;
        System.out.println("Step-by-step Matrix Chain Multiplication:");
        System.out.println("Matrices:");
        for (int i = 0; i < matrices.length; i++) {
            System.out.printf("  %s: %dx%d\n", matrices[i].name, matrices[i].rows, matrices[i].cols);
        }
        System.out.println();
        
        int[][] dp = new int[n][n];
        int[][] split = new int[n][n];
        
        // Initialize diagonal
        for (int i = 0; i < n; i++) {
            dp[i][i] = 0;
        }
        
        System.out.println("Recurrence relation:");
        System.out.println("dp[i][j] = min(dp[i][k] + dp[k+1][j] + p[i]*p[k+1]*p[j+1]) for all k from i to j-1");
        System.out.println("where p[i] represents the row dimension of matrix i");
        System.out.println();
        
        // Fill table for chains of increasing length
        for (int chainLength = 2; chainLength <= n; chainLength++) {
            System.out.printf("Chain length %d:\n", chainLength);
            
            for (int i = 0; i <= n - chainLength; i++) {
                int j = i + chainLength - 1;
                dp[i][j] = Integer.MAX_VALUE;
                
                System.out.printf("  Computing dp[%d][%d] (matrices %s to %s):\n", 
                                i, j, matrices[i].name, matrices[j].name);
                
                for (int k = i; k < j; k++) {
                    int cost = dp[i][k] + dp[k + 1][j] + dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                    System.out.printf("    k=%d: %d + %d + %d*%d*%d = %d + %d + %d = %d\n", 
                                    k, dp[i][k], dp[k + 1][j], 
                                    dimensions[i], dimensions[k + 1], dimensions[j + 1],
                                    dp[i][k], dp[k + 1][j], dimensions[i] * dimensions[k + 1] * dimensions[j + 1], cost);
                    
                    if (cost < dp[i][j]) {
                        dp[i][j] = cost;
                        split[i][j] = k;
                    }
                }
                System.out.printf("    Minimum: %d (split at k=%d)\n", dp[i][j], split[i][j]);
            }
            System.out.println();
        }
        
        System.out.printf("Final minimum cost: %d\n", dp[0][n - 1]);
        String parentheses = getOptimalParentheses(split, 0, n - 1);
        System.out.printf("Optimal parenthesization: %s\n", parentheses);
    }
    
    /**
     * Compare different parenthesizations
     */
    public static void compareParenthesizations(int[] dimensions, Matrix[] matrices) {
        System.out.println("Comparison of different parenthesizations:");
        int n = dimensions.length - 1;
        
        if (n == 3) {
            // For 3 matrices, there are 2 ways to parenthesize
            System.out.println("Matrix chain: " + Arrays.toString(matrices));
            
            // Left associative: ((M0 × M1) × M2)
            int leftCost = dimensions[0] * dimensions[1] * dimensions[2] + // M0 × M1
                          dimensions[0] * dimensions[2] * dimensions[3];   // Result × M2
            System.out.printf("Left associative ((M0 × M1) × M2): %d operations\n", leftCost);
            
            // Right associative: (M0 × (M1 × M2))
            int rightCost = dimensions[1] * dimensions[2] * dimensions[3] + // M1 × M2
                           dimensions[0] * dimensions[1] * dimensions[3];   // M0 × Result
            System.out.printf("Right associative (M0 × (M1 × M2)): %d operations\n", rightCost);
            
            int optimalCost = matrixChainOrder(dimensions);
            System.out.printf("Dynamic Programming optimal: %d operations\n", optimalCost);
            
            if (leftCost < rightCost) {
                System.out.println("Left associative is better by " + (rightCost - leftCost) + " operations");
            } else if (rightCost < leftCost) {
                System.out.println("Right associative is better by " + (leftCost - rightCost) + " operations");
            } else {
                System.out.println("Both parenthesizations have the same cost");
            }
        }
    }
    
    /**
     * Calculate the cost of a specific parenthesization (brute force for comparison)
     */
    public static int calculateParenthesizationCost(int[] dimensions, String parentheses) {
        // This is a simplified version - in practice, parsing parentheses is complex
        System.out.println("Parenthesization cost calculation for: " + parentheses);
        return -1; // Placeholder
    }
    
    public static void main(String[] args) {
        System.out.println("=== Matrix Chain Multiplication - Dynamic Programming ===");
        
        // Test Case 1: Classic 4-matrix example
        System.out.println("Test Case 1: Classic 4-matrix chain");
        Matrix[] matrices1 = {
            new Matrix(40, 20, "M0"),
            new Matrix(20, 30, "M1"),
            new Matrix(30, 10, "M2"),
            new Matrix(10, 30, "M3")
        };
        int[] dimensions1 = {40, 20, 30, 10, 30}; // dimensions[i] = rows of matrix i
        
        int minCost1 = matrixChainOrder(dimensions1);
        MatrixChainResult result1 = matrixChainOrderWithParentheses(dimensions1);
        
        System.out.println("Matrices: " + Arrays.toString(matrices1));
        System.out.println("Dimensions array: " + Arrays.toString(dimensions1));
        System.out.printf("Minimum scalar multiplications: %d\n", minCost1);
        System.out.printf("Optimal parenthesization: %s\n", result1.optimalParentheses);
        System.out.println();
        
        printDPTable(result1.dpTable, matrices1.length);
        printSplitTable(result1.splitTable, matrices1.length);
        
        // Test Case 2: Step-by-step demonstration
        System.out.println("Test Case 2: Step-by-step construction");
        Matrix[] matrices2 = {
            new Matrix(5, 10, "A"),
            new Matrix(10, 3, "B"),
            new Matrix(3, 12, "C")
        };
        int[] dimensions2 = {5, 10, 3, 12};
        
        demonstrateMatrixChain(dimensions2, matrices2);
        System.out.println();
        
        // Test Case 3: Compare different parenthesizations
        System.out.println("Test Case 3: Parenthesization comparison");
        compareParenthesizations(dimensions2, matrices2);
        System.out.println();
        
        // Test Case 4: Edge case - 2 matrices
        System.out.println("Test Case 4: Two matrices only");
        Matrix[] matrices3 = {
            new Matrix(10, 20, "X"),
            new Matrix(20, 30, "Y")
        };
        int[] dimensions3 = {10, 20, 30};
        
        int minCost3 = matrixChainOrder(dimensions3);
        MatrixChainResult result3 = matrixChainOrderWithParentheses(dimensions3);
        
        System.out.println("Matrices: " + Arrays.toString(matrices3));
        System.out.printf("Cost: %d (only one way to multiply)\n", minCost3);
        System.out.printf("Parenthesization: %s\n", result3.optimalParentheses);
        System.out.println();
        
        // Test Case 5: Large chain
        System.out.println("Test Case 5: Larger matrix chain");
        Matrix[] matrices4 = {
            new Matrix(2, 3, "P"),
            new Matrix(3, 6, "Q"),
            new Matrix(6, 4, "R"),
            new Matrix(4, 5, "S"),
            new Matrix(5, 2, "T")
        };
        int[] dimensions4 = {2, 3, 6, 4, 5, 2};
        
        MatrixChainResult result4 = matrixChainOrderWithParentheses(dimensions4);
        
        System.out.println("Matrices: " + Arrays.toString(matrices4));
        System.out.printf("Minimum cost: %d\n", result4.minCost);
        System.out.printf("Optimal parenthesization: %s\n", result4.optimalParentheses);
        System.out.println();
        
        // Test Case 6: Square matrices
        System.out.println("Test Case 6: Chain of square matrices");
        Matrix[] matrices5 = {
            new Matrix(10, 10, "S1"),
            new Matrix(10, 10, "S2"),
            new Matrix(10, 10, "S3"),
            new Matrix(10, 10, "S4")
        };
        int[] dimensions5 = {10, 10, 10, 10, 10};
        
        MatrixChainResult result5 = matrixChainOrderWithParentheses(dimensions5);
        
        System.out.println("Matrices: " + Arrays.toString(matrices5));
        System.out.printf("Minimum cost: %d\n", result5.minCost);
        System.out.printf("Optimal parenthesization: %s\n", result5.optimalParentheses);
        System.out.println("Note: For same-sized square matrices, all parenthesizations have equal cost");
        System.out.println();
        
        // Test Case 7: Demonstrate why order matters
        System.out.println("Test Case 7: Why matrix multiplication order matters");
        Matrix[] matrices6 = {
            new Matrix(1000, 1, "Tall"),
            new Matrix(1, 1000, "Wide"),
            new Matrix(1000, 1, "Tall2")
        };
        int[] dimensions6 = {1000, 1, 1000, 1};
        
        MatrixChainResult result6 = matrixChainOrderWithParentheses(dimensions6);
        
        System.out.println("Matrices: " + Arrays.toString(matrices6));
        System.out.printf("Optimal cost: %d\n", result6.minCost);
        System.out.printf("Optimal parenthesization: %s\n", result6.optimalParentheses);
        
        // Manual calculation for comparison
        int leftAssociative = 1000 * 1 * 1000 + 1000 * 1000 * 1; // ((Tall × Wide) × Tall2)
        int rightAssociative = 1 * 1000 * 1 + 1000 * 1 * 1;       // (Tall × (Wide × Tall2))
        System.out.printf("Left associative cost: %d\n", leftAssociative);
        System.out.printf("Right associative cost: %d\n", rightAssociative);
        System.out.printf("Savings with optimal order: %d operations\n", 
                         Math.max(leftAssociative, rightAssociative) - result6.minCost);
        System.out.println("This demonstrates the dramatic impact of proper parenthesization!");
    }
}