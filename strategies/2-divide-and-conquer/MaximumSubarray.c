#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>

/**
 * Divide and Conquer Strategy: Maximum Subarray Sum (Kadane's Algorithm using D&C)
 * Core Idea: Divide array into halves and find max sum in left, right, or crossing middle
 * Time Complexity: O(n log n) using divide and conquer, O(n) with Kadane's algorithm
 * Space Complexity: O(log n) for recursion stack
 */

/**
 * Structure to store subarray information
 */
typedef struct {
    int maxSum;
    int startIndex;
    int endIndex;
} SubarrayResult;

/**
 * Find maximum subarray that crosses the middle point
 * @param arr Input array
 * @param low Left boundary
 * @param mid Middle point
 * @param high Right boundary
 * @return SubarrayResult for crossing subarray
 */
SubarrayResult maxCrossingSubarray(int arr[], int low, int mid, int high) {
    // Find max sum for left half ending at mid
    int leftSum = INT_MIN;
    int sum = 0;
    int maxLeft = mid;
    
    for (int i = mid; i >= low; i--) {
        sum += arr[i];
        if (sum > leftSum) {
            leftSum = sum;
            maxLeft = i;
        }
    }
    
    // Find max sum for right half starting at mid+1
    int rightSum = INT_MIN;
    sum = 0;
    int maxRight = mid + 1;
    
    for (int i = mid + 1; i <= high; i++) {
        sum += arr[i];
        if (sum > rightSum) {
            rightSum = sum;
            maxRight = i;
        }
    }
    
    SubarrayResult result;
    result.maxSum = leftSum + rightSum;
    result.startIndex = maxLeft;
    result.endIndex = maxRight;
    return result;
}

/**
 * Recursive helper for divide and conquer approach
 * @param arr Input array
 * @param low Left boundary
 * @param high Right boundary
 * @return SubarrayResult for the range [low, high]
 */
SubarrayResult maxSubarrayHelper(int arr[], int low, int high) {
    // Base case: single element
    if (low == high) {
        SubarrayResult result;
        result.maxSum = arr[low];
        result.startIndex = low;
        result.endIndex = high;
        return result;
    }
    
    // Divide: find middle point
    int mid = low + (high - low) / 2;
    
    // Conquer: find maximum subarray in left and right halves
    SubarrayResult leftResult = maxSubarrayHelper(arr, low, mid);
    SubarrayResult rightResult = maxSubarrayHelper(arr, mid + 1, high);
    
    // Combine: find maximum subarray crossing the middle
    SubarrayResult crossResult = maxCrossingSubarray(arr, low, mid, high);
    
    // Return the maximum of the three
    if (leftResult.maxSum >= rightResult.maxSum && leftResult.maxSum >= crossResult.maxSum) {
        return leftResult;
    } else if (rightResult.maxSum >= leftResult.maxSum && rightResult.maxSum >= crossResult.maxSum) {
        return rightResult;
    } else {
        return crossResult;
    }
}

/**
 * Find maximum subarray sum using divide and conquer approach
 * @param arr Input array
 * @param n Size of array
 * @return SubarrayResult containing max sum and indices
 */
SubarrayResult maxSubarrayDivideConquer(int arr[], int n) {
    SubarrayResult emptyResult = {0, -1, -1};
    if (arr == NULL || n == 0) {
        return emptyResult;
    }
    
    return maxSubarrayHelper(arr, 0, n - 1);
}

/**
 * Find maximum subarray sum using Kadane's algorithm (more efficient)
 * @param arr Input array
 * @param n Size of array
 * @return SubarrayResult containing max sum and indices
 */
SubarrayResult maxSubarrayKadane(int arr[], int n) {
    SubarrayResult emptyResult = {0, -1, -1};
    if (arr == NULL || n == 0) {
        return emptyResult;
    }
    
    int maxSum = arr[0];
    int currentSum = arr[0];
    int start = 0, end = 0, tempStart = 0;
    
    for (int i = 1; i < n; i++) {
        // If current sum becomes negative, start new subarray
        if (currentSum < 0) {
            currentSum = arr[i];
            tempStart = i;
        } else {
            currentSum += arr[i];
        }
        
        // Update maximum sum and indices if current sum is greater
        if (currentSum > maxSum) {
            maxSum = currentSum;
            start = tempStart;
            end = i;
        }
    }
    
    SubarrayResult result;
    result.maxSum = maxSum;
    result.startIndex = start;
    result.endIndex = end;
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

/**
 * Helper function to print subarray
 */
void printSubarray(int arr[], int start, int end) {
    if (start == -1 || end == -1) {
        printf("[]");
        return;
    }
    
    printf("[");
    for (int i = start; i <= end; i++) {
        printf("%d", arr[i]);
        if (i < end) printf(", ");
    }
    printf("]");
}

/**
 * Helper function to print result
 */
void printResult(SubarrayResult result) {
    printf("Sum: %d, Indices: [%d, %d]", result.maxSum, result.startIndex, result.endIndex);
}

int main() {
    printf("=== Maximum Subarray Sum - Divide and Conquer ===\n");
    
    // Test Case 1: Mixed positive and negative numbers
    int arr1[] = {-2, -3, 4, -1, -2, 1, 5, -3};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("Test Case 1: Mixed numbers\n");
    printf("Array: ");
    printArray(arr1, n1);
    printf("\n");
    
    SubarrayResult result1DC = maxSubarrayDivideConquer(arr1, n1);
    SubarrayResult result1K = maxSubarrayKadane(arr1, n1);
    
    printf("Divide & Conquer: ");
    printResult(result1DC);
    printf("\nSubarray: ");
    printSubarray(arr1, result1DC.startIndex, result1DC.endIndex);
    printf("\n");
    
    printf("Kadane's Algorithm: ");
    printResult(result1K);
    printf("\nSubarray: ");
    printSubarray(arr1, result1K.startIndex, result1K.endIndex);
    printf("\n\n");
    
    // Test Case 2: All negative numbers
    int arr2[] = {-5, -2, -8, -1, -4};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);
    printf("Test Case 2: All negative numbers\n");
    printf("Array: ");
    printArray(arr2, n2);
    printf("\n");
    
    SubarrayResult result2DC = maxSubarrayDivideConquer(arr2, n2);
    SubarrayResult result2K = maxSubarrayKadane(arr2, n2);
    
    printf("Divide & Conquer: ");
    printResult(result2DC);
    printf("\nSubarray: ");
    printSubarray(arr2, result2DC.startIndex, result2DC.endIndex);
    printf("\n");
    
    printf("Kadane's Algorithm: ");
    printResult(result2K);
    printf("\nSubarray: ");
    printSubarray(arr2, result2K.startIndex, result2K.endIndex);
    printf("\n\n");
    
    // Test Case 3: All positive numbers
    int arr3[] = {1, 2, 3, 4, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);
    printf("Test Case 3: All positive numbers\n");
    printf("Array: ");
    printArray(arr3, n3);
    printf("\n");
    
    SubarrayResult result3DC = maxSubarrayDivideConquer(arr3, n3);
    printf("Divide & Conquer: ");
    printResult(result3DC);
    printf("\nSubarray: ");
    printSubarray(arr3, result3DC.startIndex, result3DC.endIndex);
    printf("\n\n");
    
    // Test Case 4: Single element
    int arr4[] = {-7};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);
    printf("Test Case 4: Single element\n");
    printf("Array: ");
    printArray(arr4, n4);
    printf("\n");
    
    SubarrayResult result4DC = maxSubarrayDivideConquer(arr4, n4);
    printf("Divide & Conquer: ");
    printResult(result4DC);
    printf("\nSubarray: ");
    printSubarray(arr4, result4DC.startIndex, result4DC.endIndex);
    printf("\n\n");
    
    // Test Case 5: Performance comparison
    printf("Test Case 5: Performance comparison\n");
    int largeArr[] = {1, -3, 2, 1, -1, 3, -2, 4, -1, 2, -5, 1};
    int n5 = sizeof(largeArr) / sizeof(largeArr[0]);
    printf("Array: ");
    printArray(largeArr, n5);
    printf("\n");
    
    // Time divide and conquer approach
    clock_t start1 = clock();
    SubarrayResult resultDC = maxSubarrayDivideConquer(largeArr, n5);
    clock_t end1 = clock();
    
    // Time Kadane's algorithm
    clock_t start2 = clock();
    SubarrayResult resultK = maxSubarrayKadane(largeArr, n5);
    clock_t end2 = clock();
    
    printf("Divide & Conquer: ");
    printResult(resultDC);
    printf("\nTime: %ld microseconds\n", (end1 - start1) * 1000000 / CLOCKS_PER_SEC);
    
    printf("Kadane's Algorithm: ");
    printResult(resultK);
    printf("\nTime: %ld microseconds\n", (end2 - start2) * 1000000 / CLOCKS_PER_SEC);
    
    printf("Results match: %s\n", (resultDC.maxSum == resultK.maxSum) ? "Yes" : "No");
    
    return 0;
}