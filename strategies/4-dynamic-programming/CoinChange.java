/**
 * Dynamic Programming Strategy: Coin Change Problem
 * Core Idea: Find minimum number of coins needed to make change or count ways to make change
 * Time Complexity: O(amount * n) where n is number of coin denominations
 * Space Complexity: O(amount) for 1D DP array
 */

import java.util.*;

public class CoinChange {
    
    /**
     * Find minimum number of coins needed to make the given amount
     * @param coins Array of coin denominations
     * @param amount Target amount
     * @return Minimum number of coins needed, or -1 if impossible
     */
    public static int coinChangeMinCoins(int[] coins, int amount) {
        // dp[i] = minimum coins needed to make amount i
        int[] dp = new int[amount + 1];
        Arrays.fill(dp, amount + 1); // Initialize with impossible value
        
        dp[0] = 0; // Base case: 0 coins needed for amount 0
        
        // For each amount from 1 to target
        for (int i = 1; i <= amount; i++) {
            // Try each coin denomination
            for (int coin : coins) {
                if (coin <= i) {
                    // If we can use this coin, try using it and see if it's better
                    dp[i] = Math.min(dp[i], dp[i - coin] + 1);
                }
            }
        }
        
        return dp[amount] > amount ? -1 : dp[amount];
    }
    
    /**
     * Find minimum coins and return the actual coin combination
     * @param coins Array of coin denominations
     * @param amount Target amount
     * @return CoinChangeResult with minimum count and actual coins used
     */
    public static CoinChangeResult coinChangeWithCoins(int[] coins, int amount) {
        int[] dp = new int[amount + 1];
        int[] parent = new int[amount + 1]; // To track which coin was used
        
        Arrays.fill(dp, amount + 1);
        Arrays.fill(parent, -1);
        dp[0] = 0;
        
        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (coin <= i && dp[i - coin] + 1 < dp[i]) {
                    dp[i] = dp[i - coin] + 1;
                    parent[i] = coin; // Remember which coin we used
                }
            }
        }
        
        if (dp[amount] > amount) {
            return new CoinChangeResult(-1, new ArrayList<>());
        }
        
        // Backtrack to find actual coins used
        List<Integer> coinsUsed = new ArrayList<>();
        int current = amount;
        while (current > 0) {
            int coinUsed = parent[current];
            coinsUsed.add(coinUsed);
            current -= coinUsed;
        }
        
        return new CoinChangeResult(dp[amount], coinsUsed);
    }
    
    /**
     * Count the number of ways to make change for the given amount
     * @param coins Array of coin denominations
     * @param amount Target amount
     * @return Number of different ways to make change
     */
    public static long coinChangeCountWays(int[] coins, int amount) {
        // dp[i] = number of ways to make amount i
        long[] dp = new long[amount + 1];
        dp[0] = 1; // One way to make amount 0: use no coins
        
        // For each coin denomination
        for (int coin : coins) {
            // Update dp array for all amounts that can use this coin
            for (int i = coin; i <= amount; i++) {
                dp[i] += dp[i - coin];
            }
        }
        
        return dp[amount];
    }
    
    /**
     * Find all possible ways to make change (returns actual combinations)
     * @param coins Array of coin denominations
     * @param amount Target amount
     * @return List of all possible coin combinations
     */
    public static List<List<Integer>> coinChangeAllWays(int[] coins, int amount) {
        List<List<Integer>> result = new ArrayList<>();
        findAllWays(coins, amount, 0, new ArrayList<>(), result);
        return result;
    }
    
    /**
     * Recursive helper to find all combinations
     */
    private static void findAllWays(int[] coins, int amount, int coinIndex, 
                                   List<Integer> current, List<List<Integer>> result) {
        if (amount == 0) {
            result.add(new ArrayList<>(current));
            return;
        }
        
        if (amount < 0 || coinIndex >= coins.length) {
            return;
        }
        
        // Include current coin (can use multiple times)
        current.add(coins[coinIndex]);
        findAllWays(coins, amount - coins[coinIndex], coinIndex, current, result);
        current.remove(current.size() - 1);
        
        // Skip current coin and move to next
        findAllWays(coins, amount, coinIndex + 1, current, result);
    }
    
    /**
     * Result class for coin change with actual coins
     */
    static class CoinChangeResult {
        int minCoins;
        List<Integer> coinsUsed;
        
        CoinChangeResult(int minCoins, List<Integer> coinsUsed) {
            this.minCoins = minCoins;
            this.coinsUsed = coinsUsed;
        }
    }
    
    /**
     * Demonstrate step-by-step coin change construction
     */
    public static void demonstrateCoinChange(int[] coins, int amount) {
        System.out.println("Step-by-step Coin Change construction:");
        System.out.println("Coins available: " + Arrays.toString(coins));
        System.out.println("Target amount: " + amount);
        System.out.println();
        
        int[] dp = new int[amount + 1];
        Arrays.fill(dp, amount + 1);
        dp[0] = 0;
        
        System.out.println("DP recurrence: dp[i] = min(dp[i], dp[i-coin] + 1) for each coin");
        System.out.println("Base case: dp[0] = 0 (0 coins needed for amount 0)");
        System.out.println();
        
        for (int i = 1; i <= amount; i++) {
            System.out.printf("Computing dp[%d]:\n", i);
            
            for (int coin : coins) {
                if (coin <= i) {
                    int newValue = dp[i - coin] + 1;
                    System.out.printf("  Using coin %d: dp[%d] = min(%s, dp[%d] + 1) = min(%s, %d + 1) = min(%s, %d)\n", 
                                    coin, i, 
                                    (dp[i] > amount ? "∞" : String.valueOf(dp[i])), 
                                    i - coin,
                                    (dp[i] > amount ? "∞" : String.valueOf(dp[i])), 
                                    dp[i - coin], 
                                    (dp[i] > amount ? "∞" : String.valueOf(dp[i])), 
                                    newValue);
                    
                    dp[i] = Math.min(dp[i], newValue);
                }
            }
            
            System.out.printf("  Final dp[%d] = %s\n", i, (dp[i] > amount ? "∞" : String.valueOf(dp[i])));
            System.out.println();
        }
        
        System.out.printf("Minimum coins needed for amount %d: %s\n", 
                         amount, (dp[amount] > amount ? "impossible" : String.valueOf(dp[amount])));
    }
    
    /**
     * Print DP table for visualization
     */
    public static void printDPTable(int[] coins, int amount) {
        int[] dp = new int[amount + 1];
        Arrays.fill(dp, amount + 1);
        dp[0] = 0;
        
        System.out.println("DP Table Construction:");
        System.out.print("Amount: ");
        for (int i = 0; i <= amount; i++) {
            System.out.printf("%4d", i);
        }
        System.out.println();
        System.out.print("Initial:");
        for (int i = 0; i <= amount; i++) {
            System.out.printf("%4s", (dp[i] > amount ? "∞" : String.valueOf(dp[i])));
        }
        System.out.println();
        
        for (int coin : coins) {
            for (int i = coin; i <= amount; i++) {
                dp[i] = Math.min(dp[i], dp[i - coin] + 1);
            }
            System.out.printf("Coin %2d:", coin);
            for (int i = 0; i <= amount; i++) {
                System.out.printf("%4s", (dp[i] > amount ? "∞" : String.valueOf(dp[i])));
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Compare greedy vs optimal approach
     */
    public static void compareGreedyVsOptimal(int[] coins, int amount) {
        System.out.println("Comparison: Greedy vs Dynamic Programming");
        System.out.println("Coins: " + Arrays.toString(coins));
        System.out.println("Amount: " + amount);
        
        // Optimal solution using DP
        CoinChangeResult optimal = coinChangeWithCoins(coins, amount);
        System.out.println("Optimal (DP): " + optimal.minCoins + " coins = " + optimal.coinsUsed);
        
        // Greedy solution (largest denomination first)
        Integer[] sortedCoins = Arrays.stream(coins).boxed().sorted(Collections.reverseOrder()).toArray(Integer[]::new);
        List<Integer> greedyCoins = new ArrayList<>();
        int remainingAmount = amount;
        int greedyCount = 0;
        
        for (int coin : sortedCoins) {
            while (remainingAmount >= coin) {
                greedyCoins.add(coin);
                remainingAmount -= coin;
                greedyCount++;
            }
        }
        
        if (remainingAmount == 0) {
            System.out.println("Greedy: " + greedyCount + " coins = " + greedyCoins);
            if (greedyCount == optimal.minCoins) {
                System.out.println("✓ Greedy produces optimal result for this case");
            } else {
                System.out.println("✗ Greedy is suboptimal. DP saves " + (greedyCount - optimal.minCoins) + " coins");
            }
        } else {
            System.out.println("Greedy: Cannot make exact change (remaining: " + remainingAmount + ")");
        }
        System.out.println();
    }
    
    public static void main(String[] args) {
        System.out.println("=== Coin Change Problem - Dynamic Programming ===");
        
        // Test Case 1: Standard coin change
        System.out.println("Test Case 1: Standard US coins");
        int[] coins1 = {1, 5, 10, 25};
        int amount1 = 30;
        
        int minCoins1 = coinChangeMinCoins(coins1, amount1);
        CoinChangeResult result1 = coinChangeWithCoins(coins1, amount1);
        long ways1 = coinChangeCountWays(coins1, amount1);
        
        System.out.println("Coins: " + Arrays.toString(coins1));
        System.out.println("Amount: " + amount1);
        System.out.println("Minimum coins: " + minCoins1);
        System.out.println("Coins used: " + result1.coinsUsed);
        System.out.println("Number of ways: " + ways1);
        System.out.println();
        
        printDPTable(coins1, amount1);
        
        // Test Case 2: Step-by-step demonstration
        System.out.println("Test Case 2: Step-by-step construction");
        int[] coins2 = {1, 3, 4};
        int amount2 = 6;
        demonstrateCoinChange(coins2, amount2);
        System.out.println();
        
        // Test Case 3: Impossible case
        System.out.println("Test Case 3: Impossible change");
        int[] coins3 = {2, 4};
        int amount3 = 3;
        
        int minCoins3 = coinChangeMinCoins(coins3, amount3);
        System.out.println("Coins: " + Arrays.toString(coins3));
        System.out.println("Amount: " + amount3);
        System.out.println("Minimum coins: " + (minCoins3 == -1 ? "impossible" : String.valueOf(minCoins3)));
        System.out.println("Reason: Cannot make odd amount with only even coins");
        System.out.println();
        
        // Test Case 4: Greedy vs Optimal comparison
        System.out.println("Test Case 4: When greedy fails");
        int[] coins4 = {1, 3, 4};
        int amount4 = 6;
        compareGreedyVsOptimal(coins4, amount4);
        
        // Test Case 5: All possible ways
        System.out.println("Test Case 5: All possible combinations");
        int[] coins5 = {1, 2, 5};
        int amount5 = 5;
        
        List<List<Integer>> allWays = coinChangeAllWays(coins5, amount5);
        long countWays = coinChangeCountWays(coins5, amount5);
        
        System.out.println("Coins: " + Arrays.toString(coins5));
        System.out.println("Amount: " + amount5);
        System.out.println("Number of ways: " + countWays);
        System.out.println("All combinations:");
        for (int i = 0; i < allWays.size(); i++) {
            Collections.sort(allWays.get(i), Collections.reverseOrder());
            System.out.printf("  %d. %s\n", i + 1, allWays.get(i));
        }
        System.out.println();
        
        // Test Case 6: Large denomination
        System.out.println("Test Case 6: Larger amounts");
        int[] coins6 = {1, 5, 10, 25};
        int amount6 = 100;
        
        CoinChangeResult result6 = coinChangeWithCoins(coins6, amount6);
        long ways6 = coinChangeCountWays(coins6, amount6);
        
        System.out.println("Coins: " + Arrays.toString(coins6));
        System.out.println("Amount: " + amount6);
        System.out.println("Minimum coins: " + result6.minCoins);
        System.out.println("Optimal combination: " + result6.coinsUsed);
        System.out.println("Total ways to make change: " + ways6);
        System.out.println();
        
        // Test Case 7: Edge cases
        System.out.println("Test Case 7: Edge cases");
        
        // Amount 0
        int minCoins0 = coinChangeMinCoins(coins1, 0);
        System.out.println("Amount 0: " + minCoins0 + " coins (expected: 0)");
        
        // Single coin exact match
        int minCoinsExact = coinChangeMinCoins(coins1, 25);
        System.out.println("Amount 25 with [1,5,10,25]: " + minCoinsExact + " coin (expected: 1)");
        
        // No coins
        int[] noCoins = {};
        int minCoinsNone = coinChangeMinCoins(noCoins, 5);
        System.out.println("No coins available for amount 5: " + (minCoinsNone == -1 ? "impossible" : String.valueOf(minCoinsNone)));
        System.out.println();
        
        // Test Case 8: International currency
        System.out.println("Test Case 8: International currency example");
        int[] euroCoins = {1, 2, 5, 10, 20, 50, 100, 200}; // Euro cents
        int euroAmount = 243;
        
        CoinChangeResult euroResult = coinChangeWithCoins(euroCoins, euroAmount);
        System.out.println("Euro coins (cents): " + Arrays.toString(euroCoins));
        System.out.println("Amount: " + euroAmount + " cents");
        System.out.println("Minimum coins: " + euroResult.minCoins);
        System.out.println("Coins used: " + euroResult.coinsUsed);
        
        compareGreedyVsOptimal(euroCoins, euroAmount);
        System.out.println("Note: For standard currency systems, greedy algorithm usually works optimally");
    }
}