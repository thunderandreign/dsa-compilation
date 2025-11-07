/**
 * Backtracking Strategy: Graph Coloring Problem
 * Core Idea: Color vertices of a graph such that no two adjacent vertices have the same color
 * Time Complexity: O(k^V) where k is number of colors and V is number of vertices
 * Space Complexity: O(V) for recursion stack and color assignments
 */

import java.util.*;

public class GraphColoring {
    
    /**
     * Solve graph coloring problem using backtracking
     * @param graph Adjacency matrix representation of the graph
     * @param numColors Number of colors available
     * @return Array of color assignments, or null if no solution exists
     */
    public static int[] solveGraphColoring(int[][] graph, int numColors) {
        int numVertices = graph.length;
        int[] colors = new int[numVertices]; // 0 means uncolored
        
        if (backtrackColoring(graph, colors, 0, numColors)) {
            return colors;
        }
        
        return null; // No solution found
    }
    
    /**
     * Backtracking helper function for graph coloring
     * @param graph Adjacency matrix
     * @param colors Current color assignments
     * @param vertex Current vertex to color
     * @param numColors Number of available colors
     * @return true if coloring is possible, false otherwise
     */
    private static boolean backtrackColoring(int[][] graph, int[] colors, int vertex, int numColors) {
        int numVertices = graph.length;
        
        // Base case: all vertices colored
        if (vertex == numVertices) {
            return true;
        }
        
        // Try each color for current vertex
        for (int color = 1; color <= numColors; color++) {
            if (isSafeToColor(graph, colors, vertex, color)) {
                // Assign color
                colors[vertex] = color;
                
                // Recursively color remaining vertices
                if (backtrackColoring(graph, colors, vertex + 1, numColors)) {
                    return true;
                }
                
                // Backtrack: remove color
                colors[vertex] = 0;
            }
        }
        
        return false; // No valid coloring found
    }
    
    /**
     * Check if it's safe to color a vertex with given color
     * @param graph Adjacency matrix
     * @param colors Current color assignments
     * @param vertex Vertex to check
     * @param color Color to assign
     * @return true if safe, false otherwise
     */
    private static boolean isSafeToColor(int[][] graph, int[] colors, int vertex, int color) {
        // Check all adjacent vertices
        for (int i = 0; i < graph.length; i++) {
            // If there's an edge and adjacent vertex has same color
            if (graph[vertex][i] == 1 && colors[i] == color) {
                return false;
            }
        }
        return true;
    }
    
    /**
     * Find minimum number of colors needed (chromatic number approximation)
     * @param graph Adjacency matrix
     * @return Minimum colors needed (upper bound)
     */
    public static int findChromaticNumber(int[][] graph) {
        int numVertices = graph.length;
        
        // Try from 1 color up to V colors
        for (int colors = 1; colors <= numVertices; colors++) {
            if (solveGraphColoring(graph, colors) != null) {
                return colors;
            }
        }
        
        return numVertices; // Worst case: each vertex different color
    }
    
    /**
     * Count total number of valid colorings
     * @param graph Adjacency matrix
     * @param numColors Number of colors available
     * @return Number of valid colorings
     */
    public static int countColorings(int[][] graph, int numColors) {
        int numVertices = graph.length;
        int[] colors = new int[numVertices];
        return countColoringsHelper(graph, colors, 0, numColors);
    }
    
    /**
     * Helper method to count colorings
     */
    private static int countColoringsHelper(int[][] graph, int[] colors, int vertex, int numColors) {
        if (vertex == graph.length) {
            return 1; // Found a valid coloring
        }
        
        int count = 0;
        for (int color = 1; color <= numColors; color++) {
            if (isSafeToColor(graph, colors, vertex, color)) {
                colors[vertex] = color;
                count += countColoringsHelper(graph, colors, vertex + 1, numColors);
                colors[vertex] = 0;
            }
        }
        
        return count;
    }
    
    /**
     * Solve with step tracking for demonstration
     * @param graph Adjacency matrix
     * @param numColors Number of colors
     * @param steps List to store solution steps
     * @return Color assignments or null
     */
    public static int[] solveWithSteps(int[][] graph, int numColors, List<String> steps) {
        int numVertices = graph.length;
        int[] colors = new int[numVertices];
        
        if (backtrackWithSteps(graph, colors, 0, numColors, steps)) {
            return colors;
        }
        
        return null;
    }
    
    /**
     * Backtracking with step tracking
     */
    private static boolean backtrackWithSteps(int[][] graph, int[] colors, int vertex, int numColors, List<String> steps) {
        if (vertex == graph.length) {
            steps.add("✓ All vertices colored successfully!");
            return true;
        }
        
        steps.add(String.format("Coloring vertex %d (adjacent to: %s)", vertex, getAdjacentVertices(graph, vertex)));
        
        for (int color = 1; color <= numColors; color++) {
            String colorName = getColorName(color);
            
            if (isSafeToColor(graph, colors, vertex, color)) {
                steps.add(String.format("  Trying color %s (%d) for vertex %d - Valid!", colorName, color, vertex));
                colors[vertex] = color;
                
                if (backtrackWithSteps(graph, colors, vertex + 1, numColors, steps)) {
                    return true;
                }
                
                steps.add(String.format("  Backtracking from vertex %d, removing color %s", vertex, colorName));
                colors[vertex] = 0;
            } else {
                steps.add(String.format("  Color %s (%d) for vertex %d - Invalid (conflicts with adjacent vertices)", colorName, color, vertex));
            }
        }
        
        return false;
    }
    
    /**
     * Get adjacent vertices as string
     */
    private static String getAdjacentVertices(int[][] graph, int vertex) {
        List<Integer> adjacent = new ArrayList<>();
        for (int i = 0; i < graph.length; i++) {
            if (graph[vertex][i] == 1) {
                adjacent.add(i);
            }
        }
        return adjacent.toString();
    }
    
    /**
     * Get color name for display
     */
    private static String getColorName(int color) {
        String[] colorNames = {"", "Red", "Blue", "Green", "Yellow", "Purple", "Orange", "Pink", "Brown", "Gray", "Black"};
        return color < colorNames.length ? colorNames[color] : "Color" + color;
    }
    
    /**
     * Print the graph as adjacency matrix
     * @param graph Adjacency matrix
     */
    public static void printGraph(int[][] graph) {
        int numVertices = graph.length;
        System.out.println("Adjacency Matrix:");
        System.out.print("   ");
        for (int i = 0; i < numVertices; i++) {
            System.out.printf("%2d ", i);
        }
        System.out.println();
        
        for (int i = 0; i < numVertices; i++) {
            System.out.printf("%2d ", i);
            for (int j = 0; j < numVertices; j++) {
                System.out.printf("%2d ", graph[i][j]);
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Print coloring solution
     * @param colors Color assignments
     */
    public static void printColoring(int[] colors) {
        if (colors == null) {
            System.out.println("No valid coloring found.");
            return;
        }
        
        System.out.println("Graph Coloring Solution:");
        for (int i = 0; i < colors.length; i++) {
            String colorName = getColorName(colors[i]);
            System.out.printf("Vertex %d: %s (%d)\n", i, colorName, colors[i]);
        }
        System.out.println();
    }
    
    /**
     * Validate a coloring solution
     * @param graph Adjacency matrix
     * @param colors Color assignments
     * @return true if valid, false otherwise
     */
    public static boolean validateColoring(int[][] graph, int[] colors) {
        for (int i = 0; i < graph.length; i++) {
            for (int j = 0; j < graph.length; j++) {
                // If there's an edge and vertices have same color
                if (graph[i][j] == 1 && colors[i] == colors[j]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    /**
     * Create sample graphs for testing
     */
    public static class GraphGenerator {
        
        /**
         * Create a simple triangle graph (3-clique)
         */
        public static int[][] createTriangleGraph() {
            return new int[][] {
                {0, 1, 1},
                {1, 0, 1},
                {1, 1, 0}
            };
        }
        
        /**
         * Create a square graph (4-cycle)
         */
        public static int[][] createSquareGraph() {
            return new int[][] {
                {0, 1, 0, 1},
                {1, 0, 1, 0},
                {0, 1, 0, 1},
                {1, 0, 1, 0}
            };
        }
        
        /**
         * Create a complete graph (K_n)
         */
        public static int[][] createCompleteGraph(int n) {
            int[][] graph = new int[n][n];
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (i != j) {
                        graph[i][j] = 1;
                    }
                }
            }
            return graph;
        }
        
        /**
         * Create a bipartite graph
         */
        public static int[][] createBipartiteGraph() {
            return new int[][] {
                {0, 0, 0, 1, 1, 1},
                {0, 0, 0, 1, 1, 1},
                {0, 0, 0, 1, 1, 1},
                {1, 1, 1, 0, 0, 0},
                {1, 1, 1, 0, 0, 0},
                {1, 1, 1, 0, 0, 0}
            };
        }
        
        /**
         * Create a wheel graph (hub connected to cycle)
         */
        public static int[][] createWheelGraph(int n) {
            int[][] graph = new int[n + 1][n + 1];
            
            // Create cycle
            for (int i = 0; i < n; i++) {
                graph[i][(i + 1) % n] = 1;
                graph[(i + 1) % n][i] = 1;
            }
            
            // Connect hub to all cycle vertices
            for (int i = 0; i < n; i++) {
                graph[n][i] = 1;
                graph[i][n] = 1;
            }
            
            return graph;
        }
        
        /**
         * Create a tree graph (no cycles)
         */
        public static int[][] createTreeGraph() {
            return new int[][] {
                {0, 1, 1, 0, 0, 0, 0},
                {1, 0, 0, 1, 1, 0, 0},
                {1, 0, 0, 0, 0, 1, 1},
                {0, 1, 0, 0, 0, 0, 0},
                {0, 1, 0, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0, 0},
                {0, 0, 1, 0, 0, 0, 0}
            };
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Graph Coloring Problem - Backtracking ===\n");
        
        // Test Case 1: Simple triangle (3-clique)
        System.out.println("Test Case 1: Triangle Graph (3-clique)");
        int[][] triangle = GraphGenerator.createTriangleGraph();
        printGraph(triangle);
        
        int[] triangleColoring = solveGraphColoring(triangle, 3);
        printColoring(triangleColoring);
        System.out.println("Valid coloring: " + validateColoring(triangle, triangleColoring));
        
        int triangleChromatic = findChromaticNumber(triangle);
        System.out.println("Chromatic number: " + triangleChromatic);
        System.out.println("Number of 3-colorings: " + countColorings(triangle, 3));
        System.out.println();
        
        // Test Case 2: Square graph (4-cycle)
        System.out.println("Test Case 2: Square Graph (4-cycle)");
        int[][] square = GraphGenerator.createSquareGraph();
        printGraph(square);
        
        int[] squareColoring = solveGraphColoring(square, 2);
        printColoring(squareColoring);
        System.out.println("Valid coloring: " + validateColoring(square, squareColoring));
        
        int squareChromatic = findChromaticNumber(square);
        System.out.println("Chromatic number: " + squareChromatic);
        System.out.println("Number of 2-colorings: " + countColorings(square, 2));
        System.out.println();
        
        // Test Case 3: Step-by-step demonstration
        System.out.println("Test Case 3: Step-by-step coloring (Triangle)");
        List<String> steps = new ArrayList<>();
        solveWithSteps(triangle, 3, steps);
        
        System.out.println("Solution steps:");
        for (int i = 0; i < Math.min(15, steps.size()); i++) {
            System.out.println(steps.get(i));
        }
        if (steps.size() > 15) {
            System.out.println("... (" + (steps.size() - 15) + " more steps)");
        }
        System.out.println();
        
        // Test Case 4: Complete graph K4
        System.out.println("Test Case 4: Complete Graph K4");
        int[][] k4 = GraphGenerator.createCompleteGraph(4);
        printGraph(k4);
        
        int[] k4Coloring = solveGraphColoring(k4, 4);
        printColoring(k4Coloring);
        
        int k4Chromatic = findChromaticNumber(k4);
        System.out.println("Chromatic number: " + k4Chromatic);
        System.out.println("Expected: 4 (complete graph needs n colors)");
        System.out.println();
        
        // Test Case 5: Bipartite graph
        System.out.println("Test Case 5: Bipartite Graph");
        int[][] bipartite = GraphGenerator.createBipartiteGraph();
        printGraph(bipartite);
        
        int[] bipartiteColoring = solveGraphColoring(bipartite, 2);
        printColoring(bipartiteColoring);
        
        int bipartiteChromatic = findChromaticNumber(bipartite);
        System.out.println("Chromatic number: " + bipartiteChromatic);
        System.out.println("Expected: 2 (bipartite graph needs exactly 2 colors)");
        System.out.println();
        
        // Test Case 6: Wheel graph
        System.out.println("Test Case 6: Wheel Graph W5");
        int[][] wheel = GraphGenerator.createWheelGraph(5);
        printGraph(wheel);
        
        int[] wheelColoring = solveGraphColoring(wheel, 4);
        printColoring(wheelColoring);
        
        int wheelChromatic = findChromaticNumber(wheel);
        System.out.println("Chromatic number: " + wheelChromatic);
        System.out.println("Expected: 3 or 4 depending on cycle length");
        System.out.println();
        
        // Test Case 7: Tree graph
        System.out.println("Test Case 7: Tree Graph");
        int[][] tree = GraphGenerator.createTreeGraph();
        printGraph(tree);
        
        int[] treeColoring = solveGraphColoring(tree, 2);
        printColoring(treeColoring);
        
        int treeChromatic = findChromaticNumber(tree);
        System.out.println("Chromatic number: " + treeChromatic);
        System.out.println("Expected: 2 (trees are bipartite)");
        System.out.println();
        
        // Test Case 8: Performance analysis
        System.out.println("Test Case 8: Performance Analysis");
        
        for (int n = 3; n <= 6; n++) {
            int[][] kn = GraphGenerator.createCompleteGraph(n);
            
            long startTime = System.currentTimeMillis();
            int chromatic = findChromaticNumber(kn);
            long endTime = System.currentTimeMillis();
            
            int colorings = countColorings(kn, n);
            
            System.out.printf("K%d: Chromatic number = %d, %d-colorings = %d (Time: %dms)\n", 
                            n, chromatic, n, colorings, endTime - startTime);
        }
        System.out.println();
        
        // Test Case 9: Edge cases
        System.out.println("Test Case 9: Edge Cases");
        
        // Single vertex
        int[][] singleVertex = {{0}};
        int[] singleColoring = solveGraphColoring(singleVertex, 1);
        System.out.println("Single vertex with 1 color: " + (singleColoring != null ? "Possible" : "Impossible"));
        
        // No edges
        int[][] noEdges = new int[4][4]; // All zeros
        int[] noEdgesColoring = solveGraphColoring(noEdges, 1);
        System.out.println("4 vertices, no edges, 1 color: " + (noEdgesColoring != null ? "Possible" : "Impossible"));
        
        // Impossible coloring
        int[] impossibleColoring = solveGraphColoring(triangle, 2);
        System.out.println("Triangle with 2 colors: " + (impossibleColoring != null ? "Possible" : "Impossible"));
        System.out.println();
        
        // Test Case 10: Real-world applications
        System.out.println("Test Case 10: Applications");
        System.out.println("Graph coloring applications:");
        System.out.println("- Map coloring: Color countries so no adjacent countries have same color");
        System.out.println("- Register allocation: Assign CPU registers to variables");
        System.out.println("- Scheduling: Assign time slots to tasks with conflicts");
        System.out.println("- Frequency assignment: Assign radio frequencies to avoid interference");
        System.out.println("- Sudoku solving: Each constraint is a clique that needs different values");
        
        System.out.println("\nComplexity Analysis:");
        System.out.println("- Time: O(k^V) where k is colors and V is vertices");
        System.out.println("- Space: O(V) for recursion stack and color assignments");
        System.out.println("- NP-Complete: Decision version is NP-Complete");
        System.out.println("- Heuristics: Greedy coloring, Welsh-Powell algorithm");
        System.out.println("- Bounds: χ(G) ≤ Δ(G) + 1 where Δ is max degree");
    }
}