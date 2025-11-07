/**
 * Dynamic Programming Strategy: Fibonacci Sequence
 * Core Idea: Store solutions to subproblems to avoid redundant calculations
 * Time Complexity: O(n) with memoization/tabulation vs O(2^n) naive recursion
 * Space Complexity: O(n) for storing intermediate results
 */

import java.util.*;

public class Fibonacci {
    
    /**
     * Naive recursive approach (exponential time - for comparison)
     */
    public static long fibonacciNaive(int n) {
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
    public static long fibonacciMemoization(int n, long[] memo) {
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
    public static long fibonacciTabulation(int n) {
        // Handle edge cases
        if (n <= 1) {
            return n;
        }
        
        // Create DP table
        long[] dp = new long[n + 1];
        
        // Initialize base cases
        dp[0] = 0;
        dp[1] = 1;
        
        // Fill table bottom-up using previously computed values
        for (int i = 2; i <= n; i++) {
            dp[i] = dp[i - 1] + dp[i - 2]; // Dynamic programming recurrence
        }
        
        return dp[n];
    }
    
    /**
     * Space-optimized Dynamic Programming approach
     * @param n The position in Fibonacci sequence
     * @return The nth Fibonacci number
     */
    public static long fibonacciOptimized(int n) {
        // Handle edge cases
        if (n <= 1) {
            return n;
        }
        
        // Only keep track of last two values (space optimization)
        long prev2 = 0; // F(i-2)
        long prev1 = 1; // F(i-1)
        long current = 0;
        
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
    public static void demonstrateFibonacciConstruction(int n) {
        System.out.println("Building Fibonacci sequence up to position " + n + ":");
        System.out.println("F(0) = 0 (base case)");
        System.out.println("F(1) = 1 (base case)");
        
        if (n <= 1) return;
        
        long[] fib = new long[n + 1];
        fib[0] = 0;
        fib[1] = 1;
        
        for (int i = 2; i <= n; i++) {
            fib[i] = fib[i - 1] + fib[i - 2];
            System.out.printf("F(%d) = F(%d) + F(%d) = %d + %d = %d\n", 
                            i, i - 1, i - 2, fib[i - 1], fib[i - 2], fib[i]);
        }
        
        System.out.println("Final sequence: " + Arrays.toString(Arrays.copyOf(fib, n + 1)));
    }
    
    /**
     * Analyze time complexity of different approaches
     */
    public static void analyzeTimeComplexity(int n) {
        System.out.println("Time Complexity Analysis for Fibonacci(" + n + "):");
        
        // Measure naive approach (only for small n due to exponential time)
        if (n <= 35) {
            long startTime = System.nanoTime();
            long result = fibonacciNaive(n);
            long endTime = System.nanoTime();
            System.out.printf("1. Naive Recursion: %d (Time: %.2f ms) - O(2^n)\n", 
                            result, (endTime - startTime) / 1_000_000.0);
        } else {
            System.out.println("1. Naive Recursion: Skipped (too slow for n > 35) - O(2^n)");
        }
        
        // Measure memoization approach
        long startTime = System.nanoTime();
        long[] memo = new long[n + 1];
        Arrays.fill(memo, -1);
        long result = fibonacciMemoization(n, memo);
        long endTime = System.nanoTime();
        System.out.printf("2. Memoization: %d (Time: %.2f ms) - O(n)\n", 
                        result, (endTime - startTime) / 1_000_000.0);
        
        // Measure tabulation approach
        startTime = System.nanoTime();
        result = fibonacciTabulation(n);
        endTime = System.nanoTime();
        System.out.printf("3. Tabulation: %d (Time: %.2f ms) - O(n)\n", 
                        result, (endTime - startTime) / 1_000_000.0);
        
        // Measure optimized approach
        startTime = System.nanoTime();
        result = fibonacciOptimized(n);
        endTime = System.nanoTime();
        System.out.printf("4. Space Optimized: %d (Time: %.2f ms) - O(n) time, O(1) space\n", 
                        result, (endTime - startTime) / 1_000_000.0);
    }
    
    /**
     * Generate Fibonacci sequence up to a given number of terms
     */
    public static List<Long> generateFibonacciSequence(int terms) {
        List<Long> sequence = new ArrayList<>();
        
        if (terms <= 0) return sequence;
        
        if (terms >= 1) sequence.add(0L);
        if (terms >= 2) sequence.add(1L);
        
        for (int i = 2; i < terms; i++) {
            long next = sequence.get(i - 1) + sequence.get(i - 2);
            sequence.add(next);
        }
        
        return sequence;
    }
    
    /**
     * Find the position of a given Fibonacci number
     */
    public static int findFibonacciPosition(long target) {
        if (target == 0) return 0;
        if (target == 1) return 1;
        
        long prev2 = 0, prev1 = 1;
        int position = 1;
        
        while (prev1 < target) {
            long current = prev1 + prev2;
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
    public static boolean isFibonacciNumber(long num) {
        return findFibonacciPosition(num) != -1;
    }
    
    public static void main(String[] args) {
        System.out.println("=== Fibonacci Sequence - Dynamic Programming ===");
        
        // Test Case 1: Basic Fibonacci calculation
        System.out.println("Test Case 1: Basic Fibonacci calculations");
        int[] testValues = {0, 1, 5, 10, 15, 20};
        
        for (int n : testValues) {
            System.out.printf("F(%d) = %d\n", n, fibonacciOptimized(n));
        }
        System.out.println();
        
        // Test Case 2: Step-by-step construction
        System.out.println("Test Case 2: Step-by-step construction");
        demonstrateFibonacciConstruction(8);
        System.out.println();
        
        // Test Case 3: Comparing different approaches
        System.out.println("Test Case 3: Performance comparison");
        analyzeTimeComplexity(30);
        System.out.println();
        
        // Test Case 4: Generate sequence
        System.out.println("Test Case 4: Generate Fibonacci sequence");
        List<Long> sequence = generateFibonacciSequence(15);
        System.out.println("First 15 Fibonacci numbers: " + sequence);
        System.out.println();
        
        // Test Case 5: Find position of Fibonacci numbers
        System.out.println("Test Case 5: Find positions and validate Fibonacci numbers");
        long[] testNumbers = {0, 1, 2, 3, 5, 8, 13, 21, 34, 55, 22, 35};
        
        for (long num : testNumbers) {
            int position = findFibonacciPosition(num);
            if (position != -1) {
                System.out.printf("%d is F(%d) - Fibonacci number: YES\n", num, position);
            } else {
                System.out.printf("%d - Fibonacci number: NO\n", num);
            }
        }
        System.out.println();
        
        // Test Case 6: Large Fibonacci number
        System.out.println("Test Case 6: Large Fibonacci numbers");
        int largeN = 50;
        System.out.printf("F(%d) = %d\n", largeN, fibonacciOptimized(largeN));
        System.out.println("Note: Dynamic programming handles large values efficiently");
        System.out.println();
        
        // Test Case 7: Edge cases
        System.out.println("Test Case 7: Edge cases");
        System.out.printf("F(-1) handled as: F(0) = %d\n", fibonacciOptimized(0));
        System.out.printf("F(0) = %d\n", fibonacciOptimized(0)); 
        System.out.printf("F(1) = %d\n", fibonacciOptimized(1));
        System.out.printf("F(2) = %d\n", fibonacciOptimized(2));
        System.out.println();
        
        // Test Case 8: Mathematical properties
        System.out.println("Test Case 8: Fibonacci mathematical properties");
        System.out.println("Golden Ratio approximation (F(n+1)/F(n) approaches φ ≈ 1.618):");
        for (int i = 10; i <= 20; i++) {
            double ratio = (double) fibonacciOptimized(i + 1) / fibonacciOptimized(i);
            System.out.printf("F(%d)/F(%d) = %.6f\n", i + 1, i, ratio);
        }
        System.out.println("Golden ratio φ = (1 + √5) / 2 ≈ 1.618034");
    }
}