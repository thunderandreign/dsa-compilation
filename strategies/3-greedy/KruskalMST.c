#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Greedy Strategy: Kruskal's Minimum Spanning Tree Algorithm
 * Core Idea: Always choose the edge with minimum weight that doesn't create a cycle
 * Time Complexity: O(E log E) where E is the number of edges
 * Space Complexity: O(V) for Union-Find data structure
 */

#define MAX_VERTICES 10
#define MAX_EDGES 50

/**
 * Edge structure to represent weighted edges
 */
typedef struct {
    int source, destination, weight;
} Edge;

/**
 * Union-Find (Disjoint Set) data structure for cycle detection
 */
typedef struct {
    int parent[MAX_VERTICES];
    int rank[MAX_VERTICES];
    int numVertices;
} UnionFind;

/**
 * Initialize Union-Find data structure
 */
void initUnionFind(UnionFind* uf, int numVertices) {
    uf->numVertices = numVertices;
    
    // Initialize each vertex as its own parent (separate sets)
    for (int i = 0; i < numVertices; i++) {
        uf->parent[i] = i;
        uf->rank[i] = 0;
    }
}

/**
 * Find the root of the set containing vertex x (with path compression)
 */
int find(UnionFind* uf, int x) {
    if (uf->parent[x] != x) {
        uf->parent[x] = find(uf, uf->parent[x]); // Path compression
    }
    return uf->parent[x];
}

/**
 * Union two sets containing vertices x and y (by rank)
 * @return true if union successful, false if vertices already in same set
 */
bool unionSets(UnionFind* uf, int x, int y) {
    int rootX = find(uf, x);
    int rootY = find(uf, y);
    
    // If both vertices are in the same set, adding edge would create cycle
    if (rootX == rootY) {
        return false;
    }
    
    // Union by rank: attach smaller rank tree under root of higher rank tree
    if (uf->rank[rootX] < uf->rank[rootY]) {
        uf->parent[rootX] = rootY;
    } else if (uf->rank[rootX] > uf->rank[rootY]) {
        uf->parent[rootY] = rootX;
    } else {  
        uf->parent[rootY] = rootX;
        uf->rank[rootX]++;
    }
    
    return true;
}

/**
 * Compare function for sorting edges by weight
 */
int compareEdges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeA->weight - edgeB->weight;
}

/**
 * Find Minimum Spanning Tree using Kruskal's algorithm
 * @param edges Array of all edges in the graph
 * @param numEdges Number of edges
 * @param numVertices Number of vertices
 * @param mst Array to store MST edges
 * @return Number of edges in MST
 */
int kruskalMST(Edge edges[], int numEdges, int numVertices, Edge mst[]) {
    // Step 1: Sort all edges by weight (greedy choice preparation)
    qsort(edges, numEdges, sizeof(Edge), compareEdges);
    
    // Step 2: Initialize Union-Find data structure
    UnionFind uf;
    initUnionFind(&uf, numVertices);
    
    int mstSize = 0;
    
    // Step 3: Process edges in order of increasing weight
    for (int i = 0; i < numEdges && mstSize < numVertices - 1; i++) {
        Edge currentEdge = edges[i];
        
        // Greedy choice: if edge doesn't create cycle, include it in MST
        if (unionSets(&uf, currentEdge.source, currentEdge.destination)) {
            mst[mstSize++] = currentEdge;
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
 * Create edge list from adjacency matrix
 */
int createEdgeList(int graph[][MAX_VERTICES], int numVertices, Edge edges[]) {
    int edgeCount = 0;
    
    for (int i = 0; i < numVertices; i++) {
        for (int j = i + 1; j < numVertices; j++) { // Only upper triangle to avoid duplicates
            if (graph[i][j] != 0) {
                edges[edgeCount].source = i;
                edges[edgeCount].destination = j;
                edges[edgeCount].weight = graph[i][j];
                edgeCount++;
            }
        }
    }
    
    return edgeCount;
}

/**
 * Print the graph edges
 */
void printGraphEdges(Edge edges[], int numEdges) {
    printf("Graph edges:\n");
    
    // Sort edges by weight for better readability
    qsort(edges, numEdges, sizeof(Edge), compareEdges);
    
    for (int i = 0; i < numEdges; i++) {
        printf("  (%d-%d, weight: %d)\n", 
               edges[i].source, edges[i].destination, edges[i].weight);
    }
}

/**
 * Print MST result
 */
void printMST(Edge mst[], int mstSize, int totalWeight) {
    printf("Minimum Spanning Tree:\n");
    for (int i = 0; i < mstSize; i++) {
        printf("  %d. (%d-%d, weight: %d)\n", 
               i + 1, mst[i].source, mst[i].destination, mst[i].weight);
    }
    printf("Total weight: %d\n", totalWeight);
}

int main() {
    printf("=== Kruskal's Minimum Spanning Tree - Greedy Algorithm ===\n");
    
    // Test Case 1: Simple 4-vertex graph
    printf("Test Case 1: Simple 4-vertex graph\n");
    int graph1[MAX_VERTICES][MAX_VERTICES] = {
        {0, 2, 0, 6},
        {2, 0, 3, 8},
        {0, 3, 0, 5},
        {6, 8, 5, 0}
    };
    int numVertices1 = 4;
    
    Edge edges1[MAX_EDGES];
    int numEdges1 = createEdgeList(graph1, numVertices1, edges1);
    printGraphEdges(edges1, numEdges1);
    printf("\n");
    
    Edge mst1[MAX_VERTICES - 1];
    int mstSize1 = kruskalMST(edges1, numEdges1, numVertices1, mst1);
    int weight1 = calculateMSTWeight(mst1, mstSize1);
    printMST(mst1, mstSize1, weight1);
    printf("\n");
    
    // Test Case 2: Triangle graph (simple case)
    printf("Test Case 2: Triangle graph\n");
    int graph2[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 4},
        {1, 0, 2},
        {4, 2, 0}
    };
    int numVertices2 = 3;
    
    Edge edges2[MAX_EDGES];
    int numEdges2 = createEdgeList(graph2, numVertices2, edges2);
    printGraphEdges(edges2, numEdges2);
    printf("\n");
    
    Edge mst2[MAX_VERTICES - 1];
    int mstSize2 = kruskalMST(edges2, numEdges2, numVertices2, mst2);
    int weight2 = calculateMSTWeight(mst2, mstSize2);
    printMST(mst2, mstSize2, weight2);
    printf("Note: Greedy choice always picks edges (0-1, weight:1) and (1-2, weight:2)\n");
    printf("Skips edge (0-2, weight:4) as it would create a cycle\n\n");
    
    // Test Case 3: 5-vertex graph
    printf("Test Case 3: 5-vertex graph\n");
    int graph3[MAX_VERTICES][MAX_VERTICES] = {
        {0, 4, 2, 0, 0},
        {4, 0, 8, 0, 10},
        {2, 8, 0, 7, 9},
        {0, 0, 7, 0, 14},
        {0, 10, 9, 14, 0}
    };
    
    // Add more edges
    graph3[0][3] = 7;
    graph3[3][0] = 7;
    
    int numVertices3 = 5;
    
    Edge edges3[MAX_EDGES];
    int numEdges3 = createEdgeList(graph3, numVertices3, edges3);
    printGraphEdges(edges3, numEdges3);
    printf("\n");
    
    Edge mst3[MAX_VERTICES - 1];
    int mstSize3 = kruskalMST(edges3, numEdges3, numVertices3, mst3);
    int weight3 = calculateMSTWeight(mst3, mstSize3);
    printMST(mst3, mstSize3, weight3);
    printf("\n");
    
    // Test Case 4: Disconnected graph
    printf("Test Case 4: Disconnected graph\n");
    int graph4[MAX_VERTICES][MAX_VERTICES] = {
        {0, 1, 0, 0},
        {1, 0, 0, 0},
        {0, 0, 0, 2},
        {0, 0, 2, 0}
    };
    int numVertices4 = 4;
    
    Edge edges4[MAX_EDGES];
    int numEdges4 = createEdgeList(graph4, numVertices4, edges4);
    printGraphEdges(edges4, numEdges4);
    printf("\n");
    
    Edge mst4[MAX_VERTICES - 1];
    int mstSize4 = kruskalMST(edges4, numEdges4, numVertices4, mst4);
    int weight4 = calculateMSTWeight(mst4, mstSize4);
    
    if (mstSize4 < numVertices4 - 1) { // MST should have V-1 edges
        printf("Graph is disconnected - no spanning tree exists\n");
        printf("Found edges: %d (need %d for spanning tree)\n", mstSize4, numVertices4 - 1);
        if (mstSize4 > 0) {
            printMST(mst4, mstSize4, weight4);
        }
    } else {
        printMST(mst4, mstSize4, weight4);
    }
    printf("\n");
    
    // Test Case 5: Single vertex
    printf("Test Case 5: Two vertex graph\n");
    int graph5[MAX_VERTICES][MAX_VERTICES] = {
        {0, 5},
        {5, 0}
    };
    int numVertices5 = 2;
    
    Edge edges5[MAX_EDGES];
    int numEdges5 = createEdgeList(graph5, numVertices5, edges5);
    printGraphEdges(edges5, numEdges5);
    printf("\n");
    
    Edge mst5[MAX_VERTICES - 1];
    int mstSize5 = kruskalMST(edges5, numEdges5, numVertices5, mst5);
    int weight5 = calculateMSTWeight(mst5, mstSize5);
    printMST(mst5, mstSize5, weight5);
    
    return 0;
}