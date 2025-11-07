#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/**
 * Brute Force Strategy: String Permutations
 * Core Idea: Generate all possible permutations by trying every combination
 * Time Complexity: O(n! * n) where n is the length of string
 * Space Complexity: O(n! * n) for storing all permutations
 */

/**
 * Swap two characters in a string
 */
void swap(char* str, int i, int j) {
    char temp = str[i];
    str[i] = str[j];
    str[j] = temp;
}

/**
 * Generate all permutations of a string using brute force approach
 * @param str Input string
 * @param start Starting index for current permutation level
 * @param end Ending index (length - 1)
 */
void generatePermutations(char* str, int start, int end) {
    // Base case: if we've fixed all positions, print the permutation
    if (start == end) {
        printf("%s ", str);
        return;
    }
    
    // Try each character at the current position
    for (int i = start; i <= end; i++) {
        // Swap current character with the character at start
        swap(str, start, i);
        
        // Recursively generate permutations for remaining positions
        generatePermutations(str, start + 1, end);
        
        // Backtrack: restore original order
        swap(str, start, i);
    }
}

/**
 * Wrapper function to generate and print all permutations
 */
void printAllPermutations(char* str) {
    int len = strlen(str);
    if (len == 0) {
        printf("(empty string - 1 permutation)\n");
        return;
    }
    generatePermutations(str, 0, len - 1);
    printf("\n");
}

int main() {
    printf("=== String Permutations - Brute Force ===\n");
    
    // Test Case 1: Simple 3-character string
    char test1[] = "ABC";
    printf("Input: \"%s\"\n", test1);
    printf("Output: ");
    printAllPermutations(test1);
    printf("Total permutations: 6 (3!)\n\n");
    
    // Test Case 2: String with repeated characters
    char test2[] = "AAB";
    printf("Input: \"%s\"\n", test2);
    printf("Output: ");
    printAllPermutations(test2);
    printf("Total permutations: 6 (includes duplicates)\n\n");
    
    // Test Case 3: Single character
    char test3[] = "X";
    printf("Input: \"%s\"\n", test3);
    printf("Output: ");
    printAllPermutations(test3);
    printf("Total permutations: 1\n\n");
    
    // Test Case 4: Two characters
    char test4[] = "AB";
    printf("Input: \"%s\"\n", test4);
    printf("Output: ");
    printAllPermutations(test4);
    printf("Total permutations: 2 (2!)\n\n");
    
    // Test Case 5: Empty string
    char test5[] = "";
    printf("Input: \"%s\"\n", test5);
    printf("Output: ");
    printAllPermutations(test5);
    
    return 0;
}