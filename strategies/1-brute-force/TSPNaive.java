/**
 * Brute Force Strategy: Traveling Salesman Problem (TSP) - Naive Approach
 * Core Idea: Try all possible permutations of cities to find the shortest route
 * Time Complexity: O(n!) where n is the number of cities
 * Space Complexity: O(n) for recursion stack
 */
public class TSPNaive {
    
    /**
     * Calculate the total distance for a given route
     * @param graph Adjacency matrix representing distances between cities
     * @param route Array representing the order of cities to visit
     * @return Total distance of the route
     */
    public static int calculateRouteDistance(int[][] graph, int[] route) {
        int totalDistance = 0;
        int n = route.length;
        
        // Calculate distance from each city to the next
        for (int i = 0; i < n - 1; i++) {
            totalDistance += graph[route[i]][route[i + 1]];
        }
        
        // Add distance from last city back to starting city
        totalDistance += graph[route[n - 1]][route[0]];
        
        return totalDistance;
    }
    
    /**
     * Solve TSP using brute force approach
     * @param graph Adjacency matrix representing distances between cities
     * @return Minimum distance for the shortest route
     */
    public static TSPResult solveTSP(int[][] graph) {
        int n = graph.length;
        if (n <= 1) return new TSPResult(0, new int[]{0});
        
        // Create initial route starting from city 0
        int[] cities = new int[n];
        for (int i = 0; i < n; i++) {
            cities[i] = i;
        }
        
        int minDistance = Integer.MAX_VALUE;
        int[] bestRoute = new int[n];
        
        // Try all possible permutations of cities (except starting city)
        do {
            int currentDistance = calculateRouteDistance(graph, cities);
            if (currentDistance < minDistance) {
                minDistance = currentDistance;
                System.arraycopy(cities, 0, bestRoute, 0, n);
            }
        } while (nextPermutation(cities, 1)); // Start from index 1 to keep city 0 as start
        
        return new TSPResult(minDistance, bestRoute);
    }
    
    /**
     * Generate next lexicographic permutation
     * @param arr Array to permute
     * @param start Starting index for permutation
     * @return true if next permutation exists, false otherwise
     */
    private static boolean nextPermutation(int[] arr, int start) {
        // Find the largest index i such that arr[i] < arr[i + 1]
        int i = arr.length - 2;
        while (i >= start && arr[i] >= arr[i + 1]) {
            i--;
        }
        
        if (i < start) return false; // No more permutations
        
        // Find the largest index j such that arr[i] < arr[j]
        int j = arr.length - 1;
        while (arr[j] <= arr[i]) {
            j--;
        }
        
        // Swap arr[i] and arr[j]
        swap(arr, i, j);
        
        // Reverse the suffix starting at arr[i + 1]
        reverse(arr, i + 1);
        
        return true;
    }
    
    private static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    
    private static void reverse(int[] arr, int start) {
        int end = arr.length - 1;
        while (start < end) {
            swap(arr, start, end);
            start++;
            end--;
        }
    }
    
    /**
     * Helper class to store TSP result
     */
    static class TSPResult {
        int minDistance;
        int[] bestRoute;
        
        TSPResult(int distance, int[] route) {
            this.minDistance = distance;
            this.bestRoute = route;
        }
    }
    
    /**
     * Helper method to print route
     */
    public static void printRoute(int[] route) {
        System.out.print("Route: ");
        for (int i = 0; i < route.length; i++) {
            System.out.print(route[i]);
            if (i < route.length - 1) System.out.print(" -> ");
        }
        System.out.println(" -> " + route[0]); // Return to start
    }
    
    public static void main(String[] args) {
        System.out.println("=== Traveling Salesman Problem (TSP) - Brute Force ===");
        
        // Test Case 1: Simple 4-city problem
        System.out.println("Test Case 1: 4 cities");
        int[][] graph1 = {
            {0, 10, 15, 20},
            {10, 0, 35, 25},
            {15, 35, 0, 30},
            {20, 25, 30, 0}
        };
        
        System.out.println("Distance matrix:");
        printMatrix(graph1);
        
        TSPResult result1 = solveTSP(graph1);
        System.out.println("Minimum distance: " + result1.minDistance);
        printRoute(result1.bestRoute);
        System.out.println();
        
        // Test Case 2: 3-city triangle
        System.out.println("Test Case 2: 3 cities");
        int[][] graph2 = {
            {0, 5, 8},
            {5, 0, 3},
            {8, 3, 0}
        };
        
        System.out.println("Distance matrix:");
        printMatrix(graph2);
        
        TSPResult result2 = solveTSP(graph2);
        System.out.println("Minimum distance: " + result2.minDistance);
        printRoute(result2.bestRoute);
        System.out.println();
        
        // Test Case 3: 2-city problem (trivial)
        System.out.println("Test Case 3: 2 cities");
        int[][] graph3 = {
            {0, 7},
            {7, 0}
        };
        
        System.out.println("Distance matrix:");
        printMatrix(graph3);
        
        TSPResult result3 = solveTSP(graph3);
        System.out.println("Minimum distance: " + result3.minDistance);
        printRoute(result3.bestRoute);
    }
    
    private static void printMatrix(int[][] matrix) {
        for (int i = 0; i < matrix.length; i++) {
            for (int j = 0; j < matrix[i].length; j++) {
                System.out.printf("%3d ", matrix[i][j]);
            }
            System.out.println();
        }
    }
}