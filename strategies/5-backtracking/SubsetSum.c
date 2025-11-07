#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/**
 * Backtracking Strategy: Subset Sum Problem
 * Core Idea: Find if there exists a subset of given numbers that sum to a target value
 * Time Complexity: O(2^n) where n is the number of elements (worst case)
 * Space Complexity: O(n) for recursion stack and subset tracking
 */

#define MAX_SIZE 20
#define MAX_SUBSETS 1000

/**
 * Structure to represent a subset
 */
typedef struct {
    int elements[MAX_SIZE];
    int size;
    int sum;
} Subset;

/**
 * Structure to store multiple subsets
 */
typedef struct {
    Subset subsets[MAX_SUBSETS];
    int count;
} SubsetCollection;

/**
 * Backtracking helper function for subset sum
 * @param numbers Array of numbers
 * @param size Size of the array
 * @param index Current index in the array
 * @param targetSum Target sum
 * @param currentSum Current sum of selected elements
 * @param currentSubset Current subset being built
 * @param subsetSize Current size of subset
 * @return true if target sum can be achieved, false otherwise
 */
bool backtrackSubsetSum(int numbers[], int size, int index, int targetSum, 
                       int currentSum, int currentSubset[], int subsetSize) {
    // Base case: reached target sum
    if (currentSum == targetSum) {
        return true;
    }
    
    // Base case: exceeded target or no more elements
    if (currentSum > targetSum || index >= size) {
        return false;
    }
    
    // Include current element
    currentSubset[subsetSize] = numbers[index];
    if (backtrackSubsetSum(numbers, size, index + 1, targetSum, 
                          currentSum + numbers[index], currentSubset, subsetSize + 1)) {
        return true;
    }
    
    // Exclude current element
    if (backtrackSubsetSum(numbers, size, index + 1, targetSum, 
                          currentSum, currentSubset, subsetSize)) {
        return true;
    }
    
    return false;
}

/**
 * Check if there exists a subset with given sum using backtracking
 * @param numbers Array of numbers
 * @param size Size of the array
 * @param targetSum Target sum to achieve
 * @return true if subset exists, false otherwise
 */
bool hasSubsetSum(int numbers[], int size, int targetSum) {
    int currentSubset[MAX_SIZE];
    return backtrackSubsetSum(numbers, size, 0, targetSum, 0, currentSubset, 0);
}

/**
 * Find one subset that sums to target value
 * @param numbers Array of numbers
 * @param size Size of the array
 * @param targetSum Target sum
 * @param result Array to store the result subset
 * @param resultSize Pointer to store the size of result
 * @return true if subset found, false otherwise
 */
bool findSubsetSum(int numbers[], int size, int targetSum, int result[], int* resultSize) {
    *resultSize = 0;
    return findSubsetSumHelper(numbers, size, 0, targetSum, 0, result, resultSize);
}

/**
 * Helper method to find and store one valid subset
 */
bool findSubsetSumHelper(int numbers[], int size, int index, int targetSum, 
                        int currentSum, int result[], int* resultSize) {
    if (currentSum == targetSum) {
        return true;
    }
    
    if (currentSum > targetSum || index >= size) {
        return false;
    }
    
    // Include current element
    result[*resultSize] = numbers[index];
    (*resultSize)++;
    if (findSubsetSumHelper(numbers, size, index + 1, targetSum, 
                           currentSum + numbers[index], result, resultSize)) {
        return true;
    }
    
    // Backtrack: exclude current element
    (*resultSize)--;
    if (findSubsetSumHelper(numbers, size, index + 1, targetSum, 
                           currentSum, result, resultSize)) {
        return true;
    }
    
    return false;
}

/**
 * Find all subsets that sum to target value
 * @param numbers Array of numbers
 * @param size Size of the array
 * @param targetSum Target sum
 * @param collection Structure to store all valid subsets
 */
void findAllSubsetSums(int numbers[], int size, int targetSum, SubsetCollection* collection) {
    collection->count = 0;
    int currentSubset[MAX_SIZE];
    findAllSubsetsHelper(numbers, size, 0, targetSum, 0, currentSubset, 0, collection);
}

/**
 * Helper method to find all valid subsets
 */
void findAllSubsetsHelper(int numbers[], int size, int index, int targetSum, 
                         int currentSum, int currentSubset[], int subsetSize, 
                         SubsetCollection* collection) {
    if (currentSum == targetSum) {
        if (collection->count < MAX_SUBSETS) {
            // Store the subset
            for (int i = 0; i < subsetSize; i++) {
                collection->subsets[collection->count].elements[i] = currentSubset[i];
            }
            collection->subsets[collection->count].size = subsetSize;
            collection->subsets[collection->count].sum = currentSum;
            collection->count++;
        }
        return;
    }
    
    if (currentSum > targetSum || index >= size) {
        return;
    }
    
    // Include current element
    currentSubset[subsetSize] = numbers[index];
    findAllSubsetsHelper(numbers, size, index + 1, targetSum, 
                        currentSum + numbers[index], currentSubset, subsetSize + 1, collection);
    
    // Exclude current element
    findAllSubsetsHelper(numbers, size, index + 1, targetSum, 
                        currentSum, currentSubset, subsetSize, collection);
}

/**
 * Count number of subsets with given sum
 * @param numbers Array of numbers
 * @param size Size of array
 * @param index Current index
 * @param targetSum Target sum
 * @param currentSum Current sum
 * @return Number of valid subsets
 */
int countSubsetsHelper(int numbers[], int size, int index, int targetSum, int currentSum) {
    if (currentSum == targetSum) {
        return 1;
    }
    
    if (currentSum > targetSum || index >= size) {
        return 0;
    }
    
    // Include + exclude current element
    return countSubsetsHelper(numbers, size, index + 1, targetSum, currentSum + numbers[index]) +
           countSubsetsHelper(numbers, size, index + 1, targetSum, currentSum);
}

/**
 * Count number of subsets with given sum
 */
int countSubsetSums(int numbers[], int size, int targetSum) {
    return countSubsetsHelper(numbers, size, 0, targetSum, 0);
}

/**
 * Backtracking with step tracking
 */
bool backtrackWithSteps(int numbers[], int size, int index, int targetSum, 
                       int currentSum, int currentSubset[], int subsetSize, 
                       int* stepNum) {
    printf("Step %d: Index=%d, CurrentSum=%d, Target=%d, Subset=[", 
           ++(*stepNum), index, currentSum, targetSum);
    
    for (int i = 0; i < subsetSize; i++) {
        printf("%d", currentSubset[i]);
        if (i < subsetSize - 1) printf(", ");
    }
    printf("]\n");
    
    if (currentSum == targetSum) {
        printf("✓ Found target sum! Subset: [");
        for (int i = 0; i < subsetSize; i++) {
            printf("%d", currentSubset[i]);
            if (i < subsetSize - 1) printf(", ");
        }
        printf("]\n");
        return true;
    }
    
    if (currentSum > targetSum) {
        printf("✗ Sum exceeded target, backtracking\n");
        return false;
    }
    
    if (index >= size) {
        printf("✗ No more elements, backtracking\n");
        return false;
    }
    
    // Limit output for demonstration
    if (*stepNum > 30) {
        printf("... (stopping step display after 30 steps)\n");
        return backtrackSubsetSum(numbers, size, index, targetSum, currentSum, currentSubset, subsetSize);
    }
    
    // Try including current element
    printf("  Trying to include %d\n", numbers[index]);
    currentSubset[subsetSize] = numbers[index];
    if (backtrackWithSteps(numbers, size, index + 1, targetSum, 
                          currentSum + numbers[index], currentSubset, subsetSize + 1, stepNum)) {
        return true;
    }
    
    // Try excluding current element
    printf("  Trying to exclude %d\n", numbers[index]);
    if (backtrackWithSteps(numbers, size, index + 1, targetSum, 
                          currentSum, currentSubset, subsetSize, stepNum)) {
        return true;
    }
    
    return false;
}

/**
 * Solve with step tracking for demonstration
 */
bool solveWithSteps(int numbers[], int size, int targetSum) {
    int currentSubset[MAX_SIZE];
    int stepNum = 0;
    return backtrackWithSteps(numbers, size, 0, targetSum, 0, currentSubset, 0, &stepNum);
}

/**
 * Check if array can be partitioned into two equal sum subsets
 */
bool canPartition(int numbers[], int size) {
    int totalSum = 0;
    for (int i = 0; i < size; i++) {
        totalSum += numbers[i];
    }
    
    // If total sum is odd, cannot partition into equal sums
    if (totalSum % 2 != 0) {
        return false;
    }
    
    return hasSubsetSum(numbers, size, totalSum / 2);
}

/**
 * Find subset closest to target sum
 */
void findClosestSubset(int numbers[], int size, int targetSum, int result[], int* resultSize, int* bestSum) {
    *resultSize = 0;
    *bestSum = 0;
    int bestDifference = 2147483647; // INT_MAX
    int bestSubset[MAX_SIZE];
    int bestSubsetSize = 0;
    
    findClosestHelper(numbers, size, 0, targetSum, 0, numbers, 0, 
                     &bestDifference, bestSubset, &bestSubsetSize, bestSum);
    
    // Copy best result
    for (int i = 0; i < bestSubsetSize; i++) {
        result[i] = bestSubset[i];
    }
    *resultSize = bestSubsetSize;
}

/**
 * Helper method to find closest subset
 */
void findClosestHelper(int numbers[], int size, int index, int targetSum, 
                      int currentSum, int currentSubset[], int subsetSize, 
                      int* bestDifference, int bestSubset[], int* bestSubsetSize, int* bestSum) {
    // Update best if current is closer
    int currentDifference = abs(targetSum - currentSum);
    if (currentDifference < *bestDifference) {
        *bestDifference = currentDifference;
        *bestSum = currentSum;
        *bestSubsetSize = subsetSize;
        for (int i = 0; i < subsetSize; i++) {
            bestSubset[i] = currentSubset[i];
        }
    }
    
    if (index >= size) {
        return;
    }
    
    // Include current element
    currentSubset[subsetSize] = numbers[index];
    findClosestHelper(numbers, size, index + 1, targetSum, 
                     currentSum + numbers[index], currentSubset, subsetSize + 1, 
                     bestDifference, bestSubset, bestSubsetSize, bestSum);
    
    // Exclude current element
    findClosestHelper(numbers, size, index + 1, targetSum, 
                     currentSum, currentSubset, subsetSize, 
                     bestDifference, bestSubset, bestSubsetSize, bestSum);
}

/**
 * Print array
 */
void printArray(int arr[], int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]");
}

/**
 * Print subset
 */
void printSubset(Subset* subset) {
    printf("[");
    for (int i = 0; i < subset->size; i++) {
        printf("%d", subset->elements[i]);
        if (i < subset->size - 1) printf(", ");
    }
    printf("]");
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
    printf("=== Subset Sum Problem - Backtracking ===\n\n");
    
    // Test Case 1: Basic subset sum
    printf("Test Case 1: Basic Subset Sum\n");
    int numbers1[] = {3, 34, 4, 12, 5, 2};
    int size1 = 6;
    int target1 = 9;
    
    printf("Numbers: ");
    printArray(numbers1, size1);
    printf("\nTarget sum: %d\n", target1);
    
    bool exists1 = hasSubsetSum(numbers1, size1, target1);
    printf("Subset exists: %s\n", exists1 ? "true" : "false");
    
    int result1[MAX_SIZE];
    int resultSize1;
    bool found1 = findSubsetSum(numbers1, size1, target1, result1, &resultSize1);
    if (found1) {
        printf("One valid subset: ");
        printArray(result1, resultSize1);
        printf("\n");
    }
    
    SubsetCollection collection1;
    findAllSubsetSums(numbers1, size1, target1, &collection1);
    printf("All valid subsets:\n");
    for (int i = 0; i < collection1.count; i++) {
        printf("  ");
        printSubset(&collection1.subsets[i]);
        printf(" (sum: %d)\n", collection1.subsets[i].sum);
    }
    printf("Number of subsets: %d\n\n", countSubsetSums(numbers1, size1, target1));
    
    // Test Case 2: Step-by-step demonstration
    printf("Test Case 2: Step-by-step solution\n");
    int numbers2[] = {2, 3, 7, 8};
    int size2 = 4;
    int target2 = 11;
    
    printf("Numbers: ");
    printArray(numbers2, size2);
    printf("\nTarget sum: %d\n", target2);
    
    printf("Solution steps:\n");
    bool solved = solveWithSteps(numbers2, size2, target2);
    printf("Solution found: %s\n\n", solved ? "true" : "false");
    
    // Test Case 3: No solution case
    printf("Test Case 3: No solution case\n");
    int numbers3[] = {1, 3, 5, 7};
    int size3 = 4;
    int target3 = 12;
    
    printf("Numbers: ");
    printArray(numbers3, size3);
    printf("\nTarget sum: %d\n", target3);
    
    bool exists3 = hasSubsetSum(numbers3, size3, target3);
    int result3[MAX_SIZE];
    int resultSize3;
    bool found3 = findSubsetSum(numbers3, size3, target3, result3, &resultSize3);
    
    printf("Subset exists: %s\n", exists3 ? "true" : "false");
    printf("Subset found: %s\n\n", found3 ? "true" : "false");
    
    // Test Case 4: Performance analysis
    printf("Test Case 4: Performance analysis\n");
    int numbers4[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int size4 = 8;
    int target4 = 15;
    
    printf("Numbers: ");
    printArray(numbers4, size4);
    printf("\nTarget sum: %d\n", target4);
    
    long long startTime = getCurrentTimeMillis();
    bool result4 = hasSubsetSum(numbers4, size4, target4);
    long long endTime = getCurrentTimeMillis();
    
    printf("Result: %s (Time: %lldms)\n", result4 ? "true" : "false", endTime - startTime);
    printf("Number of subsets: %d\n\n", countSubsetSums(numbers4, size4, target4));
    
    // Test Case 5: Partition problem
    printf("Test Case 5: Equal Partition Problem\n");
    int numbers5[] = {1, 5, 11, 5};
    int size5 = 4;
    
    printf("Numbers: ");
    printArray(numbers5, size5);
    printf("\n");
    
    bool canPart = canPartition(numbers5, size5);
    printf("Can partition into equal sums: %s\n", canPart ? "true" : "false");
    
    if (canPart) {
        int totalSum = 0;
        for (int i = 0; i < size5; i++) {
            totalSum += numbers5[i];
        }
        
        int partition1[MAX_SIZE];
        int partitionSize1;
        findSubsetSum(numbers5, size5, totalSum / 2, partition1, &partitionSize1);
        
        printf("First partition: ");
        printArray(partition1, partitionSize1);
        
        int sum1 = 0;
        for (int i = 0; i < partitionSize1; i++) {
            sum1 += partition1[i];
        }
        printf(" (sum: %d)\n", sum1);
    }
    printf("\n");
    
    // Test Case 6: Closest subset
    printf("Test Case 6: Closest Subset\n");
    int numbers6[] = {2, 7, 1, 4, 9, 3};
    int size6 = 6;
    int target6 = 15;
    
    printf("Numbers: ");
    printArray(numbers6, size6);
    printf("\nTarget sum: %d\n", target6);
    
    int closest[MAX_SIZE];
    int closestSize;
    int closestSum;
    findClosestSubset(numbers6, size6, target6, closest, &closestSize, &closestSum);
    
    printf("Closest subset: ");
    printArray(closest, closestSize);
    printf("\nClosest sum: %d (difference: %d)\n\n", closestSum, abs(target6 - closestSum));
    
    // Test Case 7: Edge cases
    printf("Test Case 7: Edge Cases\n");
    
    // Empty array
    printf("Empty array, target 0: %s\n", hasSubsetSum(NULL, 0, 0) ? "true" : "false");
    printf("Empty array, target 5: %s\n", hasSubsetSum(NULL, 0, 5) ? "true" : "false");
    
    // Single element
    int single[] = {5};
    printf("Single element [5], target 5: %s\n", hasSubsetSum(single, 1, 5) ? "true" : "false");
    printf("Single element [5], target 3: %s\n", hasSubsetSum(single, 1, 3) ? "true" : "false");
    
    // Target is 0
    int numbers8[] = {1, 2, 3};
    printf("Array [1,2,3], target 0: %s\n", hasSubsetSum(numbers8, 3, 0) ? "true" : "false");
    printf("Count of subsets with sum 0: %d\n\n", countSubsetSums(numbers8, 3, 0));
    
    // Test Case 8: Duplicates
    printf("Test Case 8: Array with duplicates\n");
    int duplicates[] = {2, 2, 2, 2};
    int dupSize = 4;
    int targetDup = 4;
    
    printf("Numbers: ");
    printArray(duplicates, dupSize);
    printf("\nTarget sum: %d\n", targetDup);
    
    SubsetCollection allDuplicates;
    findAllSubsetSums(duplicates, dupSize, targetDup, &allDuplicates);
    printf("All subsets with sum %d:\n", targetDup);
    for (int i = 0; i < allDuplicates.count; i++) {
        printf("  ");
        printSubset(&allDuplicates.subsets[i]);
        printf("\n");
    }
    printf("Count: %d\n\n", countSubsetSums(duplicates, dupSize, targetDup));
    
    // Test Case 9: Larger example
    printf("Test Case 9: Larger example\n");
    int numbers9[] = {15, 22, 14, 26, 32, 9, 16, 8};
    int size9 = 8;
    int target9 = 53;
    
    printf("Numbers: ");
    printArray(numbers9, size9);
    printf("\nTarget sum: %d\n", target9);
    
    startTime = getCurrentTimeMillis();
    SubsetCollection allLarge;
    findAllSubsetSums(numbers9, size9, target9, &allLarge);
    endTime = getCurrentTimeMillis();
    
    printf("Number of valid subsets: %d\n", allLarge.count);
    printf("Time taken: %lldms\n", endTime - startTime);
    
    if (allLarge.count > 0) {
        printf("First few subsets:\n");
        for (int i = 0; i < (allLarge.count < 5 ? allLarge.count : 5); i++) {
            printf("  ");
            printSubset(&allLarge.subsets[i]);
            printf(" (sum: %d)\n", allLarge.subsets[i].sum);
        }
    }
    printf("\n");
    
    printf("Complexity Analysis:\n");
    printf("- Time: O(2^n) where n is number of elements (worst case)\n");
    printf("- Space: O(n) for recursion stack\n");
    printf("- Optimizations: pruning, sorting, duplicate skipping\n");
    printf("- Applications: partition problem, knapsack, change making\n");
    printf("- Related: subset sum is NP-Complete problem\n");
    
    return 0;
}