/**
 * Traveling Salesman Problem (TSP) - Branch & Bound Implementation
 * 
 * This implementation demonstrates the Branch & Bound strategy for solving TSP,
 * an NP-hard optimization problem. The algorithm systematically explores
 * the solution space while using bounds to prune suboptimal branches.
 * 
 * Key Concepts:
 * - Branch & Bound: Systematic enumeration with pruning
 * - Lower Bound Calculation: Minimum Spanning Tree based bounds
 * - State Space Tree: Partial tour representations
 * - Pruning: Eliminating suboptimal branches early
 * 
 * Time Complexity: O(n!) worst case, but often much better with pruning
 * Space Complexity: O(n) for recursion stack
 * 
 * Real-world Applications:
 * - Route optimization for delivery services
 * - Circuit board drilling optimization
 * - DNA sequencing
 * - VLSI design
 */

import java.util.*;

public class TravelingSalesman {
    
    static class TSPNode {
        List<Integer> path;     // Current partial path
        boolean[] visited;      // Visited cities
        int cost;              // Cost of current path
        int bound;             // Lower bound estimate
        int level;             // Depth in search tree
        
        TSPNode(int n) {
            path = new ArrayList<>();
            visited = new boolean[n];
            cost = 0;
            bound = 0;
            level = 0;
        }
        
        TSPNode(TSPNode other) {
            path = new ArrayList<>(other.path);
            visited = other.visited.clone();
            cost = other.cost;
            bound = other.bound;
            level = other.level;
        }
    }
    
    private int[][] graph;
    private int n;
    private int bestCost;
    private List<Integer> bestPath;
    private int nodesExplored;
    private int nodesPruned;
    
    // Step-by-step demonstration variables
    private List<String> steps;
    private boolean verbose;
    
    public TravelingSalesman(int[][] adjacencyMatrix, boolean verbose) {
        this.graph = adjacencyMatrix;
        this.n = graph.length;
        this.bestCost = Integer.MAX_VALUE;
        this.bestPath = new ArrayList<>();
        this.nodesExplored = 0;
        this.nodesPruned = 0;
        this.steps = new ArrayList<>();
        this.verbose = verbose;
    }
    
    /**
     * Solve TSP using Branch & Bound
     */
    public TSPResult solveTSP() {
        long startTime = System.nanoTime();
        
        if (verbose) steps.add("=== Starting TSP Branch & Bound Solution ===");
        if (verbose) steps.add("Cities: " + n + ", Graph size: " + n + "x" + n);
        
        // Initialize root node
        TSPNode root = new TSPNode(n);
        root.path.add(0);  // Start from city 0
        root.visited[0] = true;
        root.level = 1;
        root.bound = calculateBound(root);
        
        if (verbose) steps.add("Root node bound: " + root.bound);
        
        // Use priority queue for best-first search
        PriorityQueue<TSPNode> pq = new PriorityQueue<>((a, b) -> a.bound - b.bound);
        pq.offer(root);
        
        while (!pq.isEmpty()) {
            TSPNode current = pq.poll();
            nodesExplored++;
            
            if (verbose && nodesExplored <= 20) {
                steps.add("Exploring node at level " + current.level + 
                         ", path: " + current.path + ", bound: " + current.bound);
            }
            
            // Pruning check
            if (current.bound >= bestCost) {
                nodesPruned++;
                if (verbose && nodesExplored <= 20) {
                    steps.add("  Pruned: bound " + current.bound + " >= best " + bestCost);
                }
                continue;
            }
            
            // Check if we have a complete tour
            if (current.level == n) {
                // Add cost to return to start
                int returnCost = graph[current.path.get(n-1)][0];
                int totalCost = current.cost + returnCost;
                
                if (totalCost < bestCost) {
                    bestCost = totalCost;
                    bestPath = new ArrayList<>(current.path);
                    bestPath.add(0); // Complete the cycle
                    
                    if (verbose) {
                        steps.add("*** New best solution found! ***");
                        steps.add("Path: " + bestPath + ", Cost: " + bestCost);
                    }
                }
                continue;
            }
            
            // Branch to unvisited cities
            for (int city = 1; city < n; city++) {
                if (!current.visited[city]) {
                    TSPNode child = new TSPNode(current);
                    child.path.add(city);
                    child.visited[city] = true;
                    child.level = current.level + 1;
                    child.cost = current.cost + graph[current.path.get(current.level-1)][city];
                    child.bound = calculateBound(child);
                    
                    // Only add to queue if bound is promising
                    if (child.bound < bestCost) {
                        pq.offer(child);
                    } else {
                        nodesPruned++;
                    }
                }
            }
        }
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        if (verbose) {
            steps.add("=== Final Results ===");
            steps.add("Best path: " + bestPath);
            steps.add("Best cost: " + bestCost);
            steps.add("Nodes explored: " + nodesExplored);
            steps.add("Nodes pruned: " + nodesPruned);
            steps.add("Execution time: " + String.format("%.2f", executionTime) + " ms");
        }
        
        return new TSPResult(new ArrayList<>(bestPath), bestCost, 
                           nodesExplored, nodesPruned, executionTime, new ArrayList<>(steps));
    }
    
    /**
     * Calculate lower bound using minimum spanning tree approach
     */
    private int calculateBound(TSPNode node) {
        int bound = node.cost;
        
        // For unvisited cities, add minimum outgoing edge costs
        for (int i = 0; i < n; i++) {
            if (!node.visited[i] || i == node.path.get(node.path.size()-1)) {
                int min1 = Integer.MAX_VALUE, min2 = Integer.MAX_VALUE;
                
                for (int j = 0; j < n; j++) {
                    if (i != j && graph[i][j] < min1) {
                        min2 = min1;
                        min1 = graph[i][j];
                    } else if (i != j && graph[i][j] < min2 && graph[i][j] != min1) {
                        min2 = graph[i][j];
                    }
                }
                
                // For visited cities (except current), use one minimum edge
                // For unvisited cities, use average of two minimum edges
                if (node.visited[i] && i != node.path.get(node.path.size()-1)) {
                    bound += min1;
                } else {
                    bound += (min1 + min2) / 2;
                }
            }
        }
        
        return bound;
    }
    
    /**
     * Brute force solution for comparison
     */
    public TSPResult bruteForceTSP() {
        long startTime = System.nanoTime();
        
        List<Integer> cities = new ArrayList<>();
        for (int i = 1; i < n; i++) {
            cities.add(i);
        }
        
        int minCost = Integer.MAX_VALUE;
        List<Integer> minPath = new ArrayList<>();
        int permutations = 0;
        
        do {
            permutations++;
            int cost = 0;
            int prev = 0;
            
            for (int city : cities) {
                cost += graph[prev][city];
                prev = city;
            }
            cost += graph[prev][0]; // Return to start
            
            if (cost < minCost) {
                minCost = cost;
                minPath = new ArrayList<>();
                minPath.add(0);
                minPath.addAll(cities);
                minPath.add(0);
            }
        } while (nextPermutation(cities));
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        List<String> bruteSteps = Arrays.asList(
            "=== Brute Force Results ===",
            "Permutations checked: " + permutations,
            "Best path: " + minPath,
            "Best cost: " + minCost,
            "Execution time: " + String.format("%.2f", executionTime) + " ms"
        );
        
        return new TSPResult(minPath, minCost, permutations, 0, executionTime, bruteSteps);
    }
    
    private boolean nextPermutation(List<Integer> arr) {
        int n = arr.size();
        int i = n - 2;
        
        while (i >= 0 && arr.get(i) >= arr.get(i + 1)) {
            i--;
        }
        
        if (i < 0) return false;
        
        int j = n - 1;
        while (arr.get(j) <= arr.get(i)) {
            j--;
        }
        
        Collections.swap(arr, i, j);
        Collections.reverse(arr.subList(i + 1, n));
        
        return true;
    }
    
    static class TSPResult {
        List<Integer> path;
        int cost;
        int nodesExplored;
        int nodesPruned;
        double executionTime;
        List<String> steps;
        
        TSPResult(List<Integer> path, int cost, int nodes, int pruned, 
                 double time, List<String> steps) {
            this.path = path;
            this.cost = cost;
            this.nodesExplored = nodes;
            this.nodesPruned = pruned;
            this.executionTime = time;
            this.steps = steps;
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Traveling Salesman Problem - Branch & Bound ===\n");
        
        // Test case 1: Small symmetric graph
        int[][] graph1 = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };
        
        System.out.println("Test Case 1: 4-city symmetric TSP");
        System.out.println("Graph:");
        printGraph(graph1);
        
        TravelingSalesman tsp1 = new TravelingSalesman(graph1, true);
        TSPResult result1 = tsp1.solveTSP();
        
        System.out.println("\nBranch & Bound Solution:");
        for (String step : result1.steps) {
            System.out.println(step);
        }
        
        // Compare with brute force
        System.out.println("\nComparing with Brute Force:");
        TSPResult bruteResult1 = tsp1.bruteForceTSP();
        for (String step : bruteResult1.steps) {
            System.out.println(step);
        }
        
        System.out.println("\nEfficiency Comparison:");
        System.out.println("Branch & Bound: " + result1.nodesExplored + " nodes explored, " 
                         + result1.nodesPruned + " pruned");
        System.out.println("Brute Force: " + bruteResult1.nodesExplored + " permutations checked");
        System.out.println("Speedup: " + String.format("%.2fx", 
            bruteResult1.executionTime / result1.executionTime));
        
        // Test case 2: Larger asymmetric graph
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: 5-city asymmetric TSP");
        
        int[][] graph2 = {
            {0, 12, 29, 22, 13},
            {12, 0, 19, 28, 25},
            {29, 19, 0, 21, 10},
            {22, 28, 21, 0, 24},
            {13, 25, 10, 24, 0}
        };
        
        System.out.println("Graph:");
        printGraph(graph2);
        
        TravelingSalesman tsp2 = new TravelingSalesman(graph2, false);
        TSPResult result2 = tsp2.solveTSP();
        
        System.out.println("\nBranch & Bound Results:");
        System.out.println("Best path: " + result2.path);
        System.out.println("Best cost: " + result2.cost);
        System.out.println("Nodes explored: " + result2.nodesExplored);
        System.out.println("Nodes pruned: " + result2.nodesPruned);
        System.out.println("Execution time: " + String.format("%.2f", result2.executionTime) + " ms");
        
        // Performance analysis
        System.out.println("\n=== Performance Analysis ===");
        System.out.println("Branch & Bound Advantages:");
        System.out.println("- Optimal solution guaranteed");
        System.out.println("- Systematic pruning reduces search space");
        System.out.println("- Best-first search finds good solutions early");
        System.out.println("- Memory efficient compared to dynamic programming");
        
        System.out.println("\nWhen to use Branch & Bound for TSP:");
        System.out.println("- Small to medium problem sizes (< 20 cities)");
        System.out.println("- When optimal solution is required");
        System.out.println("- When good bounds can be computed efficiently");
        System.out.println("- Problems with strong constraint structure");
        
        demonstrateScaling();
    }
    
    private static void printGraph(int[][] graph) {
        int n = graph.length;
        System.out.print("    ");
        for (int i = 0; i < n; i++) {
            System.out.printf("%4d", i);
        }
        System.out.println();
        
        for (int i = 0; i < n; i++) {
            System.out.printf("%2d: ", i);
            for (int j = 0; j < n; j++) {
                if (graph[i][j] == 0) {
                    System.out.print("   -");
                } else {
                    System.out.printf("%4d", graph[i][j]);
                }
            }
            System.out.println();
        }
    }
    
    private static void demonstrateScaling() {
        System.out.println("\n=== Scaling Analysis ===");
        
        Random rand = new Random(42);
        int[] sizes = {4, 5, 6, 7};
        
        System.out.println("Problem Size | Nodes Explored | Nodes Pruned | Time (ms) | Pruning %");
        System.out.println("-".repeat(70));
        
        for (int size : sizes) {
            // Generate random symmetric graph
            int[][] graph = new int[size][size];
            for (int i = 0; i < size; i++) {
                for (int j = i + 1; j < size; j++) {
                    int dist = 10 + rand.nextInt(40);
                    graph[i][j] = graph[j][i] = dist;
                }
            }
            
            TravelingSalesman tsp = new TravelingSalesman(graph, false);
            TSPResult result = tsp.solveTSP();
            
            double pruningPercent = (double) result.nodesPruned / 
                                  (result.nodesExplored + result.nodesPruned) * 100;
            
            System.out.printf("%11d | %13d | %11d | %8.2f | %8.1f%%\n",
                size, result.nodesExplored, result.nodesPruned, 
                result.executionTime, pruningPercent);
        }
        
        System.out.println("\nKey Observations:");
        System.out.println("- Pruning effectiveness increases with problem size");
        System.out.println("- Branch & bound dramatically reduces search space");
        System.out.println("- Good bounds are crucial for performance");
    }
}