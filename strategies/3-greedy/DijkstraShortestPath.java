import java.util.*;

/**
 * Greedy Strategy: Dijkstra's Shortest Path Algorithm
 * Core Idea: Always choose the vertex with minimum distance that hasn't been processed
 * Time Complexity: O(V²) with adjacency matrix, O((V + E) log V) with priority queue
 * Space Complexity: O(V) for distance and visited arrays
 */
public class DijkstraShortestPath {
    
    /**
     * Result class to store path information
     */
    static class PathResult {
        int[] distances;
        int[] predecessors;
        
        PathResult(int[] distances, int[] predecessors) {
            this.distances = distances;
            this.predecessors = predecessors;
        }
    }
    
    /**
     * Find shortest paths from source to all vertices using Dijkstra's algorithm
     * @param graph Adjacency matrix representation of weighted graph
     * @param source Source vertex (0-indexed)
     * @return PathResult containing distances and predecessors
     */
    public static PathResult dijkstra(int[][] graph, int source) {
        int vertices = graph.length;
        int[] distances = new int[vertices];
        int[] predecessors = new int[vertices];
        boolean[] visited = new boolean[vertices];
        
        // Initialize distances to infinity and predecessors to -1
        Arrays.fill(distances, Integer.MAX_VALUE);
        Arrays.fill(predecessors, -1);
        
        // Distance from source to itself is 0
        distances[source] = 0;
        
        // Find shortest path for all vertices
        for (int count = 0; count < vertices - 1; count++) {
            // Greedy choice: pick the minimum distance vertex not yet processed
            int u = getMinDistanceVertex(distances, visited);
            visited[u] = true;
            
            // Update distances of adjacent vertices
            for (int v = 0; v < vertices; v++) {
                // Update distance[v] if:
                // 1. v is not visited
                // 2. there's an edge from u to v
                // 3. total weight through u is less than current distance[v]
                if (!visited[v] && graph[u][v] != 0 && 
                    distances[u] != Integer.MAX_VALUE &&
                    distances[u] + graph[u][v] < distances[v]) {
                    
                    distances[v] = distances[u] + graph[u][v];
                    predecessors[v] = u;
                }
            }
        }
        
        return new PathResult(distances, predecessors);
    }
    
    /**
     * Find vertex with minimum distance that hasn't been visited (greedy choice)
     * @param distances Array of current distances
     * @param visited Array of visited flags
     * @return Index of vertex with minimum distance
     */
    private static int getMinDistanceVertex(int[] distances, boolean[] visited) {
        int minDistance = Integer.MAX_VALUE;
        int minIndex = -1;
        
        for (int v = 0; v < distances.length; v++) {
            if (!visited[v] && distances[v] <= minDistance) {
                minDistance = distances[v];
                minIndex = v;
            }
        }
        
        return minIndex;
    }
    
    /**
     * Reconstruct path from source to destination
     * @param predecessors Array of predecessors from Dijkstra's result
     * @param source Source vertex
     * @param destination Destination vertex
     * @return List representing the path, empty if no path exists
     */
    public static List<Integer> getPath(int[] predecessors, int source, int destination) {
        List<Integer> path = new ArrayList<>();
        
        // If destination is unreachable
        if (predecessors[destination] == -1 && destination != source) {
            return path;
        }
        
        // Build path backwards from destination
        int current = destination;
        while (current != -1) {
            path.add(0, current); // Add to beginning
            current = predecessors[current];
        }
        
        return path;
    }
    
    /**
     * Print the shortest path results
     */
    public static void printResults(PathResult result, int source) {
        System.out.println("Shortest distances from vertex " + source + ":");
        for (int i = 0; i < result.distances.length; i++) {
            if (result.distances[i] == Integer.MAX_VALUE) {
                System.out.println("To vertex " + i + ": UNREACHABLE");
            } else {
                System.out.println("To vertex " + i + ": " + result.distances[i]);
            }
        }
    }
    
    /**
     * Print the graph in adjacency matrix format
     */
    public static void printGraph(int[][] graph) {
        System.out.println("Graph (adjacency matrix):");
        System.out.print("     ");
        for (int i = 0; i < graph.length; i++) {
            System.out.printf("%3d ", i);
        }
        System.out.println();
        
        for (int i = 0; i < graph.length; i++) {
            System.out.printf("%3d: ", i);
            for (int j = 0; j < graph[i].length; j++) {
                if (graph[i][j] == 0) {
                    System.out.print("  - ");
                } else {
                    System.out.printf("%3d ", graph[i][j]);
                }
            }
            System.out.println();
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Dijkstra's Shortest Path - Greedy Algorithm ===");
        
        // Test Case 1: Simple 5-vertex graph
        System.out.println("Test Case 1: Simple 5-vertex graph");
        int[][] graph1 = {
            {0, 10, 0, 30, 100},
            {10, 0, 50, 0, 0},
            {0, 50, 0, 20, 10},
            {30, 0, 20, 0, 60},  
            {100, 0, 10, 60, 0}
        };
        
        printGraph(graph1);
        System.out.println();
        
        PathResult result1 = dijkstra(graph1, 0);
        printResults(result1, 0);
        
        // Show specific paths
        System.out.println("\nSample paths from vertex 0:");
        for (int dest = 1; dest < 5; dest++) {
            List<Integer> path = getPath(result1.predecessors, 0, dest);
            System.out.println("Path to " + dest + ": " + path + 
                             " (distance: " + result1.distances[dest] + ")");
        }
        System.out.println();
        
        // Test Case 2: Graph with unreachable vertices
        System.out.println("Test Case 2: Graph with unreachable vertices");
        int[][] graph2 = {
            {0, 4, 0, 0, 0},
            {4, 0, 8, 0, 0},
            {0, 8, 0, 7, 9},
            {0, 0, 7, 0, 14},
            {0, 0, 9, 14, 0}
        };
        
        // Make vertex 4 unreachable from 0
        graph2[2][4] = 0;
        graph2[4][2] = 0;
        graph2[3][4] = 0;
        graph2[4][3] = 0;
        
        printGraph(graph2);
        System.out.println();
        
        PathResult result2 = dijkstra(graph2, 0);
        printResults(result2, 0);
        System.out.println();
        
        // Test Case 3: Single vertex
        System.out.println("Test Case 3: Single vertex graph");
        int[][] graph3 = {{0}};
        
        PathResult result3 = dijkstra(graph3, 0);
        printResults(result3, 0);
        System.out.println();
        
        // Test Case 4: Triangle graph
        System.out.println("Test Case 4: Triangle graph");
        int[][] graph4 = {
            {0, 1, 4},
            {1, 0, 2},
            {4, 2, 0}
        };
        
        printGraph(graph4);
        System.out.println();
        
        PathResult result4 = dijkstra(graph4, 0);
        printResults(result4, 0);
        
        // Show how greedy choice works
        System.out.println("\nPath analysis (showing greedy choices):");
        System.out.println("Direct path 0->2: distance = 4");
        System.out.println("Path 0->1->2: distance = 1+2 = 3 (chosen by algorithm)");
        
        List<Integer> pathTo2 = getPath(result4.predecessors, 0, 2);
        System.out.println("Algorithm chose path: " + pathTo2);
    }
}