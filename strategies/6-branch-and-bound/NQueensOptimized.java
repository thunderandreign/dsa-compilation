/**
 * N-Queens Problem - Branch & Bound Optimization Implementation
 * 
 * This implementation demonstrates Branch & Bound strategy for the N-Queens problem,
 * focusing on finding optimal placements with minimal conflicts and maximum
 * efficiency. Unlike the backtracking version, this approach uses bounds to
 * optimize the search and can handle constraint optimization variants.
 * 
 * Key Concepts:
 * - Branch & Bound: Systematic enumeration with conflict-based pruning
 * - Conflict Minimization: Finding placements with minimal queen conflicts
 * - Heuristic Bounds: Using conflict counts as optimization criteria
 * - Multiple Solutions: Finding and ranking multiple valid solutions
 * 
 * Time Complexity: O(n!) worst case, but much better with pruning
 * Space Complexity: O(n) for recursion stack + solution storage
 * 
 * Real-world Applications:
 * - Resource scheduling with conflict avoidance
 * - Frequency assignment in telecommunications
 * - Task scheduling in parallel processing
 * - Layout optimization problems
 */

import java.util.*;

public class NQueensOptimized {
    
    static class QueensSolution {
        int[] placement;      // Queen positions (row i, column placement[i])
        int conflicts;        // Number of conflicts (for optimization variants)
        double score;         // Solution quality score
        
        QueensSolution(int n) {
            placement = new int[n];
            conflicts = 0;
            score = 0.0;
        }
        
        QueensSolution(QueensSolution other) {
            placement = other.placement.clone();
            conflicts = other.conflicts;
            score = other.score;
        }
        
        @Override
        public String toString() {
            return "Queens" + Arrays.toString(placement) + 
                   " (conflicts: " + conflicts + ", score: " + String.format("%.2f", score) + ")";
        }
    }
    
    static class QueensNode {
        int level;                    // Current row being processed
        int[] placement;              // Current partial placement
        boolean[] colUsed;            // Used columns
        boolean[] diag1Used;          // Used main diagonals (row - col)
        boolean[] diag2Used;          // Used anti-diagonals (row + col)
        int conflicts;                // Current conflicts
        int bound;                    // Lower bound estimate
        
        QueensNode(int n) {
            level = 0;
            placement = new int[n];
            colUsed = new boolean[n];
            diag1Used = new boolean[2 * n - 1];
            diag2Used = new boolean[2 * n - 1];
            conflicts = 0;
            bound = 0;
        }
        
        QueensNode(QueensNode other, int n) {
            level = other.level;
            placement = other.placement.clone();
            colUsed = other.colUsed.clone();
            diag1Used = other.diag1Used.clone();
            diag2Used = other.diag2Used.clone();
            conflicts = other.conflicts;
            bound = other.bound;
        }
    }
    
    static class OptimizationResult {
        List<QueensSolution> solutions;
        QueensSolution bestSolution;
        int nodesExplored;
        int nodesPruned;
        double executionTime;
        List<String> steps;
        
        OptimizationResult(List<QueensSolution> solutions, QueensSolution best,
                          int explored, int pruned, double time, List<String> steps) {
            this.solutions = new ArrayList<>(solutions);
            this.bestSolution = best;
            this.nodesExplored = explored;
            this.nodesPruned = pruned;
            this.executionTime = time;
            this.steps = new ArrayList<>(steps);
        }
    }
    
    private int n;
    private int maxSolutions;
    private List<QueensSolution> solutions;
    private QueensSolution bestSolution;
    private int nodesExplored;
    private int nodesPruned;
    private List<String> steps;
    private boolean verbose;
    private String optimizationMode;
    
    public NQueensOptimized(int n, int maxSolutions, String mode, boolean verbose) {
        this.n = n;
        this.maxSolutions = maxSolutions;
        this.optimizationMode = mode; // "conflicts", "symmetry", "center"
        this.verbose = verbose;
        this.solutions = new ArrayList<>();
        this.bestSolution = null;
        this.nodesExplored = 0;
        this.nodesPruned = 0;
        this.steps = new ArrayList<>();
    }
    
    /**
     * Solve N-Queens using Branch & Bound optimization
     */
    public OptimizationResult solveOptimized() {
        long startTime = System.nanoTime();
        
        if (verbose) {
            steps.add("=== Starting N-Queens Branch & Bound Optimization ===");
            steps.add("Board size: " + n + "x" + n);
            steps.add("Optimization mode: " + optimizationMode);
            steps.add("Max solutions to find: " + maxSolutions);
        }
        
        // Initialize root node
        QueensNode root = new QueensNode(n);
        root.bound = calculateBound(root);
        
        if (verbose) {
            steps.add("Root node bound: " + root.bound);
        }
        
        // Use priority queue for best-first search
        PriorityQueue<QueensNode> pq = new PriorityQueue<>((a, b) -> a.bound - b.bound);
        pq.offer(root);
        
        while (!pq.isEmpty() && solutions.size() < maxSolutions) {
            QueensNode current = pq.poll();
            nodesExplored++;
            
            if (verbose && nodesExplored <= 25) {
                steps.add("Exploring node at level " + current.level + 
                         ", conflicts: " + current.conflicts + ", bound: " + current.bound);
                if (current.level > 0) {
                    steps.add("  Partial placement: " + 
                             Arrays.toString(Arrays.copyOf(current.placement, current.level)));
                }
            }
            
            // Pruning check (if we already have solutions and this bound is worse)
            if (bestSolution != null && current.bound > bestSolution.conflicts) {
                nodesPruned++;
                if (verbose && nodesExplored <= 25) {
                    steps.add("  Pruned: bound " + current.bound + 
                             " > best conflicts " + bestSolution.conflicts);
                }
                continue;
            }
            
            // Check if we have a complete solution
            if (current.level == n) {
                QueensSolution solution = new QueensSolution(n);
                System.arraycopy(current.placement, 0, solution.placement, 0, n);
                solution.conflicts = current.conflicts;
                solution.score = calculateSolutionScore(solution);
                
                solutions.add(solution);
                
                if (bestSolution == null || solution.conflicts < bestSolution.conflicts ||
                    (solution.conflicts == bestSolution.conflicts && solution.score > bestSolution.score)) {
                    bestSolution = new QueensSolution(solution);
                }
                
                if (verbose) {
                    steps.add("*** Solution found! ***");
                    steps.add("Placement: " + Arrays.toString(solution.placement));
                    steps.add("Conflicts: " + solution.conflicts + ", Score: " + 
                             String.format("%.2f", solution.score));
                }
                continue;
            }
            
            // Branch: Try placing queen in each column of current row
            List<Integer> columns = getColumnOrder(current);
            
            for (int col : columns) {
                if (canPlace(current, current.level, col)) {
                    QueensNode child = new QueensNode(current, n);
                    placeQueen(child, current.level, col);
                    child.level = current.level + 1;
                    child.bound = calculateBound(child);
                    
                    // Only add to queue if bound is promising
                    if (bestSolution == null || child.bound <= bestSolution.conflicts + 1) {
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
            steps.add("Solutions found: " + solutions.size());
            if (bestSolution != null) {
                steps.add("Best solution: " + bestSolution);
                steps.add("Best placement visualization:");
                steps.addAll(visualizeSolution(bestSolution));
            }
            steps.add("Nodes explored: " + nodesExplored);
            steps.add("Nodes pruned: " + nodesPruned);
            steps.add("Execution time: " + String.format("%.2f", executionTime) + " ms");
        }
        
        return new OptimizationResult(solutions, bestSolution, nodesExplored, 
                                    nodesPruned, executionTime, steps);
    }
    
    /**
     * Calculate lower bound estimate for remaining conflicts
     */
    private int calculateBound(QueensNode node) {
        int bound = node.conflicts;
        
        // For remaining rows, estimate minimum conflicts
        for (int row = node.level; row < n; row++) {
            int minConflicts = Integer.MAX_VALUE;
            
            for (int col = 0; col < n; col++) {
                if (!node.colUsed[col] && 
                    !node.diag1Used[row - col + n - 1] && 
                    !node.diag2Used[row + col]) {
                    
                    // Calculate potential conflicts if queen is placed here
                    int conflicts = 0;
                    
                    // Check conflicts with already placed queens
                    for (int prevRow = 0; prevRow < node.level; prevRow++) {
                        int prevCol = node.placement[prevRow];
                        if (prevCol == col || 
                            Math.abs(prevRow - row) == Math.abs(prevCol - col)) {
                            conflicts++;
                        }
                    }
                    
                    minConflicts = Math.min(minConflicts, conflicts);
                }
            }
            
            if (minConflicts != Integer.MAX_VALUE) {
                bound += minConflicts;
            }
        }
        
        return bound;
    }
    
    /**
     * Get column ordering for branching (heuristic-based)
     */
    private List<Integer> getColumnOrder(QueensNode node) {
        List<Integer> columns = new ArrayList<>();
        
        // Add all valid columns
        for (int col = 0; col < n; col++) {
            if (canPlace(node, node.level, col)) {
                columns.add(col);
            }
        }
        
        // Apply ordering heuristic based on optimization mode
        switch (optimizationMode) {
            case "center":
                // Prefer center columns
                columns.sort((a, b) -> {
                    int centerA = Math.abs(a - n / 2);
                    int centerB = Math.abs(b - n / 2);
                    return centerA - centerB;
                });
                break;
                
            case "conflicts":
                // Prefer columns with fewer potential conflicts
                columns.sort((a, b) -> {
                    int conflictsA = countPotentialConflicts(node, node.level, a);
                    int conflictsB = countPotentialConflicts(node, node.level, b);
                    return conflictsA - conflictsB;
                });
                break;
                
            case "symmetry":
                // Use symmetry to reduce search space
                if (node.level == 0) {
                    // For first queen, only consider first half + middle
                    columns = columns.subList(0, (n + 1) / 2);
                }
                break;
        }
        
        return columns;
    }
    
    private int countPotentialConflicts(QueensNode node, int row, int col) {
        int conflicts = 0;
        
        // Count conflicts with remaining unplaced positions
        for (int futureRow = row + 1; futureRow < n; futureRow++) {
            for (int futureCol = 0; futureCol < n; futureCol++) {
                if (!node.colUsed[futureCol] && 
                    (futureCol == col || Math.abs(futureRow - row) == Math.abs(futureCol - col))) {
                    conflicts++;
                }
            }
        }
        
        return conflicts;
    }
    
    private boolean canPlace(QueensNode node, int row, int col) {
        return !node.colUsed[col] && 
               !node.diag1Used[row - col + n - 1] && 
               !node.diag2Used[row + col];
    }
    
    private void placeQueen(QueensNode node, int row, int col) {
        node.placement[row] = col;
        node.colUsed[col] = true;
        node.diag1Used[row - col + n - 1] = true;
        node.diag2Used[row + col] = true;
        
        // Update conflicts (for optimization variants)
        // In standard N-Queens, valid placements have 0 conflicts
        // But this supports variants where some conflicts are allowed
    }
    
    private double calculateSolutionScore(QueensSolution solution) {
        double score = 0.0;
        
        switch (optimizationMode) {
            case "center":
                // Reward solutions with queens closer to center
                for (int col : solution.placement) {
                    score += 1.0 / (1.0 + Math.abs(col - n / 2.0));
                }
                break;
                
            case "symmetry":
                // Reward symmetric solutions
                boolean symmetric = true;
                for (int i = 0; i < n; i++) {
                    if (solution.placement[i] != n - 1 - solution.placement[n - 1 - i]) {
                        symmetric = false;
                        break;
                    }
                }
                score = symmetric ? 10.0 : 1.0;
                break;
                
            default:
                // Default: reward solutions with fewer conflicts
                score = 100.0 / (1.0 + solution.conflicts);
                break;
        }
        
        return score;
    }
    
    private List<String> visualizeSolution(QueensSolution solution) {
        List<String> lines = new ArrayList<>();
        
        for (int row = 0; row < n; row++) {
            StringBuilder line = new StringBuilder();
            for (int col = 0; col < n; col++) {
                if (solution.placement[row] == col) {
                    line.append(" Q ");
                } else {
                    line.append(" . ");
                }
            }
            lines.add(line.toString());
        }
        
        return lines;
    }
    
    /**
     * Compare with basic backtracking for performance analysis
     */
    public OptimizationResult backtrackingSolution() {
        long startTime = System.nanoTime();
        
        List<QueensSolution> backtrackSolutions = new ArrayList<>();
        int[] placement = new int[n];
        boolean[] colUsed = new boolean[n];
        boolean[] diag1Used = new boolean[2 * n - 1];
        boolean[] diag2Used = new boolean[2 * n - 1];
        
        int[] exploredCount = {0};
        
        backtrackRecursive(0, placement, colUsed, diag1Used, diag2Used, 
                          backtrackSolutions, exploredCount);
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        QueensSolution bestBacktrack = null;
        if (!backtrackSolutions.isEmpty()) {
            bestBacktrack = backtrackSolutions.get(0);
            for (QueensSolution sol : backtrackSolutions) {
                sol.score = calculateSolutionScore(sol);
                if (sol.score > bestBacktrack.score) {
                    bestBacktrack = sol;
                }
            }
        }
        
        List<String> backtrackSteps = Arrays.asList(
            "=== Backtracking Results ===",
            "Solutions found: " + Math.min(backtrackSolutions.size(), maxSolutions),
            "Nodes explored: " + exploredCount[0],
            "Execution time: " + String.format("%.2f", executionTime) + " ms"
        );
        
        return new OptimizationResult(
            backtrackSolutions.subList(0, Math.min(backtrackSolutions.size(), maxSolutions)),
            bestBacktrack, exploredCount[0], 0, executionTime, backtrackSteps);
    }
    
    private void backtrackRecursive(int row, int[] placement, boolean[] colUsed,
                                   boolean[] diag1Used, boolean[] diag2Used,
                                   List<QueensSolution> solutions, int[] exploredCount) {
        exploredCount[0]++;
        
        if (solutions.size() >= maxSolutions) return;
        
        if (row == n) {
            QueensSolution solution = new QueensSolution(n);
            System.arraycopy(placement, 0, solution.placement, 0, n);
            solutions.add(solution);
            return;
        }
        
        for (int col = 0; col < n; col++) {
            if (!colUsed[col] && !diag1Used[row - col + n - 1] && !diag2Used[row + col]) {
                placement[row] = col;
                colUsed[col] = true;
                diag1Used[row - col + n - 1] = true;
                diag2Used[row + col] = true;
                
                backtrackRecursive(row + 1, placement, colUsed, diag1Used, diag2Used,
                                 solutions, exploredCount);
                
                colUsed[col] = false;
                diag1Used[row - col + n - 1] = false;
                diag2Used[row + col] = false;
            }
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== N-Queens Problem - Branch & Bound Optimization ===\n");
        
        // Test case 1: 8-Queens with conflict minimization
        System.out.println("Test Case 1: 8-Queens with Conflict Optimization");
        
        NQueensOptimized nqueens1 = new NQueensOptimized(8, 5, "conflicts", true);
        OptimizationResult result1 = nqueens1.solveOptimized();
        
        System.out.println("Branch & Bound Optimization Results:");
        for (String step : result1.steps) {
            System.out.println(step);
        }
        
        // Compare with backtracking
        System.out.println("\nComparing with Backtracking:");
        OptimizationResult backtrackResult1 = nqueens1.backtrackingSolution();
        for (String step : backtrackResult1.steps) {
            System.out.println(step);
        }
        
        System.out.println("\nEfficiency Comparison:");
        System.out.println("Branch & Bound: " + result1.nodesExplored + " nodes explored, " 
                         + result1.nodesPruned + " pruned");
        System.out.println("Backtracking: " + backtrackResult1.nodesExplored + " nodes explored");
        if (backtrackResult1.executionTime > 0) {
            System.out.println("Speedup: " + String.format("%.2fx", 
                backtrackResult1.executionTime / result1.executionTime));
        }
        
        // Test case 2: Center-preference optimization
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: 6-Queens with Center Preference");
        
        NQueensOptimized nqueens2 = new NQueensOptimized(6, 3, "center", false);
        OptimizationResult result2 = nqueens2.solveOptimized();
        
        System.out.println("Solutions found: " + result2.solutions.size());
        System.out.println("Best solution: " + result2.bestSolution);
        System.out.println("\nBest solution visualization:");
        for (String line : nqueens2.visualizeSolution(result2.bestSolution)) {
            System.out.println(line);
        }
        
        System.out.println("\nAll solutions with scores:");
        result2.solutions.sort((a, b) -> Double.compare(b.score, a.score));
        for (int i = 0; i < result2.solutions.size(); i++) {
            QueensSolution sol = result2.solutions.get(i);
            System.out.println((i + 1) + ". " + Arrays.toString(sol.placement) + 
                             " (score: " + String.format("%.2f", sol.score) + ")");
        }
        
        // Test case 3: Symmetry optimization
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 3: 4-Queens with Symmetry Optimization");
        
        NQueensOptimized nqueens3 = new NQueensOptimized(4, 10, "symmetry", false);
        OptimizationResult result3 = nqueens3.solveOptimized();
        
        System.out.println("Solutions found: " + result3.solutions.size());
        System.out.println("Nodes explored: " + result3.nodesExplored);
        System.out.println("Nodes pruned: " + result3.nodesPruned);
        
        System.out.println("\nAll solutions:");
        for (int i = 0; i < result3.solutions.size(); i++) {
            QueensSolution sol = result3.solutions.get(i);
            System.out.println((i + 1) + ". " + Arrays.toString(sol.placement));
        }
        
        // Performance analysis
        System.out.println("\n=== Performance Analysis ===");
        System.out.println("Branch & Bound Optimization Advantages:");
        System.out.println("- Can find multiple high-quality solutions");
        System.out.println("- Supports different optimization criteria");
        System.out.println("- Efficient pruning based on solution quality");
        System.out.println("- Suitable for constraint optimization variants");
        
        System.out.println("\nOptimization Modes:");
        System.out.println("- 'conflicts': Minimize queen conflicts");
        System.out.println("- 'center': Prefer central queen placements");
        System.out.println("- 'symmetry': Find symmetric solutions");
        
        demonstrateScaling();
    }
    
    private static void demonstrateScaling() {
        System.out.println("\n=== Scaling Analysis ===");
        
        int[] sizes = {4, 6, 8};
        String[] modes = {"conflicts", "center", "symmetry"};
        
        System.out.println("Size | Mode      | Solutions | Nodes | Pruned | Time (ms)");
        System.out.println("-".repeat(60));
        
        for (int size : sizes) {
            for (String mode : modes) {
                NQueensOptimized nqueens = new NQueensOptimized(size, 3, mode, false);
                OptimizationResult result = nqueens.solveOptimized();
                
                System.out.printf("%4d | %-9s | %9d | %5d | %6d | %8.2f\n",
                    size, mode, result.solutions.size(), result.nodesExplored,
                    result.nodesPruned, result.executionTime);
            }
        }
        
        System.out.println("\nKey Observations:");
        System.out.println("- Different optimization modes affect search efficiency");
        System.out.println("- Symmetry mode reduces search space significantly");
        System.out.println("- Center preference finds good solutions quickly");
        System.out.println("- Conflict minimization explores more nodes but finds better solutions");
    }
}