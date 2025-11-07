/**
 * 0/1 Knapsack Problem - Branch & Bound Implementation
 * 
 * This implementation demonstrates Branch & Bound strategy for the classic
 * 0/1 Knapsack optimization problem. Unlike dynamic programming, this approach
 * explores the solution space systematically while using bounds to prune
 * suboptimal branches early.
 * 
 * Key Concepts:
 * - Branch & Bound: Systematic enumeration with intelligent pruning
 * - Upper Bound: Fractional knapsack relaxation for optimistic estimate
 * - State Space Tree: Binary decisions (include/exclude item)
 * - Best-First Search: Explore most promising nodes first
 * 
 * Time Complexity: O(2^n) worst case, but often much better with pruning
 * Space Complexity: O(n) for recursion stack + queue storage
 * 
 * Real-world Applications:
 * - Resource allocation optimization
 * - Portfolio selection with constraints
 * - Cargo loading optimization
 * - Budget allocation problems
 */

import java.util.*;

public class KnapsackBranchBound {
    
    static class Item {
        int weight, value, index;
        double ratio; // value-to-weight ratio
        
        Item(int weight, int value, int index) {
            this.weight = weight;
            this.value = value;
            this.index = index;
            this.ratio = (double) value / weight;
        }
        
        @Override
        public String toString() {
            return String.format("Item%d(w=%d, v=%d, r=%.2f)", 
                               index, weight, value, ratio);
        }
    }
    
    static class KnapsackNode {
        int level;              // Current item being considered
        int profit;             // Current profit
        int weight;             // Current weight
        double bound;           // Upper bound estimate
        boolean[] included;     // Items included so far
        
        KnapsackNode(int n) {
            level = 0;
            profit = 0;
            weight = 0;
            bound = 0.0;
            included = new boolean[n];
        }
        
        KnapsackNode(KnapsackNode other) {
            level = other.level;
            profit = other.profit;
            weight = other.weight;
            bound = other.bound;
            included = other.included.clone();
        }
    }
    
    static class KnapsackResult {
        boolean[] solution;
        int maxProfit;
        int totalWeight;
        int nodesExplored;
        int nodesPruned;
        double executionTime;
        List<String> steps;
        
        KnapsackResult(boolean[] solution, int profit, int weight, int explored, 
                      int pruned, double time, List<String> steps) {
            this.solution = solution.clone();
            this.maxProfit = profit;
            this.totalWeight = weight;
            this.nodesExplored = explored;
            this.nodesPruned = pruned;
            this.executionTime = time;
            this.steps = new ArrayList<>(steps);
        }
    }
    
    private Item[] items;
    private int capacity;
    private int n;
    private int maxProfit;
    private boolean[] bestSolution;
    private int nodesExplored;
    private int nodesPruned;
    private List<String> steps;
    private boolean verbose;
    
    public KnapsackBranchBound(int[] weights, int[] values, int capacity, boolean verbose) {
        this.n = weights.length;
        this.capacity = capacity;
        this.verbose = verbose;
        this.steps = new ArrayList<>();
        
        // Create items and sort by value-to-weight ratio (descending)
        this.items = new Item[n];
        for (int i = 0; i < n; i++) {
            items[i] = new Item(weights[i], values[i], i);
        }
        
        // Sort items by ratio in descending order for better bounds
        Arrays.sort(items, (a, b) -> Double.compare(b.ratio, a.ratio));
        
        if (verbose) {
            steps.add("=== Items sorted by value-to-weight ratio ===");
            for (Item item : items) {
                steps.add("  " + item);
            }
        }
    }
    
    /**
     * Solve knapsack using Branch & Bound
     */
    public KnapsackResult solveKnapsack() {
        long startTime = System.nanoTime();
        
        if (verbose) {
            steps.add("=== Starting Knapsack Branch & Bound Solution ===");
            steps.add("Capacity: " + capacity + ", Items: " + n);
        }
        
        maxProfit = 0;
        bestSolution = new boolean[n];
        nodesExplored = 0;
        nodesPruned = 0;
        
        // Initialize root node
        KnapsackNode root = new KnapsackNode(n);
        root.bound = calculateBound(root);
        
        if (verbose) {
            steps.add("Root node bound: " + String.format("%.2f", root.bound));
        }
        
        // Use priority queue for best-first search (max-heap by bound)
        PriorityQueue<KnapsackNode> pq = new PriorityQueue<>((a, b) -> 
            Double.compare(b.bound, a.bound));
        pq.offer(root);
        
        while (!pq.isEmpty()) {
            KnapsackNode current = pq.poll();
            nodesExplored++;
            
            if (verbose && nodesExplored <= 15) {
                steps.add("Exploring node at level " + current.level + 
                         ", profit: " + current.profit + 
                         ", weight: " + current.weight + 
                         ", bound: " + String.format("%.2f", current.bound));
            }
            
            // Pruning check
            if (current.bound <= maxProfit) {
                nodesPruned++;
                if (verbose && nodesExplored <= 15) {
                    steps.add("  Pruned: bound " + String.format("%.2f", current.bound) + 
                             " <= best " + maxProfit);
                }
                continue;
            }
            
            // Check if we've reached a leaf node
            if (current.level == n) {
                if (current.profit > maxProfit) {
                    maxProfit = current.profit;
                    System.arraycopy(current.included, 0, bestSolution, 0, n);
                    
                    if (verbose) {
                        steps.add("*** New best solution found! ***");
                        steps.add("Profit: " + maxProfit + ", Weight: " + current.weight);
                        steps.add("Items: " + getSelectedItemsString(current.included));
                    }
                }
                continue;
            }
            
            Item currentItem = items[current.level];
            
            // Branch 1: Include current item (if it fits)
            if (current.weight + currentItem.weight <= capacity) {
                KnapsackNode includeNode = new KnapsackNode(current);
                includeNode.level = current.level + 1;
                includeNode.profit = current.profit + currentItem.value;
                includeNode.weight = current.weight + currentItem.weight;
                includeNode.included[current.level] = true;
                includeNode.bound = calculateBound(includeNode);
                
                if (includeNode.bound > maxProfit) {
                    pq.offer(includeNode);
                } else {
                    nodesPruned++;
                }
            }
            
            // Branch 2: Exclude current item
            KnapsackNode excludeNode = new KnapsackNode(current);
            excludeNode.level = current.level + 1;
            excludeNode.bound = calculateBound(excludeNode);
            
            if (excludeNode.bound > maxProfit) {
                pq.offer(excludeNode);
            } else {
                nodesPruned++;
            }
        }
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        if (verbose) {
            steps.add("=== Final Results ===");
            steps.add("Maximum profit: " + maxProfit);
            steps.add("Total weight: " + getTotalWeight(bestSolution));
            steps.add("Selected items: " + getSelectedItemsString(bestSolution));
            steps.add("Nodes explored: " + nodesExplored);
            steps.add("Nodes pruned: " + nodesPruned);
            steps.add("Execution time: " + String.format("%.2f", executionTime) + " ms");
        }
        
        // Convert solution back to original item indices
        boolean[] originalSolution = new boolean[n];
        for (int i = 0; i < n; i++) {
            if (bestSolution[i]) {
                originalSolution[items[i].index] = true;
            }
        }
        
        return new KnapsackResult(originalSolution, maxProfit, getTotalWeight(bestSolution),
                                nodesExplored, nodesPruned, executionTime, steps);
    }
    
    /**
     * Calculate upper bound using fractional knapsack relaxation
     */
    private double calculateBound(KnapsackNode node) {
        if (node.weight >= capacity) return 0;
        
        double bound = node.profit;
        int remainingCapacity = capacity - node.weight;
        
        // Add items greedily (fractionally if necessary)
        for (int i = node.level; i < n; i++) {
            if (items[i].weight <= remainingCapacity) {
                // Include entire item
                bound += items[i].value;
                remainingCapacity -= items[i].weight;
            } else {
                // Include fraction of item
                bound += items[i].ratio * remainingCapacity;
                break;
            }
        }
        
        return bound;
    }
    
    private int getTotalWeight(boolean[] solution) {
        int weight = 0;
        for (int i = 0; i < n; i++) {
            if (solution[i]) {
                weight += items[i].weight;
            }
        }
        return weight;
    }
    
    private String getSelectedItemsString(boolean[] solution) {
        StringBuilder sb = new StringBuilder();
        boolean first = true;
        for (int i = 0; i < n; i++) {
            if (solution[i]) {
                if (!first) sb.append(", ");
                sb.append(items[i].toString());
                first = false;
            }
        }
        return sb.toString();
    }
    
    /**
     * Brute force solution for comparison
     */
    public KnapsackResult bruteForceSolution() {
        long startTime = System.nanoTime();
        
        int maxProfit = 0;
        boolean[] bestSolution = new boolean[n];
        int combinations = 1 << n; // 2^n combinations
        
        for (int mask = 0; mask < combinations; mask++) {
            int currentWeight = 0, currentProfit = 0;
            boolean[] currentSolution = new boolean[n];
            
            for (int i = 0; i < n; i++) {
                if ((mask & (1 << i)) != 0) {
                    currentWeight += items[i].weight;
                    currentProfit += items[i].value;
                    currentSolution[i] = true;
                }
            }
            
            if (currentWeight <= capacity && currentProfit > maxProfit) {
                maxProfit = currentProfit;
                System.arraycopy(currentSolution, 0, bestSolution, 0, n);
            }
        }
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        // Convert back to original indices
        boolean[] originalSolution = new boolean[n];
        for (int i = 0; i < n; i++) {
            if (bestSolution[i]) {
                originalSolution[items[i].index] = true;
            }
        }
        
        List<String> bruteSteps = Arrays.asList(
            "=== Brute Force Results ===",
            "Combinations checked: " + combinations,
            "Maximum profit: " + maxProfit,
            "Execution time: " + String.format("%.2f", executionTime) + " ms"
        );
        
        return new KnapsackResult(originalSolution, maxProfit, getTotalWeight(bestSolution),
                                combinations, 0, executionTime, bruteSteps);
    }
    
    /**
     * Dynamic programming solution for comparison
     */
    public KnapsackResult dynamicProgrammingSolution() {
        long startTime = System.nanoTime();
        
        // Sort items back to original order for DP
        Item[] originalItems = new Item[n];
        for (int i = 0; i < n; i++) {
            originalItems[items[i].index] = items[i];
        }
        
        int[][] dp = new int[n + 1][capacity + 1];
        
        // Fill DP table
        for (int i = 1; i <= n; i++) {
            for (int w = 1; w <= capacity; w++) {
                if (originalItems[i-1].weight <= w) {
                    dp[i][w] = Math.max(dp[i-1][w], 
                                      dp[i-1][w - originalItems[i-1].weight] + originalItems[i-1].value);
                } else {
                    dp[i][w] = dp[i-1][w];
                }
            }
        }
        
        // Backtrack to find solution
        boolean[] solution = new boolean[n];
        int w = capacity;
        for (int i = n; i > 0 && dp[i][w] > 0; i--) {
            if (dp[i][w] != dp[i-1][w]) {
                solution[i-1] = true;
                w -= originalItems[i-1].weight;
            }
        }
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        int totalWeight = 0, totalProfit = dp[n][capacity];
        for (int i = 0; i < n; i++) {
            if (solution[i]) {
                totalWeight += originalItems[i].weight;
            }
        }
        
        List<String> dpSteps = Arrays.asList(
            "=== Dynamic Programming Results ===",
            "DP table size: " + (n + 1) + " x " + (capacity + 1),
            "Maximum profit: " + totalProfit,
            "Execution time: " + String.format("%.2f", executionTime) + " ms"
        );
        
        return new KnapsackResult(solution, totalProfit, totalWeight, 
                                (n + 1) * (capacity + 1), 0, executionTime, dpSteps);
    }
    
    public static void main(String[] args) {
        System.out.println("=== 0/1 Knapsack Problem - Branch & Bound ===\n");
        
        // Test case 1: Classic example
        int[] weights1 = {10, 20, 30};
        int[] values1 = {60, 100, 120};
        int capacity1 = 50;
        
        System.out.println("Test Case 1: Classic 3-item knapsack");
        System.out.println("Items: ");
        for (int i = 0; i < weights1.length; i++) {
            System.out.printf("  Item%d: weight=%d, value=%d, ratio=%.2f\n", 
                            i, weights1[i], values1[i], (double)values1[i]/weights1[i]);
        }
        System.out.println("Capacity: " + capacity1);
        
        KnapsackBranchBound knapsack1 = new KnapsackBranchBound(weights1, values1, capacity1, true);
        KnapsackResult result1 = knapsack1.solveKnapsack();
        
        System.out.println("\nBranch & Bound Solution:");
        for (String step : result1.steps) {
            System.out.println(step);
        }
        
        // Compare with other methods
        System.out.println("\nComparing with other methods:");
        
        KnapsackResult bruteResult1 = knapsack1.bruteForceSolution();
        for (String step : bruteResult1.steps) {
            System.out.println(step);
        }
        
        KnapsackResult dpResult1 = knapsack1.dynamicProgrammingSolution();
        for (String step : dpResult1.steps) {
            System.out.println(step);
        }
        
        // Test case 2: Larger problem
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: Larger knapsack problem");
        
        int[] weights2 = {5, 4, 6, 3, 7, 2, 8};
        int[] values2 = {10, 40, 30, 50, 20, 60, 25};
        int capacity2 = 15;
        
        System.out.println("Items: ");
        for (int i = 0; i < weights2.length; i++) {
            System.out.printf("  Item%d: weight=%d, value=%d, ratio=%.2f\n", 
                            i, weights2[i], values2[i], (double)values2[i]/weights2[i]);
        }
        System.out.println("Capacity: " + capacity2);
        
        KnapsackBranchBound knapsack2 = new KnapsackBranchBound(weights2, values2, capacity2, false);
        KnapsackResult result2 = knapsack2.solveKnapsack();
        
        System.out.println("\nBranch & Bound Results:");
        System.out.println("Maximum profit: " + result2.maxProfit);
        System.out.println("Total weight: " + result2.totalWeight);
        System.out.print("Selected items: ");
        for (int i = 0; i < result2.solution.length; i++) {
            if (result2.solution[i]) {
                System.out.print("Item" + i + " ");
            }
        }
        System.out.println();
        System.out.println("Nodes explored: " + result2.nodesExplored);
        System.out.println("Nodes pruned: " + result2.nodesPruned);
        System.out.println("Execution time: " + String.format("%.2f", result2.executionTime) + " ms");
        
        // Performance comparison
        KnapsackResult bruteResult2 = knapsack2.bruteForceSolution();
        KnapsackResult dpResult2 = knapsack2.dynamicProgrammingSolution();
        
        System.out.println("\n=== Performance Comparison ===");
        System.out.printf("%-15s | %12s | %12s | %10s\n", "Method", "Operations", "Space", "Time (ms)");
        System.out.println("-".repeat(55));
        System.out.printf("%-15s | %12d | %12s | %10.2f\n", "Branch & Bound", 
                         result2.nodesExplored, "O(n)", result2.executionTime);
        System.out.printf("%-15s | %12d | %12s | %10.2f\n", "Brute Force", 
                         bruteResult2.nodesExplored, "O(1)", bruteResult2.executionTime);
        System.out.printf("%-15s | %12d | %12s | %10.2f\n", "Dynamic Prog", 
                         dpResult2.nodesExplored, "O(nW)", dpResult2.executionTime);
        
        System.out.println("\n=== When to use each approach ===");
        System.out.println("Branch & Bound:");
        System.out.println("- When you need optimal solution");
        System.out.println("- Items have good value-to-weight ratios");
        System.out.println("- Memory is limited");
        System.out.println("- Can afford variable execution time");
        
        System.out.println("\nDynamic Programming:");
        System.out.println("- When capacity is not too large");
        System.out.println("- Consistent execution time needed");
        System.out.println("- Plenty of memory available");
        
        System.out.println("\nBrute Force:");
        System.out.println("- Very small problem sizes only");
        System.out.println("- When implementation simplicity matters");
        
        demonstrateScaling();
    }
    
    private static void demonstrateScaling() {
        System.out.println("\n=== Scaling Analysis ===");
        
        Random rand = new Random(42);
        int[] sizes = {5, 7, 10, 12};
        int baseCapacity = 20;
        
        System.out.println("Items | Capacity | Nodes Explored | Nodes Pruned | Pruning % | Time (ms)");
        System.out.println("-".repeat(75));
        
        for (int size : sizes) {
            int[] weights = new int[size];
            int[] values = new int[size];
            
            for (int i = 0; i < size; i++) {
                weights[i] = 1 + rand.nextInt(10);
                values[i] = 5 + rand.nextInt(45);
            }
            
            int capacity = baseCapacity + size * 2;
            
            KnapsackBranchBound knapsack = new KnapsackBranchBound(weights, values, capacity, false);
            KnapsackResult result = knapsack.solveKnapsack();
            
            double pruningPercent = (double) result.nodesPruned / 
                                  (result.nodesExplored + result.nodesPruned) * 100;
            
            System.out.printf("%5d | %8d | %14d | %12d | %8.1f%% | %8.2f\n",
                size, capacity, result.nodesExplored, result.nodesPruned, 
                pruningPercent, result.executionTime);
        }
        
        System.out.println("\nKey Observations:");
        System.out.println("- Pruning becomes more effective with larger problems");
        System.out.println("- Good bounds are essential for performance");
        System.out.println("- Best-first search finds optimal solutions quickly");
    }
}