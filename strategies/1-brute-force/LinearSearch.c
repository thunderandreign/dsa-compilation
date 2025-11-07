#include <stdio.h>
#include <stdlib.h>

/**
 * Brute Force Strategy: Linear Search
 * Core Idea: Check every element one by one until target is found or array ends
 * Time Complexity: O(n) where n is the number of elements
 * Space Complexity: O(1) - constant extra space
 */

/**
 * Search for a target element in an array using linear search
 * @param arr Array to search in
 * @param size Size of the array
 * @param target Element to search for
 * @return Index of target element if found, -1 otherwise
 */
int linearSearch(int arr[], int size, int target) {
    // Check each element one by one
    for (int i = 0; i < size; i++) {
        // If current element matches target, return its index
        if (arr[i] == target) {
            return i;
        }
    }
    // Target not found
    return -1;
}

/**
 * Search for all occurrences of a target element
 * @param arr Array to search in
 * @param size Size of the array
 * @param target Element to search for
 * @param indices Array to store found indices
 * @return Number of occurrences found
 */
int findAllOccurrences(int arr[], int size, int target, int indices[]) {
    int count = 0;
    
    // Check each element and store matching indices
    for (int i = 0; i < size; i++) {
        if (arr[i] == target) {
            indices[count] = i;
            count++;
        }
    }
    
    return count;
}

/**
 * Helper function to print array
 */
void printArray(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]");
}

int main() {
    printf("=== Linear Search - Brute Force ===\n");
    
    // Test Case 1: Target found in middle
    int arr1[] = {10, 25, 30, 45, 60, 75};
    int size1 = sizeof(arr1) / sizeof(arr1[0]);
    int target1 = 30;
    printf("Array: ");
    printArray(arr1, size1);
    printf("\nTarget: %d\n", target1);
    int result1 = linearSearch(arr1, size1, target1);
    printf("Result: %s\n", (result1 != -1) ? "Found" : "Not found");
    if (result1 != -1) printf("Index: %d\n", result1);
    printf("\n");
    
    // Test Case 2: Target not found
    int arr2[] = {5, 12, 18, 26, 33};
    int size2 = sizeof(arr2) / sizeof(arr2[0]);
    int target2 = 20;
    printf("Array: ");
    printArray(arr2, size2);
    printf("\nTarget: %d\n", target2);
    int result2 = linearSearch(arr2, size2, target2);
    printf("Result: %s\n", (result2 != -1) ? "Found" : "Not found");
    if (result2 != -1) printf("Index: %d\n", result2);
    printf("\n");
    
    // Test Case 3: Target at first position
    int arr3[] = {99, 42, 17, 8, 3};
    int size3 = sizeof(arr3) / sizeof(arr3[0]);
    int target3 = 99;
    printf("Array: ");
    printArray(arr3, size3);
    printf("\nTarget: %d\n", target3);
    int result3 = linearSearch(arr3, size3, target3);
    printf("Result: %s\n", (result3 != -1) ? "Found" : "Not found");
    if (result3 != -1) printf("Index: %d\n", result3);
    printf("\n");
    
    // Test Case 4: Multiple occurrences
    int arr4[] = {7, 3, 7, 1, 7, 9, 7};
    int size4 = sizeof(arr4) / sizeof(arr4[0]);
    int target4 = 7;
    int indices[size4]; // Maximum possible matches
    printf("Array: ");
    printArray(arr4, size4);
    printf("\nTarget: %d\n", target4);
    int count = findAllOccurrences(arr4, size4, target4, indices);
    printf("Found %d occurrences at indices: ", count);
    printArray(indices, count);
    printf("\n\n");
    
    // Test Case 5: Single element array
    int arr5[] = {42};
    int size5 = sizeof(arr5) / sizeof(arr5[0]);
    int target5 = 42;
    printf("Array: ");
    printArray(arr5, size5);
    printf("\nTarget: %d\n", target5);
    int result5 = linearSearch(arr5, size5, target5);
    printf("Result: %s\n", (result5 != -1) ? "Found" : "Not found");
    if (result5 != -1) printf("Index: %d\n", result5);
    
    return 0;
}