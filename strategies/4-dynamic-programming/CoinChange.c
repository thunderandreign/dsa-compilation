#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/**
 * Dynamic Programming Strategy: Coin Change Problem
 * Core Idea: Find minimum number of coins needed to make change or count ways to make change
 * Time Complexity: O(amount * n) where n is number of coin denominations
 * Space Complexity: O(amount) for 1D DP array
 */

#define MAX_AMOUNT 10000
#define MAX_COINS 100

/**
 * Result structure for coin change with actual coins
 */
typedef struct {
    int minCoins;
    int coinsUsed[MAX_COINS];
    int numCoinsUsed;
} CoinChangeResult;

/**
 * Find minimum of two integers
 */
int min(int a, int b) {
    return (a < b) ? a : b;
}

/**
 * Find minimum number of coins needed to make the given amount
 * @param coins Array of coin denominations
 * @param numCoins Number of coin types
 * @param amount Target amount
 * @return Minimum number of coins needed, or -1 if impossible
 */
int coinChangeMinCoins(int coins[], int numCoins, int amount) {
    // dp[i] = minimum coins needed to make amount i
    int dp[MAX_AMOUNT + 1];
    
    // Initialize with impossible value
    for (int i = 0; i <= amount; i++) {
        dp[i] = amount + 1;
    }
    
    dp[0] = 0; // Base case: 0 coins needed for amount 0
    
    // For each amount from 1 to target
    for (int i = 1; i <= amount; i++) {
        // Try each coin denomination
        for (int j = 0; j < numCoins; j++) {
            if (coins[j] <= i) {
                // If we can use this coin, try using it and see if it's better
                dp[i] = min(dp[i], dp[i - coins[j]] + 1);
            }
        }
    }
    
    return dp[amount] > amount ? -1 : dp[amount];
}

/**
 * Find minimum coins and return the actual coin combination
 * @param coins Array of coin denominations
 * @param numCoins Number of coin types
 * @param amount Target amount
 * @param result Result structure to store solution
 */
void coinChangeWithCoins(int coins[], int numCoins, int amount, CoinChangeResult* result) {
    int dp[MAX_AMOUNT + 1];
    int parent[MAX_AMOUNT + 1]; // To track which coin was used
    
    // Initialize
    for (int i = 0; i <= amount; i++) {
        dp[i] = amount + 1;
        parent[i] = -1;
    }
    dp[0] = 0;
    
    for (int i = 1; i <= amount; i++) {
        for (int j = 0; j < numCoins; j++) {
            if (coins[j] <= i && dp[i - coins[j]] + 1 < dp[i]) {
                dp[i] = dp[i - coins[j]] + 1;
                parent[i] = coins[j]; // Remember which coin we used
            }
        }
    }
    
    if (dp[amount] > amount) {
        result->minCoins = -1;
        result->numCoinsUsed = 0;
        return;
    }
    
    result->minCoins = dp[amount];
    
    // Backtrack to find actual coins used
    result->numCoinsUsed = 0;
    int current = amount;
    while (current > 0) {
        int coinUsed = parent[current];
        result->coinsUsed[result->numCoinsUsed++] = coinUsed;
        current -= coinUsed;
    }
}

/**
 * Count the number of ways to make change for the given amount
 * @param coins Array of coin denominations
 * @param numCoins Number of coin types
 * @param amount Target amount
 * @return Number of different ways to make change
 */
long long coinChangeCountWays(int coins[], int numCoins, int amount) {
    // dp[i] = number of ways to make amount i
    long long dp[MAX_AMOUNT + 1];
    
    // Initialize
    for (int i = 0; i <= amount; i++) {
        dp[i] = 0;
    }
    dp[0] = 1; // One way to make amount 0: use no coins
    
    // For each coin denomination
    for (int i = 0; i < numCoins; i++) {
        // Update dp array for all amounts that can use this coin
        for (int j = coins[i]; j <= amount; j++) {
            dp[j] += dp[j - coins[i]];
        }
    }
    
    return dp[amount];
}

/**
 * Print array of integers
 */
void printArray(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]");
}

/**
 * Demonstrate step-by-step coin change construction
 */
void demonstrateCoinChange(int coins[], int numCoins, int amount) {
    printf("Step-by-step Coin Change construction:\n");
    printf("Coins available: ");
    printArray(coins, numCoins);
    printf("\nTarget amount: %d\n\n", amount);
    
    int dp[MAX_AMOUNT + 1];
    for (int i = 0; i <= amount; i++) {
        dp[i] = amount + 1;
    }
    dp[0] = 0;
    
    printf("DP recurrence: dp[i] = min(dp[i], dp[i-coin] + 1) for each coin\n");
    printf("Base case: dp[0] = 0 (0 coins needed for amount 0)\n\n");
    
    for (int i = 1; i <= amount; i++) {
        printf("Computing dp[%d]:\n", i);
        
        for (int j = 0; j < numCoins; j++) {
            if (coins[j] <= i) {
                int newValue = dp[i - coins[j]] + 1;
                printf("  Using coin %d: dp[%d] = min(%s, dp[%d] + 1) = min(%s, %d + 1) = min(%s, %d)\n", 
                       coins[j], i, 
                       (dp[i] > amount ? "∞" : ""), 
                       i - coins[j],
                       (dp[i] > amount ? "∞" : ""), 
                       dp[i - coins[j]], 
                       (dp[i] > amount ? "∞" : ""), 
                       newValue);
                
                dp[i] = min(dp[i], newValue);
            }
        }
        
        printf("  Final dp[%d] = %s\n", i, (dp[i] > amount ? "∞" : ""));
        if (dp[i] <= amount) printf("  Final dp[%d] = %d\n", i, dp[i]);
        printf("\n");
    }
    
    printf("Minimum coins needed for amount %d: %s\n", 
           amount, (dp[amount] > amount ? "impossible" : ""));
    if (dp[amount] <= amount) {
        printf("Minimum coins needed for amount %d: %d\n", amount, dp[amount]);
    }
}

/**
 * Print DP table for visualization
 */
void printDPTable(int coins[], int numCoins, int amount) {
    int dp[MAX_AMOUNT + 1];
    for (int i = 0; i <= amount; i++) {
        dp[i] = amount + 1;
    }
    dp[0] = 0;
    
    printf("DP Table Construction:\n");
    printf("Amount: ");
    for (int i = 0; i <= amount; i++) {
        printf("%4d", i);
    }
    printf("\n");
    
    printf("Initial:");
    for (int i = 0; i <= amount; i++) {
        if (dp[i] > amount) {
            printf("   ∞");
        } else {
            printf("%4d", dp[i]);
        }
    }
    printf("\n");
    
    for (int coinIdx = 0; coinIdx < numCoins; coinIdx++) {
        for (int i = coins[coinIdx]; i <= amount; i++) {
            dp[i] = min(dp[i], dp[i - coins[coinIdx]] + 1);
        }
        printf("Coin %2d:", coins[coinIdx]);
        for (int i = 0; i <= amount; i++) {
            if (dp[i] > amount) {
                printf("   ∞");
            } else {
                printf("%4d", dp[i]);
            }
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Compare greedy vs optimal approach
 */
void compareGreedyVsOptimal(int coins[], int numCoins, int amount) {
    printf("Comparison: Greedy vs Dynamic Programming\n");
    printf("Coins: ");
    printArray(coins, numCoins);
    printf("\nAmount: %d\n", amount);
    
    // Optimal solution using DP
    CoinChangeResult optimal;
    coinChangeWithCoins(coins, numCoins, amount, &optimal);
    printf("Optimal (DP): %d coins = ", optimal.minCoins);
    printArray(optimal.coinsUsed, optimal.numCoinsUsed);
    printf("\n");
    
    // Sort coins in descending order for greedy approach
    int sortedCoins[MAX_COINS];
    for (int i = 0; i < numCoins; i++) {
        sortedCoins[i] = coins[i];
    }
    
    // Simple bubble sort (descending)
    for (int i = 0; i < numCoins - 1; i++) {
        for (int j = 0; j < numCoins - i - 1; j++) {
            if (sortedCoins[j] < sortedCoins[j + 1]) {
                int temp = sortedCoins[j];
                sortedCoins[j] = sortedCoins[j + 1];
                sortedCoins[j + 1] = temp;
            }
        }
    }
    
    // Greedy solution (largest denomination first)
    int greedyCoins[MAX_COINS];
    int greedyCount = 0;
    int remainingAmount = amount;
    
    for (int i = 0; i < numCoins; i++) {
        while (remainingAmount >= sortedCoins[i]) {
            greedyCoins[greedyCount++] = sortedCoins[i];
            remainingAmount -= sortedCoins[i];
        }
    }
    
    if (remainingAmount == 0) {
        printf("Greedy: %d coins = ", greedyCount);
        printArray(greedyCoins, greedyCount);
        printf("\n");
        
        if (greedyCount == optimal.minCoins) {
            printf("✓ Greedy produces optimal result for this case\n");
        } else {
            printf("✗ Greedy is suboptimal. DP saves %d coins\n", greedyCount - optimal.minCoins);
        }
    } else {
        printf("Greedy: Cannot make exact change (remaining: %d)\n", remainingAmount);
    }
    printf("\n");
}

int main() {
    printf("=== Coin Change Problem - Dynamic Programming ===\n");
    
    // Test Case 1: Standard coin change
    printf("Test Case 1: Standard US coins\n");
    int coins1[] = {1, 5, 10, 25};
    int numCoins1 = 4;
    int amount1 = 30;
    
    int minCoins1 = coinChangeMinCoins(coins1, numCoins1, amount1);
    CoinChangeResult result1;
    coinChangeWithCoins(coins1, numCoins1, amount1, &result1);
    long long ways1 = coinChangeCountWays(coins1, numCoins1, amount1);
    
    printf("Coins: ");
    printArray(coins1, numCoins1);
    printf("\nAmount: %d\n", amount1);
    printf("Minimum coins: %d\n", minCoins1);
    printf("Coins used: ");
    printArray(result1.coinsUsed, result1.numCoinsUsed);
    printf("\nNumber of ways: %lld\n\n", ways1);
    
    printDPTable(coins1, numCoins1, amount1);
    
    // Test Case 2: Step-by-step demonstration
    printf("Test Case 2: Step-by-step construction\n");
    int coins2[] = {1, 3, 4};
    int numCoins2 = 3;
    int amount2 = 6;
    demonstrateCoinChange(coins2, numCoins2, amount2);
    printf("\n");
    
    // Test Case 3: Impossible case
    printf("Test Case 3: Impossible change\n");
    int coins3[] = {2, 4};
    int numCoins3 = 2;
    int amount3 = 3;
    
    int minCoins3 = coinChangeMinCoins(coins3, numCoins3, amount3);
    printf("Coins: ");
    printArray(coins3, numCoins3);
    printf("\nAmount: %d\n", amount3);
    printf("Minimum coins: %s\n", (minCoins3 == -1 ? "impossible" : ""));
    if (minCoins3 != -1) printf("Minimum coins: %d\n", minCoins3);
    printf("Reason: Cannot make odd amount with only even coins\n\n");
    
    // Test Case 4: Greedy vs Optimal comparison
    printf("Test Case 4: When greedy fails\n");
    int coins4[] = {1, 3, 4};
    int numCoins4 = 3;
    int amount4 = 6;
    compareGreedyVsOptimal(coins4, numCoins4, amount4);
    
    // Test Case 5: Count ways
    printf("Test Case 5: Counting ways\n");
    int coins5[] = {1, 2, 5};
    int numCoins5 = 3;
    int amount5 = 5;
    
    long long countWays = coinChangeCountWays(coins5, numCoins5, amount5);
    
    printf("Coins: ");
    printArray(coins5, numCoins5);
    printf("\nAmount: %d\n", amount5);
    printf("Number of ways: %lld\n", countWays);
    printf("Ways: [5], [2,2,1], [2,1,1,1], [1,1,1,1,1]\n\n");
    
    // Test Case 6: Large denomination
    printf("Test Case 6: Larger amounts\n");
    int coins6[] = {1, 5, 10, 25};
    int numCoins6 = 4;
    int amount6 = 100;
    
    CoinChangeResult result6;
    coinChangeWithCoins(coins6, numCoins6, amount6, &result6);
    long long ways6 = coinChangeCountWays(coins6, numCoins6, amount6);
    
    printf("Coins: ");
    printArray(coins6, numCoins6);
    printf("\nAmount: %d\n", amount6);
    printf("Minimum coins: %d\n", result6.minCoins);
    printf("Optimal combination: ");
    printArray(result6.coinsUsed, result6.numCoinsUsed);
    printf("\nTotal ways to make change: %lld\n\n", ways6);
    
    // Test Case 7: Edge cases
    printf("Test Case 7: Edge cases\n");
    
    // Amount 0
    int minCoins0 = coinChangeMinCoins(coins1, numCoins1, 0);
    printf("Amount 0: %d coins (expected: 0)\n", minCoins0);
    
    // Single coin exact match
    int minCoinsExact = coinChangeMinCoins(coins1, numCoins1, 25);
    printf("Amount 25 with [1,5,10,25]: %d coin (expected: 1)\n", minCoinsExact);
    
    // No coins
    int noCoins[] = {};
    int minCoinsNone = coinChangeMinCoins(noCoins, 0, 5);
    printf("No coins available for amount 5: %s\n", (minCoinsNone == -1 ? "impossible" : ""));
    if (minCoinsNone != -1) printf("No coins available for amount 5: %d\n", minCoinsNone);
    printf("\n");
    
    // Test Case 8: International currency
    printf("Test Case 8: International currency example\n");
    int euroCoins[] = {1, 2, 5, 10, 20, 50, 100, 200}; // Euro cents
    int numEuroCoins = 8;
    int euroAmount = 243;
    
    CoinChangeResult euroResult;
    coinChangeWithCoins(euroCoins, numEuroCoins, euroAmount, &euroResult);
    printf("Euro coins (cents): ");
    printArray(euroCoins, numEuroCoins);
    printf("\nAmount: %d cents\n", euroAmount);
    printf("Minimum coins: %d\n", euroResult.minCoins);
    printf("Coins used: ");
    printArray(euroResult.coinsUsed, euroResult.numCoinsUsed);
    printf("\n\n");
    
    compareGreedyVsOptimal(euroCoins, numEuroCoins, euroAmount);
    printf("Note: For standard currency systems, greedy algorithm usually works optimally\n");
    
    return 0;
}