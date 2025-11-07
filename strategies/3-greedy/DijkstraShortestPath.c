#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

/**
 * Greedy Strategy: Dijkstra's Shortest Path Algorithm
 * Core Idea: Always choose the vertex with minimum distance that hasn't been processed
 * Time Complexity: O(V²) with adjacency matrix, O((V + E) log V) with priority queue
 * Space Complexity: O(V) for distance and visited arrays
 */

#define MAX_VERTICES 10

/**
 * Structure to store path results
 */
typedef struct {
    int distances[MAX_VERTICES];
    int predecessors[MAX_VERTICES];
    int numVertices;
} PathResult;

/**
 * Find vertex with minimum distance that hasn't been visited (greedy choice)
 * @param distances Array of current distances
 * @param visited Array of visited flags
 * @param numVertices Number of vertices
 * @return Index of vertex with minimum distance
 */
int getMinDistanceVertex(int distances[], bool visited[], int numVertices) {
    int minDistance = INT_MAX;
    int minIndex = -1;
    
    for (int v = 0; v < numVertices; v++) {
        if (!visited[v] && distances[v] <= minDistance) {
            minDistance = distances[v];
            minIndex = v;
        }
    }
    
    return minIndex;
}

/**
 * Find shortest paths from source to all vertices using Dijkstra's algorithm
 * @param graph Adjacency matrix representation of weighted graph
 * @param numVertices Number of vertices in the graph
 * @param source Source vertex (0-indexed)
 * @return PathResult containing distances and predecessors
 */
PathResult dijkstra(int graph[][MAX_VERTICES], int numVertices, int source) {
    PathResult result;
    result.numVertices = numVertices;
    bool visited[MAX_VERTICES] = {false};
    
    // Initialize distances to infinity and predecessors to -1
    for (int i = 0; i < numVertices; i++) {
        result.distances[i] = INT_MAX;
        result.predecessors[i] = -1;
    }
    
    // Distance from source to itself is 0
    result.distances[source] = 0;
    
    // Find shortest path for all vertices
    for (int count = 0; count < numVertices - 1; count++) {
        // Greedy choice: pick the minimum distance vertex not yet processed
        int u = getMinDistanceVertex(result.distances, visited, numVertices);
        visited[u] = true;
        
        // Update distances of adjacent vertices
        for (int v = 0; v < numVertices; v++) {
            // Update distance[v] if:
            // 1. v is not visited
            // 2. there's an edge from u to v
            // 3. total weight through u is less than current distance[v]
            if (!visited[v] && graph[u][v] != 0 && 
                result.distances[u] != INT_MAX &&
                result.distances[u] + graph[u][v] < result.distances[v]) {
                
                result.distances[v] = result.distances[u] + graph[u][v];
                result.predecessors[v] = u;
            }
        }
    }
    
    return result;
}

/**
 * Reconstruct path from source to destination
 * @param predecessors Array of predecessors from Dijkstra's result
 * @param source Source vertex
 * @param destination Destination vertex
 * @param path Array to store the path
 * @return Length of path, 0 if no path exists
 */
int getPath(int predecessors[], int source, int destination, int path[]) {
    // If destination is unreachable
    if (predecessors[destination] == -1 && destination != source) {
        return 0;
    }
    
    // Build path backwards from destination
    int temp[MAX_VERTICES];
    int length = 0;
    int current = destination;
    
    while (current != -1) {
        temp[length++] = current;
        current = predecessors[current];
    }
    
    // Reverse the path
    for (int i = 0; i < length; i++) {
        path[i] = temp[length - 1 - i];
    }
    
    return length;
}

/**
 * Print the shortest path results
 */
void printResults(PathResult result, int source) {
    printf("Shortest distances from vertex %d:\n", source);
    for (int i = 0; i < result.numVertices; i++) {
        if (result.distances[i] == INT_MAX) {
            printf("To vertex %d: UNREACHABLE\n", i);
        } else {
            printf("To vertex %d: %d\n", i, result.distances[i]);
        }
    }
}

/**
 * Print the graph in adjacency matrix format
 */
void printGraph(int graph[][MAX_VERTICES], int numVertices) {
    printf("Graph (adjacency matrix):\n");
    printf("     ");
    for (int i = 0; i < numVertices; i++) {
        printf("%3d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < numVertices; i++) {
        printf("%3d: ", i);
        for (int j = 0; j < numVertices; j++) {
            if (graph[i][j] == 0) {
                printf("  - ");
            } else {
                printf("%3d ", graph[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * Print array as path
 */
void printPath(int path[], int length) {
    printf("[");
    for (int i = 0; i < length; i++) {
        printf("%d", path[i]);
        if (i < length - 1) printf(" -> ");
    }
    printf("]");
}

int main() {
    printf("=== Dijkstra's Shortest Path - Greedy Algorithm ===\n");
    
    // Test Case 1: Simple 5-vertex graph
    printf("Test Case 1: Simple 5-vertex graph\n");
    int graph1[MAX_VERTICES][MAX_VERTICES] = {
        {0, 10, 0, 30, 100},
        {10, 0, 50, 0, 0},
        {0, 50, 0, 20, 10},
        {30, 0, 20, 0, 60},  
        {100, 0, 10, 60, 0}
    };
    int numVertices1 = 5;
    
    printGraph(graph1, numVertices1);
    printf("\n");
    
    PathResult result1 = dijkstra(graph1, numVertices1, 0);
    printResults(result1, 0);
    
    // Show specific paths
    printf("\nSample paths from vertex 0:\n");
    for (int dest = 1; dest < numVertices1; dest++) {
        int path[MAX_VERTICES];
        int pathLength = getPath(result1.predecessors, 0, dest, path);
        printf("Path to %d: ", dest);
        if (pathLength > 0) {
            printPath(path, pathLength);
            printf(" (distance: %d)\n", result1.distances[dest]);
        } else {
            printf("No path\n");
        }
    }
    printf("\n");
    
    // Test Case 2: Triangle graph
    printf("Test Case 2: Triangle graph\n");
    int graph2[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 4},
        {1, 0, 2},
        {4, 2, 0}
    };
    int numVertices2 = 3;
    
    printGraph(graph2, numVertices2);
    printf("\n");
    
    PathResult result2 = dijkstra(graph2, numVertices2, 0);
    printResults(result2, 0);
    
    // Show how greedy choice works
    printf("\nPath analysis (showing greedy choices):\n");
    printf("Direct path 0->2: distance = 4\n");
    printf("Path 0->1->2: distance = 1+2 = 3 (chosen by algorithm)\n");
    
    int pathTo2[MAX_VERTICES];
    int pathLength = getPath(result2.predecessors, 0, 2, pathTo2);
    printf("Algorithm chose path: ");
    printPath(pathTo2, pathLength);
    printf("\n\n");
    
    // Test Case 3: Single vertex
    printf("Test Case 3: Single vertex graph\n");
    int graph3[MAX_VERTICES][MAX_VERTICES] = {{0}};
    int numVertices3 = 1;
    
    PathResult result3 = dijkstra(graph3, numVertices3, 0);
    printResults(result3, 0);
    printf("\n");
    
    // Test Case 4: Linear chain
    printf("Test Case 4: Linear chain graph\n");
    int graph4[MAX_VERTICES][MAX_VERTICES] = {
        {0, 2, 0, 0},
        {2, 0, 3, 0},
        {0, 3, 0, 1},
        {0, 0, 1, 0}
    };
    int numVertices4 = 4;
    
    printGraph(graph4, numVertices4);
    printf("\n");
    
    PathResult result4 = dijkstra(graph4, numVertices4, 0);
    printResults(result4, 0);
    
    return 0;
}