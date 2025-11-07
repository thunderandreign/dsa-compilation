#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/**
 * Divide and Conquer Strategy: Quick Sort
 * Core Idea: Choose a pivot, partition array around pivot, then sort partitions
 * Time Complexity: O(n log n) average case, O(n²) worst case
 * Space Complexity: O(log n) average case for recursion stack
 */

/**
 * Swap two elements in an array
 */
void swap(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

/**
 * Partition the array around a pivot element
 * @param arr Array to partition
 * @param low Starting index
 * @param high Ending index
 * @return Index of the pivot after partitioning
 */
int partition(int arr[], int low, int high) {
    // Choose the rightmost element as pivot
    int pivot = arr[high];
    
    // Index of smaller element, indicates right position of pivot
    int i = low - 1;
    
    for (int j = low; j < high; j++) {
        // If current element is smaller than or equal to pivot
        if (arr[j] <= pivot) {
            i++; // Increment index of smaller element
            swap(arr, i, j);
        }
    }
    
    // Place pivot in correct position
    swap(arr, i + 1, high);
    return i + 1;
}

/**
 * Recursive helper method for quick sort
 * @param arr Array to sort
 * @param low Starting index
 * @param high Ending index
 */
void quickSortHelper(int arr[], int low, int high) {
    // Base case: if low >= high, subarray has 0 or 1 element
    if (low < high) {
        // Divide: partition the array and get pivot index
        int pivotIndex = partition(arr, low, high);
        
        // Conquer: recursively sort elements before and after partition
        quickSortHelper(arr, low, pivotIndex - 1);     // Sort left partition
        quickSortHelper(arr, pivotIndex + 1, high);    // Sort right partition
    }
}

/**
 * Sort an array using quick sort algorithm
 * @param arr Array to be sorted
 * @param n Size of the array
 */
void quickSort(int arr[], int n) {
    if (arr == NULL || n <= 1) {
        return; // Base case: array is already sorted
    }
    
    quickSortHelper(arr, 0, n - 1);
}

/**
 * Randomized partition for better average case performance
 */
int randomizedPartition(int arr[], int low, int high) {
    // Randomly choose pivot and swap with last element
    int randomIndex = low + rand() % (high - low + 1);
    swap(arr, randomIndex, high);
    
    return partition(arr, low, high);
}

/**
 * Helper for randomized quick sort
 */
void randomizedQuickSortHelper(int arr[], int low, int high) {
    if (low < high) {
        // Partition and recurse
        int pivotIndex = randomizedPartition(arr, low, high);
        randomizedQuickSortHelper(arr, low, pivotIndex - 1);
        randomizedQuickSortHelper(arr, pivotIndex + 1, high);
    }
}

/**
 * Randomized version of quick sort (better average performance)
 * @param arr Array to be sorted
 * @param n Size of the array
 */
void randomizedQuickSort(int arr[], int n) {
    if (arr == NULL || n <= 1) {
        return;
    }
    
    randomizedQuickSortHelper(arr, 0, n - 1);
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
    printf("]\n");
}

/**
 * Helper function to copy array
 */
void copyArray(int source[], int dest[], int n) {
    for (int i = 0; i < n; i++) {
        dest[i] = source[i];
    }
}

/**
 * Helper function to verify if array is sorted
 */
int isSorted(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        if (arr[i] > arr[i + 1]) {
            return 0; // Not sorted
        }
    }
    return 1; // Sorted
}

int main() {
    srand(time(NULL)); // Seed random number generator
    
    printf("=== Quick Sort - Divide and Conquer ===\n");
    
    // Test Case 1: Random unsorted array
    int arr1[] = {10, 7, 8, 9, 1, 5};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int sorted1[n1];
    copyArray(arr1, sorted1, n1);
    
    printf("Test Case 1: Random array (standard quicksort)\n");
    printf("Before: ");
    printArray(arr1, n1);
    
    quickSort(sorted1, n1);
    printf("After:  ");
    printArray(sorted1, n1);
    printf("\n");
    
    // Test Case 2: Same array with randomized quicksort
    int sorted2[n1];
    copyArray(arr1, sorted2, n1);
    
    printf("Test Case 2: Same array (randomized quicksort)\n");
    printf("Before: ");
    printArray(arr1, n1);
    
    randomizedQuickSort(sorted2, n1);
    printf("After:  ");
    printArray(sorted2, n1);
    printf("\n");
    
    // Test Case 3: Already sorted array (worst case for standard quicksort)
    int arr3[] = {1, 2, 3, 4, 5, 6};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int sorted3[n3];
    copyArray(arr3, sorted3, n3);
    
    printf("Test Case 3: Already sorted (worst case)\n");
    printf("Before: ");
    printArray(arr3, n3);
    
    quickSort(sorted3, n3);
    printf("After:  ");
    printArray(sorted3, n3);
    printf("\n");
    
    // Test Case 4: Array with duplicates
    int arr4[] = {4, 1, 3, 4, 2, 4, 1};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int sorted4[n4];
    copyArray(arr4, sorted4, n4);
    
    printf("Test Case 4: With duplicates\n");
    printf("Before: ");
    printArray(arr4, n4);
    
    quickSort(sorted4, n4);
    printf("After:  ");
    printArray(sorted4, n4);
    printf("\n");
    
    // Test Case 5: Performance comparison with small array
    printf("Test Case 5: Performance comparison\n");
    int largeSize = 100;
    int* largeArr = (int*)malloc(largeSize * sizeof(int));
    int* copy1 = (int*)malloc(largeSize * sizeof(int));
    int* copy2 = (int*)malloc(largeSize * sizeof(int));
    
    // Fill with random data
    for (int i = 0; i < largeSize; i++) {
        largeArr[i] = rand() % 1000;
    }
    
    copyArray(largeArr, copy1, largeSize);
    copyArray(largeArr, copy2, largeSize);
    
    // Time standard quicksort
    clock_t start1 = clock();
    quickSort(copy1, largeSize);
    clock_t end1 = clock();
    
    // Time randomized quicksort
    clock_t start2 = clock();
    randomizedQuickSort(copy2, largeSize);
    clock_t end2 = clock();
    
    printf("Array size: %d\n", largeSize);
    printf("Standard quicksort time: %ld microseconds\n", 
           (end1 - start1) * 1000000 / CLOCKS_PER_SEC);
    printf("Randomized quicksort time: %ld microseconds\n", 
           (end2 - start2) * 1000000 / CLOCKS_PER_SEC);
    
    // Verify both are sorted correctly
    printf("Standard quicksort correct: %s\n", isSorted(copy1, largeSize) ? "Yes" : "No");
    printf("Randomized quicksort correct: %s\n", isSorted(copy2, largeSize) ? "Yes" : "No");
    
    // Clean up
    free(largeArr);
    free(copy1);
    free(copy2);
    
    return 0;
}