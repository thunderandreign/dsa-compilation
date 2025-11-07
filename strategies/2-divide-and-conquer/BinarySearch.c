#include <stdio.h>
#include <stdlib.h>

/**
 * Divide and Conquer Strategy: Binary Search
 * Core Idea: Divide search space in half by comparing with middle element
 * Time Complexity: O(log n) where n is the number of elements
 * Space Complexity: O(1) iterative, O(log n) recursive
 */

/**
 * Iterative binary search implementation
 * @param arr Sorted array to search in
 * @param n Size of the array
 * @param target Element to search for
 * @return Index of target element if found, -1 otherwise
 */
int binarySearchIterative(int arr[], int n, int target) {
    if (arr == NULL || n == 0) {
        return -1;
    }
    
    int left = 0;
    int right = n - 1;
    
    while (left <= right) {
        // Find middle index (avoid overflow)
        int mid = left + (right - left) / 2;
        
        // Check if target is at mid
        if (arr[mid] == target) {
            return mid;
        }
        
        // If target is greater, ignore left half
        if (arr[mid] < target) {
            left = mid + 1;
        }
        // If target is smaller, ignore right half
        else {
            right = mid - 1;
        }
    }
    
    // Target not found
    return -1;
}

/**
 * Recursive helper method for binary search
 * @param arr Sorted array
 * @param target Target element
 * @param left Left boundary
 * @param right Right boundary
 * @return Index of target if found, -1 otherwise
 */
int binarySearchRecursiveHelper(int arr[], int target, int left, int right) {
    // Base case: element not found
    if (left > right) {
        return -1;
    }
    
    // Divide: find middle point
    int mid = left + (right - left) / 2;
    
    // Base case: element found
    if (arr[mid] == target) {
        return mid;
    }
    
    // Conquer: search in appropriate half
    if (arr[mid] > target) {
        // Search in left half
        return binarySearchRecursiveHelper(arr, target, left, mid - 1);
    } else {
        // Search in right half
        return binarySearchRecursiveHelper(arr, target, mid + 1, right);
    }
}

/**
 * Recursive binary search implementation
 * @param arr Sorted array to search in
 * @param n Size of the array
 * @param target Element to search for
 * @return Index of target element if found, -1 otherwise
 */
int binarySearchRecursive(int arr[], int n, int target) {
    if (arr == NULL || n == 0) {
        return -1;
    }
    
    return binarySearchRecursiveHelper(arr, target, 0, n - 1);
}

/**
 * Find the first occurrence of target in array with duplicates
 * @param arr Sorted array (may contain duplicates)
 * @param n Size of the array
 * @param target Element to find first occurrence of
 * @return Index of first occurrence, -1 if not found
 */
int findFirstOccurrence(int arr[], int n, int target) {
    if (arr == NULL || n == 0) {
        return -1;
    }
    
    int left = 0;
    int right = n - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            result = mid; // Store potential result
            right = mid - 1; // Continue searching in left half
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
}

/**
 * Find the last occurrence of target in array with duplicates
 * @param arr Sorted array (may contain duplicates)
 * @param n Size of the array
 * @param target Element to find last occurrence of
 * @return Index of last occurrence, -1 if not found
 */
int findLastOccurrence(int arr[], int n, int target) {
    if (arr == NULL || n == 0) {
        return -1;
    }
    
    int left = 0;
    int right = n - 1;
    int result = -1;
    
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (arr[mid] == target) {
            result = mid; // Store potential result
            left = mid + 1; // Continue searching in right half
        } else if (arr[mid] < target) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }
    
    return result;
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
    printf("=== Binary Search - Divide and Conquer ===\n");
    
    // Test Case 1: Standard binary search
    int arr1[] = {2, 3, 4, 10, 40, 50, 80};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int target1 = 10;
    printf("Array: ");
    printArray(arr1, n1);
    printf("\nTarget: %d\n", target1);
    
    int resultIter = binarySearchIterative(arr1, n1, target1);
    int resultRec = binarySearchRecursive(arr1, n1, target1);
    
    printf("Iterative result: %s", (resultIter != -1) ? "Found" : "Not found");
    if (resultIter != -1) printf(" at index %d", resultIter);
    printf("\n");
    
    printf("Recursive result: %s", (resultRec != -1) ? "Found" : "Not found");
    if (resultRec != -1) printf(" at index %d", resultRec);
    printf("\n\n");
    
    // Test Case 2: Target not found
    int target2 = 25;
    printf("Array: ");
    printArray(arr1, n1);
    printf("\nTarget: %d\n", target2);
    
    int result2 = binarySearchIterative(arr1, n1, target2);
    printf("Result: %s\n", (result2 != -1) ? "Found" : "Not found");
    printf("\n");
    
    // Test Case 3: Target at boundaries
    printf("Test Case 3: Boundary elements\n");
    printf("Array: ");
    printArray(arr1, n1);
    printf("\n");
    
    int firstResult = binarySearchIterative(arr1, n1, 2);
    int lastResult = binarySearchIterative(arr1, n1, 80);
    
    printf("Searching for first element (2): %s", (firstResult != -1) ? "Found" : "Not found");
    if (firstResult != -1) printf(" at index %d", firstResult);
    printf("\n");
    
    printf("Searching for last element (80): %s", (lastResult != -1) ? "Found" : "Not found");
    if (lastResult != -1) printf(" at index %d", lastResult);
    printf("\n\n");
    
    // Test Case 4: Array with duplicates
    int arr4[] = {1, 2, 2, 2, 3, 4, 4, 5};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int target4 = 2;
    printf("Test Case 4: Array with duplicates\n");
    printf("Array: ");
    printArray(arr4, n4);
    printf("\nTarget: %d\n", target4);
    
    int anyOccurrence = binarySearchIterative(arr4, n4, target4);
    int firstOccurrence = findFirstOccurrence(arr4, n4, target4);
    int lastOccurrence = findLastOccurrence(arr4, n4, target4);
    
    printf("Any occurrence: %s", (anyOccurrence != -1) ? "Found" : "Not found");
    if (anyOccurrence != -1) printf(" at index %d", anyOccurrence);
    printf("\n");
    
    printf("First occurrence: %s", (firstOccurrence != -1) ? "Found" : "Not found");
    if (firstOccurrence != -1) printf(" at index %d", firstOccurrence);
    printf("\n");
    
    printf("Last occurrence: %s", (lastOccurrence != -1) ? "Found" : "Not found");
    if (lastOccurrence != -1) printf(" at index %d", lastOccurrence);
    printf("\n\n");
    
    // Test Case 5: Edge cases
    printf("Test Case 5: Edge cases\n");
    
    // Single element array
    int singleArr[] = {5};
    int n5 = 1;
    printf("Single element array [5], searching for 5: %s\n",
           (binarySearchIterative(singleArr, n5, 5) != -1) ? "Found" : "Not found");
    printf("Single element array [5], searching for 3: %s\n",
           (binarySearchIterative(singleArr, n5, 3) != -1) ? "Found" : "Not found");
    
    // Two element array
    int twoArr[] = {1, 3};
    int n6 = 2;
    int result6a = binarySearchIterative(twoArr, n6, 1);
    int result6b = binarySearchIterative(twoArr, n6, 3);
    
    printf("Two element array [1, 3], searching for 1: %s", 
           (result6a != -1) ? "Found" : "Not found");
    if (result6a != -1) printf(" at index %d", result6a);
    printf("\n");
    
    printf("Two element array [1, 3], searching for 3: %s", 
           (result6b != -1) ? "Found" : "Not found");
    if (result6b != -1) printf(" at index %d", result6b);
    printf("\n");
    
    return 0;
}