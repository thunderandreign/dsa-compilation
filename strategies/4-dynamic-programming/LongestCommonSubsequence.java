/**
 * Dynamic Programming Strategy: Longest Common Subsequence (LCS)
 * Core Idea: Find the longest sequence that appears in both strings in the same order
 * Time Complexity: O(m * n) where m and n are lengths of the strings
 * Space Complexity: O(m * n) for 2D table, can be optimized to O(min(m, n))
 */

import java.util.*;

public class LongestCommonSubsequence {
    
    /**
     * Find the length of LCS using 2D Dynamic Programming table
     * @param str1 First string
     * @param str2 Second string
     * @return Length of the longest common subsequence
     */
    public static int lcsLength(String str1, String str2) {
        int m = str1.length();
        int n = str2.length();
        
        // Create DP table: dp[i][j] = LCS length of str1[0..i-1] and str2[0..j-1]
        int[][] dp = new int[m + 1][n + 1];
        
        // Base case: empty string has LCS length 0 with any string
        // (already initialized to 0)
        
        // Fill table using dynamic programming recurrence
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                    // Characters match: LCS length increases by 1
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    // Characters don't match: take maximum from excluding either character
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        return dp[m][n];
    }
    
    /**
     * Find the actual LCS string using backtracking
     * @param str1 First string
     * @param str2 Second string
     * @return The longest common subsequence as a string
     */
    public static String lcsString(String str1, String str2) {
        int m = str1.length();
        int n = str2.length();
        int[][] dp = new int[m + 1][n + 1];
        
        // Fill DP table
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        // Backtrack to find the actual LCS
        StringBuilder lcs = new StringBuilder();
        int i = m, j = n;
        
        while (i > 0 && j > 0) {
            if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                // Character is part of LCS
                lcs.append(str1.charAt(i - 1));
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
        
        // Reverse since we built it backwards
        return lcs.reverse().toString();
    }
    
    /**
     * Find all possible LCS strings (there might be multiple)
     * @param str1 First string
     * @param str2 Second string
     * @return Set of all longest common subsequences
     */
    public static Set<String> allLCS(String str1, String str2) {
        int m = str1.length();
        int n = str2.length();
        int[][] dp = new int[m + 1][n + 1];
        
        // Fill DP table
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        // Backtrack to find all LCS
        Set<String> allLCSSet = new HashSet<>();
        findAllLCS(str1, str2, dp, m, n, "", allLCSSet);
        return allLCSSet;
    }
    
    /**
     * Recursive helper to find all LCS strings
     */
    private static void findAllLCS(String str1, String str2, int[][] dp, int i, int j, 
                                  String currentLCS, Set<String> allLCSSet) {
        if (i == 0 || j == 0) {
            allLCSSet.add(new StringBuilder(currentLCS).reverse().toString());
            return;
        }
        
        if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
            findAllLCS(str1, str2, dp, i - 1, j - 1, currentLCS + str1.charAt(i - 1), allLCSSet);
        } else {
            if (dp[i - 1][j] == dp[i][j]) {
                findAllLCS(str1, str2, dp, i - 1, j, currentLCS, allLCSSet);
            }
            if (dp[i][j - 1] == dp[i][j]) {
                findAllLCS(str1, str2, dp, i, j - 1, currentLCS, allLCSSet);
            }
        }
    }
    
    /**
     * Space-optimized LCS (only length, not the actual string)
     * @param str1 First string
     * @param str2 Second string
     * @return Length of LCS
     */
    public static int lcsLengthOptimized(String str1, String str2) {
        int m = str1.length();
        int n = str2.length();
        
        // Use only two rows instead of full table
        int[] prev = new int[n + 1];
        int[] curr = new int[n + 1];
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                    curr[j] = prev[j - 1] + 1;
                } else {
                    curr[j] = Math.max(prev[j], curr[j - 1]);
                }
            }
            // Swap rows
            int[] temp = prev;
            prev = curr;
            curr = temp;
        }
        
        return prev[n];
    }
    
    /**
     * Print the DP table for educational purposes
     */
    public static void printDPTable(String str1, String str2) {
        int m = str1.length();
        int n = str2.length();
        int[][] dp = new int[m + 1][n + 1];
        
        // Fill DP table
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                } else {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                }
            }
        }
        
        System.out.println("DP Table for LCS:");
        System.out.println("String 1: \"" + str1 + "\"");
        System.out.println("String 2: \"" + str2 + "\"");
        System.out.println();
        
        // Print header
        System.out.print("    ε ");
        for (char c : str2.toCharArray()) {
            System.out.printf("%2c ", c);
        }
        System.out.println();
        
        // Print table
        for (int i = 0; i <= m; i++) {
            if (i == 0) {
                System.out.print("ε ");
            } else {
                System.out.printf("%c ", str1.charAt(i - 1));
            }
            
            for (int j = 0; j <= n; j++) {
                System.out.printf("%2d ", dp[i][j]);
            }
            System.out.println();
        }
        System.out.println();
    }
    
    /**
     * Demonstrate step-by-step LCS construction
     */
    public static void demonstrateLCSConstruction(String str1, String str2) {
        System.out.println("Step-by-step LCS construction:");
        System.out.println("String 1: \"" + str1 + "\"");
        System.out.println("String 2: \"" + str2 + "\"");
        System.out.println();
        
        int m = str1.length();
        int n = str2.length();
        int[][] dp = new int[m + 1][n + 1];
        
        System.out.println("Recurrence relation:");
        System.out.println("If str1[i-1] == str2[j-1]: dp[i][j] = dp[i-1][j-1] + 1");
        System.out.println("Else: dp[i][j] = max(dp[i-1][j], dp[i][j-1])");
        System.out.println();
        
        for (int i = 1; i <= m; i++) {
            System.out.printf("Processing str1[%d] = '%c':\n", i - 1, str1.charAt(i - 1));
            
            for (int j = 1; j <= n; j++) {
                if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    System.out.printf("  Match with str2[%d] = '%c': dp[%d][%d] = %d + 1 = %d\n", 
                                    j - 1, str2.charAt(j - 1), i, j, dp[i - 1][j - 1], dp[i][j]);
                } else {
                    dp[i][j] = Math.max(dp[i - 1][j], dp[i][j - 1]);
                    System.out.printf("  No match with str2[%d] = '%c': dp[%d][%d] = max(%d, %d) = %d\n", 
                                    j - 1, str2.charAt(j - 1), i, j, dp[i - 1][j], dp[i][j - 1], dp[i][j]);
                }
            }
            System.out.println();
        }
        
        System.out.printf("Final LCS length: %d\n", dp[m][n]);
        System.out.printf("Actual LCS: \"%s\"\n", lcsString(str1, str2));
    }
    
    /**
     * Find Longest Common Substring (different from subsequence)
     */
    public static String longestCommonSubstring(String str1, String str2) {
        int m = str1.length();
        int n = str2.length();
        int[][] dp = new int[m + 1][n + 1];
        
        int maxLength = 0;
        int endIndex = 0;
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (str1.charAt(i - 1) == str2.charAt(j - 1)) {
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
            return "";
        }
        
        return str1.substring(endIndex - maxLength, endIndex);
    }
    
    public static void main(String[] args) {
        System.out.println("=== Longest Common Subsequence - Dynamic Programming ===");
        
        // Test Case 1: Basic LCS
        System.out.println("Test Case 1: Basic LCS");
        String str1 = "ABCDGH";
        String str2 = "AEDFHR";
        
        int lcsLen = lcsLength(str1, str2);
        String lcs = lcsString(str1, str2);
        
        System.out.printf("String 1: \"%s\"\n", str1);
        System.out.printf("String 2: \"%s\"\n", str2);
        System.out.printf("LCS Length: %d\n", lcsLen);
        System.out.printf("LCS String: \"%s\"\n", lcs);
        System.out.println();
        
        printDPTable(str1, str2);
        
        // Test Case 2: Step-by-step demonstration
        System.out.println("Test Case 2: Step-by-step construction");
        String str3 = "AGGTAB";
        String str4 = "GXTXAYB";
        demonstrateLCSConstruction(str3, str4);
        System.out.println();
        
        // Test Case 3: Multiple LCS
        System.out.println("Test Case 3: Multiple possible LCS");
        String str5 = "ABCD";
        String str6 = "ACBD";
        
        Set<String> allLCSStrings = allLCS(str5, str6);
        System.out.printf("String 1: \"%s\"\n", str5);
        System.out.printf("String 2: \"%s\"\n", str6);
        System.out.printf("LCS Length: %d\n", lcsLength(str5, str6));
        System.out.println("All possible LCS: " + allLCSStrings);
        System.out.println();
        
        // Test Case 4: Edge cases
        System.out.println("Test Case 4: Edge cases");
        
        // Same strings
        String same1 = "HELLO";
        String same2 = "HELLO";
        System.out.printf("Same strings: \"%s\" and \"%s\"\n", same1, same2);
        System.out.printf("LCS: \"%s\" (length: %d)\n", lcsString(same1, same2), lcsLength(same1, same2));
        
        // No common characters
        String diff1 = "ABC";
        String diff2 = "XYZ";
        System.out.printf("No common chars: \"%s\" and \"%s\"\n", diff1, diff2);
        System.out.printf("LCS: \"%s\" (length: %d)\n", lcsString(diff1, diff2), lcsLength(diff1, diff2));
        
        // Empty string
        String empty = "";
        String nonEmpty = "ABC";
        System.out.printf("Empty string: \"%s\" and \"%s\"\n", empty, nonEmpty);
        System.out.printf("LCS: \"%s\" (length: %d)\n", lcsString(empty, nonEmpty), lcsLength(empty, nonEmpty));
        System.out.println();
        
        // Test Case 5: LCS vs LCS (Longest Common Substring)
        System.out.println("Test Case 5: LCS vs Longest Common Substring");
        String sub1 = "GeeksforGeeks";
        String sub2 = "GeeksQuiz";
        
        String lcsResult = lcsString(sub1, sub2);
        String substringResult = longestCommonSubstring(sub1, sub2);
        
        System.out.printf("String 1: \"%s\"\n", sub1);
        System.out.printf("String 2: \"%s\"\n", sub2);
        System.out.printf("Longest Common Subsequence: \"%s\" (length: %d)\n", lcsResult, lcsResult.length());
        System.out.printf("Longest Common Substring: \"%s\" (length: %d)\n", substringResult, substringResult.length());
        System.out.println("Note: Subsequence allows gaps, substring requires contiguous characters");
        System.out.println();
        
        // Test Case 6: Performance comparison
        System.out.println("Test Case 6: Performance comparison");
        String perf1 = "ABCDEFGHIJKLMNOP";
        String perf2 = "ADEFHIJKMNOPQRS";
        
        long startTime = System.nanoTime();
        int result2D = lcsLength(perf1, perf2);
        long time2D = System.nanoTime() - startTime;
        
        startTime = System.nanoTime();
        int resultOptimized = lcsLengthOptimized(perf1, perf2);
        long timeOptimized = System.nanoTime() - startTime;
        
        System.out.printf("2D DP: %d (Time: %d μs)\n", result2D, time2D / 1000);
        System.out.printf("Space optimized: %d (Time: %d μs)\n", resultOptimized, timeOptimized / 1000);
        System.out.println("Both produce same result: " + (result2D == resultOptimized));
        System.out.println();
        
        // Test Case 7: DNA sequence example
        System.out.println("Test Case 7: DNA sequence alignment example");
        String dna1 = "ATCGTACG";
        String dna2 = "ACGTGACG";
        
        String dnaLCS = lcsString(dna1, dna2);
        System.out.printf("DNA Sequence 1: %s\n", dna1);
        System.out.printf("DNA Sequence 2: %s\n", dna2);
        System.out.printf("Common subsequence: %s (length: %d)\n", dnaLCS, dnaLCS.length());
        System.out.printf("Similarity: %.1f%%\n", (double) dnaLCS.length() / Math.max(dna1.length(), dna2.length()) * 100);
        
        // Test Case 8: Palindrome relationship
        System.out.println("\nTest Case 8: Palindrome check using LCS");
        String palindromeTest = "racecar";
        String reversed = new StringBuilder(palindromeTest).reverse().toString();
        int palindromeLCS = lcsLength(palindromeTest, reversed);
        
        System.out.printf("String: \"%s\"\n", palindromeTest);
        System.out.printf("Reversed: \"%s\"\n", reversed);
        System.out.printf("LCS length: %d\n", palindromeLCS);
        System.out.printf("Is palindrome: %s\n", (palindromeLCS == palindromeTest.length()) ? "YES" : "NO");
        System.out.println("Note: If LCS(string, reverse) = length, then string is palindrome");
    }
}