import java.util.*;

/**
 * Greedy Strategy: Fractional Knapsack Problem
 * Core Idea: Always choose item with highest value-to-weight ratio first
 * Time Complexity: O(n log n) due to sorting
 * Space Complexity: O(n) for storing items and results
 */
public class FractionalKnapsack {
    
    /**
     * Item class to represent knapsack items
     */
    static class Item {
        int weight;
        int value;
        double valuePerWeight;
        String name;
        
        Item(String name, int weight, int value) {
            this.name = name;
            this.weight = weight;
            this.value = value;
            this.valuePerWeight = (double) value / weight;
        }
        
        @Override
        public String toString() {
            return String.format("%s (w:%d, v:%d, v/w:%.2f)", 
                               name, weight, value, valuePerWeight);
        }
    }
    
    /**
     * Result class to store knapsack solution
     */
    static class KnapsackResult {
        List<Item> selectedItems;
        List<Double> fractions;
        double totalValue;
        int totalWeight;
        
        KnapsackResult() {
            selectedItems = new ArrayList<>();
            fractions = new ArrayList<>();
            totalValue = 0;
            totalWeight = 0;
        }
        
        void addItem(Item item, double fraction) {
            selectedItems.add(item);
            fractions.add(fraction);
            totalValue += item.value * fraction;
            totalWeight += item.weight * fraction;
        }
    }
    
    /**
     * Solve fractional knapsack using greedy approach
     * @param items Array of available items
     * @param capacity Maximum weight capacity of knapsack
     * @return KnapsackResult containing optimal solution
     */
    public static KnapsackResult solveKnapsack(Item[] items, int capacity) {
        KnapsackResult result = new KnapsackResult();
        
        // Step 1: Sort items by value-to-weight ratio in descending order (greedy choice)
        Arrays.sort(items, (a, b) -> Double.compare(b.valuePerWeight, a.valuePerWeight));
        
        int remainingCapacity = capacity;
        
        // Step 2: Process items in order of decreasing value-to-weight ratio
        for (Item item : items) {
            if (remainingCapacity == 0) {
                break; // Knapsack is full
            }
            
            // Greedy choice: take as much of current item as possible
            if (item.weight <= remainingCapacity) {
                // Take entire item
                result.addItem(item, 1.0);
                remainingCapacity -= item.weight;
            } else {
                // Take fractional part of item
                double fraction = (double) remainingCapacity / item.weight;
                result.addItem(item, fraction);
                remainingCapacity = 0; // Knapsack is now full
            }
        }
        
        return result;
    }
    
    /**
     * Print detailed solution
     */
    public static void printSolution(KnapsackResult result, int capacity) {
        System.out.println("Optimal solution:");
        System.out.println("Items selected (in order of selection):");
        
        for (int i = 0; i < result.selectedItems.size(); i++) {
            Item item = result.selectedItems.get(i);
            double fraction = result.fractions.get(i);
            
            if (fraction == 1.0) {
                System.out.printf("  %s - Full item (fraction: %.0f%%)\n", 
                                item, fraction * 100);
            } else {
                System.out.printf("  %s - Partial item (fraction: %.1f%%)\n", 
                                item, fraction * 100);
            }
        }
        
        System.out.printf("Total value: %.2f\n", result.totalValue);
        System.out.printf("Total weight: %.1f / %d\n", result.totalWeight, capacity);
        System.out.printf("Efficiency: %.2f%% capacity used\n", 
                         (result.totalWeight / capacity) * 100);
    }
    
    /**
     * Print all available items sorted by value-to-weight ratio
     */
    public static void printItemsByRatio(Item[] items) {
        // Create copy to avoid modifying original array
        Item[] sortedItems = Arrays.copyOf(items, items.length);
        Arrays.sort(sortedItems, (a, b) -> Double.compare(b.valuePerWeight, a.valuePerWeight));
        
        System.out.println("Items sorted by value-to-weight ratio (greedy order):");
        for (int i = 0; i < sortedItems.length; i++) {
            System.out.printf("  %d. %s\n", i + 1, sortedItems[i]);
        }
    }
    
    /**
     * Compare with 0/1 knapsack approach (taking whole items only)
     */
    public static void compareWith01Knapsack(Item[] items, int capacity) {
        // Sort by value-to-weight ratio
        Item[] sortedItems = Arrays.copyOf(items, items.length);
        Arrays.sort(sortedItems, (a, b) -> Double.compare(b.valuePerWeight, a.valuePerWeight));
        
        int totalValue = 0;
        int totalWeight = 0;
        List<Item> selectedItems = new ArrayList<>();
        
        for (Item item : sortedItems) {
            if (totalWeight + item.weight <= capacity) {
                selectedItems.add(item);
                totalValue += item.value;
                totalWeight += item.weight;
            }
        }
        
        System.out.println("0/1 Knapsack comparison (whole items only):");
        System.out.println("Selected items: " + selectedItems.size());
        for (Item item : selectedItems) {
            System.out.println("  " + item);
        }
        System.out.println("Total value: " + totalValue);
        System.out.println("Total weight: " + totalWeight + " / " + capacity);
    }
    
    public static void main(String[] args) {
        System.out.println("=== Fractional Knapsack - Greedy Algorithm ===");
        
        // Test Case 1: Classic example
        System.out.println("Test Case 1: Classic fractional knapsack");
        Item[] items1 = {
            new Item("Gold", 10, 60),
            new Item("Silver", 20, 100),
            new Item("Bronze", 30, 120)
        };
        int capacity1 = 50;
        
        System.out.println("Available items:");
        for (Item item : items1) {
            System.out.println("  " + item);
        }
        System.out.println("Knapsack capacity: " + capacity1);
        System.out.println();
        
        printItemsByRatio(items1);
        System.out.println();
        
        KnapsackResult result1 = solveKnapsack(items1, capacity1);
        printSolution(result1, capacity1);
        System.out.println();
        
        compareWith01Knapsack(items1, capacity1);
        System.out.println();
        
        // Test Case 2: All items fit exactly
        System.out.println("Test Case 2: All items fit exactly");
        Item[] items2 = {
            new Item("Item A", 5, 10),
            new Item("Item B", 4, 40),
            new Item("Item C", 6, 30),
            new Item("Item D", 3, 50)
        };
        int capacity2 = 18; // Exactly sum of all weights
        
        System.out.println("Available items:");
        for (Item item : items2) {
            System.out.println("  " + item);
        }
        System.out.println("Knapsack capacity: " + capacity2);
        System.out.println();
        
        KnapsackResult result2 = solveKnapsack(items2, capacity2);
        printSolution(result2, capacity2);
        System.out.println();
        
        // Test Case 3: Very small capacity
        System.out.println("Test Case 3: Very small capacity");
        Item[] items3 = {
            new Item("Diamond", 1, 15),
            new Item("Ruby", 2, 20),
            new Item("Emerald", 3, 25)
        };
        int capacity3 = 2;
        
        System.out.println("Available items:");
        for (Item item : items3) {
            System.out.println("  " + item);
        }
        System.out.println("Knapsack capacity: " + capacity3);
        System.out.println();
        
        KnapsackResult result3 = solveKnapsack(items3, capacity3);
        printSolution(result3, capacity3);
        System.out.println();
        
        // Test Case 4: Single item larger than capacity
        System.out.println("Test Case 4: Single item larger than capacity");
        Item[] items4 = {
            new Item("Heavy Gold", 15, 100)
        };
        int capacity4 = 10;
        
        System.out.println("Available items:");
        for (Item item : items4) {
            System.out.println("  " + item);
        }
        System.out.println("Knapsack capacity: " + capacity4);
        System.out.println();
        
        KnapsackResult result4 = solveKnapsack(items4, capacity4);
        printSolution(result4, capacity4);
        System.out.println();
        
        // Test Case 5: Items with same value-to-weight ratio
        System.out.println("Test Case 5: Items with same value-to-weight ratio");
        Item[] items5 = {
            new Item("Item X", 2, 10), // ratio = 5.0
            new Item("Item Y", 4, 20), // ratio = 5.0
            new Item("Item Z", 6, 30)  // ratio = 5.0
        };
        int capacity5 = 8;
        
        System.out.println("Available items:");
        for (Item item : items5) {
            System.out.println("  " + item);
        }
        System.out.println("Knapsack capacity: " + capacity5);
        System.out.println();
        
        KnapsackResult result5 = solveKnapsack(items5, capacity5);
        printSolution(result5, capacity5);
        System.out.println("Note: When ratios are equal, algorithm processes in current order");
    }
}