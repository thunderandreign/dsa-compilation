import java.util.*;

/**
 * Brute Force Strategy: Subset Sum Problem
 * Core Idea: Check all possible subsets to find one that sums to target
 * Time Complexity: O(2^n) where n is the number of elements
 * Space Complexity: O(2^n) for storing all subsets
 */
public class SubsetSum {
    
    /**
     * Find all subsets that sum to the target value using brute force
     * @param arr Input array
     * @param target Target sum
     * @return List of all subsets that sum to target
     */
    public static List<List<Integer>> findSubsetsWithSum(int[] arr, int target) {
        List<List<Integer>> result = new ArrayList<>();
        int n = arr.length;
        
        // Generate all possible subsets using bit manipulation
        // There are 2^n possible subsets
        for (int mask = 0; mask < (1 << n); mask++) {
            List<Integer> subset = new ArrayList<>();
            int sum = 0;
            
            // Check each bit in the mask
            for (int i = 0; i < n; i++) {
                // If i-th bit is set, include arr[i] in subset
                if ((mask & (1 << i)) != 0) {
                    subset.add(arr[i]);
                    sum += arr[i];
                }
            }
            
            // If subset sum equals target, add to result
            if (sum == target) {
                result.add(subset);
            }
        }
        
        return result;
    }
    
    /**
     * Check if there exists any subset that sums to target
     * @param arr Input array
     * @param target Target sum
     * @return true if such subset exists, false otherwise
     */
    public static boolean hasSubsetWithSum(int[] arr, int target) {
        int n = arr.length;
        
        // Generate all possible subsets
        for (int mask = 0; mask < (1 << n); mask++) {
            int sum = 0;
            
            // Calculate sum of current subset
            for (int i = 0; i < n; i++) {
                if ((mask & (1 << i)) != 0) {
                    sum += arr[i];
                }
            }
            
            // If sum equals target, we found a solution
            if (sum == target) {
                return true;
            }
        }
        
        return false;
    }
    
    /**
     * Find the first subset that sums to target
     * @param arr Input array
     * @param target Target sum
     * @return First subset found, or empty list if none exists
     */
    public static List<Integer> findFirstSubsetWithSum(int[] arr, int target) {
        int n = arr.length;
        
        for (int mask = 0; mask < (1 << n); mask++) {
            List<Integer> subset = new ArrayList<>();
            int sum = 0;
            
            for (int i = 0; i < n; i++) {
                if ((mask & (1 << i)) != 0) {
                    subset.add(arr[i]);
                    sum += arr[i];
                }
            }
            
            if (sum == target) {
                return subset;
            }
        }
        
        return new ArrayList<>(); // Empty list if no solution
    }
    
    /**
     * Helper method to print array
     */
    public static void printArray(int[] arr) {
        System.out.print("[");
        for (int i = 0; i < arr.length; i++) {
            System.out.print(arr[i]);
            if (i < arr.length - 1) System.out.print(", ");
        }
        System.out.print("]");
    }
    
    public static void main(String[] args) {
        System.out.println("=== Subset Sum Problem - Brute Force ===");
        
        // Test Case 1: Multiple solutions exist
        int[] arr1 = {3, 34, 4, 12, 5, 2};
        int target1 = 9;
        System.out.print("Array: ");
        printArray(arr1);
        System.out.println("\nTarget sum: " + target1);
        
        List<List<Integer>> solutions1 = findSubsetsWithSum(arr1, target1);
        System.out.println("Number of solutions: " + solutions1.size());
        for (int i = 0; i < solutions1.size(); i++) {
            System.out.println("Solution " + (i + 1) + ": " + solutions1.get(i));
        }
        System.out.println();
        
        // Test Case 2: No solution exists
        int[] arr2 = {1, 3, 5, 7};
        int target2 = 2;
        System.out.print("Array: ");
        printArray(arr2);
        System.out.println("\nTarget sum: " + target2);
        
        boolean hasSolution2 = hasSubsetWithSum(arr2, target2);
        System.out.println("Has solution: " + hasSolution2);
        
        List<Integer> firstSolution2 = findFirstSubsetWithSum(arr2, target2);
        System.out.println("First solution: " + (firstSolution2.isEmpty() ? "None" : firstSolution2));
        System.out.println();
        
        // Test Case 3: Target is 0 (empty subset)
        int[] arr3 = {1, 2, 3, 4};
        int target3 = 0;
        System.out.print("Array: ");
        printArray(arr3);
        System.out.println("\nTarget sum: " + target3);
        
        List<Integer> solution3 = findFirstSubsetWithSum(arr3, target3);
        System.out.println("Solution: " + solution3 + " (empty subset)");
        System.out.println();
        
        // Test Case 4: Single element array
        int[] arr4 = {5};
        int target4 = 5;
        System.out.print("Array: ");
        printArray(arr4);
        System.out.println("\nTarget sum: " + target4);
        
        List<Integer> solution4 = findFirstSubsetWithSum(arr4, target4);
        System.out.println("Solution: " + solution4);
        System.out.println();
        
        // Test Case 5: All elements sum to target
        int[] arr5 = {2, 3, 4};
        int target5 = 9;
        System.out.print("Array: ");
        printArray(arr5);
        System.out.println("\nTarget sum: " + target5);
        
        List<Integer> solution5 = findFirstSubsetWithSum(arr5, target5);
        System.out.println("Solution: " + solution5);
    }
}