/**
 * Dynamic Programming Strategy: 0/1 Knapsack Problem
 * Core Idea: Build optimal solution by considering each item and weight capacity
 * Time Complexity: O(n * W) where n is number of items and W is capacity
 * Space Complexity: O(n * W) for 2D table, can be optimized to O(W)
 */

import java.util.*;

public class ZeroOneKnapsack {
    
    /**
     * Item class to represent knapsack items
     */
    static class Item {
        int weight;
        int value;
        String name;
        
        Item(int weight, int value, String name) {
            this.weight = weight;
            this.value = value;
            this.name = name;
        }
        
        @Override
        public String toString() {
            return String.format("%s(w:%d, v:%d)", name, weight, value);
        }
    }
    
    /**
     * Solve 0/1 Knapsack using 2D Dynamic Programming table
     * @param items Array of items
     * @param capacity Knapsack capacity
     * @return Maximum value that can be obtained
     */
    public static int knapsack2D(Item[] items, int capacity) {
        int n = items.length;
        
        // Create DP table: dp[i][w] = max value using first i items with capacity w
        int[][] dp = new int[n + 1][capacity + 1];
        
        // Base case: 0 items or 0 capacity = 0 value
        // (already initialized to 0)
        
        // Fill table using dynamic programming recurrence
        for (int i = 1; i <= n; i++) {
            for (int w = 0; w <= capacity; w++) {
                // Option 1: Don't include current item
                dp[i][w] = dp[i - 1][w];
                
                // Option 2: Include current item (if it fits)
                if (items[i - 1].weight <= w) {
                    int includeValue = items[i - 1].value + dp[i - 1][w - items[i - 1].weight];
                    dp[i][w] = Math.max(dp[i][w], includeValue);
                }
            }
        }
        
        return dp[n][capacity];
    }
    
    /**
     * Solve 0/1 Knapsack with space optimization (1D array)
     * @param items Array of items
     * @param capacity Knapsack capacity
     * @return Maximum value that can be obtained
     */
    public static int knapsackOptimized(Item[] items, int capacity) {
        int n = items.length;
        
        // Use 1D array instead of 2D (space optimization)
        int[] dp = new int[capacity + 1];
        
        // Process each item
        for (int i = 0; i < n; i++) {
            // Traverse backwards to avoid using updated values in same iteration
            for (int w = capacity; w >= items[i].weight; w--) {
                dp[w] = Math.max(dp[w], dp[w - items[i].weight] + items[i].value);
            }
        }
        
        return dp[capacity];
    }
    
    /**
     * Solve 0/1 Knapsack and return both max value and selected items
     * @param items Array of items
     * @param capacity Knapsack capacity
     * @return KnapsackResult containing max value and selected items
     */
    public static KnapsackResult knapsackWithItems(Item[] items, int capacity) {
        int n = items.length;
        int[][] dp = new int[n + 1][capacity + 1];
        
        // Fill DP table
        for (int i = 1; i <= n; i++) {
            for (int w = 0; w <= capacity; w++) {
                dp[i][w] = dp[i - 1][w]; // Don't include item i-1
                
                if (items[i - 1].weight <= w) {
                    int includeValue = items[i - 1].value + dp[i - 1][w - items[i - 1].weight];
                    dp[i][w] = Math.max(dp[i][w], includeValue);
                }
            }
        }
        
        // Backtrack to find selected items
        List<Item> selectedItems = new ArrayList<>();
        int w = capacity;
        for (int i = n; i > 0 && w > 0; i--) {
            // If value changed from dp[i-1][w], then item i-1 was included
            if (dp[i][w] != dp[i - 1][w]) {
                selectedItems.add(items[i - 1]);
                w -= items[i - 1].weight;
            }
        }
        
        Collections.reverse(selectedItems); // Reverse to get original order
        return new KnapsackResult(dp[n][capacity], selectedItems);
    }
    
    /**
     * Result class to hold knapsack solution
     */
    static class KnapsackResult {
        int maxValue;
        List<Item> selectedItems;
        
        KnapsackResult(int maxValue, List<Item> selectedItems) {
            this.maxValue = maxValue;
            this.selectedItems = selectedItems;
        }
    }
    
    /**
     * Print the DP table for educational purposes
     */
    public static void printDPTable(Item[] items, int capacity) {
        int n = items.length;
        int[][] dp = new int[n + 1][capacity + 1];
        
        // Fill DP table
        for (int i = 1; i <= n; i++) {
            for (int w = 0; w <= capacity; w++) {
                dp[i][w] = dp[i - 1][w];
                if (items[i - 1].weight <= w) {
                    int includeValue = items[i - 1].value + dp[i - 1][w - items[i - 1].weight];
                    dp[i][w] = Math.max(dp[i][w], includeValue);
                }
            }
        }
        
        // Print table header
        System.out.println("DP Table (rows: items, columns: capacity):");
        System.out.print("Item\\Cap ");
        for (int w = 0; w <= capacity; w++) {
            System.out.printf("%3d", w);
        }
        System.out.println();
        
        // Print table rows
        for (int i = 0; i <= n; i++) {
            if (i == 0) {
                System.out.print("   -    ");
            } else {
                System.out.printf("%s", items[i - 1].name);
                for (int spaces = items[i - 1].name.length(); spaces < 8; spaces++) {
                    System.out.print(" ");
                }
            }
            
            for (int w = 0; w <= capacity; w++) {
                System.out.printf("%3d", dp[i][w]);
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Demonstrate the decision process step by step
     */
    public static void demonstrateDecisionProcess(Item[] items, int capacity) {
        System.out.println("Decision Process for 0/1 Knapsack:");
        System.out.println("Capacity: " + capacity);
        System.out.println("Items available:");
        for (int i = 0; i < items.length; i++) {
            System.out.printf("  %d. %s\n", i + 1, items[i]);
        }
        System.out.println();
        
        int n = items.length;
        int[][] dp = new int[n + 1][capacity + 1];
        
        for (int i = 1; i <= n; i++) {
            System.out.printf("Processing item %d: %s\n", i, items[i - 1]);
            
            for (int w = 1; w <= capacity; w++) {
                dp[i][w] = dp[i - 1][w];
                
                if (items[i - 1].weight <= w) {
                    int includeValue = items[i - 1].value + dp[i - 1][w - items[i - 1].weight];
                    if (includeValue > dp[i][w]) {
                        dp[i][w] = includeValue;
                        System.out.printf("  Capacity %d: Include item (value %d + %d = %d)\n", 
                                        w, items[i - 1].value, dp[i - 1][w - items[i - 1].weight], includeValue);
                    }
                }
            }
            System.out.println();
        }
        
        System.out.printf("Final maximum value: %d\n", dp[n][capacity]);
    }
    
    /**
     * Compare 0/1 knapsack with fractional knapsack
     */
    public static void compareWithFractionalKnapsack(Item[] items, int capacity) {
        System.out.println("Comparison: 0/1 Knapsack vs Fractional Knapsack");
        
        // 0/1 Knapsack result
        KnapsackResult result01 = knapsackWithItems(items, capacity);
        System.out.println("0/1 Knapsack (can't split items):");
        System.out.println("  Max value: " + result01.maxValue);
        System.out.println("  Selected items: " + result01.selectedItems);
        
        // Calculate total weight used
        int totalWeight = result01.selectedItems.stream().mapToInt(item -> item.weight).sum();
        System.out.println("  Total weight: " + totalWeight + "/" + capacity);
        
        // Fractional knapsack (greedy approach for comparison)
        Item[] sortedItems = Arrays.copyOf(items, items.length);
        Arrays.sort(sortedItems, (a, b) -> Double.compare(
            (double) b.value / b.weight, (double) a.value / a.weight));
        
        double fractionalValue = 0.0;
        int remainingCapacity = capacity;
        System.out.println("\nFractional Knapsack (can split items, greedy approach):");
        
        for (Item item : sortedItems) {
            if (remainingCapacity >= item.weight) {
                fractionalValue += item.value;
                remainingCapacity -= item.weight;
                System.out.printf("  Take full %s (value: %d)\n", item.name, item.value);
            } else if (remainingCapacity > 0) {
                double fraction = (double) remainingCapacity / item.weight;
                fractionalValue += item.value * fraction;
                System.out.printf("  Take %.1f%% of %s (value: %.1f)\n", 
                                fraction * 100, item.name, item.value * fraction);
                remainingCapacity = 0;
                break;
            }
        }
        
        System.out.printf("  Max fractional value: %.1f\n", fractionalValue);
        System.out.println("Note: Fractional >= 0/1 knapsack value (more flexibility)");
    }
    
    public static void main(String[] args) {
        System.out.println("=== 0/1 Knapsack Problem - Dynamic Programming ===");
        
        // Test Case 1: Classic knapsack problem
        System.out.println("Test Case 1: Classic 0/1 knapsack problem");
        Item[] items1 = {
            new Item(10, 60, "A"),
            new Item(20, 100, "B"),
            new Item(30, 120, "C")
        };
        int capacity1 = 50;
        
        System.out.println("Items: " + Arrays.toString(items1));
        System.out.println("Capacity: " + capacity1);
        
        int maxValue1 = knapsack2D(items1, capacity1);
        System.out.println("Maximum value: " + maxValue1);
        
        KnapsackResult result1 = knapsackWithItems(items1, capacity1);
        System.out.println("Selected items: " + result1.selectedItems);
        System.out.println();
        
        // Show DP table
        printDPTable(items1, capacity1);
        
        // Test Case 2: More complex problem
        System.out.println("Test Case 2: More complex problem");
        Item[] items2 = {
            new Item(5, 10, "Item1"),
            new Item(4, 40, "Item2"),
            new Item(6, 30, "Item3"),
            new Item(3, 50, "Item4")
        };
        int capacity2 = 10;
        
        System.out.println("Items: " + Arrays.toString(items2));
        System.out.println("Capacity: " + capacity2);
        
        KnapsackResult result2 = knapsackWithItems(items2, capacity2);
        System.out.println("Maximum value: " + result2.maxValue);
        System.out.println("Selected items: " + result2.selectedItems);
        System.out.println();
        
        demonstrateDecisionProcess(items2, capacity2);
        System.out.println();
        
        // Test Case 3: Edge case - exact capacity
        System.out.println("Test Case 3: Items exactly fill capacity");
        Item[] items3 = {
            new Item(3, 30, "X"),
            new Item(2, 20, "Y"),
            new Item(5, 50, "Z")
        };
        int capacity3 = 5;
        
        System.out.println("Items: " + Arrays.toString(items3));
        System.out.println("Capacity: " + capacity3);
        
        KnapsackResult result3 = knapsackWithItems(items3, capacity3);
        System.out.println("Maximum value: " + result3.maxValue);
        System.out.println("Selected items: " + result3.selectedItems);
        System.out.println("Note: Item Z exactly fills the knapsack");
        System.out.println();
        
        // Test Case 4: All items too heavy
        System.out.println("Test Case 4: All items too heavy");
        Item[] items4 = {
            new Item(10, 100, "Heavy1"),
            new Item(15, 150, "Heavy2"),
            new Item(20, 200, "Heavy3")
        };
        int capacity4 = 5;
        
        System.out.println("Items: " + Arrays.toString(items4));
        System.out.println("Capacity: " + capacity4);
        
        KnapsackResult result4 = knapsackWithItems(items4, capacity4);
        System.out.println("Maximum value: " + result4.maxValue);
        System.out.println("Selected items: " + result4.selectedItems);
        System.out.println("Note: No items can fit in the knapsack");
        System.out.println();
        
        // Test Case 5: Compare approaches
        System.out.println("Test Case 5: Compare 2D vs Optimized approaches");
        Item[] items5 = {
            new Item(1, 1, "T1"),
            new Item(3, 4, "T2"),
            new Item(4, 5, "T3"),
            new Item(5, 7, "T4"),
            new Item(7, 9, "T5")
        };
        int capacity5 = 10;
        
        long startTime = System.nanoTime();
        int value2D = knapsack2D(items5, capacity5);
        long time2D = System.nanoTime() - startTime;
        
        startTime = System.nanoTime();
        int valueOptimized = knapsackOptimized(items5, capacity5);
        long timeOptimized = System.nanoTime() - startTime;
        
        System.out.println("2D DP approach: " + value2D + " (Time: " + time2D / 1000 + " μs)");
        System.out.println("Optimized approach: " + valueOptimized + " (Time: " + timeOptimized / 1000 + " μs)");
        System.out.println("Both produce same result: " + (value2D == valueOptimized));
        System.out.println();
        
        // Test Case 6: Comparison with fractional knapsack
        System.out.println("Test Case 6: 0/1 vs Fractional Knapsack comparison");
        compareWithFractionalKnapsack(items1, capacity1);
    }
}