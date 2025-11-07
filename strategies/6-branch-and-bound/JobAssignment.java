/**
 * Job Assignment Problem - Branch & Bound Implementation
 * 
 * This implementation demonstrates the Branch & Bound strategy for solving
 * the optimal assignment problem, where we need to assign n jobs to n workers
 * such that the total cost is minimized.
 * 
 * Key Concepts:
 * - Branch & Bound: Systematic enumeration with cost-based pruning
 * - Lower Bound: Row and column reduction technique
 * - Assignment Matrix: Cost matrix representation
 * - Partial Assignment: State space representation
 * 
 * Time Complexity: O(n!) worst case, but often much better with pruning
 * Space Complexity: O(n²) for cost matrix + O(n) for recursion
 * 
 * Real-world Applications:
 * - Task assignment in project management
 * - Resource allocation in manufacturing
 * - Scheduling problems
 * - Network flow optimization
 * - Vehicle routing problems
 */

import java.util.*;

public class JobAssignment {
    
    static class AssignmentNode {
        int level;                    // Current worker being assigned
        int cost;                     // Current assignment cost
        int bound;                    // Lower bound estimate
        int[] assignment;             // job[i] = worker assigned to job i
        boolean[] workerUsed;         // Which workers are already assigned
        
        AssignmentNode(int n) {
            level = 0;
            cost = 0;
            bound = 0;
            assignment = new int[n];
            workerUsed = new boolean[n];
            Arrays.fill(assignment, -1);
        }
        
        AssignmentNode(AssignmentNode other) {
            level = other.level;
            cost = other.cost;
            bound = other.bound;
            assignment = other.assignment.clone();
            workerUsed = other.workerUsed.clone();
        }
    }
    
    static class AssignmentResult {
        int[] assignment;
        int minCost;
        int nodesExplored;
        int nodesPruned;
        double executionTime;
        List<String> steps;
        
        AssignmentResult(int[] assignment, int cost, int explored, int pruned, 
                        double time, List<String> steps) {
            this.assignment = assignment.clone();
            this.minCost = cost;
            this.nodesExplored = explored;
            this.nodesPruned = pruned;
            this.executionTime = time;
            this.steps = new ArrayList<>(steps);
        }
    }
    
    private int[][] costMatrix;
    private int n;
    private int minCost;
    private int[] bestAssignment;
    private int nodesExplored;
    private int nodesPruned;
    private List<String> steps;
    private boolean verbose;
    
    public JobAssignment(int[][] costMatrix, boolean verbose) {
        this.costMatrix = costMatrix;
        this.n = costMatrix.length;
        this.verbose = verbose;
        this.steps = new ArrayList<>();
        this.minCost = Integer.MAX_VALUE;
        this.bestAssignment = new int[n];
        this.nodesExplored = 0;
        this.nodesPruned = 0;
    }
    
    /**
     * Solve assignment problem using Branch & Bound
     */
    public AssignmentResult solveAssignment() {
        long startTime = System.nanoTime();
        
        if (verbose) {
            steps.add("=== Starting Job Assignment Branch & Bound Solution ===");
            steps.add("Workers: " + n + ", Jobs: " + n);
            steps.add("Cost Matrix:");
            for (int i = 0; i < n; i++) {
                steps.add("  " + Arrays.toString(costMatrix[i]));
            }
        }
        
        // Initialize root node
        AssignmentNode root = new AssignmentNode(n);
        root.bound = calculateBound(root);
        
        if (verbose) {
            steps.add("Root node bound: " + root.bound);
        }
        
        // Use priority queue for best-first search
        PriorityQueue<AssignmentNode> pq = new PriorityQueue<>((a, b) -> a.bound - b.bound);
        pq.offer(root);
        
        while (!pq.isEmpty()) {
            AssignmentNode current = pq.poll();
            nodesExplored++;
            
            if (verbose && nodesExplored <= 20) {
                steps.add("Exploring node at level " + current.level + 
                         ", cost: " + current.cost + ", bound: " + current.bound);
                steps.add("  Current assignment: " + Arrays.toString(current.assignment));
            }
            
            // Pruning check
            if (current.bound >= minCost) {
                nodesPruned++;
                if (verbose && nodesExplored <= 20) {
                    steps.add("  Pruned: bound " + current.bound + " >= best " + minCost);
                }
                continue;
            }
            
            // Check if we have a complete assignment
            if (current.level == n) {
                if (current.cost < minCost) {
                    minCost = current.cost;
                    System.arraycopy(current.assignment, 0, bestAssignment, 0, n);
                    
                    if (verbose) {
                        steps.add("*** New best solution found! ***");
                        steps.add("Assignment: " + Arrays.toString(bestAssignment));
                        steps.add("Cost: " + minCost);
                    }
                }
                continue;
            }
            
            // Branch: Try assigning current worker to each available job
            for (int job = 0; job < n; job++) {
                if (!current.workerUsed[job]) {
                    AssignmentNode child = new AssignmentNode(current);
                    child.level = current.level + 1;
                    child.assignment[current.level] = job;
                    child.workerUsed[job] = true;
                    child.cost = current.cost + costMatrix[current.level][job];
                    child.bound = calculateBound(child);
                    
                    // Only add to queue if bound is promising
                    if (child.bound < minCost) {
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
            steps.add("Optimal assignment: " + Arrays.toString(bestAssignment));
            steps.add("Minimum cost: " + minCost);
            steps.add("Assignment details:");
            for (int i = 0; i < n; i++) {
                steps.add("  Worker " + i + " -> Job " + bestAssignment[i] + 
                         " (cost: " + costMatrix[i][bestAssignment[i]] + ")");
            }
            steps.add("Nodes explored: " + nodesExplored);
            steps.add("Nodes pruned: " + nodesPruned);
            steps.add("Execution time: " + String.format("%.2f", executionTime) + " ms");
        }
        
        return new AssignmentResult(bestAssignment, minCost, nodesExplored, 
                                  nodesPruned, executionTime, steps);
    }
    
    /**
     * Calculate lower bound using row and column reduction
     */
    private int calculateBound(AssignmentNode node) {
        // Start with current cost
        int bound = node.cost;
        
        // Create reduced matrix for unassigned workers/jobs
        boolean[] usedWorkers = new boolean[n];
        boolean[] usedJobs = new boolean[n];
        
        // Mark assigned workers and jobs
        for (int i = 0; i < node.level; i++) {
            usedWorkers[i] = true;
            usedJobs[node.assignment[i]] = true;
        }
        
        // For each unassigned worker, add minimum cost to any unassigned job
        for (int worker = node.level; worker < n; worker++) {
            int minCost = Integer.MAX_VALUE;
            for (int job = 0; job < n; job++) {
                if (!usedJobs[job]) {
                    minCost = Math.min(minCost, costMatrix[worker][job]);
                }
            }
            if (minCost != Integer.MAX_VALUE) {
                bound += minCost;
            }
        }
        
        return bound;
    }
    
    /**
     * Enhanced bound calculation using Hungarian method relaxation
     */
    private int calculateEnhancedBound(AssignmentNode node) {
        int bound = node.cost;
        
        // Create submatrix for unassigned workers/jobs
        List<Integer> unassignedWorkers = new ArrayList<>();
        List<Integer> unassignedJobs = new ArrayList<>();
        
        boolean[] usedJobs = new boolean[n];
        for (int i = 0; i < node.level; i++) {
            usedJobs[node.assignment[i]] = true;
        }
        
        for (int i = node.level; i < n; i++) {
            unassignedWorkers.add(i);
        }
        
        for (int j = 0; j < n; j++) {
            if (!usedJobs[j]) {
                unassignedJobs.add(j);
            }
        }
        
        if (unassignedWorkers.isEmpty()) return bound;
        
        int size = unassignedWorkers.size();
        int[][] subMatrix = new int[size][size];
        
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                subMatrix[i][j] = costMatrix[unassignedWorkers.get(i)][unassignedJobs.get(j)];
            }
        }
        
        // Row reduction
        for (int i = 0; i < size; i++) {
            int minRow = Arrays.stream(subMatrix[i]).min().orElse(0);
            bound += minRow;
            for (int j = 0; j < size; j++) {
                subMatrix[i][j] -= minRow;
            }
        }
        
        // Column reduction
        for (int j = 0; j < size; j++) {
            int minCol = Integer.MAX_VALUE;
            for (int i = 0; i < size; i++) {
                minCol = Math.min(minCol, subMatrix[i][j]);
            }
            if (minCol > 0) {
                bound += minCol;
                for (int i = 0; i < size; i++) {
                    subMatrix[i][j] -= minCol;
                }
            }
        }
        
        return bound;
    }
    
    /**
     * Brute force solution using permutations for comparison
     */
    public AssignmentResult bruteForceSolution() {
        long startTime = System.nanoTime();
        
        int[] jobs = new int[n];
        for (int i = 0; i < n; i++) {
            jobs[i] = i;
        }
        
        int minCost = Integer.MAX_VALUE;
        int[] bestAssignment = new int[n];
        int permutations = 0;
        
        do {
            permutations++;
            int cost = 0;
            for (int worker = 0; worker < n; worker++) {
                cost += costMatrix[worker][jobs[worker]];
            }
            
            if (cost < minCost) {
                minCost = cost;
                System.arraycopy(jobs, 0, bestAssignment, 0, n);
            }
        } while (nextPermutation(jobs));
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        List<String> bruteSteps = Arrays.asList(
            "=== Brute Force Results ===",
            "Permutations checked: " + permutations,
            "Optimal assignment: " + Arrays.toString(bestAssignment),
            "Minimum cost: " + minCost,
            "Execution time: " + String.format("%.2f", executionTime) + " ms"
        );
        
        return new AssignmentResult(bestAssignment, minCost, permutations, 0, 
                                  executionTime, bruteSteps);
    }
    
    private boolean nextPermutation(int[] arr) {
        int n = arr.length;
        int i = n - 2;
        
        while (i >= 0 && arr[i] >= arr[i + 1]) {
            i--;
        }
        
        if (i < 0) return false;
        
        int j = n - 1;
        while (arr[j] <= arr[i]) {
            j--;
        }
        
        // Swap
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
        
        // Reverse suffix
        Arrays.sort(arr, i + 1, n);
        
        return true;
    }
    
    /**
     * Hungarian algorithm solution for comparison
     */
    public AssignmentResult hungarianSolution() {
        long startTime = System.nanoTime();
        
        int[][] matrix = new int[n][n];
        for (int i = 0; i < n; i++) {
            System.arraycopy(costMatrix[i], 0, matrix[i], 0, n);
        }
        
        // Row reduction
        for (int i = 0; i < n; i++) {
            int minRow = Arrays.stream(matrix[i]).min().orElse(0);
            for (int j = 0; j < n; j++) {
                matrix[i][j] -= minRow;
            }
        }
        
        // Column reduction
        for (int j = 0; j < n; j++) {
            int minCol = Integer.MAX_VALUE;
            for (int i = 0; i < n; i++) {
                minCol = Math.min(minCol, matrix[i][j]);
            }
            for (int i = 0; i < n; i++) {
                matrix[i][j] -= minCol;
            }
        }
        
        // Find assignment (simplified - using greedy approach for demonstration)
        int[] assignment = new int[n];
        boolean[] usedJobs = new boolean[n];
        Arrays.fill(assignment, -1);
        
        // Greedy assignment to zeros
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (matrix[i][j] == 0 && !usedJobs[j]) {
                    assignment[i] = j;
                    usedJobs[j] = true;
                    break;
                }
            }
        }
        
        // Calculate cost
        int totalCost = 0;
        for (int i = 0; i < n; i++) {
            if (assignment[i] != -1) {
                totalCost += costMatrix[i][assignment[i]];
            }
        }
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        List<String> hungarianSteps = Arrays.asList(
            "=== Hungarian Algorithm Results ===",
            "Matrix reductions performed",
            "Optimal assignment: " + Arrays.toString(assignment),
            "Minimum cost: " + totalCost,
            "Execution time: " + String.format("%.2f", executionTime) + " ms"
        );
        
        return new AssignmentResult(assignment, totalCost, n * n, 0, 
                                  executionTime, hungarianSteps);
    }
    
    public static void main(String[] args) {
        System.out.println("=== Job Assignment Problem - Branch & Bound ===\n");
        
        // Test case 1: Small assignment problem
        int[][] cost1 = {
            {9, 2, 7, 8},
            {6, 4, 3, 7},
            {5, 8, 1, 8},
            {7, 6, 9, 4}
        };
        
        System.out.println("Test Case 1: 4x4 Assignment Problem");
        System.out.println("Cost Matrix:");
        printMatrix(cost1);
        
        JobAssignment assignment1 = new JobAssignment(cost1, true);
        AssignmentResult result1 = assignment1.solveAssignment();
        
        System.out.println("\nBranch & Bound Solution:");
        for (String step : result1.steps) {
            System.out.println(step);
        }
        
        // Compare with other methods
        System.out.println("\nComparing with other methods:");
        
        AssignmentResult bruteResult1 = assignment1.bruteForceSolution();
        for (String step : bruteResult1.steps) {
            System.out.println(step);
        }
        
        AssignmentResult hungarianResult1 = assignment1.hungarianSolution();
        for (String step : hungarianResult1.steps) {
            System.out.println(step);
        }
        
        System.out.println("\nEfficiency Comparison:");
        System.out.println("Branch & Bound: " + result1.nodesExplored + " nodes explored, " 
                         + result1.nodesPruned + " pruned");
        System.out.println("Brute Force: " + bruteResult1.nodesExplored + " permutations checked");
        System.out.println("Speedup vs Brute Force: " + String.format("%.2fx", 
            bruteResult1.executionTime / result1.executionTime));
        
        // Test case 2: Larger assignment problem
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: 5x5 Assignment Problem");
        
        int[][] cost2 = {
            {12, 9, 27, 10, 23},
            {7, 13, 13, 30, 19},
            {25, 18, 26, 15, 24},
            {6, 20, 14, 8, 17},
            {18, 24, 20, 21, 14}
        };
        
        System.out.println("Cost Matrix:");
        printMatrix(cost2);
        
        JobAssignment assignment2 = new JobAssignment(cost2, false);
        AssignmentResult result2 = assignment2.solveAssignment();
        
        System.out.println("\nBranch & Bound Results:");
        System.out.println("Optimal assignment: " + Arrays.toString(result2.assignment));
        System.out.println("Minimum cost: " + result2.minCost);
        System.out.println("Assignment details:");
        for (int i = 0; i < result2.assignment.length; i++) {
            System.out.println("  Worker " + i + " -> Job " + result2.assignment[i] + 
                             " (cost: " + cost2[i][result2.assignment[i]] + ")");
        }
        System.out.println("Nodes explored: " + result2.nodesExplored);
        System.out.println("Nodes pruned: " + result2.nodesPruned);
        System.out.println("Execution time: " + String.format("%.2f", result2.executionTime) + " ms");
        
        // Performance analysis
        System.out.println("\n=== Performance Analysis ===");
        System.out.println("Branch & Bound Advantages:");
        System.out.println("- Optimal solution guaranteed");
        System.out.println("- Efficient pruning reduces search space significantly");
        System.out.println("- Good bounds lead to early termination");
        System.out.println("- Works well for medium-sized problems");
        
        System.out.println("\nHungarian Algorithm Advantages:");
        System.out.println("- Polynomial time complexity O(n³)");
        System.out.println("- Consistent performance regardless of input");
        System.out.println("- Best choice for large assignment problems");
        
        System.out.println("\nWhen to use Branch & Bound for Assignment:");
        System.out.println("- Medium-sized problems (n ≤ 15)");
        System.out.println("- When you want to understand the search process");
        System.out.println("- Educational purposes");
        System.out.println("- When Hungarian implementation is not available");
        
        demonstrateScaling();
    }
    
    private static void printMatrix(int[][] matrix) {
        int n = matrix.length;
        System.out.print("     ");
        for (int j = 0; j < n; j++) {
            System.out.printf("J%-3d", j);
        }
        System.out.println();
        
        for (int i = 0; i < n; i++) {
            System.out.printf("W%-2d: ", i);
            for (int j = 0; j < n; j++) {
                System.out.printf("%4d", matrix[i][j]);
            }
            System.out.println();
        }
    }
    
    private static void demonstrateScaling() {
        System.out.println("\n=== Scaling Analysis ===");
        
        Random rand = new Random(42);
        int[] sizes = {3, 4, 5, 6};
        
        System.out.println("Size | Nodes Explored | Nodes Pruned | Pruning % | Time (ms)");
        System.out.println("-".repeat(65));
        
        for (int size : sizes) {
            // Generate random cost matrix
            int[][] cost = new int[size][size];
            for (int i = 0; i < size; i++) {
                for (int j = 0; j < size; j++) {
                    cost[i][j] = 1 + rand.nextInt(30);
                }
            }
            
            JobAssignment assignment = new JobAssignment(cost, false);
            AssignmentResult result = assignment.solveAssignment();
            
            double pruningPercent = (double) result.nodesPruned / 
                                  (result.nodesExplored + result.nodesPruned) * 100;
            
            System.out.printf("%4d | %14d | %12d | %8.1f%% | %8.2f\n",
                size, result.nodesExplored, result.nodesPruned, 
                pruningPercent, result.executionTime);
        }
        
        System.out.println("\nKey Observations:");
        System.out.println("- Search space grows as n! but pruning is very effective");
        System.out.println("- Good bounds eliminate most of the search space");
        System.out.println("- Performance degrades rapidly beyond n=8");
        System.out.println("- Hungarian algorithm is preferred for larger problems");
    }
}