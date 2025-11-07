#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/**
 * Backtracking Strategy: Graph Coloring Problem
 * Core Idea: Color vertices of a graph such that no two adjacent vertices have the same color
 * Time Complexity: O(k^V) where k is number of colors and V is number of vertices
 * Space Complexity: O(V) for recursion stack and color assignments
 */

#define MAX_VERTICES 20
#define MAX_COLORS 10

/**
 * Check if it's safe to color a vertex with given color
 * @param graph Adjacency matrix
 * @param colors Current color assignments
 * @param numVertices Number of vertices
 * @param vertex Vertex to check
 * @param color Color to assign
 * @return true if safe, false otherwise
 */
bool isSafeToColor(int graph[MAX_VERTICES][MAX_VERTICES], int colors[], int numVertices, int vertex, int color) {
    // Check all adjacent vertices
    for (int i = 0; i < numVertices; i++) {
        // If there's an edge and adjacent vertex has same color
        if (graph[vertex][i] == 1 && colors[i] == color) {
            return false;
        }
    }
    return true;
}

/**
 * Backtracking helper function for graph coloring
 * @param graph Adjacency matrix
 * @param colors Current color assignments
 * @param numVertices Number of vertices
 * @param vertex Current vertex to color
 * @param numColors Number of available colors
 * @return true if coloring is possible, false otherwise
 */
bool backtrackColoring(int graph[MAX_VERTICES][MAX_VERTICES], int colors[], int numVertices, int vertex, int numColors) {
    // Base case: all vertices colored
    if (vertex == numVertices) {
        return true;
    }
    
    // Try each color for current vertex
    for (int color = 1; color <= numColors; color++) {
        if (isSafeToColor(graph, colors, numVertices, vertex, color)) {
            // Assign color
            colors[vertex] = color;
            
            // Recursively color remaining vertices
            if (backtrackColoring(graph, colors, numVertices, vertex + 1, numColors)) {
                return true;
            }
            
            // Backtrack: remove color
            colors[vertex] = 0;
        }
    }
    
    return false; // No valid coloring found
}

/**
 * Solve graph coloring problem using backtracking
 * @param graph Adjacency matrix representation of the graph
 * @param numVertices Number of vertices
 * @param numColors Number of colors available
 * @param colors Array to store color assignments (output)
 * @return true if solution exists, false otherwise
 */
bool solveGraphColoring(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices, int numColors, int colors[]) {
    // Initialize colors to 0 (uncolored)
    for (int i = 0; i < numVertices; i++) {
        colors[i] = 0;
    }
    
    return backtrackColoring(graph, colors, numVertices, 0, numColors);
}

/**
 * Find minimum number of colors needed (chromatic number approximation)
 * @param graph Adjacency matrix
 * @param numVertices Number of vertices
 * @return Minimum colors needed (upper bound)
 */
int findChromaticNumber(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices) {
    int colors[MAX_VERTICES];
    
    // Try from 1 color up to V colors
    for (int numColors = 1; numColors <= numVertices; numColors++) {
        if (solveGraphColoring(graph, numVertices, numColors, colors)) {
            return numColors;
        }
    }
    
    return numVertices; // Worst case: each vertex different color
}

/**
 * Count total number of valid colorings
 * @param graph Adjacency matrix
 * @param colors Current color assignments
 * @param numVertices Number of vertices
 * @param vertex Current vertex
 * @param numColors Number of colors available
 * @return Number of valid colorings
 */
int countColoringsHelper(int graph[MAX_VERTICES][MAX_VERTICES], int colors[], int numVertices, int vertex, int numColors) {
    if (vertex == numVertices) {
        return 1; // Found a valid coloring
    }
    
    int count = 0;
    for (int color = 1; color <= numColors; color++) {
        if (isSafeToColor(graph, colors, numVertices, vertex, color)) {
            colors[vertex] = color;
            count += countColoringsHelper(graph, colors, numVertices, vertex + 1, numColors);
            colors[vertex] = 0;
        }
    }
    
    return count;
}

/**
 * Count total number of valid colorings
 */
int countColorings(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices, int numColors) {
    int colors[MAX_VERTICES];
    for (int i = 0; i < numVertices; i++) {
        colors[i] = 0;
    }
    return countColoringsHelper(graph, colors, numVertices, 0, numColors);
}

/**
 * Backtracking with step tracking
 */
bool backtrackWithSteps(int graph[MAX_VERTICES][MAX_VERTICES], int colors[], int numVertices, int vertex, int numColors, int* stepCount) {
    if (vertex == numVertices) {
        printf("Step %d: ✓ All vertices colored successfully!\n", ++(*stepCount));
        return true;
    }
    
    printf("Step %d: Coloring vertex %d (adjacent to: ", ++(*stepCount), vertex);
    
    // Print adjacent vertices
    bool first = true;
    for (int i = 0; i < numVertices; i++) {
        if (graph[vertex][i] == 1) {
            if (!first) printf(", ");
            printf("%d", i);
            first = false;
        }
    }
    printf(")\n");
    
    for (int color = 1; color <= numColors; color++) {
        const char* colorNames[] = {"", "Red", "Blue", "Green", "Yellow", "Purple", "Orange", "Pink", "Brown", "Gray", "Black"};
        const char* colorName = (color < 11) ? colorNames[color] : "Color";
        
        if (isSafeToColor(graph, colors, numVertices, vertex, color)) {
            printf("  Trying color %s (%d) for vertex %d - Valid!\n", colorName, color, vertex);
            colors[vertex] = color;
            
            if (backtrackWithSteps(graph, colors, numVertices, vertex + 1, numColors, stepCount)) {
                return true;
            }
            
            printf("  Backtracking from vertex %d, removing color %s\n", vertex, colorName);
            colors[vertex] = 0;
        } else {
            printf("  Color %s (%d) for vertex %d - Invalid (conflicts with adjacent vertices)\n", colorName, color, vertex);
        }
        
        // Limit output for demonstration
        if (*stepCount > 30) {
            printf("  ... (stopping step display after 30 steps)\n");
            return backtrackColoring(graph, colors, numVertices, vertex, numColors);
        }
    }
    
    return false;
}

/**
 * Solve with step tracking for demonstration
 */
bool solveWithSteps(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices, int numColors, int colors[], int* stepCount) {
    for (int i = 0; i < numVertices; i++) {
        colors[i] = 0;
    }
    
    *stepCount = 0;
    return backtrackWithSteps(graph, colors, numVertices, 0, numColors, stepCount);
}

/**
 * Print the graph as adjacency matrix
 */
void printGraph(int graph[MAX_VERTICES][MAX_VERTICES], int numVertices) {
    printf("Adjacency Matrix:\n");
    printf("   ");
    for (int i = 0; i < numVertices; i++) {
        printf("%2d ", i);
    }
    printf("\n");
    
    for (int i = 0; i < numVertices; i++) {
        printf("%2d ", i);
        for (int j = 0; j < numVertices; j++) {
            printf("%2d ", graph[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Print coloring solution
 */
void printColoring(int colors[], int numVertices, bool solved) {
    if (!solved) {
        printf("No valid coloring found.\n");
        return;
    }
    
    const char* colorNames[] = {"", "Red", "Blue", "Green", "Yellow", "Purple", "Orange", "Pink", "Brown", "Gray", "Black"};
    
    printf("Graph Coloring Solution:\n");
    for (int i = 0; i < numVertices; i++) {
        const char* colorName = (colors[i] < 11) ? colorNames[colors[i]] : "Color";
        printf("Vertex %d: %s (%d)\n", i, colorName, colors[i]);
    }
    printf("\n");
}

/**
 * Validate a coloring solution
 */
bool validateColoring(int graph[MAX_VERTICES][MAX_VERTICES], int colors[], int numVertices) {
    for (int i = 0; i < numVertices; i++) {
        for (int j = 0; j < numVertices; j++) {
            // If there's an edge and vertices have same color
            if (graph[i][j] == 1 && colors[i] == colors[j]) {
                return false;
            }
        }
    }
    return true;
}

/**
 * Create a triangle graph (3-clique)
 */
void createTriangleGraph(int graph[MAX_VERTICES][MAX_VERTICES]) {
    int triangle[3][3] = {
        {0, 1, 1},
        {1, 0, 1},
        {1, 1, 0}
    };
    
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            graph[i][j] = triangle[i][j];
        }
    }
}

/**
 * Create a square graph (4-cycle)
 */
void createSquareGraph(int graph[MAX_VERTICES][MAX_VERTICES]) {
    int square[4][4] = {
        {0, 1, 0, 1},
        {1, 0, 1, 0},
        {0, 1, 0, 1},
        {1, 0, 1, 0}
    };
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            graph[i][j] = square[i][j];
        }
    }
}

/**
 * Create a complete graph (K_n)
 */
void createCompleteGraph(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i != j) {
                graph[i][j] = 1;
            } else {
                graph[i][j] = 0;
            }
        }
    }
}

/**
 * Create a bipartite graph
 */
void createBipartiteGraph(int graph[MAX_VERTICES][MAX_VERTICES]) {
    int bipartite[6][6] = {
        {0, 0, 0, 1, 1, 1},
        {0, 0, 0, 1, 1, 1},
        {0, 0, 0, 1, 1, 1},
        {1, 1, 1, 0, 0, 0},
        {1, 1, 1, 0, 0, 0},
        {1, 1, 1, 0, 0, 0}
    };
    
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            graph[i][j] = bipartite[i][j];
        }
    }
}

/**
 * Create a wheel graph (hub connected to cycle)
 */
void createWheelGraph(int graph[MAX_VERTICES][MAX_VERTICES], int n) {
    // Initialize
    for (int i = 0; i <= n; i++) {
        for (int j = 0; j <= n; j++) {
            graph[i][j] = 0;
        }
    }
    
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
}

/**
 * Create a tree graph (no cycles)
 */
void createTreeGraph(int graph[MAX_VERTICES][MAX_VERTICES]) {
    int tree[7][7] = {
        {0, 1, 1, 0, 0, 0, 0},
        {1, 0, 0, 1, 1, 0, 0},
        {1, 0, 0, 0, 0, 1, 1},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0},
        {0, 0, 1, 0, 0, 0, 0}
    };
    
    for (int i = 0; i < 7; i++) {
        for (int j = 0; j < 7; j++) {
            graph[i][j] = tree[i][j];
        }
    }
}

/**
 * Get current time in milliseconds
 */
long long getCurrentTimeMillis() {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec * 1000LL + ts.tv_nsec / 1000000LL;
}

int main() {
    printf("=== Graph Coloring Problem - Backtracking ===\n\n");
    
    int graph[MAX_VERTICES][MAX_VERTICES];
    int colors[MAX_VERTICES];
    bool solved;
    
    // Test Case 1: Simple triangle (3-clique)
    printf("Test Case 1: Triangle Graph (3-clique)\n");
    createTriangleGraph(graph);
    printGraph(graph, 3);
    
    solved = solveGraphColoring(graph, 3, 3, colors);
    printColoring(colors, 3, solved);
    printf("Valid coloring: %s\n", validateColoring(graph, colors, 3) ? "true" : "false");
    
    int triangleChromatic = findChromaticNumber(graph, 3);
    printf("Chromatic number: %d\n", triangleChromatic);
    printf("Number of 3-colorings: %d\n\n", countColorings(graph, 3, 3));
    
    // Test Case 2: Square graph (4-cycle)
    printf("Test Case 2: Square Graph (4-cycle)\n");
    createSquareGraph(graph);
    printGraph(graph, 4);
    
    solved = solveGraphColoring(graph, 4, 2, colors);
    printColoring(colors, 4, solved);
    printf("Valid coloring: %s\n", validateColoring(graph, colors, 4) ? "true" : "false");
    
    int squareChromatic = findChromaticNumber(graph, 4);
    printf("Chromatic number: %d\n", squareChromatic);
    printf("Number of 2-colorings: %d\n\n", countColorings(graph, 4, 2));
    
    // Test Case 3: Step-by-step demonstration
    printf("Test Case 3: Step-by-step coloring (Triangle)\n");
    createTriangleGraph(graph);
    
    int stepCount;
    printf("Solution steps:\n");
    solveWithSteps(graph, 3, 3, colors, &stepCount);
    printf("\n");
    
    // Test Case 4: Complete graph K4
    printf("Test Case 4: Complete Graph K4\n");
    createCompleteGraph(graph, 4);
    printGraph(graph, 4);
    
    solved = solveGraphColoring(graph, 4, 4, colors);
    printColoring(colors, 4, solved);
    
    int k4Chromatic = findChromaticNumber(graph, 4);
    printf("Chromatic number: %d\n", k4Chromatic);
    printf("Expected: 4 (complete graph needs n colors)\n\n");
    
    // Test Case 5: Bipartite graph
    printf("Test Case 5: Bipartite Graph\n");
    createBipartiteGraph(graph);
    printGraph(graph, 6);
    
    solved = solveGraphColoring(graph, 6, 2, colors);
    printColoring(colors, 6, solved);
    
    int bipartiteChromatic = findChromaticNumber(graph, 6);
    printf("Chromatic number: %d\n", bipartiteChromatic);
    printf("Expected: 2 (bipartite graph needs exactly 2 colors)\n\n");
    
    // Test Case 6: Wheel graph
    printf("Test Case 6: Wheel Graph W5\n");
    createWheelGraph(graph, 5);
    printGraph(graph, 6);
    
    solved = solveGraphColoring(graph, 6, 4, colors);
    printColoring(colors, 6, solved);
    
    int wheelChromatic = findChromaticNumber(graph, 6);
    printf("Chromatic number: %d\n", wheelChromatic);
    printf("Expected: 3 or 4 depending on cycle length\n\n");
    
    // Test Case 7: Tree graph
    printf("Test Case 7: Tree Graph\n");
    createTreeGraph(graph);
    printGraph(graph, 7);
    
    solved = solveGraphColoring(graph, 7, 2, colors);
    printColoring(colors, 7, solved);
    
    int treeChromatic = findChromaticNumber(graph, 7);
    printf("Chromatic number: %d\n", treeChromatic);
    printf("Expected: 2 (trees are bipartite)\n\n");
    
    // Test Case 8: Performance analysis
    printf("Test Case 8: Performance Analysis\n");
    
    for (int n = 3; n <= 6; n++) {
        createCompleteGraph(graph, n);
        
        long long startTime = getCurrentTimeMillis();
        int chromatic = findChromaticNumber(graph, n);
        long long endTime = getCurrentTimeMillis();
        
        int colorings = countColorings(graph, n, n);
        
        printf("K%d: Chromatic number = %d, %d-colorings = %d (Time: %lldms)\n", 
               n, chromatic, n, colorings, endTime - startTime);
    }
    printf("\n");
    
    // Test Case 9: Edge cases
    printf("Test Case 9: Edge Cases\n");
    
    // Single vertex
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            graph[i][j] = 0;
        }
    }
    solved = solveGraphColoring(graph, 1, 1, colors);
    printf("Single vertex with 1 color: %s\n", solved ? "Possible" : "Impossible");
    
    // No edges (4 vertices)
    solved = solveGraphColoring(graph, 4, 1, colors);
    printf("4 vertices, no edges, 1 color: %s\n", solved ? "Possible" : "Impossible");
    
    // Impossible coloring
    createTriangleGraph(graph);
    solved = solveGraphColoring(graph, 3, 2, colors);
    printf("Triangle with 2 colors: %s\n\n", solved ? "Possible" : "Impossible");
    
    // Test Case 10: Applications
    printf("Test Case 10: Applications\n");
    printf("Graph coloring applications:\n");
    printf("- Map coloring: Color countries so no adjacent countries have same color\n");
    printf("- Register allocation: Assign CPU registers to variables\n");
    printf("- Scheduling: Assign time slots to tasks with conflicts\n");
    printf("- Frequency assignment: Assign radio frequencies to avoid interference\n");
    printf("- Sudoku solving: Each constraint is a clique that needs different values\n");
    
    printf("\nComplexity Analysis:\n");
    printf("- Time: O(k^V) where k is colors and V is vertices\n");
    printf("- Space: O(V) for recursion stack and color assignments\n");
    printf("- NP-Complete: Decision version is NP-Complete\n");
    printf("- Heuristics: Greedy coloring, Welsh-Powell algorithm\n");
    printf("- Bounds: χ(G) ≤ Δ(G) + 1 where Δ is max degree\n");
    
    return 0;
}