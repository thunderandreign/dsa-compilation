#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/**
 * Dynamic Programming Strategy: Fibonacci Sequence
 * Core Idea: Store solutions to subproblems to avoid redundant calculations
 * Time Complexity: O(n) with memoization/tabulation vs O(2^n) naive recursion
 * Space Complexity: O(n) for storing intermediate results
 */

/**
 * Naive recursive approach (exponential time - for comparison)
 */
long long fibonacciNaive(int n) {
    // Base cases
    if (n <= 1) {
        return n;
    }
    
    // Recursive calls without storing results (inefficient)
    return fibonacciNaive(n - 1) + fibonacciNaive(n - 2);
}

/**
 * Top-down Dynamic Programming approach (Memoization)
 * @param n The position in Fibonacci sequence
 * @param memo Memoization array to store computed values
 * @return The nth Fibonacci number
 */
long long fibonacciMemoization(int n, long long memo[]) {
    // Base cases
    if (n <= 1) {
        return n;
    }
    
    // Check if already computed (dynamic programming optimization)
    if (memo[n] != -1) {
        return memo[n];
    }
    
    // Compute and store result
    memo[n] = fibonacciMemoization(n - 1, memo) + fibonacciMemoization(n - 2, memo);
    return memo[n];
}

/**
 * Bottom-up Dynamic Programming approach (Tabulation)
 * @param n The position in Fibonacci sequence
 * @return The nth Fibonacci number
 */
long long fibonacciTabulation(int n) {
    // Handle edge cases
    if (n <= 1) {
        return n;
    }
    
    // Create DP table
    long long* dp = (long long*)malloc((n + 1) * sizeof(long long));
    
    // Initialize base cases
    dp[0] = 0;
    dp[1] = 1;
    
    // Fill table bottom-up using previously computed values
    for (int i = 2; i <= n; i++) {
        dp[i] = dp[i - 1] + dp[i - 2]; // Dynamic programming recurrence
    }
    
    long long result = dp[n];
    free(dp);
    return result;
}

/**
 * Space-optimized Dynamic Programming approach
 * @param n The position in Fibonacci sequence
 * @return The nth Fibonacci number
 */
long long fibonacciOptimized(int n) {
    // Handle edge cases
    if (n <= 1) {
        return n;
    }
    
    // Only keep track of last two values (space optimization)
    long long prev2 = 0; // F(i-2)
    long long prev1 = 1; // F(i-1)
    long long current = 0;
    
    for (int i = 2; i <= n; i++) {
        current = prev1 + prev2; // F(i) = F(i-1) + F(i-2)
        prev2 = prev1; // Update F(i-2)
        prev1 = current; // Update F(i-1)
    }
    
    return current;
}

/**
 * Demonstrate the construction of Fibonacci sequence step by step
 */
void demonstrateFibonacciConstruction(int n) {
    printf("Building Fibonacci sequence up to position %d:\n", n);
    printf("F(0) = 0 (base case)\n");
    printf("F(1) = 1 (base case)\n");
    
    if (n <= 1) return;
    
    long long* fib = (long long*)malloc((n + 1) * sizeof(long long));
    fib[0] = 0;
    fib[1] = 1;
    
    for (int i = 2; i <= n; i++) {
        fib[i] = fib[i - 1] + fib[i - 2];
        printf("F(%d) = F(%d) + F(%d) = %lld + %lld = %lld\n", 
               i, i - 1, i - 2, fib[i - 1], fib[i - 2], fib[i]);
    }
    
    printf("Final sequence: [");
    for (int i = 0; i <= n; i++) {
        printf("%lld", fib[i]);
        if (i < n) printf(", ");
    }
    printf("]\n");
    
    free(fib);
}

/**
 * Analyze time complexity of different approaches
 */
void analyzeTimeComplexity(int n) {
    printf("Time Complexity Analysis for Fibonacci(%d):\n", n);
    
    clock_t start, end;
    double cpu_time_used;
    
    // Measure naive approach (only for small n due to exponential time)
    if (n <= 35) {
        start = clock();
        long long result = fibonacciNaive(n);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
        printf("1. Naive Recursion: %lld (Time: %.2f ms) - O(2^n)\n", result, cpu_time_used);
    } else {
        printf("1. Naive Recursion: Skipped (too slow for n > 35) - O(2^n)\n");
    }
    
    // Measure memoization approach
    start = clock();
    long long* memo = (long long*)malloc((n + 1) * sizeof(long long));
    for (int i = 0; i <= n; i++) {
        memo[i] = -1;
    }
    long long result = fibonacciMemoization(n, memo);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("2. Memoization: %lld (Time: %.2f ms) - O(n)\n", result, cpu_time_used);
    free(memo);
    
    // Measure tabulation approach
    start = clock();
    result = fibonacciTabulation(n);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("3. Tabulation: %lld (Time: %.2f ms) - O(n)\n", result, cpu_time_used);
    
    // Measure optimized approach
    start = clock();
    result = fibonacciOptimized(n);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC * 1000;
    printf("4. Space Optimized: %lld (Time: %.2f ms) - O(n) time, O(1) space\n", result, cpu_time_used);
}

/**
 * Generate Fibonacci sequence up to a given number of terms
 */
void generateFibonacciSequence(int terms, long long sequence[]) {
    if (terms <= 0) return;
    
    if (terms >= 1) sequence[0] = 0;
    if (terms >= 2) sequence[1] = 1;
    
    for (int i = 2; i < terms; i++) {
        sequence[i] = sequence[i - 1] + sequence[i - 2];
    }
}

/**
 * Find the position of a given Fibonacci number
 */
int findFibonacciPosition(long long target) {
    if (target == 0) return 0;
    if (target == 1) return 1;
    
    long long prev2 = 0, prev1 = 1;
    int position = 1;
    
    while (prev1 < target) {
        long long current = prev1 + prev2;
        prev2 = prev1;
        prev1 = current;
        position++;
        
        if (current == target) {
            return position;
        }
    }
    
    return -1; // Not a Fibonacci number
}

/**
 * Check if a number is a Fibonacci number
 */
int isFibonacciNumber(long long num) {
    return findFibonacciPosition(num) != -1;
}

int main() {
    printf("=== Fibonacci Sequence - Dynamic Programming ===\n");
    
    // Test Case 1: Basic Fibonacci calculation
    printf("Test Case 1: Basic Fibonacci calculations\n");
    int testValues[] = {0, 1, 5, 10, 15, 20};
    int numTests = sizeof(testValues) / sizeof(testValues[0]);
    
    for (int i = 0; i < numTests; i++) {
        int n = testValues[i];
        printf("F(%d) = %lld\n", n, fibonacciOptimized(n));
    }
    printf("\n");
    
    // Test Case 2: Step-by-step construction
    printf("Test Case 2: Step-by-step construction\n");
    demonstrateFibonacciConstruction(8);
    printf("\n");
    
    // Test Case 3: Comparing different approaches
    printf("Test Case 3: Performance comparison\n");
    analyzeTimeComplexity(30);
    printf("\n");
    
    // Test Case 4: Generate sequence
    printf("Test Case 4: Generate Fibonacci sequence\n");
    int terms = 15;
    long long* sequence = (long long*)malloc(terms * sizeof(long long));
    generateFibonacciSequence(terms, sequence);
    
    printf("First %d Fibonacci numbers: [", terms);
    for (int i = 0; i < terms; i++) {
        printf("%lld", sequence[i]);
        if (i < terms - 1) printf(", ");
    }
    printf("]\n");
    free(sequence);
    printf("\n");
    
    // Test Case 5: Find position of Fibonacci numbers
    printf("Test Case 5: Find positions and validate Fibonacci numbers\n");
    long long testNumbers[] = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 22, 35};
    int numTestNumbers = sizeof(testNumbers) / sizeof(testNumbers[0]);
    
    for (int i = 0; i < numTestNumbers; i++) {
        long long num = testNumbers[i];
        int position = findFibonacciPosition(num);
        if (position != -1) {
            printf("%lld is F(%d) - Fibonacci number: YES\n", num, position);
        } else {
            printf("%lld - Fibonacci number: NO\n", num);
        }
    }
    printf("\n");
    
    // Test Case 6: Large Fibonacci number
    printf("Test Case 6: Large Fibonacci numbers\n");
    int largeN = 50;
    printf("F(%d) = %lld\n", largeN, fibonacciOptimized(largeN));
    printf("Note: Dynamic programming handles large values efficiently\n");
    printf("\n");
    
    // Test Case 7: Edge cases
    printf("Test Case 7: Edge cases\n");
    printf("F(0) = %lld\n", fibonacciOptimized(0)); 
    printf("F(1) = %lld\n", fibonacciOptimized(1));
    printf("F(2) = %lld\n", fibonacciOptimized(2));
    printf("\n");
    
    // Test Case 8: Mathematical properties
    printf("Test Case 8: Fibonacci mathematical properties\n");
    printf("Golden Ratio approximation (F(n+1)/F(n) approaches φ ≈ 1.618):\n");
    for (int i = 10; i <= 20; i++) {
        double ratio = (double) fibonacciOptimized(i + 1) / fibonacciOptimized(i);
        printf("F(%d)/F(%d) = %.6f\n", i + 1, i, ratio);
    }
    printf("Golden ratio φ = (1 + √5) / 2 ≈ 1.618034\n");
    
    return 0;
}