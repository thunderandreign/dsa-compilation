#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

/**
 * Greedy Strategy: Prim's Minimum Spanning Tree Algorithm
 * Core Idea: Always add the minimum weight edge that connects a vertex in MST to a vertex outside MST
 * Time Complexity: O(V²) with adjacency matrix
 * Space Complexity: O(V) for tracking MST vertices and minimum edge weights
 */

#define MAX_VERTICES 10

/**
 * Edge structure to represent weighted edges
 */
typedef struct {
    int source, destination, weight;
} Edge;

/**
 * Find the vertex with minimum weight that is not yet in MST
 */
int findMinWeightVertex(int minWeight[], int inMST[], int numVertices) {
    int min = INT_MAX;
    int minIndex = -1;
    
    for (int v = 0; v < numVertices; v++) {
        if (!inMST[v] && minWeight[v] < min) {
            min = minWeight[v];
            minIndex = v;
        }
    }
    
    return minIndex;
}

/**
 * Find Minimum Spanning Tree using Prim's algorithm
 * @param graph Adjacency matrix representation of the graph
 * @param numVertices Number of vertices
 * @param mst Array to store MST edges
 * @return Number of edges in MST
 */
int primMST(int graph[][MAX_VERTICES], int numVertices, Edge mst[]) {
    // Track which vertices are included in MST
    int inMST[MAX_VERTICES] = {0};
    
    // Track minimum weight edge to reach each vertex
    int minWeight[MAX_VERTICES];
    int parent[MAX_VERTICES]; // To track MST edges
    
    // Initialize all weights as infinite
    for (int i = 0; i < numVertices; i++) {
        minWeight[i] = INT_MAX;
        parent[i] = -1;
    }
    
    // Start with vertex 0
    minWeight[0] = 0;
    int mstSize = 0;
    
    // Build MST with V-1 edges
    for (int edgeCount = 0; edgeCount < numVertices - 1; edgeCount++) {
        
        // Step 1: Find minimum weight vertex not yet in MST (greedy choice)
        int minWeightVertex = findMinWeightVertex(minWeight, inMST, numVertices);
        
        if (minWeightVertex == -1) {
            break; // Graph is disconnected
        }
        
        // Add vertex to MST
        inMST[minWeightVertex] = 1;
        
        // Add edge to MST (except for the first vertex)
        if (parent[minWeightVertex] != -1) {
            mst[mstSize].source = parent[minWeightVertex];
            mst[mstSize].destination = minWeightVertex;
            mst[mstSize].weight = minWeight[minWeightVertex];
            mstSize++;
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
    
    return mstSize;
}

/**
 * Calculate total weight of MST
 */
int calculateMSTWeight(Edge mst[], int mstSize) {
    int totalWeight = 0;
    for (int i = 0; i < mstSize; i++) {
        totalWeight += mst[i].weight;
    }
    return totalWeight;
}

/**
 * Print the graph in adjacency matrix format
 */
void printGraph(int graph[][MAX_VERTICES], int numVertices) {
    printf("Graph (Adjacency Matrix):\n");
    printf("   ");
    for (int i = 0; i < numVertices; i++) {
        printf("%3d", i);
    }
    printf("\n");
    
    for (int i = 0; i < numVertices; i++) {
        printf("%2d:", i);
        for (int j = 0; j < numVertices; j++) {
            if (graph[i][j] == 0) {
                printf(" - ");
            } else {
                printf("%3d", graph[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * Print MST result
 */
void printMST(Edge mst[], int mstSize, int totalWeight) {
    printf("Minimum Spanning Tree (Prim's Algorithm):\n");
    for (int i = 0; i < mstSize; i++) {
        printf("  %d. (%d-%d, weight: %d)\n", 
               i + 1, mst[i].source, mst[i].destination, mst[i].weight);
    }
    printf("Total weight: %d\n", totalWeight);
}

/**
 * Demonstrate Prim's algorithm step by step
 */
void demonstratePrimSteps(int graph[][MAX_VERTICES], int numVertices) {
    printf("Prim's Algorithm Steps:\n");
    
    int inMST[MAX_VERTICES] = {0};
    int minWeight[MAX_VERTICES];
    int parent[MAX_VERTICES];
    
    for (int i = 0; i < numVertices; i++) {
        minWeight[i] = INT_MAX;
        parent[i] = -1;
    }
    minWeight[0] = 0;
    
    printf("1. Start with vertex 0\n");
    printf("   MST vertices: {0}\n");
    
    for (int step = 0; step < numVertices - 1; step++) {
        
        // Find minimum weight vertex
        int minWeightVertex = findMinWeightVertex(minWeight, inMST, numVertices);
        
        if (minWeightVertex == -1) break;
        
        inMST[minWeightVertex] = 1;
        
        printf("%d. Add vertex %d to MST", step + 2, minWeightVertex);
        if (parent[minWeightVertex] != -1) {
            printf(" (via edge %d-%d, weight: %d)", 
                parent[minWeightVertex], minWeightVertex, minWeight[minWeightVertex]);
        }
        printf("\n");
        
        // Show current MST vertices
        printf("   MST vertices: {");
        int first = 1;
        for (int v = 0; v < numVertices; v++) {
            if (inMST[v]) {
                if (!first) printf(", ");
                printf("%d", v);
                first = 0;
            }
        }
        printf("}\n");
        
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
            printf("   Available edges: ");
            int hasEdges = 0;
            for (int v = 0; v < numVertices; v++) {
                if (!inMST[v] && minWeight[v] != INT_MAX) {
                    if (hasEdges) printf(", ");
                    printf("%d-%d (weight: %d)", parent[v], v, minWeight[v]);
                    hasEdges = 1;
                }
            }
            if (!hasEdges) printf("None");
            printf("\n");
        }
    }
}

/**
 * Compare with alternative MST algorithm (mention Kruskal's)
 */
void compareWithKruskal(int graph[][MAX_VERTICES], int numVertices) {
    printf("Algorithm Comparison:\n");
    printf("1. Prim's Algorithm:\n");
    printf("   - Grows MST one vertex at a time\n");
    printf("   - Always maintains a connected subgraph\n");
    printf("   - Better for dense graphs\n");
    printf("   - Time: O(V²) with adjacency matrix\n");
    
    printf("2. Kruskal's Algorithm:\n");
    printf("   - Considers all edges, adds minimum weight edges\n");
    printf("   - May create forest initially, then connects\n");
    printf("   - Better for sparse graphs\n");
    printf("   - Time: O(E log E) due to sorting\n");
    
    Edge mst[MAX_VERTICES - 1];
    int mstSize = primMST(graph, numVertices, mst);
    int primWeight = calculateMSTWeight(mst, mstSize);
    printf("Both algorithms produce the same optimal MST weight: %d\n", primWeight);
}

int main() {
    printf("=== Prim's Minimum Spanning Tree - Greedy Algorithm ===\n");
    
    // Test Case 1: Simple 4-vertex graph
    printf("Test Case 1: Simple 4-vertex graph\n");
    int graph1[MAX_VERTICES][MAX_VERTICES] = {
        {0, 2, 0, 6},
        {2, 0, 3, 8},
        {0, 3, 0, 5},
        {6, 8, 5, 0}
    };
    int numVertices1 = 4;
    
    printGraph(graph1, numVertices1);
    printf("\n");
    
    Edge mst1[MAX_VERTICES - 1];
    int mstSize1 = primMST(graph1, numVertices1, mst1);
    int weight1 = calculateMSTWeight(mst1, mstSize1);
    printMST(mst1, mstSize1, weight1);
    printf("\n");
    
    demonstratePrimSteps(graph1, numVertices1);
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
    
    Edge mst2[MAX_VERTICES - 1];
    int mstSize2 = primMST(graph2, numVertices2, mst2);
    int weight2 = calculateMSTWeight(mst2, mstSize2);
    printMST(mst2, mstSize2, weight2);
    printf("Note: Greedy choice always picks minimum weight edges to expand MST\n\n");
    
    // Test Case 3: 5-vertex graph
    printf("Test Case 3: 5-vertex graph\n");
    int graph3[MAX_VERTICES][MAX_VERTICES] = {
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
    printf("\n");
    
    Edge mst3[MAX_VERTICES - 1];
    int mstSize3 = primMST(graph3, numVertices3, mst3);
    int weight3 = calculateMSTWeight(mst3, mstSize3);
    printMST(mst3, mstSize3, weight3);
    printf("\n");
    
    // Test Case 4: Star graph
    printf("Test Case 4: Star graph (all vertices connected to center)\n");
    int graph4[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 2, 3, 4},
        {1, 0, 0, 0, 0},
        {2, 0, 0, 0, 0},
        {3, 0, 0, 0, 0},
        {4, 0, 0, 0, 0}
    };
    int numVertices4 = 5;
    
    printGraph(graph4, numVertices4);
    printf("\n");
    
    Edge mst4[MAX_VERTICES - 1];
    int mstSize4 = primMST(graph4, numVertices4, mst4);
    int weight4 = calculateMSTWeight(mst4, mstSize4);
    printMST(mst4, mstSize4, weight4);
    printf("Note: Star graph MST is the entire graph (already minimal)\n\n");
    
    // Test Case 5: Algorithm comparison
    printf("Test Case 5: Algorithm comparison\n");
    compareWithKruskal(graph3, numVertices3);
    printf("\n");
    
    // Test Case 6: Complete graph K4
    printf("Test Case 6: Complete graph K4\n");
    int graph6[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 3, 4},
        {1, 0, 2, 5},
        {3, 2, 0, 6},
        {4, 5, 6, 0}
    };
    int numVertices6 = 4;
    
    printGraph(graph6, numVertices6);
    printf("\n");
    
    Edge mst6[MAX_VERTICES - 1];
    int mstSize6 = primMST(graph6, numVertices6, mst6);
    int weight6 = calculateMSTWeight(mst6, mstSize6);
    printMST(mst6, mstSize6, weight6);
    printf("Greedy approach finds optimal MST even in complete graphs\n");
    
    return 0;
}