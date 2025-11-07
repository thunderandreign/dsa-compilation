#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Dynamic Programming Strategy: 0/1 Knapsack Problem
 * Core Idea: Build optimal solution by considering each item and weight capacity
 * Time Complexity: O(n * W) where n is number of items and W is capacity
 * Space Complexity: O(n * W) for 2D table, can be optimized to O(W)
 */

#define MAX_ITEMS 100
#define MAX_CAPACITY 1000
#define MAX_NAME_LEN 20

/**
 * Item structure to represent knapsack items
 */
typedef struct {
    int weight;
    int value;
    char name[MAX_NAME_LEN];
} Item;

/**
 * Result structure to hold knapsack solution
 */
typedef struct {
    int maxValue;
    int selectedItems[MAX_ITEMS];
    int numSelected;
} KnapsackResult;

/**
 * Utility function to find maximum of two integers
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * Solve 0/1 Knapsack using 2D Dynamic Programming table
 * @param items Array of items
 * @param n Number of items
 * @param capacity Knapsack capacity
 * @param dp 2D DP table (output parameter)
 * @return Maximum value that can be obtained
 */
int knapsack2D(Item items[], int n, int capacity, int dp[][MAX_CAPACITY + 1]) {
    // Base case: 0 items or 0 capacity = 0 value
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (i == 0 || w == 0) {
                dp[i][w] = 0;
            } else if (items[i - 1].weight <= w) {
                // Max of (include item, exclude item)
                dp[i][w] = max(
                    items[i - 1].value + dp[i - 1][w - items[i - 1].weight],
                    dp[i - 1][w]
                );
            } else {
                // Item doesn't fit, exclude it
                dp[i][w] = dp[i - 1][w];
            }
        }
    }
    
    return dp[n][capacity];
}

/**
 * Solve 0/1 Knapsack with space optimization (1D array)
 * @param items Array of items
 * @param n Number of items
 * @param capacity Knapsack capacity
 * @return Maximum value that can be obtained
 */
int knapsackOptimized(Item items[], int n, int capacity) {
    // Use 1D array instead of 2D (space optimization)
    int dp[MAX_CAPACITY + 1] = {0};
    
    // Process each item
    for (int i = 0; i < n; i++) {
        // Traverse backwards to avoid using updated values in same iteration
        for (int w = capacity; w >= items[i].weight; w--) {
            dp[w] = max(dp[w], dp[w - items[i].weight] + items[i].value);
        }
    }
    
    return dp[capacity];
}

/**
 * Solve 0/1 Knapsack and return both max value and selected items
 * @param items Array of items
 * @param n Number of items
 * @param capacity Knapsack capacity
 * @param result KnapsackResult structure to store results
 */
void knapsackWithItems(Item items[], int n, int capacity, KnapsackResult* result) {
    int dp[MAX_ITEMS + 1][MAX_CAPACITY + 1];
    
    // Fill DP table
    result->maxValue = knapsack2D(items, n, capacity, dp);
    
    // Backtrack to find selected items
    result->numSelected = 0;
    int w = capacity;
    
    for (int i = n; i > 0 && w > 0; i--) {
        // If value changed from dp[i-1][w], then item i-1 was included
        if (dp[i][w] != dp[i - 1][w]) {
            result->selectedItems[result->numSelected++] = i - 1; // Store index
            w -= items[i - 1].weight;
        }
    }
    
    // Reverse the selected items array to get original order
    for (int i = 0; i < result->numSelected / 2; i++) {
        int temp = result->selectedItems[i];
        result->selectedItems[i] = result->selectedItems[result->numSelected - 1 - i];
        result->selectedItems[result->numSelected - 1 - i] = temp;
    }
}

/**
 * Print items information
 */
void printItems(Item items[], int n) {
    printf("Items:\n");
    for (int i = 0; i < n; i++) {
        printf("  %s: weight=%d, value=%d\n", items[i].name, items[i].weight, items[i].value);
    }
}

/**
 * Print the DP table for educational purposes
 */
void printDPTable(Item items[], int n, int capacity) {
    int dp[MAX_ITEMS + 1][MAX_CAPACITY + 1];
    knapsack2D(items, n, capacity, dp);
    
    printf("DP Table (rows: items, columns: capacity):\n");
    printf("Item\\Cap ");
    for (int w = 0; w <= capacity; w++) {
        printf("%3d", w);
    }
    printf("\n");
    
    for (int i = 0; i <= n; i++) {
        if (i == 0) {
            printf("   -    ");
        } else {
            printf("%-8s ", items[i - 1].name);
        }
        
        for (int w = 0; w <= capacity; w++) {
            printf("%3d", dp[i][w]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Demonstrate the decision process step by step
 */
void demonstrateDecisionProcess(Item items[], int n, int capacity) {
    printf("Decision Process for 0/1 Knapsack:\n");
    printf("Capacity: %d\n", capacity);
    printf("Items available:\n");
    for (int i = 0; i < n; i++) {
        printf("  %d. %s (weight: %d, value: %d)\n", i + 1, items[i].name, items[i].weight, items[i].value);
    }
    printf("\n");
    
    int dp[MAX_ITEMS + 1][MAX_CAPACITY + 1];
    
    // Initialize base cases
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = 0;
        }
    }
    
    for (int i = 1; i <= n; i++) {
        printf("Processing item %d: %s (weight: %d, value: %d)\n", 
               i, items[i - 1].name, items[i - 1].weight, items[i - 1].value);
        
        for (int w = 1; w <= capacity; w++) {
            dp[i][w] = dp[i - 1][w]; // Don't include item
            
            if (items[i - 1].weight <= w) {
                int includeValue = items[i - 1].value + dp[i - 1][w - items[i - 1].weight];
                if (includeValue > dp[i][w]) {
                    dp[i][w] = includeValue;
                    printf("  Capacity %d: Include item (value %d + %d = %d)\n", 
                           w, items[i - 1].value, dp[i - 1][w - items[i - 1].weight], includeValue);
                }
            }
        }
        printf("\n");
    }
    
    printf("Final maximum value: %d\n", dp[n][capacity]);
}

/**
 * Compare 0/1 knapsack with fractional knapsack
 */
void compareWithFractionalKnapsack(Item items[], int n, int capacity) {
    printf("Comparison: 0/1 Knapsack vs Fractional Knapsack\n");
    
    // 0/1 Knapsack result
    KnapsackResult result;
    knapsackWithItems(items, n, capacity, &result);
    
    printf("0/1 Knapsack (can't split items):\n");
    printf("  Max value: %d\n", result.maxValue);
    printf("  Selected items: ");
    for (int i = 0; i < result.numSelected; i++) {
        printf("%s", items[result.selectedItems[i]].name);
        if (i < result.numSelected - 1) printf(", ");
    }
    printf("\n");
    
    // Calculate total weight used
    int totalWeight = 0;
    for (int i = 0; i < result.numSelected; i++) {
        totalWeight += items[result.selectedItems[i]].weight;
    }
    printf("  Total weight: %d/%d\n", totalWeight, capacity);
    
    // For fractional knapsack comparison, we'd need to sort by value/weight ratio
    // This is a simplified comparison
    printf("\nFractional Knapsack (can split items, greedy approach):\n");
    printf("  Would potentially achieve higher value by splitting items\n");
    printf("  Note: Fractional >= 0/1 knapsack value (more flexibility)\n");
}

int main() {
    printf("=== 0/1 Knapsack Problem - Dynamic Programming ===\n");
    
    // Test Case 1: Classic knapsack problem
    printf("Test Case 1: Classic 0/1 knapsack problem\n");
    Item items1[] = {
        {10, 60, "A"},
        {20, 100, "B"},
        {30, 120, "C"}
    };
    int n1 = 3;
    int capacity1 = 50;
    
    printItems(items1, n1);
    printf("Capacity: %d\n", capacity1);
    
    KnapsackResult result1;
    knapsackWithItems(items1, n1, capacity1, &result1);
    printf("Maximum value: %d\n", result1.maxValue);
    printf("Selected items: ");
    for (int i = 0; i < result1.numSelected; i++) {
        printf("%s", items1[result1.selectedItems[i]].name);
        if (i < result1.numSelected - 1) printf(", ");
    }
    printf("\n\n");
    
    // Show DP table
    printDPTable(items1, n1, capacity1);
    
    // Test Case 2: More complex problem
    printf("Test Case 2: More complex problem\n");
    Item items2[] = {
        {5, 10, "Item1"},
        {4, 40, "Item2"},
        {6, 30, "Item3"},
        {3, 50, "Item4"}
    };
    int n2 = 4;
    int capacity2 = 10;
    
    printItems(items2, n2);
    printf("Capacity: %d\n", capacity2);
    
    KnapsackResult result2;
    knapsackWithItems(items2, n2, capacity2, &result2);
    printf("Maximum value: %d\n", result2.maxValue);
    printf("Selected items: ");
    for (int i = 0; i < result2.numSelected; i++) {
        printf("%s", items2[result2.selectedItems[i]].name);
        if (i < result2.numSelected - 1) printf(", ");
    }
    printf("\n\n");
    
    demonstrateDecisionProcess(items2, n2, capacity2);
    printf("\n");
    
    // Test Case 3: Edge case - exact capacity
    printf("Test Case 3: Items exactly fill capacity\n");
    Item items3[] = {
        {3, 30, "X"},
        {2, 20, "Y"},
        {5, 50, "Z"}
    };
    int n3 = 3;
    int capacity3 = 5;
    
    printItems(items3, n3);
    printf("Capacity: %d\n", capacity3);
    
    KnapsackResult result3;
    knapsackWithItems(items3, n3, capacity3, &result3);
    printf("Maximum value: %d\n", result3.maxValue);
    printf("Selected items: ");
    for (int i = 0; i < result3.numSelected; i++) {
        printf("%s", items3[result3.selectedItems[i]].name);
        if (i < result3.numSelected - 1) printf(", ");
    }
    printf("\n");
    printf("Note: Item Z exactly fills the knapsack\n\n");
    
    // Test Case 4: All items too heavy
    printf("Test Case 4: All items too heavy\n");
    Item items4[] = {
        {10, 100, "Heavy1"},
        {15, 150, "Heavy2"},
        {20, 200, "Heavy3"}
    };
    int n4 = 3;
    int capacity4 = 5;
    
    printItems(items4, n4);
    printf("Capacity: %d\n", capacity4);
    
    KnapsackResult result4;
    knapsackWithItems(items4, n4, capacity4, &result4);
    printf("Maximum value: %d\n", result4.maxValue);
    printf("Selected items: ");
    if (result4.numSelected == 0) {
        printf("None");
    } else {
        for (int i = 0; i < result4.numSelected; i++) {
            printf("%s", items4[result4.selectedItems[i]].name);
            if (i < result4.numSelected - 1) printf(", ");
        }
    }
    printf("\n");
    printf("Note: No items can fit in the knapsack\n\n");
    
    // Test Case 5: Compare approaches
    printf("Test Case 5: Compare 2D vs Optimized approaches\n");
    Item items5[] = {
        {1, 1, "T1"},
        {3, 4, "T2"},
        {4, 5, "T3"},
        {5, 7, "T4"},
        {7, 9, "T5"}
    };
    int n5 = 5;
    int capacity5 = 10;
    
    int dp[MAX_ITEMS + 1][MAX_CAPACITY + 1];
    int value2D = knapsack2D(items5, n5, capacity5, dp);
    int valueOptimized = knapsackOptimized(items5, n5, capacity5);
    
    printf("2D DP approach: %d\n", value2D);
    printf("Optimized approach: %d\n", valueOptimized);
    printf("Both produce same result: %s\n", (value2D == valueOptimized) ? "YES" : "NO");
    printf("\n");
    
    // Test Case 6: Comparison with fractional knapsack
    printf("Test Case 6: 0/1 vs Fractional Knapsack comparison\n");
    compareWithFractionalKnapsack(items1, n1, capacity1);
    
    return 0;
}