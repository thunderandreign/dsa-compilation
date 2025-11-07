#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Brute Force Strategy: Subset Sum Problem
 * Core Idea: Check all possible subsets to find one that sums to target
 * Time Complexity: O(2^n) where n is the number of elements
 * Space Complexity: O(2^n) for storing all subsets
 */

#define MAX_SIZE 20

/**
 * Print a subset represented by a bitmask
 * @param arr Input array
 * @param n Size of array
 * @param mask Bitmask representing the subset
 */
void printSubset(int arr[], int n, int mask) {
    printf("[");
    bool first = true;
    for (int i = 0; i < n; i++) {
        if ((mask & (1 << i)) != 0) {
            if (!first) printf(", ");
            printf("%d", arr[i]);
            first = false;
        }
    }
    printf("]");
}

/**
 * Calculate sum of elements in a subset represented by bitmask
 * @param arr Input array
 * @param n Size of array
 * @param mask Bitmask representing the subset
 * @return Sum of elements in the subset
 */
int calculateSubsetSum(int arr[], int n, int mask) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        if ((mask & (1 << i)) != 0) {
            sum += arr[i];
        }
    }
    return sum;
}

/**
 * Find all subsets that sum to the target value using brute force
 * @param arr Input array
 * @param n Size of array
 * @param target Target sum
 * @return Number of solutions found
 */
int findSubsetsWithSum(int arr[], int n, int target) {
    int solutionCount = 0;
    int totalSubsets = 1 << n; // 2^n subsets
    
    printf("All subsets that sum to %d:\n", target);
    
    // Generate all possible subsets using bit manipulation
    for (int mask = 0; mask < totalSubsets; mask++) {
        int sum = calculateSubsetSum(arr, n, mask);
        
        // If subset sum equals target, print it
        if (sum == target) {
            printf("Solution %d: ", ++solutionCount);
            printSubset(arr, n, mask);
            printf(" (sum = %d)\n", sum);
        }
    }
    
    return solutionCount;
}

/**
 * Check if there exists any subset that sums to target
 * @param arr Input array
 * @param n Size of array
 * @param target Target sum
 * @return true if such subset exists, false otherwise
 */
bool hasSubsetWithSum(int arr[], int n, int target) {
    int totalSubsets = 1 << n;
    
    for (int mask = 0; mask < totalSubsets; mask++) {
        int sum = calculateSubsetSum(arr, n, mask);
        if (sum == target) {
            return true;
        }
    }
    
    return false;
}

/**
 * Find the first subset that sums to target
 * @param arr Input array
 * @param n Size of array
 * @param target Target sum
 * @return Bitmask of first solution, or -1 if no solution
 */
int findFirstSubsetWithSum(int arr[], int n, int target) {
    int totalSubsets = 1 << n;
    
    for (int mask = 0; mask < totalSubsets; mask++) {
        int sum = calculateSubsetSum(arr, n, mask);
        if (sum == target) {
            return mask;
        }
    }
    
    return -1; // No solution found
}

/**
 * Helper function to print array
 */
void printArray(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) printf(", ");
    }
    printf("]");
}

int main() {
    printf("=== Subset Sum Problem - Brute Force ===\n");
    
    // Test Case 1: Multiple solutions exist
    int arr1[] = {3, 34, 4, 12, 5, 2};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int target1 = 9;
    printf("Test Case 1:\n");
    printf("Array: ");
    printArray(arr1, n1);
    printf("\nTarget sum: %d\n", target1);
    
    int solutions1 = findSubsetsWithSum(arr1, n1, target1);
    printf("Total solutions found: %d\n\n", solutions1);
    
    // Test Case 2: No solution exists
    int arr2[] = {1, 3, 5, 7};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int target2 = 2;
    printf("Test Case 2:\n");
    printf("Array: ");
    printArray(arr2, n2);
    printf("\nTarget sum: %d\n", target2);
    
    bool hasSolution2 = hasSubsetWithSum(arr2, n2, target2);
    printf("Has solution: %s\n\n", hasSolution2 ? "Yes" : "No");
    
    // Test Case 3: Target is 0 (empty subset)
    int arr3[] = {1, 2, 3, 4};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int target3 = 0;
    printf("Test Case 3:\n");
    printf("Array: ");
    printArray(arr3, n3);
    printf("\nTarget sum: %d\n", target3);
    
    int firstSolution3 = findFirstSubsetWithSum(arr3, n3, target3);
    if (firstSolution3 != -1) {
        printf("First solution: ");
        printSubset(arr3, n3, firstSolution3);
        printf(" (empty subset)\n");
    } else {
        printf("No solution found\n");
    }
    printf("\n");
    
    // Test Case 4: Single element array
    int arr4[] = {5};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int target4 = 5;
    printf("Test Case 4:\n");
    printf("Array: ");
    printArray(arr4, n4);
    printf("\nTarget sum: %d\n", target4);
    
    int solutions4 = findSubsetsWithSum(arr4, n4, target4);
    printf("Total solutions found: %d\n\n", solutions4);
    
    // Test Case 5: All elements sum to target
    int arr5[] = {2, 3, 4};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    int target5 = 9;
    printf("Test Case 5:\n");
    printf("Array: ");
    printArray(arr5, n5);
    printf("\nTarget sum: %d\n", target5);
    
    int firstSolution5 = findFirstSubsetWithSum(arr5, n5, target5);
    if (firstSolution5 != -1) {
        printf("First solution: ");
        printSubset(arr5, n5, firstSolution5);
        printf("\n");
    } else {
        printf("No solution found\n");
    }
    
    return 0;
}