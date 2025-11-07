import java.util.*;

/**
 * Greedy Strategy: Kruskal's Minimum Spanning Tree Algorithm
 * Core Idea: Always choose the edge with minimum weight that doesn't create a cycle
 * Time Complexity: O(E log E) where E is the number of edges
 * Space Complexity: O(V) for Union-Find data structure
 */
public class KruskalMST {
    
    /**
     * Edge class to represent weighted edges
     */
    static class Edge implements Comparable<Edge> {
        int source, destination, weight;
        
        Edge(int source, int destination, int weight) {
            this.source = source;
            this.destination = destination;
            this.weight = weight;
        }
        
        @Override
        public int compareTo(Edge other) {
            return Integer.compare(this.weight, other.weight);
        }
        
        @Override
        public String toString() {
            return String.format("(%d-%d, weight: %d)", source, destination, weight);
        }
    }
    
    /**
     * Union-Find (Disjoint Set) data structure for cycle detection
     */
    static class UnionFind {
        private int[] parent;
        private int[] rank;
        
        public UnionFind(int numVertices) {
            parent = new int[numVertices];
            rank = new int[numVertices];
            
            // Initialize each vertex as its own parent (separate sets)
            for (int i = 0; i < numVertices; i++) {
                parent[i] = i;
                rank[i] = 0;
            }
        }
        
        /**
         * Find the root of the set containing vertex x (with path compression)
         */
        public int find(int x) {
            if (parent[x] != x) {
                parent[x] = find(parent[x]); // Path compression
            }
            return parent[x];
        }
        
        /**
         * Union two sets containing vertices x and y (by rank)
         */
        public boolean union(int x, int y) {
            int rootX = find(x);
            int rootY = find(y);
            
            // If both vertices are in the same set, adding edge would create cycle
            if (rootX == rootY) {
                return false;
            }
            
            // Union by rank: attach smaller rank tree under root of higher rank tree
            if (rank[rootX] < rank[rootY]) {
                parent[rootX] = rootY;
            } else if (rank[rootX] > rank[rootY]) {
                parent[rootY] = rootX;
            } else {
                parent[rootY] = rootX;
                rank[rootX]++;
            }
            
            return true;
        }
    }
    
    /**
     * Find Minimum Spanning Tree using Kruskal's algorithm
     * @param numVertices Number of vertices in the graph
     * @param edges List of all edges in the graph
     * @return List of edges in the MST
     */
    public static List<Edge> kruskalMST(int numVertices, List<Edge> edges) {
        List<Edge> mst = new ArrayList<>();
        
        // Step 1: Sort all edges by weight (greedy choice preparation)
        Collections.sort(edges);
        
        // Step 2: Initialize Union-Find data structure
        UnionFind unionFind = new UnionFind(numVertices);
        
        // Step 3: Process edges in order of increasing weight
        for (Edge edge : edges) {
            // Greedy choice: if edge doesn't create cycle, include it in MST
            if (unionFind.union(edge.source, edge.destination)) {
                mst.add(edge);
                
                // MST has exactly (V-1) edges
                if (mst.size() == numVertices - 1) {
                    break;
                }
            }
        }
        
        return mst;
    }
    
    /**
     * Calculate total weight of MST
     */
    public static int calculateMSTWeight(List<Edge> mst) {
        int totalWeight = 0;
        for (Edge edge : mst) {
            totalWeight += edge.weight;
        }
        return totalWeight;
    }
    
    /**
     * Create edge list from adjacency matrix
     */
    public static List<Edge> createEdgeList(int[][] graph) {
        List<Edge> edges = new ArrayList<>();
        int numVertices = graph.length;
        
        for (int i = 0; i < numVertices; i++) {
            for (int j = i + 1; j < numVertices; j++) { // Only upper triangle to avoid duplicates
                if (graph[i][j] != 0) {
                    edges.add(new Edge(i, j, graph[i][j]));
                }
            }
        }
        
        return edges;
    }
    
    /**
     * Print the graph edges
     */
    public static void printGraphEdges(List<Edge> edges) {
        System.out.println("Graph edges:");
        Collections.sort(edges); // Sort by weight for better readability
        for (Edge edge : edges) {
            System.out.println("  " + edge);
        }
    }
    
    /**
     * Print MST result
     */
    public static void printMST(List<Edge> mst, int totalWeight) {
        System.out.println("Minimum Spanning Tree:");
        for (int i = 0; i < mst.size(); i++) {
            System.out.println("  " + (i + 1) + ". " + mst.get(i));
        }
        System.out.println("Total weight: " + totalWeight);
    }
    
    public static void main(String[] args) {
        System.out.println("=== Kruskal's Minimum Spanning Tree - Greedy Algorithm ===");
        
        // Test Case 1: Simple 4-vertex graph
        System.out.println("Test Case 1: Simple 4-vertex graph");
        int[][] graph1 = {
            {0, 2, 0, 6},
            {2, 0, 3, 8},
            {0, 3, 0, 5},
            {6, 8, 5, 0}
        };
        
        List<Edge> edges1 = createEdgeList(graph1);
        printGraphEdges(edges1);
        System.out.println();
        
        List<Edge> mst1 = kruskalMST(4, new ArrayList<>(edges1));
        int weight1 = calculateMSTWeight(mst1);
        printMST(mst1, weight1);
        System.out.println();
        
        // Test Case 2: More complex 5-vertex graph
        System.out.println("Test Case 2: 5-vertex graph");
        int[][] graph2 = {
            {0, 4, 0, 0, 0},
            {4, 0, 8, 0, 0},
            {0, 8, 0, 7, 9},
            {0, 0, 7, 0, 14},
            {0, 0, 9, 14, 0}
        };
        
        // Add more edges to make it interesting
        graph2[0][2] = 2;
        graph2[2][0] = 2;
        graph2[0][3] = 7;
        graph2[3][0] = 7;
        graph2[1][4] = 10;
        graph2[4][1] = 10;
        
        List<Edge> edges2 = createEdgeList(graph2);
        printGraphEdges(edges2);
        System.out.println();
        
        List<Edge> mst2 = kruskalMST(5, new ArrayList<>(edges2));
        int weight2 = calculateMSTWeight(mst2);
        printMST(mst2, weight2);
        System.out.println();
        
        // Test Case 3: Triangle graph (simple case)
        System.out.println("Test Case 3: Triangle graph");
        int[][] graph3 = {
            {0, 1, 4},
            {1, 0, 2},
            {4, 2, 0}
        };
        
        List<Edge> edges3 = createEdgeList(graph3);
        printGraphEdges(edges3);
        System.out.println();
        
        List<Edge> mst3 = kruskalMST(3, new ArrayList<>(edges3));
        int weight3 = calculateMSTWeight(mst3);
        printMST(mst3, weight3);
        System.out.println("Note: Greedy choice always picks edges (0-1, weight:1) and (1-2, weight:2)");
        System.out.println("Skips edge (0-2, weight:4) as it would create a cycle");
        System.out.println();
        
        // Test Case 4: Disconnected graph
        System.out.println("Test Case 4: Disconnected graph");
        int[][] graph4 = {
            {0, 1, 0, 0},
            {1, 0, 0, 0},
            {0, 0, 0, 2},
            {0, 0, 2, 0}
        };
        
        List<Edge> edges4 = createEdgeList(graph4);
        printGraphEdges(edges4);
        System.out.println();
        
        List<Edge> mst4 = kruskalMST(4, new ArrayList<>(edges4));
        int weight4 = calculateMSTWeight(mst4);
        
        if (mst4.size() < 3) { // MST should have V-1 edges
            System.out.println("Graph is disconnected - no spanning tree exists");
            System.out.println("Found edges: " + mst4.size() + " (need 3 for spanning tree)");
            if (!mst4.isEmpty()) {
                printMST(mst4, weight4);
            }
        } else {
            printMST(mst4, weight4);
        }
        System.out.println();
        
        // Test Case 5: Single vertex
        System.out.println("Test Case 5: Single vertex graph");
        List<Edge> mst5 = kruskalMST(1, new ArrayList<>());
        System.out.println("MST for single vertex: " + mst5.size() + " edges (empty MST)");
        System.out.println("Total weight: 0");
    }
}