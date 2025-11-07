#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Dynamic Programming Strategy: Longest Common Subsequence (LCS)
 * Core Idea: Find the longest sequence that appears in both strings in the same order
 * Time Complexity: O(m * n) where m and n are lengths of the strings
 * Space Complexity: O(m * n) for 2D table, can be optimized to O(min(m, n))
 */

#define MAX_LENGTH 1000

/**
 * Utility function to find maximum of two integers
 */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/**
 * Find the length of LCS using 2D Dynamic Programming table
 * @param str1 First string
 * @param str2 Second string
 * @param m Length of first string
 * @param n Length of second string
 * @param dp 2D DP table (output parameter)
 * @return Length of the longest common subsequence
 */
int lcsLength(char* str1, char* str2, int m, int n, int dp[][MAX_LENGTH]) {
    // Base case: empty string has LCS length 0 with any string
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
            } else if (str1[i - 1] == str2[j - 1]) {
                // Characters match: LCS length increases by 1
                dp[i][j] = dp[i - 1][j - 1] + 1;
            } else {
                // Characters don't match: take maximum from excluding either character
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
            }
        }
    }
    
    return dp[m][n];
}

/**
 * Find the actual LCS string using backtracking
 * @param str1 First string
 * @param str2 Second string
 * @param m Length of first string
 * @param n Length of second string
 * @param result Buffer to store the LCS string
 */
void lcsString(char* str1, char* str2, int m, int n, char* result) {
    int dp[MAX_LENGTH][MAX_LENGTH];
    
    // Fill DP table
    lcsLength(str1, str2, m, n, dp);
    
    // Backtrack to find the actual LCS
    int lcsLen = dp[m][n];
    result[lcsLen] = '\0'; // Null terminate
    
    int i = m, j = n;
    int index = lcsLen - 1;
    
    while (i > 0 && j > 0) {
        if (str1[i - 1] == str2[j - 1]) {
            // Character is part of LCS
            result[index--] = str1[i - 1];
            i--;
            j--;
        } else if (dp[i - 1][j] > dp[i][j - 1]) {
            // Move up
            i--;
        } else {
            // Move left
            j--;
        }
    }
}

/**
 * Space-optimized LCS (only length, not the actual string)
 * @param str1 First string
 * @param str2 Second string
 * @param m Length of first string
 * @param n Length of second string
 * @return Length of LCS
 */
int lcsLengthOptimized(char* str1, char* str2, int m, int n) {
    // Use only two rows instead of full table
    int prev[MAX_LENGTH] = {0};
    int curr[MAX_LENGTH] = {0};
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                curr[j] = prev[j - 1] + 1;
            } else {
                curr[j] = max(prev[j], curr[j - 1]);
            }
        }
        // Swap rows
        for (int k = 0; k <= n; k++) {
            prev[k] = curr[k];
            curr[k] = 0;
        }
    }
    
    return prev[n];
}

/**
 * Print the DP table for educational purposes
 */
void printDPTable(char* str1, char* str2, int m, int n) {
    int dp[MAX_LENGTH][MAX_LENGTH];
    
    // Fill DP table
    lcsLength(str1, str2, m, n, dp);
    
    printf("DP Table for LCS:\n");
    printf("String 1: \"%s\"\n", str1);
    printf("String 2: \"%s\"\n", str2);
    printf("\n");
    
    // Print header
    printf("    ε ");
    for (int j = 0; j < n; j++) {
        printf("%2c ", str2[j]);
    }
    printf("\n");
    
    // Print table
    for (int i = 0; i <= m; i++) {
        if (i == 0) {
            printf("ε ");
        } else {
            printf("%c ", str1[i - 1]);
        }
        
        for (int j = 0; j <= n; j++) {
            printf("%2d ", dp[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

/**
 * Demonstrate step-by-step LCS construction
 */
void demonstrateLCSConstruction(char* str1, char* str2, int m, int n) {
    printf("Step-by-step LCS construction:\n");
    printf("String 1: \"%s\"\n", str1);
    printf("String 2: \"%s\"\n", str2);
    printf("\n");
    
    int dp[MAX_LENGTH][MAX_LENGTH];
    
    // Initialize base cases
    for (int i = 0; i <= m; i++) {
        dp[i][0] = 0;
    }
    for (int j = 0; j <= n; j++) {
        dp[0][j] = 0;
    }
    
    printf("Recurrence relation:\n");
    printf("If str1[i-1] == str2[j-1]: dp[i][j] = dp[i-1][j-1] + 1\n");
    printf("Else: dp[i][j] = max(dp[i-1][j], dp[i][j-1])\n");
    printf("\n");
    
    for (int i = 1; i <= m; i++) {
        printf("Processing str1[%d] = '%c':\n", i - 1, str1[i - 1]);
        
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                printf("  Match with str2[%d] = '%c': dp[%d][%d] = %d + 1 = %d\n", 
                       j - 1, str2[j - 1], i, j, dp[i - 1][j - 1], dp[i][j]);
            } else {
                dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
                printf("  No match with str2[%d] = '%c': dp[%d][%d] = max(%d, %d) = %d\n", 
                       j - 1, str2[j - 1], i, j, dp[i - 1][j], dp[i][j - 1], dp[i][j]);
            }
        }
        printf("\n");
    }
    
    printf("Final LCS length: %d\n", dp[m][n]);
    
    char result[MAX_LENGTH];
    lcsString(str1, str2, m, n, result);
    printf("Actual LCS: \"%s\"\n", result);
}

/**
 * Find Longest Common Substring (different from subsequence)
 */
void longestCommonSubstring(char* str1, char* str2, int m, int n, char* result) {
    int dp[MAX_LENGTH][MAX_LENGTH];
    int maxLength = 0;
    int endIndex = 0;
    
    // Initialize
    for (int i = 0; i <= m; i++) {
        for (int j = 0; j <= n; j++) {
            dp[i][j] = 0;
        }
    }
    
    for (int i = 1; i <= m; i++) {
        for (int j = 1; j <= n; j++) {
            if (str1[i - 1] == str2[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLength) {
                    maxLength = dp[i][j];
                    endIndex = i;
                }
            } else {
                dp[i][j] = 0; // Reset for substring (must be contiguous)
            }
        }
    }
    
    if (maxLength == 0) {
        result[0] = '\0';
        return;
    }
    
    // Extract substring
    strncpy(result, str1 + endIndex - maxLength, maxLength);
    result[maxLength] = '\0';
}

/**
 * Reverse a string
 */
void reverseString(char* str, char* reversed) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        reversed[i] = str[len - 1 - i];
    }
    reversed[len] = '\0';
}

int main() {
    printf("=== Longest Common Subsequence - Dynamic Programming ===\n");
    
    // Test Case 1: Basic LCS
    printf("Test Case 1: Basic LCS\n");
    char str1[] = "ABCDGH";
    char str2[] = "AEDFHR";
    int m1 = strlen(str1);
    int n1 = strlen(str2);
    
    int dp1[MAX_LENGTH][MAX_LENGTH];
    int lcsLen = lcsLength(str1, str2, m1, n1, dp1);
    char lcs[MAX_LENGTH];
    lcsString(str1, str2, m1, n1, lcs);
    
    printf("String 1: \"%s\"\n", str1);
    printf("String 2: \"%s\"\n", str2);
    printf("LCS Length: %d\n", lcsLen);
    printf("LCS String: \"%s\"\n", lcs);
    printf("\n");
    
    printDPTable(str1, str2, m1, n1);
    
    // Test Case 2: Step-by-step demonstration
    printf("Test Case 2: Step-by-step construction\n");
    char str3[] = "AGGTAB";
    char str4[] = "GXTXAYB";
    int m2 = strlen(str3);
    int n2 = strlen(str4);
    demonstrateLCSConstruction(str3, str4, m2, n2);
    printf("\n");
    
    // Test Case 3: Edge cases
    printf("Test Case 3: Edge cases\n");
    
    // Same strings
    char same1[] = "HELLO";
    char same2[] = "HELLO";
    int m3 = strlen(same1);
    int n3 = strlen(same2);
    char sameLCS[MAX_LENGTH];
    lcsString(same1, same2, m3, n3, sameLCS);
    
    int dp3[MAX_LENGTH][MAX_LENGTH];
    int sameLCSLen = lcsLength(same1, same2, m3, n3, dp3);
    printf("Same strings: \"%s\" and \"%s\"\n", same1, same2);
    printf("LCS: \"%s\" (length: %d)\n", sameLCS, sameLCSLen);
    
    // No common characters
    char diff1[] = "ABC";
    char diff2[] = "XYZ";
    int m4 = strlen(diff1);
    int n4 = strlen(diff2);
    char diffLCS[MAX_LENGTH];
    lcsString(diff1, diff2, m4, n4, diffLCS);
    
    int dp4[MAX_LENGTH][MAX_LENGTH];
    int diffLCSLen = lcsLength(diff1, diff2, m4, n4, dp4);
    printf("No common chars: \"%s\" and \"%s\"\n", diff1, diff2);
    printf("LCS: \"%s\" (length: %d)\n", diffLCS, diffLCSLen);
    
    // Empty string
    char empty[] = "";
    char nonEmpty[] = "ABC";
    int m5 = strlen(empty);
    int n5 = strlen(nonEmpty);
    char emptyLCS[MAX_LENGTH];
    lcsString(empty, nonEmpty, m5, n5, emptyLCS);
    
    int dp5[MAX_LENGTH][MAX_LENGTH];
    int emptyLCSLen = lcsLength(empty, nonEmpty, m5, n5, dp5);
    printf("Empty string: \"%s\" and \"%s\"\n", empty, nonEmpty);
    printf("LCS: \"%s\" (length: %d)\n", emptyLCS, emptyLCSLen);
    printf("\n");
    
    // Test Case 4: LCS vs LCS (Longest Common Substring)
    printf("Test Case 4: LCS vs Longest Common Substring\n");
    char sub1[] = "GeeksforGeeks";
    char sub2[] = "GeeksQuiz";
    int m6 = strlen(sub1);
    int n6 = strlen(sub2);
    
    char lcsResult[MAX_LENGTH];
    lcsString(sub1, sub2, m6, n6, lcsResult);
    
    char substringResult[MAX_LENGTH];
    longestCommonSubstring(sub1, sub2, m6, n6, substringResult);
    
    printf("String 1: \"%s\"\n", sub1);
    printf("String 2: \"%s\"\n", sub2);
    printf("Longest Common Subsequence: \"%s\" (length: %d)\n", lcsResult, (int)strlen(lcsResult));
    printf("Longest Common Substring: \"%s\" (length: %d)\n", substringResult, (int)strlen(substringResult));
    printf("Note: Subsequence allows gaps, substring requires contiguous characters\n");
    printf("\n");
    
    // Test Case 5: Performance comparison
    printf("Test Case 5: Performance comparison\n");
    char perf1[] = "ABCDEFGHIJKLMNOP";
    char perf2[] = "ADEFHIJKMNOPQRS";
    int m7 = strlen(perf1);
    int n7 = strlen(perf2);
    
    int dp7[MAX_LENGTH][MAX_LENGTH];
    int result2D = lcsLength(perf1, perf2, m7, n7, dp7);
    int resultOptimized = lcsLengthOptimized(perf1, perf2, m7, n7);
    
    printf("2D DP: %d\n", result2D);
    printf("Space optimized: %d\n", resultOptimized);
    printf("Both produce same result: %s\n", (result2D == resultOptimized) ? "YES" : "NO");
    printf("\n");
    
    // Test Case 6: DNA sequence example
    printf("Test Case 6: DNA sequence alignment example\n");
    char dna1[] = "ATCGTACG";
    char dna2[] = "ACGTGACG";
    int m8 = strlen(dna1);
    int n8 = strlen(dna2);
    
    char dnaLCS[MAX_LENGTH];
    lcsString(dna1, dna2, m8, n8, dnaLCS);
    
    int maxLen = (m8 > n8) ? m8 : n8;
    double similarity = (double)strlen(dnaLCS) / maxLen * 100;
    
    printf("DNA Sequence 1: %s\n", dna1);
    printf("DNA Sequence 2: %s\n", dna2);
    printf("Common subsequence: %s (length: %d)\n", dnaLCS, (int)strlen(dnaLCS));
    printf("Similarity: %.1f%%\n", similarity);
    printf("\n");
    
    // Test Case 7: Palindrome relationship
    printf("Test Case 7: Palindrome check using LCS\n");
    char palindromeTest[] = "racecar";
    char reversed[MAX_LENGTH];
    reverseString(palindromeTest, reversed);
    
    int m9 = strlen(palindromeTest);
    int n9 = strlen(reversed);
    int dp9[MAX_LENGTH][MAX_LENGTH];
    int palindromeLCS = lcsLength(palindromeTest, reversed, m9, n9, dp9);
    
    printf("String: \"%s\"\n", palindromeTest);
    printf("Reversed: \"%s\"\n", reversed);
    printf("LCS length: %d\n", palindromeLCS);
    printf("Is palindrome: %s\n", (palindromeLCS == m9) ? "YES" : "NO");
    printf("Note: If LCS(string, reverse) = length, then string is palindrome\n");
    
    return 0;
}