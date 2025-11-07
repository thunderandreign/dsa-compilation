#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Divide and Conquer Strategy: Merge Sort
 * Core Idea: Divide array into halves, sort each half, then merge sorted halves
 * Time Complexity: O(n log n) in all cases
 * Space Complexity: O(n) for temporary arrays during merge
 */

/**
 * Merge two sorted subarrays into one sorted array
 * @param arr Original array
 * @param temp Temporary array
 * @param left Start of left subarray
 * @param mid End of left subarray (start of right is mid+1)
 * @param right End of right subarray
 */
void merge(int arr[], int temp[], int left, int mid, int right) {
    // Copy data to temporary array
    for (int i = left; i <= right; i++) {
        temp[i] = arr[i];
    }
    
    // Merge the two sorted subarrays back into arr[left..right]
    int i = left;      // Initial index of left subarray
    int j = mid + 1;   // Initial index of right subarray
    int k = left;      // Initial index of merged subarray
    
    // Compare elements from both subarrays and merge in sorted order
    while (i <= mid && j <= right) {
        if (temp[i] <= temp[j]) {
            arr[k] = temp[i];
            i++;
        } else {
            arr[k] = temp[j];
            j++;
        }
        k++;
    }
    
    // Copy remaining elements of left subarray, if any
    while (i <= mid) {
        arr[k] = temp[i];
        i++;
        k++;
    }
    
    // Copy remaining elements of right subarray, if any
    while (j <= right) {
        arr[k] = temp[j];
        j++;
        k++;
    }
}

/**
 * Recursive helper method for merge sort
 * @param arr Original array
 * @param temp Temporary array for merging
 * @param left Left boundary of current subarray
 * @param right Right boundary of current subarray
 */
void mergeSortHelper(int arr[], int temp[], int left, int right) {
    // Base case: single element or empty subarray
    if (left >= right) {
        return;
    }
    
    // Divide: find the middle point
    int mid = left + (right - left) / 2;
    
    // Conquer: recursively sort both halves
    mergeSortHelper(arr, temp, left, mid);      // Sort left half
    mergeSortHelper(arr, temp, mid + 1, right); // Sort right half
    
    // Combine: merge the sorted halves
    merge(arr, temp, left, mid, right);
}

/**
 * Sort an array using merge sort algorithm
 * @param arr Array to be sorted
 * @param n Size of the array
 */
void mergeSort(int arr[], int n) {
    if (arr == NULL || n <= 1) {
        return; // Base case: array is already sorted
    }
    
    // Create a temporary array for merging
    int* temp = (int*)malloc(n * sizeof(int));
    if (temp == NULL) {
        printf("Memory allocation failed\n");
        return;
    }
    
    mergeSortHelper(arr, temp, 0, n - 1);
    
    // Free temporary array
    free(temp);
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

int main() {
    printf("=== Merge Sort - Divide and Conquer ===\n");
    
    // Test Case 1: Random unsorted array
    int arr1[] = {38, 27, 43, 3, 9, 82, 10};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    int sorted1[n1];
    copyArray(arr1, sorted1, n1);
    
    printf("Test Case 1: Random array\n");
    printf("Before: ");
    printArray(arr1, n1);
    
    mergeSort(sorted1, n1);
    printf("After:  ");
    printArray(sorted1, n1);
    printf("\n");
    
    // Test Case 2: Already sorted array
    int arr2[] = {1, 2, 3, 4, 5, 6};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    int sorted2[n2];
    copyArray(arr2, sorted2, n2);
    
    printf("Test Case 2: Already sorted\n");
    printf("Before: ");
    printArray(arr2, n2);
    
    mergeSort(sorted2, n2);
    printf("After:  ");
    printArray(sorted2, n2);
    printf("\n");
    
    // Test Case 3: Reverse sorted array
    int arr3[] = {6, 5, 4, 3, 2, 1};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    int sorted3[n3];
    copyArray(arr3, sorted3, n3);
    
    printf("Test Case 3: Reverse sorted\n");
    printf("Before: ");
    printArray(arr3, n3);
    
    mergeSort(sorted3, n3);
    printf("After:  ");
    printArray(sorted3, n3);
    printf("\n");
    
    // Test Case 4: Array with duplicates
    int arr4[] = {5, 2, 8, 2, 9, 5, 1, 8};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    int sorted4[n4];
    copyArray(arr4, sorted4, n4);
    
    printf("Test Case 4: With duplicates\n");
    printf("Before: ");
    printArray(arr4, n4);
    
    mergeSort(sorted4, n4);
    printf("After:  ");
    printArray(sorted4, n4);
    printf("\n");
    
    // Test Case 5: Single element and two elements
    int arr5[] = {42};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);
    int sorted5[n5];
    copyArray(arr5, sorted5, n5);
    
    int arr6[] = {10, 5};
    int n6 = sizeof(arr6) / sizeof(arr6[0]);
    int sorted6[n6];
    copyArray(arr6, sorted6, n6);
    
    printf("Test Case 5: Edge cases\n");
    printf("Single element before: ");
    printArray(arr5, n5);
    mergeSort(sorted5, n5);
    printf("Single element after:  ");
    printArray(sorted5, n5);
    
    printf("Two elements before: ");
    printArray(arr6, n6);
    mergeSort(sorted6, n6);
    printf("Two elements after:  ");
    printArray(sorted6, n6);
    
    return 0;
}