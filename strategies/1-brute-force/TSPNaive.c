#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

/**
 * Brute Force Strategy: Traveling Salesman Problem (TSP) - Naive Approach
 * Core Idea: Try all possible permutations of cities to find the shortest route
 * Time Complexity: O(n!) where n is the number of cities
 * Space Complexity: O(n) for recursion stack
 */

#define MAX_CITIES 10

/**
 * Calculate the total distance for a given route
 * @param graph Adjacency matrix representing distances between cities
 * @param route Array representing the order of cities to visit
 * @param n Number of cities
 * @return Total distance of the route
 */
int calculateRouteDistance(int graph[][MAX_CITIES], int route[], int n) {
    int totalDistance = 0;
    
    // Calculate distance from each city to the next
    for (int i = 0; i < n - 1; i++) {
        totalDistance += graph[route[i]][route[i + 1]];
    }
    
    // Add distance from last city back to starting city
    totalDistance += graph[route[n - 1]][route[0]];
    
    return totalDistance;
}

/**
 * Generate all permutations and find minimum distance
 * @param graph Distance matrix
 * @param route Current route being built
 * @param visited Array to track visited cities
 * @param n Number of cities
 * @param level Current level in recursion
 * @param minDistance Pointer to minimum distance found so far
 * @param bestRoute Array to store the best route
 */
void tspBruteForce(int graph[][MAX_CITIES], int route[], int visited[], 
                   int n, int level, int* minDistance, int bestRoute[]) {
    // Base case: all cities visited
    if (level == n) {
        int currentDistance = calculateRouteDistance(graph, route, n);
        if (currentDistance < *minDistance) {
            *minDistance = currentDistance;
            // Copy current route to best route
            for (int i = 0; i < n; i++) {
                bestRoute[i] = route[i];
            }
        }
        return;
    }
    
    // Try all unvisited cities at current level
    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            visited[i] = 1;
            route[level] = i;
            
            // Recurse to next level
            tspBruteForce(graph, route, visited, n, level + 1, minDistance, bestRoute);
            
            // Backtrack
            visited[i] = 0;
        }
    }
}

/**
 * Solve TSP using brute force approach
 * @param graph Adjacency matrix representing distances between cities
 * @param n Number of cities
 * @param bestRoute Array to store the optimal route
 * @return Minimum distance for the shortest route
 */
int solveTSP(int graph[][MAX_CITIES], int n, int bestRoute[]) {
    if (n <= 1) {
        if (n == 1) bestRoute[0] = 0;
        return 0;
    }
    
    int route[MAX_CITIES];
    int visited[MAX_CITIES] = {0};
    int minDistance = INT_MAX;
    
    // Start from city 0
    visited[0] = 1;
    route[0] = 0;
    
    // Find optimal route
    tspBruteForce(graph, route, visited, n, 1, &minDistance, bestRoute);
    
    return minDistance;
}

/**
 * Helper function to print route
 */
void printRoute(int route[], int n) {
    printf("Route: ");
    for (int i = 0; i < n; i++) {
        printf("%d", route[i]);
        if (i < n - 1) printf(" -> ");
    }
    printf(" -> %d\n", route[0]); // Return to start
}

/**
 * Helper function to print distance matrix
 */
void printMatrix(int graph[][MAX_CITIES], int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", graph[i][j]);
        }
        printf("\n");
    }
}

int main() {
    printf("=== Traveling Salesman Problem (TSP) - Brute Force ===\n");
    
    // Test Case 1: Simple 4-city problem
    printf("Test Case 1: 4 cities\n");
    int graph1[MAX_CITIES][MAX_CITIES] = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    int n1 = 4;
    int bestRoute1[MAX_CITIES];
    
    printf("Distance matrix:\n");
    printMatrix(graph1, n1);
    
    int minDistance1 = solveTSP(graph1, n1, bestRoute1);
    printf("Minimum distance: %d\n", minDistance1);
    printRoute(bestRoute1, n1);
    printf("\n");
    
    // Test Case 2: 3-city triangle
    printf("Test Case 2: 3 cities\n");
    int graph2[MAX_CITIES][MAX_CITIES] = {
        {0, 5, 8},
        {5, 0, 3},
        {8, 3, 0}
    };
    int n2 = 3;
    int bestRoute2[MAX_CITIES];
    
    printf("Distance matrix:\n");
    printMatrix(graph2, n2);
    
    int minDistance2 = solveTSP(graph2, n2, bestRoute2);
    printf("Minimum distance: %d\n", minDistance2);
    printRoute(bestRoute2, n2);
    printf("\n");
    
    // Test Case 3: 2-city problem (trivial)
    printf("Test Case 3: 2 cities\n");
    int graph3[MAX_CITIES][MAX_CITIES] = {
        {0, 7},
        {7, 0}
    };
    int n3 = 2;
    int bestRoute3[MAX_CITIES];
    
    printf("Distance matrix:\n");
    printMatrix(graph3, n3);
    
    int minDistance3 = solveTSP(graph3, n3, bestRoute3);
    printf("Minimum distance: %d\n", minDistance3);
    printRoute(bestRoute3, n3);
    printf("\n");
    
    // Test Case 4: Single city
    printf("Test Case 4: 1 city\n");
    int graph4[MAX_CITIES][MAX_CITIES] = {{0}};
    int n4 = 1;
    int bestRoute4[MAX_CITIES];
    
    printf("Distance matrix:\n");
    printMatrix(graph4, n4);
    
    int minDistance4 = solveTSP(graph4, n4, bestRoute4);
    printf("Minimum distance: %d\n", minDistance4);
    printRoute(bestRoute4, n4);
    
    return 0;
}