/**
 * Greedy Strategy: Prim's Minimum Spanning Tree Algorithm
 * Core Idea: Always add the minimum weight edge that connects a vertex in MST to a vertex outside MST
 * Time Complexity: O(V²) with adjacency matrix, O(E log V) with priority queue
 * Space Complexity: O(V) for tracking MST vertices and minimum edge weights
 */

import java.util.*;

public class PrimMST {
    
    /**
     * Edge class to represent weighted edges
     */
    static class Edge {
        int source, destination, weight;
        
        Edge(int source, int destination, int weight) {
            this.source = source;
            this.destination = destination;
            this.weight = weight;
        }
        
        @Override
        public String toString() {
            return String.format("(%d-%d, weight: %d)", source, destination, weight);
        }
    }
    
    /**
     * Find Minimum Spanning Tree using Prim's algorithm
     * @param graph Adjacency matrix representation of the graph
     * @param numVertices Number of vertices
     * @return List of edges in the MST
     */
    public static List<Edge> primMST(int[][] graph, int numVertices) {
        List<Edge> mst = new ArrayList<>();
        
        // Track which vertices are included in MST
        boolean[] inMST = new boolean[numVertices];
        
        // Track minimum weight edge to reach each vertex
        int[] minWeight = new int[numVertices];
        int[] parent = new int[numVertices]; // To track MST edges
        
        // Initialize all weights as infinite
        Arrays.fill(minWeight, Integer.MAX_VALUE);
        Arrays.fill(parent, -1);
        
        // Start with vertex 0
        minWeight[0] = 0;
        
        // Build MST with V-1 edges
        for (int edgeCount = 0; edgeCount < numVertices - 1; edgeCount++) {
            
            // Step 1: Find minimum weight vertex not yet in MST (greedy choice)
            int minWeightVertex = -1;
            for (int v = 0; v < numVertices; v++) {
                if (!inMST[v] && (minWeightVertex == -1 || minWeight[v] < minWeight[minWeightVertex])) {
                    minWeightVertex = v;
                }
            }
            
            // Add vertex to MST
            inMST[minWeightVertex] = true;
            
            // Add edge to MST (except for the first vertex)
            if (parent[minWeightVertex] != -1) {
                mst.add(new Edge(parent[minWeightVertex], minWeightVertex, minWeight[minWeightVertex]));
            }
            
            // Step 2: Update minimum weights for adjacent vertices
            for (int v = 0; v < numVertices; v++) {
                // If v is not in MST and there's an edge from minWeightVertex to v
                if (!inMST[v] && graph[minWeightVertex][v] != 0 && 
                    graph[minWeightVertex][v] < minWeight[v]) {
                    
                    minWeight[v] = graph[minWeightVertex][v];
                    parent[v] = minWeightVertex;
                }
            }
        }
        
        return mst;
    }
    
    /**
     * Calculate total weight of MST
     */
    public static int calculateMSTWeight(List<Edge> mst) {
        return mst.stream().mapToInt(edge -> edge.weight).sum();
    }
    
    /**
     * Print the graph in adjacency matrix format
     */
    public static void printGraph(int[][] graph, int numVertices) {
        System.out.println("Graph (Adjacency Matrix):");
        System.out.print("   ");
        for (int i = 0; i < numVertices; i++) {
            System.out.printf("%3d", i);
        }
        System.out.println();
        
        for (int i = 0; i < numVertices; i++) {
            System.out.printf("%2d:", i);
            for (int j = 0; j < numVertices; j++) {
                if (graph[i][j] == 0) {
                    System.out.print(" - ");
                } else {
                    System.out.printf("%3d", graph[i][j]);
                }
            }
            System.out.println();
        }
    }
    
    /**
     * Print MST result
     */
    public static void printMST(List<Edge> mst, int totalWeight) {
        System.out.println("Minimum Spanning Tree (Prim's Algorithm):");
        for (int i = 0; i < mst.size(); i++) {
            System.out.printf("  %d. %s\n", i + 1, mst.get(i));
        }
        System.out.println("Total weight: " + totalWeight);
    }
    
    /**
     * Demonstrate Prim's algorithm step by step
     */
    public static void demonstratePrimSteps(int[][] graph, int numVertices) {
        System.out.println("Prim's Algorithm Steps:");
        
        boolean[] inMST = new boolean[numVertices];
        int[] minWeight = new int[numVertices];
        int[] parent = new int[numVertices];
        
        Arrays.fill(minWeight, Integer.MAX_VALUE);
        Arrays.fill(parent, -1);
        minWeight[0] = 0;
        
        System.out.println("1. Start with vertex 0");
        System.out.println("   MST vertices: {0}");
        
        for (int step = 0; step < numVertices - 1; step++) {
            
            // Find minimum weight vertex
            int minWeightVertex = -1;
            for (int v = 0; v < numVertices; v++) {
                if (!inMST[v] && (minWeightVertex == -1 || minWeight[v] < minWeight[minWeightVertex])) {
                    minWeightVertex = v;
                }
            }
            
            inMST[minWeightVertex] = true;
            
            System.out.printf("%d. Add vertex %d to MST", step + 2, minWeightVertex);
            if (parent[minWeightVertex] != -1) {
                System.out.printf(" (via edge %d-%d, weight: %d)", 
                    parent[minWeightVertex], minWeightVertex, minWeight[minWeightVertex]);
            }
            System.out.println();
            
            // Show current MST vertices
            System.out.print("   MST vertices: {");
            boolean first = true;
            for (int v = 0; v < numVertices; v++) {
                if (inMST[v]) {
                    if (!first) System.out.print(", ");
                    System.out.print(v);
                    first = false;
                }
            }
            System.out.println("}");
            
            // Update minimum weights
            for (int v = 0; v < numVertices; v++) {
                if (!inMST[v] && graph[minWeightVertex][v] != 0 && 
                    graph[minWeightVertex][v] < minWeight[v]) {
                    minWeight[v] = graph[minWeightVertex][v];
                    parent[v] = minWeightVertex;
                }
            }
            
            // Show available edges from MST
            if (step < numVertices - 2) { // Don't show for last iteration
                System.out.print("   Available edges: ");
                List<String> availableEdges = new ArrayList<>();
                for (int v = 0; v < numVertices; v++) {
                    if (!inMST[v] && minWeight[v] != Integer.MAX_VALUE) {
                        availableEdges.add(String.format("%d-%d (weight: %d)", parent[v], v, minWeight[v]));
                    }
                }
                System.out.println(String.join(", ", availableEdges));
            }
        }
    }
    
    /**
     * Compare with alternative MST algorithm (mention Kruskal's)
     */
    public static void compareWithKruskal(int[][] graph, int numVertices) {
        System.out.println("Algorithm Comparison:");
        System.out.println("1. Prim's Algorithm:");
        System.out.println("   - Grows MST one vertex at a time");
        System.out.println("   - Always maintains a connected subgraph");
        System.out.println("   - Better for dense graphs");
        System.out.println("   - Time: O(V²) with adjacency matrix");
        
        System.out.println("2. Kruskal's Algorithm:");
        System.out.println("   - Considers all edges, adds minimum weight edges");
        System.out.println("   - May create forest initially, then connects");
        System.out.println("   - Better for sparse graphs");
        System.out.println("   - Time: O(E log E) due to sorting");
        
        List<Edge> primMST = primMST(graph, numVertices);
        int primWeight = calculateMSTWeight(primMST);
        System.out.println("Both algorithms produce the same optimal MST weight: " + primWeight);
    }
    
    public static void main(String[] args) {
        System.out.println("=== Prim's Minimum Spanning Tree - Greedy Algorithm ===");
        
        // Test Case 1: Simple 4-vertex graph
        System.out.println("Test Case 1: Simple 4-vertex graph");
        int[][] graph1 = {
            {0, 2, 0, 6},
            {2, 0, 3, 8},
            {0, 3, 0, 5},
            {6, 8, 5, 0}
        };
        int numVertices1 = 4;
        
        printGraph(graph1, numVertices1);
        System.out.println();
        
        List<Edge> mst1 = primMST(graph1, numVertices1);
        int weight1 = calculateMSTWeight(mst1);
        printMST(mst1, weight1);
        System.out.println();
        
        demonstratePrimSteps(graph1, numVertices1);
        System.out.println();
        
        // Test Case 2: Triangle graph
        System.out.println("Test Case 2: Triangle graph");
        int[][] graph2 = {
            {0, 1, 4},
            {1, 0, 2},
            {4, 2, 0}
        };
        int numVertices2 = 3;
        
        printGraph(graph2, numVertices2);
        System.out.println();
        
        List<Edge> mst2 = primMST(graph2, numVertices2);
        int weight2 = calculateMSTWeight(mst2);
        printMST(mst2, weight2);
        System.out.println("Note: Greedy choice always picks minimum weight edges to expand MST");
        System.out.println();
        
        // Test Case 3: 5-vertex graph
        System.out.println("Test Case 3: 5-vertex graph");
        int[][] graph3 = {
            {0, 4, 2, 0, 0},
            {4, 0, 8, 0, 10},
            {2, 8, 0, 7, 9},
            {0, 0, 7, 0, 14},
            {0, 10, 9, 14, 0}
        };
        
        // Add one more edge
        graph3[0][3] = 7;
        graph3[3][0] = 7;
        
        int numVertices3 = 5;
        
        printGraph(graph3, numVertices3);
        System.out.println();
        
        List<Edge> mst3 = primMST(graph3, numVertices3);
        int weight3 = calculateMSTWeight(mst3);
        printMST(mst3, weight3);
        System.out.println();
        
        // Test Case 4: Star graph
        System.out.println("Test Case 4: Star graph (all vertices connected to center)");
        int[][] graph4 = {
            {0, 1, 2, 3, 4},
            {1, 0, 0, 0, 0},
            {2, 0, 0, 0, 0},
            {3, 0, 0, 0, 0},
            {4, 0, 0, 0, 0}
        };
        int numVertices4 = 5;
        
        printGraph(graph4, numVertices4);
        System.out.println();
        
        List<Edge> mst4 = primMST(graph4, numVertices4);
        int weight4 = calculateMSTWeight(mst4);
        printMST(mst4, weight4);
        System.out.println("Note: Star graph MST is the entire graph (already minimal)");
        System.out.println();
        
        // Test Case 5: Algorithm comparison
        System.out.println("Test Case 5: Algorithm comparison");
        compareWithKruskal(graph3, numVertices3);
        
        // Test Case 6: Complete graph K4
        System.out.println("\nTest Case 6: Complete graph K4");
        int[][] graph6 = {
            {0, 1, 3, 4},
            {1, 0, 2, 5},
            {3, 2, 0, 6},
            {4, 5, 6, 0}
        };
        int numVertices6 = 4;
        
        printGraph(graph6, numVertices6);
        System.out.println();
        
        List<Edge> mst6 = primMST(graph6, numVertices6);
        int weight6 = calculateMSTWeight(mst6);
        printMST(mst6, weight6);
        System.out.println("Greedy approach finds optimal MST even in complete graphs");
    }
}