#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

/**
 * Backtracking Strategy: Permutation Generation
 * Core Idea: Generate all possible arrangements (permutations) of a given set of elements
 * Time Complexity: O(n! * n) where n is the number of elements
 * Space Complexity: O(n) for recursion stack and current permutation
 */

#define MAX_SIZE 10
#define MAX_PERMUTATIONS 3628800 // 10! = 3,628,800

/**
 * Structure to store a single permutation
 */
typedef struct {
    int elements[MAX_SIZE];
    int size;
} Permutation;

/**
 * Structure to store all permutations
 */
typedef struct {
    Permutation permutations[MAX_PERMUTATIONS];
    int count;
} PermutationSet;

/**
 * Backtracking helper for generating permutations
 * @param elements Original array
 * @param size Size of array
 * @param current Current permutation being built
 * @param currentSize Current size of permutation
 * @param used Boolean array to track used elements
 * @param result Structure to store all permutations
 */
void backtrackPermutations(int elements[], int size, int current[], int currentSize, 
                          bool used[], PermutationSet* result) {
    // Base case: permutation is complete
    if (currentSize == size) {
        if (result->count < MAX_PERMUTATIONS) {
            for (int i = 0; i < size; i++) {
                result->permutations[result->count].elements[i] = current[i];
            }
            result->permutations[result->count].size = size;
            result->count++;
        }
        return;
    }
    
    // Try each unused element
    for (int i = 0; i < size; i++) {
        if (!used[i]) {
            // Choose
            current[currentSize] = elements[i];
            used[i] = true;
            
            // Explore
            backtrackPermutations(elements, size, current, currentSize + 1, used, result);
            
            // Unchoose (backtrack)
            used[i] = false;
        }
    }
}

/**
 * Generate all permutations of an array
 * @param elements Array of elements to permute
 * @param size Size of the array
 * @param result Structure to store all permutations
 */
void generatePermutations(int elements[], int size, PermutationSet* result) {
    result->count = 0;
    int current[MAX_SIZE];
    bool used[MAX_SIZE] = {false};
    
    backtrackPermutations(elements, size, current, 0, used, result);
}

/**
 * Compare function for sorting
 */
int compare(const void* a, const void* b) {
    return (*(int*)a - *(int*)b);
}

/**
 * Backtracking for unique permutations (handles duplicates)
 */
void backtrackUniquePermutations(int elements[], int size, int current[], int currentSize, 
                                bool used[], PermutationSet* result) {
    if (currentSize == size) {
        if (result->count < MAX_PERMUTATIONS) {
            for (int i = 0; i < size; i++) {
                result->permutations[result->count].elements[i] = current[i];
            }
            result->permutations[result->count].size = size;
            result->count++;
        }
        return;
    }
    
    for (int i = 0; i < size; i++) {
        if (used[i]) continue;
        
        // Skip duplicates: if current element is same as previous and previous is not used
        if (i > 0 && elements[i] == elements[i - 1] && !used[i - 1]) {
            continue;
        }
        
        current[currentSize] = elements[i];
        used[i] = true;
        
        backtrackUniquePermutations(elements, size, current, currentSize + 1, used, result);
        
        used[i] = false;
    }
}

/**
 * Generate permutations with duplicates handled
 */
void generateUniquePermutations(int elements[], int size, PermutationSet* result) {
    result->count = 0;
    int current[MAX_SIZE];
    bool used[MAX_SIZE] = {false};
    
    // Sort to handle duplicates
    qsort(elements, size, sizeof(int), compare);
    
    backtrackUniquePermutations(elements, size, current, 0, used, result);
}

/**
 * Backtracking for k-permutations
 */
void backtrackKPermutations(int elements[], int size, int k, int current[], int currentSize, 
                           bool used[], PermutationSet* result) {
    if (currentSize == k) {
        if (result->count < MAX_PERMUTATIONS) {
            for (int i = 0; i < k; i++) {
                result->permutations[result->count].elements[i] = current[i];
            }
            result->permutations[result->count].size = k;
            result->count++;
        }
        return;
    }
    
    for (int i = 0; i < size; i++) {
        if (!used[i]) {
            current[currentSize] = elements[i];
            used[i] = true;
            
            backtrackKPermutations(elements, size, k, current, currentSize + 1, used, result);
            
            used[i] = false;
        }
    }
}

/**
 * Generate k-permutations (permutations of length k)
 */
void generateKPermutations(int elements[], int size, int k, PermutationSet* result) {
    result->count = 0;
    int current[MAX_SIZE];
    bool used[MAX_SIZE] = {false};
    
    backtrackKPermutations(elements, size, k, current, 0, used, result);
}

/**
 * Backtracking with step tracking
 */
void backtrackWithSteps(int elements[], int size, int current[], int currentSize, 
                       bool used[], PermutationSet* result, int* stepNum) {
    printf("Step %d: Current permutation: [", ++(*stepNum));
    for (int i = 0; i < currentSize; i++) {
        printf("%d", current[i]);
        if (i < currentSize - 1) printf(", ");
    }
    printf("], Used: [");
    for (int i = 0; i < size; i++) {
        printf("%s", used[i] ? "T" : "F");
        if (i < size - 1) printf(", ");
    }
    printf("]\n");
    
    if (currentSize == size) {
        printf("✓ Complete permutation found: [");
        for (int i = 0; i < size; i++) {
            printf("%d", current[i]);
            if (i < size - 1) printf(", ");
        }
        printf("]\n");
        
        if (result->count < MAX_PERMUTATIONS) {
            for (int i = 0; i < size; i++) {
                result->permutations[result->count].elements[i] = current[i];
            }
            result->permutations[result->count].size = size;
            result->count++;
        }
        return;
    }
    
    printf("  Need %d more elements\n", size - currentSize);
    
    // Limit output for demonstration
    if (*stepNum > 30) {
        printf("... (stopping step display after 30 steps)\n");
        backtrackPermutations(elements, size, current, currentSize, used, result);
        return;
    }
    
    for (int i = 0; i < size; i++) {
        if (!used[i]) {
            printf("  Trying element %d at position %d\n", elements[i], currentSize);
            
            // Choose
            current[currentSize] = elements[i];
            used[i] = true;
            
            // Explore
            backtrackWithSteps(elements, size, current, currentSize + 1, used, result, stepNum);
            
            // Unchoose
            printf("  Backtracking: removing %d from position %d\n", elements[i], currentSize);
            used[i] = false;
        }
    }
}

/**
 * Generate permutations with step tracking
 */
void generateWithSteps(int elements[], int size, PermutationSet* result) {
    result->count = 0;
    int current[MAX_SIZE];
    bool used[MAX_SIZE] = {false};
    int stepNum = 0;
    
    backtrackWithSteps(elements, size, current, 0, used, result, &stepNum);
}

/**
 * Swap two elements in array
 */
void swap(int arr[], int i, int j) {
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
}

/**
 * Generate next lexicographic permutation
 * @param arr Array to permute (modified in place)
 * @param size Size of array
 * @return true if next permutation exists, false if this is the last permutation
 */
bool nextPermutation(int arr[], int size) {
    // Find largest index i such that arr[i] < arr[i + 1]
    int i = size - 2;
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }
    
    if (i == -1) {
        return false; // No next permutation (this is the last one)
    }
    
    // Find largest index j such that arr[i] < arr[j]
    int j = size - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }
    
    // Swap arr[i] and arr[j]
    swap(arr, i, j);
    
    // Reverse suffix starting at arr[i + 1]
    int start = i + 1;
    int end = size - 1;
    while (start < end) {
        swap(arr, start, end);
        start++;
        end--;
    }
    
    return true;
}

/**
 * Generate all permutations using next permutation method
 */
void generatePermutationsIterative(int elements[], int size, PermutationSet* result) {
    result->count = 0;
    int arr[MAX_SIZE];
    
    // Copy and sort to start with smallest permutation
    for (int i = 0; i < size; i++) {
        arr[i] = elements[i];
    }
    qsort(arr, size, sizeof(int), compare);
    
    do {
        // Add current permutation
        if (result->count < MAX_PERMUTATIONS) {
            for (int i = 0; i < size; i++) {
                result->permutations[result->count].elements[i] = arr[i];
            }
            result->permutations[result->count].size = size;
            result->count++;
        }
    } while (nextPermutation(arr, size));
}

/**
 * Calculate factorial
 */
long long factorial(int n) {
    long long result = 1;
    for (int i = 1; i <= n; i++) {
        result *= i;
    }
    return result;
}

/**
 * Count total number of permutations without generating them
 */
long long countPermutations(int elements[], int size) {
    long long fact = factorial(size);
    
    // Account for duplicates
    int frequency[1000] = {0}; // Assuming elements are in reasonable range
    int offset = 500; // Offset for negative numbers
    
    for (int i = 0; i < size; i++) {
        frequency[elements[i] + offset]++;
    }
    
    for (int i = 0; i < 1000; i++) {
        if (frequency[i] > 1) {
            fact /= factorial(frequency[i]);
        }
    }
    
    return fact;
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
 * Print permutation
 */
void printPermutation(Permutation* perm) {
    printf("[");
    for (int i = 0; i < perm->size; i++) {
        printf("%d", perm->elements[i]);
        if (i < perm->size - 1) printf(", ");
    }
    printf("]");
}

/**
 * Print a set of permutations (limited output)
 */
void printPermutations(PermutationSet* permSet, int limit) {
    printf("Permutations (showing first %d):\n", (permSet->count < limit ? permSet->count : limit));
    for (int i = 0; i < (permSet->count < limit ? permSet->count : limit); i++) {
        printf("  ");
        printPermutation(&permSet->permutations[i]);
        printf("\n");
    }
    if (permSet->count > limit) {
        printf("  ... (%d more)\n", permSet->count - limit);
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
    printf("=== Permutation Generation - Backtracking ===\n\n");
    
    PermutationSet result;
    
    // Test Case 1: Basic permutations
    printf("Test Case 1: Basic Permutations\n");
    int elements1[] = {1, 2, 3};
    int size1 = 3;
    
    printf("Elements: ");
    printArray(elements1, size1);
    printf("\n");
    
    generatePermutations(elements1, size1, &result);
    printPermutations(&result, 10);
    printf("Total permutations: %d\n", result.count);
    printf("Expected: 3! = 6\n\n");
    
    // Test Case 2: Step-by-step demonstration
    printf("Test Case 2: Step-by-step generation\n");
    int elements2[] = {1, 2};
    int size2 = 2;
    
    printf("Elements: ");
    printArray(elements2, size2);
    printf("\n");
    
    printf("Generation steps:\n");
    generateWithSteps(elements2, size2, &result);
    printf("\n");
    
    // Test Case 3: Permutations with duplicates
    printf("Test Case 3: Permutations with duplicates\n");
    int elements3[] = {1, 1, 2};
    int size3 = 3;
    
    printf("Elements: ");
    printArray(elements3, size3);
    printf("\n");
    
    generatePermutations(elements3, size3, &result);
    printf("All permutations (including duplicates): %d\n", result.count);
    printPermutations(&result, 8);
    
    generateUniquePermutations(elements3, size3, &result);
    printf("Unique permutations: %d\n", result.count);
    printPermutations(&result, 8);
    printf("\n");
    
    // Test Case 4: K-permutations
    printf("Test Case 4: K-Permutations\n");
    int elements4[] = {1, 2, 3, 4};
    int size4 = 4;
    int k = 2;
    
    printf("Elements: ");
    printArray(elements4, size4);
    printf("\nK = %d\n", k);
    
    generateKPermutations(elements4, size4, k, &result);
    printPermutations(&result, 15);
    printf("Total %d-permutations: %d\n", k, result.count);
    printf("Expected: P(4,2) = 4!/(4-2)! = 12\n\n");
    
    // Test Case 5: Performance comparison
    printf("Test Case 5: Performance Comparison\n");
    int elements5[] = {1, 2, 3, 4, 5};
    int size5 = 5;
    
    printf("Elements: ");
    printArray(elements5, size5);
    printf("\n");
    
    long long startTime = getCurrentTimeMillis();
    generatePermutations(elements5, size5, &result);
    long long backtrackTime = getCurrentTimeMillis() - startTime;
    int backtrackCount = result.count;
    
    startTime = getCurrentTimeMillis();
    generatePermutationsIterative(elements5, size5, &result);
    long long iterativeTime = getCurrentTimeMillis() - startTime;
    int iterativeCount = result.count;
    
    printf("Backtracking: %d permutations (Time: %lldms)\n", backtrackCount, backtrackTime);
    printf("Iterative: %d permutations (Time: %lldms)\n", iterativeCount, iterativeTime);
    printf("\n");
    
    // Test Case 6: Next permutation demonstration
    printf("Test Case 6: Next Permutation Method\n");
    int arr[] = {1, 2, 3};
    int arrSize = 3;
    
    printf("Starting with: ");
    printArray(arr, arrSize);
    printf("\n");
    printf("All permutations in lexicographic order:\n");
    
    int count = 1;
    printf("%d: ", count);
    printArray(arr, arrSize);
    printf("\n");
    
    while (nextPermutation(arr, arrSize)) {
        count++;
        printf("%d: ", count);
        printArray(arr, arrSize);
        printf("\n");
    }
    printf("\n");
    
    // Test Case 7: Large scale counting
    printf("Test Case 7: Counting Permutations\n");
    
    for (int n = 1; n <= 8; n++) {
        int elements[8];
        for (int i = 0; i < n; i++) {
            elements[i] = i + 1;
        }
        
        long long count = countPermutations(elements, n);
        printf("Elements [1..%d]: %lld permutations\n", n, count);
    }
    printf("\n");
    
    // Test Case 8: Duplicates counting
    printf("Test Case 8: Permutations with duplicates counting\n");
    
    int dup1[] = {1, 1, 2};
    int dup2[] = {1, 2, 2, 3};
    int dup3[] = {1, 1, 1, 2, 2};
    
    printf("Array ");
    printArray(dup1, 3);
    printf(": %lld unique permutations\n", countPermutations(dup1, 3));
    
    printf("Array ");
    printArray(dup2, 4);
    printf(": %lld unique permutations\n", countPermutations(dup2, 4));
    
    printf("Array ");
    printArray(dup3, 5);
    printf(": %lld unique permutations\n", countPermutations(dup3, 5));
    printf("\n");
    
    // Test Case 9: Edge cases
    printf("Test Case 9: Edge Cases\n");
    
    // Single element
    int single[] = {42};
    generatePermutations(single, 1, &result);
    printf("Single element: %d permutations\n", result.count);
    printPermutations(&result, 5);
    
    // All same elements
    int allSame[] = {2, 2, 2};
    generateUniquePermutations(allSame, 3, &result);
    printf("All same elements: %d unique permutations\n", result.count);
    printPermutations(&result, 5);
    printf("\n");
    
    // Test Case 10: Larger example
    printf("Test Case 10: Larger example\n");
    int elements10[] = {1, 2, 3, 4, 5, 6};
    int size10 = 6;
    
    printf("Elements: ");
    printArray(elements10, size10);
    printf("\n");
    
    startTime = getCurrentTimeMillis();
    generatePermutations(elements10, size10, &result);
    long long endTime = getCurrentTimeMillis();
    
    printf("Generated %d permutations in %lldms\n", result.count, endTime - startTime);
    printf("Expected: 6! = 720\n");
    
    printf("First 10 permutations:\n");
    printPermutations(&result, 10);
    printf("\n");
    
    printf("Applications:\n");
    printf("- Traveling Salesman Problem: permutations of cities\n");
    printf("- Anagram generation: permutations of letters\n");
    printf("- Task scheduling: permutations of tasks\n");
    printf("- Combinatorial optimization\n");
    printf("- Cryptography: key generation\n");
    
    printf("\nComplexity Analysis:\n");
    printf("- Time: O(n! * n) for generating all permutations\n");
    printf("- Space: O(n) for recursion stack\n");
    printf("- Total permutations: n! for distinct elements\n");
    printf("- With duplicates: n! / (n1! * n2! * ... * nk!)\n");
    printf("- Optimizations: Heap's algorithm, iterative generation\n");
    
    return 0;
}