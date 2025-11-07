/**
 * Brute Force Strategy: Linear Search
 * Core Idea: Check every element one by one until target is found or array ends
 * Time Complexity: O(n) where n is the number of elements
 * Space Complexity: O(1) - constant extra space
 */
public class LinearSearch {
    
    /**
     * Search for a target element in an array using linear search
     * @param arr Array to search in
     * @param target Element to search for
     * @return Index of target element if found, -1 otherwise
     */
    public static int linearSearch(int[] arr, int target) {
        // Check each element one by one
        for (int i = 0; i < arr.length; i++) {
            // If current element matches target, return its index
            if (arr[i] == target) {
                return i;
            }
        }
        // Target not found
        return -1;
    }
    
    /**
     * Search for all occurrences of a target element
     * @param arr Array to search in
     * @param target Element to search for
     * @return Array of indices where target is found
     */
    public static int[] findAllOccurrences(int[] arr, int target) {
        // First pass: count occurrences
        int count = 0;
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] == target) {
                count++;
            }
        }
        
        // If no occurrences found
        if (count == 0) {
            return new int[0];
        }
        
        // Second pass: collect indices
        int[] indices = new int[count];
        int index = 0;
        for (int i = 0; i < arr.length; i++) {
            if (arr[i] == target) {
                indices[index++] = i;
            }
        }
        
        return indices;
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
        System.out.println("=== Linear Search - Brute Force ===");
        
        // Test Case 1: Target found in middle
        int[] arr1 = {10, 25, 30, 45, 60, 75};
        int target1 = 30;
        System.out.print("Array: ");
        printArray(arr1);
        System.out.println("\nTarget: " + target1);
        int result1 = linearSearch(arr1, target1);
        System.out.println("Result: " + (result1 != -1 ? "Found at index " + result1 : "Not found"));
        System.out.println();
        
        // Test Case 2: Target not found
        int[] arr2 = {5, 12, 18, 26, 33};
        int target2 = 20;
        System.out.print("Array: ");
        printArray(arr2);
        System.out.println("\nTarget: " + target2);
        int result2 = linearSearch(arr2, target2);
        System.out.println("Result: " + (result2 != -1 ? "Found at index " + result2 : "Not found"));
        System.out.println();
        
        // Test Case 3: Target at first position
        int[] arr3 = {99, 42, 17, 8, 3};
        int target3 = 99;
        System.out.print("Array: ");
        printArray(arr3);
        System.out.println("\nTarget: " + target3);
        int result3 = linearSearch(arr3, target3);
        System.out.println("Result: " + (result3 != -1 ? "Found at index " + result3 : "Not found"));
        System.out.println();
        
        // Test Case 4: Multiple occurrences
        int[] arr4 = {7, 3, 7, 1, 7, 9, 7};
        int target4 = 7;
        System.out.print("Array: ");
        printArray(arr4);
        System.out.println("\nTarget: " + target4);
        int[] allIndices = findAllOccurrences(arr4, target4);
        System.out.print("All occurrences at indices: ");
        printArray(allIndices);
        System.out.println();
        System.out.println();
        
        // Test Case 5: Empty array
        int[] arr5 = {};
        int target5 = 10;
        System.out.print("Array: ");
        printArray(arr5);
        System.out.println("\nTarget: " + target5);
        int result5 = linearSearch(arr5, target5);
        System.out.println("Result: " + (result5 != -1 ? "Found at index " + result5 : "Not found"));
    }
}