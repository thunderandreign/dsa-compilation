import java.util.*;

/**
 * Brute Force Strategy: String Permutations
 * Core Idea: Generate all possible permutations by trying every combination
 * Time Complexity: O(n! * n) where n is the length of string
 * Space Complexity: O(n! * n) for storing all permutations
 */
public class StringPermutations {
    
    /**
     * Generate all permutations of a string using brute force approach
     * @param str Input string
     * @return List of all permutations
     */
    public static List<String> generatePermutations(String str) {
        List<String> result = new ArrayList<>();
        generatePermutationsHelper(str.toCharArray(), 0, result);
        return result;
    }
    
    /**
     * Helper method to generate permutations recursively
     * @param chars Character array to permute
     * @param index Current position being fixed
     * @param result List to store all permutations
     */
    private static void generatePermutationsHelper(char[] chars, int index, List<String> result) {
        // Base case: if we've fixed all positions, add the permutation
        if (index == chars.length) {
            result.add(new String(chars));
            return;
        }
        
        // Try each character at the current position
        for (int i = index; i < chars.length; i++) {
            // Swap current character with the character at index
            swap(chars, index, i);
            
            // Recursively generate permutations for remaining positions
            generatePermutationsHelper(chars, index + 1, result);
            
            // Backtrack: restore original order
            swap(chars, index, i);
        }
    }
    
    /**
     * Swap two characters in an array
     */
    private static void swap(char[] chars, int i, int j) {
        char temp = chars[i];
        chars[i] = chars[j];
        chars[j] = temp;
    }
    
    public static void main(String[] args) {
        // Test Case 1: Simple 3-character string
        System.out.println("=== String Permutations - Brute Force ===");
        String test1 = "ABC";
        System.out.println("Input: \"" + test1 + "\"");
        List<String> permutations1 = generatePermutations(test1);
        System.out.println("Output: " + permutations1);
        System.out.println("Total permutations: " + permutations1.size());
        System.out.println();
        
        // Test Case 2: String with repeated characters
        String test2 = "AAB";
        System.out.println("Input: \"" + test2 + "\"");
        List<String> permutations2 = generatePermutations(test2);
        System.out.println("Output: " + permutations2);
        System.out.println("Total permutations: " + permutations2.size());
        System.out.println();
        
        // Test Case 3: Single character
        String test3 = "X";
        System.out.println("Input: \"" + test3 + "\"");
        List<String> permutations3 = generatePermutations(test3);
        System.out.println("Output: " + permutations3);
        System.out.println("Total permutations: " + permutations3.size());
        System.out.println();
        
        // Test Case 4: Empty string
        String test4 = "";
        System.out.println("Input: \"" + test4 + "\"");
        List<String> permutations4 = generatePermutations(test4);
        System.out.println("Output: " + permutations4);
        System.out.println("Total permutations: " + permutations4.size());
    }
}