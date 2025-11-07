#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * Dynamic Programming Strategy: Matrix Chain Multiplication
 * Core Idea: Find the optimal way to parenthesize matrix multiplication to minimize scalar multiplications
 * Time Complexity: O(n³) where n is the number of matrices
 * Space Complexity: O(n²) for storing optimal costs and split points
 */

#define MAX_MATRICES 100

/**
 * Matrix structure to represent matrix dimensions
 */
typedef struct {
    int rows;
    int cols;
    char name[10];
} Matrix;

/**
 * Result structure to hold matrix chain multiplication solution
 */
typedef struct {
    int minCost;
    int dpTable[MAX_MATRICES][MAX_MATRICES];
    int splitTable[MAX_MATRICES][MAX_MATRICES];
} MatrixChainResult;

/**
 * Find minimum of two integers
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * Find minimum number of scalar multiplications needed to multiply chain of matrices
 * @param dimensions Array where dimensions[i] represents rows of matrix i and cols of matrix i-1
 * @param n Number of matrices
 * @return Minimum number of scalar multiplications
 */
int matrixChainOrder(int dimensions[], int n) {
    // dp[i][j] = minimum cost to multiply matrices from i to j
    int dp[MAX_MATRICES][MAX_MATRICES];
    
    // Cost is zero when multiplying one matrix (diagonal)
    for (int i = 0; i < n; i++) {
        dp[i][i] = 0;
    }
    
    // Fill table for chains of increasing length
    for (int chainLength = 2; chainLength <= n; chainLength++) {
        for (int i = 0; i <= n - chainLength; i++) {
            int j = i + chainLength - 1;
            dp[i][j] = INT_MAX;
            
            // Try all possible split points k
            for (int k = i; k < j; k++) {
                // Cost = left chain + right chain + cost of multiplying results
                int cost = dp[i][k] + dp[k + 1][j] + dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                dp[i][j] = min(dp[i][j], cost);
            }
        }
    }
    
    return dp[0][n - 1];
}

/**
 * Find optimal parenthesization along with minimum cost
 * @param dimensions Array of matrix dimensions
 * @param n Number of matrices
 * @param result Result structure to store solution
 */
void matrixChainOrderWithParentheses(int dimensions[], int n, MatrixChainResult* result) {
    // Initialize tables
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            result->dpTable[i][j] = 0;
            result->splitTable[i][j] = 0;
        }
    }
    
    // Fill table for chains of increasing length
    for (int chainLength = 2; chainLength <= n; chainLength++) {
        for (int i = 0; i <= n - chainLength; i++) {
            int j = i + chainLength - 1;
            result->dpTable[i][j] = INT_MAX;
            
            for (int k = i; k < j; k++) {
                int cost = result->dpTable[i][k] + result->dpTable[k + 1][j] + 
                          dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                if (cost < result->dpTable[i][j]) {
                    result->dpTable[i][j] = cost;
                    result->splitTable[i][j] = k; // Remember optimal split point
                }
            }
        }
    }
    
    result->minCost = result->dpTable[0][n - 1];
}

/**
 * Recursively construct optimal parenthesization string
 */
void getOptimalParentheses(int split[][MAX_MATRICES], int i, int j, char* result) {
    if (i == j) {
        char temp[10];
        sprintf(temp, "M%d", i);
        strcat(result, temp);
    } else {
        strcat(result, "(");
        getOptimalParentheses(split, i, split[i][j], result);
        strcat(result, " × ");
        getOptimalParentheses(split, split[i][j] + 1, j, result);
        strcat(result, ")");
    }
}

/**
 * Print the DP table for educational purposes
 */
void printDPTable(int dp[][MAX_MATRICES], int n) {
    printf("DP Table (minimum costs):\n");
    printf("   ");
    for (int j = 0; j < n; j++) {
        printf("%8d", j);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        printf("%2d:", i);
        for (int j = 0; j < n; j++) {
            if (i <= j) {
                printf("%8d", dp[i][j]);
            } else {
                printf("       -");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Print the split table
 */
void printSplitTable(int split[][MAX_MATRICES], int n) {
    printf("Split Table (optimal k values):\n");
    printf("   ");
    for (int j = 0; j < n; j++) {
        printf("%4d", j);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        printf("%2d:", i);
        for (int j = 0; j < n; j++) {
            if (i < j) {
                printf("%4d", split[i][j]);
            } else {
                printf("   -");
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Print matrix information
 */
void printMatrices(Matrix matrices[], int n) {
    printf("Matrices:\n");
    for (int i = 0; i < n; i++) {
        printf("  %s: %dx%d\n", matrices[i].name, matrices[i].rows, matrices[i].cols);
    }
}

/**
 * Demonstrate step-by-step matrix chain multiplication
 */
void demonstrateMatrixChain(int dimensions[], Matrix matrices[], int n) {
    printf("Step-by-step Matrix Chain Multiplication:\n");
    printMatrices(matrices, n);
    printf("\n");
    
    int dp[MAX_MATRICES][MAX_MATRICES];
    int split[MAX_MATRICES][MAX_MATRICES];
    
    // Initialize diagonal
    for (int i = 0; i < n; i++) {
        dp[i][i] = 0;
    }
    
    printf("Recurrence relation:\n");
    printf("dp[i][j] = min(dp[i][k] + dp[k+1][j] + p[i]*p[k+1]*p[j+1]) for all k from i to j-1\n");
    printf("where p[i] represents the row dimension of matrix i\n");
    printf("\n");
    
    // Fill table for chains of increasing length
    for (int chainLength = 2; chainLength <= n; chainLength++) {
        printf("Chain length %d:\n", chainLength);
        
        for (int i = 0; i <= n - chainLength; i++) {
            int j = i + chainLength - 1;
            dp[i][j] = INT_MAX;
            
            printf("  Computing dp[%d][%d] (matrices %s to %s):\n", 
                   i, j, matrices[i].name, matrices[j].name);
            
            for (int k = i; k < j; k++) {
                int cost = dp[i][k] + dp[k + 1][j] + dimensions[i] * dimensions[k + 1] * dimensions[j + 1];
                printf("    k=%d: %d + %d + %d*%d*%d = %d + %d + %d = %d\n", 
                       k, dp[i][k], dp[k + 1][j], 
                       dimensions[i], dimensions[k + 1], dimensions[j + 1],
                       dp[i][k], dp[k + 1][j], dimensions[i] * dimensions[k + 1] * dimensions[j + 1], cost);
                
                if (cost < dp[i][j]) {
                    dp[i][j] = cost;
                    split[i][j] = k;
                }
            }
            printf("    Minimum: %d (split at k=%d)\n", dp[i][j], split[i][j]);
        }
        printf("\n");
    }
    
    printf("Final minimum cost: %d\n", dp[0][n - 1]);
    char parentheses[1000] = "";
    getOptimalParentheses(split, 0, n - 1, parentheses);
    printf("Optimal parenthesization: %s\n", parentheses);
}

/**
 * Compare different parenthesizations for 3 matrices
 */
void compareParenthesizations(int dimensions[], Matrix matrices[], int n) {
    printf("Comparison of different parenthesizations:\n");
    
    if (n == 3) {
        printf("Matrix chain: ");
        for (int i = 0; i < n; i++) {
            printf("%s", matrices[i].name);
            if (i < n - 1) printf(" × ");
        }
        printf("\n");
        
        // Left associative: ((M0 × M1) × M2)
        int leftCost = dimensions[0] * dimensions[1] * dimensions[2] + // M0 × M1
                      dimensions[0] * dimensions[2] * dimensions[3];   // Result × M2
        printf("Left associative ((%s × %s) × %s): %d operations\n", 
               matrices[0].name, matrices[1].name, matrices[2].name, leftCost);
        
        // Right associative: (M0 × (M1 × M2))
        int rightCost = dimensions[1] * dimensions[2] * dimensions[3] + // M1 × M2
                       dimensions[0] * dimensions[1] * dimensions[3];   // M0 × Result
        printf("Right associative (%s × (%s × %s)): %d operations\n", 
               matrices[0].name, matrices[1].name, matrices[2].name, rightCost);
        
        int optimalCost = matrixChainOrder(dimensions, n);
        printf("Dynamic Programming optimal: %d operations\n", optimalCost);
        
        if (leftCost < rightCost) {
            printf("Left associative is better by %d operations\n", rightCost - leftCost);
        } else if (rightCost < leftCost) {
            printf("Right associative is better by %d operations\n", leftCost - rightCost);
        } else {
            printf("Both parenthesizations have the same cost\n");
        }
    }
}

int main() {
    printf("=== Matrix Chain Multiplication - Dynamic Programming ===\n");
    
    // Test Case 1: Classic 4-matrix example
    printf("Test Case 1: Classic 4-matrix chain\n");
    Matrix matrices1[] = {
        {40, 20, "M0"},
        {20, 30, "M1"},
        {30, 10, "M2"},
        {10, 30, "M3"}
    };
    int dimensions1[] = {40, 20, 30, 10, 30}; // dimensions[i] = rows of matrix i
    int n1 = 4;
    
    int minCost1 = matrixChainOrder(dimensions1, n1);
    MatrixChainResult result1;
    matrixChainOrderWithParentheses(dimensions1, n1, &result1);
    
    printMatrices(matrices1, n1);
    printf("Dimensions array: [");
    for (int i = 0; i <= n1; i++) {
        printf("%d", dimensions1[i]);
        if (i < n1) printf(", ");
    }
    printf("]\n");
    printf("Minimum scalar multiplications: %d\n", minCost1);
    
    char parentheses1[1000] = "";
    getOptimalParentheses(result1.splitTable, 0, n1 - 1, parentheses1);
    printf("Optimal parenthesization: %s\n", parentheses1);
    printf("\n");
    
    printDPTable(result1.dpTable, n1);
    printSplitTable(result1.splitTable, n1);
    
    // Test Case 2: Step-by-step demonstration
    printf("Test Case 2: Step-by-step construction\n");
    Matrix matrices2[] = {
        {5, 10, "A"},
        {10, 3, "B"},
        {3, 12, "C"}
    };
    int dimensions2[] = {5, 10, 3, 12};
    int n2 = 3;
    
    demonstrateMatrixChain(dimensions2, matrices2, n2);
    printf("\n");
    
    // Test Case 3: Compare different parenthesizations
    printf("Test Case 3: Parenthesization comparison\n");
    compareParenthesizations(dimensions2, matrices2, n2);
    printf("\n");
    
    // Test Case 4: Edge case - 2 matrices
    printf("Test Case 4: Two matrices only\n");
    Matrix matrices3[] = {
        {10, 20, "X"},
        {20, 30, "Y"}
    };
    int dimensions3[] = {10, 20, 30};
    int n3 = 2;
    
    int minCost3 = matrixChainOrder(dimensions3, n3);
    MatrixChainResult result3;
    matrixChainOrderWithParentheses(dimensions3, n3, &result3);
    
    printMatrices(matrices3, n3);
    printf("Cost: %d (only one way to multiply)\n", minCost3);
    
    char parentheses3[1000] = "";
    getOptimalParentheses(result3.splitTable, 0, n3 - 1, parentheses3);
    printf("Parenthesization: %s\n", parentheses3);
    printf("\n");
    
    // Test Case 5: Large chain
    printf("Test Case 5: Larger matrix chain\n");
    Matrix matrices4[] = {
        {2, 3, "P"},
        {3, 6, "Q"},
        {6, 4, "R"},
        {4, 5, "S"},
        {5, 2, "T"}
    };
    int dimensions4[] = {2, 3, 6, 4, 5, 2};
    int n4 = 5;
    
    MatrixChainResult result4;
    matrixChainOrderWithParentheses(dimensions4, n4, &result4);
    
    printMatrices(matrices4, n4);
    printf("Minimum cost: %d\n", result4.minCost);
    
    char parentheses4[1000] = "";
    getOptimalParentheses(result4.splitTable, 0, n4 - 1, parentheses4);
    printf("Optimal parenthesization: %s\n", parentheses4);
    printf("\n");
    
    // Test Case 6: Square matrices
    printf("Test Case 6: Chain of square matrices\n");
    Matrix matrices5[] = {
        {10, 10, "S1"},
        {10, 10, "S2"},
        {10, 10, "S3"},
        {10, 10, "S4"}
    };
    int dimensions5[] = {10, 10, 10, 10, 10};
    int n5 = 4;
    
    MatrixChainResult result5;
    matrixChainOrderWithParentheses(dimensions5, n5, &result5);
    
    printMatrices(matrices5, n5);
    printf("Minimum cost: %d\n", result5.minCost);
    
    char parentheses5[1000] = "";
    getOptimalParentheses(result5.splitTable, 0, n5 - 1, parentheses5);
    printf("Optimal parenthesization: %s\n", parentheses5);
    printf("Note: For same-sized square matrices, all parenthesizations have equal cost\n");
    printf("\n");
    
    // Test Case 7: Demonstrate why order matters
    printf("Test Case 7: Why matrix multiplication order matters\n");
    Matrix matrices6[] = {
        {1000, 1, "Tall"},
        {1, 1000, "Wide"},
        {1000, 1, "Tall2"}
    };
    int dimensions6[] = {1000, 1, 1000, 1};
    int n6 = 3;
    
    MatrixChainResult result6;
    matrixChainOrderWithParentheses(dimensions6, n6, &result6);
    
    printMatrices(matrices6, n6);
    printf("Optimal cost: %d\n", result6.minCost);
    
    char parentheses6[1000] = "";
    getOptimalParentheses(result6.splitTable, 0, n6 - 1, parentheses6);
    printf("Optimal parenthesization: %s\n", parentheses6);
    
    // Manual calculation for comparison
    int leftAssociative = 1000 * 1 * 1000 + 1000 * 1000 * 1; // ((Tall × Wide) × Tall2)
    int rightAssociative = 1 * 1000 * 1 + 1000 * 1 * 1;       // (Tall × (Wide × Tall2))
    printf("Left associative cost: %d\n", leftAssociative);
    printf("Right associative cost: %d\n", rightAssociative);
    
    int maxCost = (leftAssociative > rightAssociative) ? leftAssociative : rightAssociative;
    printf("Savings with optimal order: %d operations\n", maxCost - result6.minCost);
    printf("This demonstrates the dramatic impact of proper parenthesization!\n");
    
    return 0;
}