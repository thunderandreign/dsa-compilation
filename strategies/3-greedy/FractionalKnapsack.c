#include <stdio.h>
#include <stdlib.h>

/**
 * Greedy Strategy: Fractional Knapsack Problem
 * Core Idea: Always pick items with highest value-to-weight ratio first
 * Time Complexity: O(n log n) due to sorting
 * Space Complexity: O(1) excluding input
 */

/**
 * Item structure for the knapsack problem
 */
typedef struct {
    int value;
    int weight;
    double valueToWeightRatio;
    int index; // Original index for tracking
} Item;

/**
 * Compare function for sorting items by value-to-weight ratio (descending)
 */
int compareByRatio(const void* a, const void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    
    // Sort in descending order of value-to-weight ratio
    if (itemA->valueToWeightRatio < itemB->valueToWeightRatio) return 1;
    if (itemA->valueToWeightRatio > itemB->valueToWeightRatio) return -1;
    return 0;
}

/**
 * Calculate value-to-weight ratios for all items
 */
void calculateRatios(Item items[], int n) {
    for (int i = 0; i < n; i++) {
        if (items[i].weight > 0) {
            items[i].valueToWeightRatio = (double)items[i].value / items[i].weight;
        } else {
            items[i].valueToWeightRatio = 0.0; // Handle zero weight edge case
        }
    }
}

/**
 * Solve fractional knapsack using greedy approach
 * @param items Array of items
 * @param n Number of items
 * @param capacity Knapsack capacity
 * @param selected Array to store selection ratios (0.0 to 1.0)
 * @return Maximum value achievable
 */
double fractionalKnapsack(Item items[], int n, int capacity, double selected[]) {
    // Step 1: Calculate value-to-weight ratios
    calculateRatios(items, n);
    
    // Step 2: Sort items by value-to-weight ratio (greedy choice preparation)
    qsort(items, n, sizeof(Item), compareByRatio);
    
    // Initialize selection array
    for (int i = 0; i < n; i++) {
        selected[items[i].index] = 0.0;
    }
    
    double totalValue = 0.0;
    int remainingCapacity = capacity;
    
    // Step 3: Greedily select items
    for (int i = 0; i < n && remainingCapacity > 0; i++) {
        if (items[i].weight <= remainingCapacity) {
            // Take the whole item
            selected[items[i].index] = 1.0;
            totalValue += items[i].value;
            remainingCapacity -= items[i].weight;
        } else {
            // Take fraction of the item
            double fraction = (double)remainingCapacity / items[i].weight;
            selected[items[i].index] = fraction;
            totalValue += items[i].value * fraction;
            remainingCapacity = 0;
        }
    }
    
    return totalValue;
}

/**
 * Print items information
 */
void printItems(Item originalItems[], int n) {
    printf("Items (Value, Weight, Ratio):\n");
    for (int i = 0; i < n; i++) {
        double ratio = (originalItems[i].weight > 0) ? 
                      (double)originalItems[i].value / originalItems[i].weight : 0.0;
        printf("  Item %d: (%d, %d, %.2f)\n", 
               i, originalItems[i].value, originalItems[i].weight, ratio);
    }
}

/**
 * Print selection details
 */
void printSelection(Item originalItems[], int n, double selected[], 
                   double totalValue, int capacity) {
    printf("Selection (Item, Fraction, Value Obtained, Weight Used):\n");
    double totalWeight = 0.0;
    
    for (int i = 0; i < n; i++) {
        if (selected[i] > 0.0) {
            double valueObtained = originalItems[i].value * selected[i];
            double weightUsed = originalItems[i].weight * selected[i];
            totalWeight += weightUsed;
            
            printf("  Item %d: %.1f%%, Value: %.2f, Weight: %.2f\n", 
                   i, selected[i] * 100, valueObtained, weightUsed);
        }
    }
    
    printf("Total Value: %.2f\n", totalValue);
    printf("Total Weight Used: %.2f / %d\n", totalWeight, capacity);
}

/**
 * Demonstrate the greedy approach step by step
 */
void demonstrateGreedySteps(Item items[], int n, int capacity) {
    printf("Greedy Algorithm Steps:\n");
    
    // Calculate and display ratios
    calculateRatios(items, n);
    
    // Create a copy for sorting without affecting original
    Item* sortedItems = (Item*)malloc(n * sizeof(Item));
    for (int i = 0; i < n; i++) {
        sortedItems[i] = items[i];
    }
    
    qsort(sortedItems, n, sizeof(Item), compareByRatio);
    
    printf("1. Sort by value-to-weight ratio (descending):\n");
    for (int i = 0; i < n; i++) {
        printf("   Item %d: ratio %.2f\n", 
               sortedItems[i].index, sortedItems[i].valueToWeightRatio);
    }
    
    printf("2. Greedy selection:\n");
    int remainingCapacity = capacity;
    double totalValue = 0.0;
    
    for (int i = 0; i < n && remainingCapacity > 0; i++) {
        if (sortedItems[i].weight <= remainingCapacity) {
            printf("   Take full Item %d (weight %d, value %d) - Remaining capacity: %d\n",
                   sortedItems[i].index, sortedItems[i].weight, 
                   sortedItems[i].value, remainingCapacity - sortedItems[i].weight);
            totalValue += sortedItems[i].value;
            remainingCapacity -= sortedItems[i].weight;
        } else {
            double fraction = (double)remainingCapacity / sortedItems[i].weight;
            printf("   Take %.1f%% of Item %d (weight %.2f, value %.2f) - Capacity exhausted\n",
                   fraction * 100, sortedItems[i].index, 
                   sortedItems[i].weight * fraction, sortedItems[i].value * fraction);
            totalValue += sortedItems[i].value * fraction;
            remainingCapacity = 0;
        }
    }
    
    printf("3. Final total value: %.2f\n", totalValue);
    free(sortedItems);
}

int main() {
    printf("=== Fractional Knapsack - Greedy Algorithm ===\n");
    
    // Test Case 1: Classic example
    printf("Test Case 1: Classic fractional knapsack\n");
    Item items1[] = {
        {60, 10, 0.0, 0},   // Ratio: 6.0
        {100, 20, 0.0, 1},  // Ratio: 5.0
        {120, 30, 0.0, 2}   // Ratio: 4.0
    };
    int n1 = 3;
    int capacity1 = 50;
    double selected1[3];
    
    printItems(items1, n1);
    printf("Knapsack capacity: %d\n\n", capacity1);
    
    double maxValue1 = fractionalKnapsack(items1, n1, capacity1, selected1);
    printSelection(items1, n1, selected1, maxValue1, capacity1);
    printf("\n");
    
    // Demonstrate greedy approach
    Item items1Copy[] = {
        {60, 10, 0.0, 0},
        {100, 20, 0.0, 1},
        {120, 30, 0.0, 2}
    };
    demonstrateGreedySteps(items1Copy, n1, capacity1);
    printf("Expected: Take Item 0 (full), Item 1 (full), Item 2 (2/3)\n");
    printf("Optimal value: %.2f\n\n", maxValue1);
    
    // Test Case 2: All items fit
    printf("Test Case 2: All items fit in knapsack\n");
    Item items2[] = {
        {10, 5, 0.0, 0},    // Ratio: 2.0
        {40, 4, 0.0, 1},    // Ratio: 10.0
        {30, 6, 0.0, 2},    // Ratio: 5.0
        {50, 3, 0.0, 3}     // Ratio: 16.67
    };
    int n2 = 4;
    int capacity2 = 20;
    double selected2[4];
    
    printItems(items2, n2);
    printf("Knapsack capacity: %d\n\n", capacity2);
    
    double maxValue2 = fractionalKnapsack(items2, n2, capacity2, selected2);
    printSelection(items2, n2, selected2, maxValue2, capacity2);
    printf("Note: All items fit, so we take everything\n\n");
    
    // Test Case 3: Only one item fits
    printf("Test Case 3: Only one item fits completely\n");
    Item items3[] = {
        {20, 15, 0.0, 0},   // Ratio: 1.33
        {30, 10, 0.0, 1},   // Ratio: 3.0
        {40, 20, 0.0, 2}    // Ratio: 2.0
    };
    int n3 = 3;
    int capacity3 = 15;
    double selected3[3];
    
    printItems(items3, n3);
    printf("Knapsack capacity: %d\n\n", capacity3);
    
    double maxValue3 = fractionalKnapsack(items3, n3, capacity3, selected3);
    printSelection(items3, n3, selected3, maxValue3, capacity3);
    printf("Greedy choice: Take item with highest ratio first\n\n");
    
    // Test Case 4: High-value heavy item vs multiple light items
    printf("Test Case 4: Heavy valuable item vs light items\n");
    Item items4[] = {
        {100, 50, 0.0, 0},  // Ratio: 2.0
        {60, 20, 0.0, 1},   // Ratio: 3.0
        {40, 10, 0.0, 2},   // Ratio: 4.0
        {20, 5, 0.0, 3}     // Ratio: 4.0
    };
    int n4 = 4;
    int capacity4 = 40;
    double selected4[4];
    
    printItems(items4, n4);
    printf("Knapsack capacity: %d\n\n", capacity4);
    
    double maxValue4 = fractionalKnapsack(items4, n4, capacity4, selected4);
    printSelection(items4, n4, selected4, maxValue4, capacity4);
    printf("Greedy correctly chooses lighter items with better ratios\n\n");
    
    // Test Case 5: Edge case - zero capacity
    printf("Test Case 5: Zero capacity knapsack\n");
    Item items5[] = {
        {10, 1, 0.0, 0},
        {20, 2, 0.0, 1}
    };
    int n5 = 2;
    int capacity5 = 0;
    double selected5[2];
    
    printItems(items5, n5);
    printf("Knapsack capacity: %d\n\n", capacity5);
    
    double maxValue5 = fractionalKnapsack(items5, n5, capacity5, selected5);
    printSelection(items5, n5, selected5, maxValue5, capacity5);
    printf("Expected: No items selected due to zero capacity\n");
    
    return 0;
}