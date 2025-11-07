/**
 * Backtracking Strategy: Permutation Generation
 * Core Idea: Generate all possible arrangements (permutations) of a given set of elements
 * Time Complexity: O(n! * n) where n is the number of elements
 * Space Complexity: O(n) for recursion stack and current permutation
 */

import java.util.*;

public class PermutationGeneration {
    
    /**
     * Generate all permutations of an array
     * @param elements Array of elements to permute
     * @return List of all permutations
     */
    public static List<List<Integer>> generatePermutations(int[] elements) {
        List<List<Integer>> result = new ArrayList<>();
        List<Integer> current = new ArrayList<>();
        boolean[] used = new boolean[elements.length];
        
        backtrackPermutations(elements, current, used, result);
        return result;
    }
    
    /**
     * Backtracking helper for generating permutations
     * @param elements Original array
     * @param current Current permutation being built
     * @param used Boolean array to track used elements
     * @param result List to store all permutations
     */
    private static void backtrackPermutations(int[] elements, List<Integer> current, 
                                            boolean[] used, List<List<Integer>> result) {
        // Base case: permutation is complete
        if (current.size() == elements.length) {
            result.add(new ArrayList<>(current));
            return;
        }
        
        // Try each unused element
        for (int i = 0; i < elements.length; i++) {
            if (!used[i]) {
                // Choose
                current.add(elements[i]);
                used[i] = true;
                
                // Explore
                backtrackPermutations(elements, current, used, result);
                
                // Unchoose (backtrack)
                current.remove(current.size() - 1);
                used[i] = false;
            }
        }
    }
    
    /**
     * Generate permutations with duplicates handled
     * @param elements Array that may contain duplicates
     * @return List of unique permutations
     */
    public static List<List<Integer>> generateUniquePermutations(int[] elements) {
        List<List<Integer>> result = new ArrayList<>();
        List<Integer> current = new ArrayList<>();
        boolean[] used = new boolean[elements.length];
        
        // Sort to handle duplicates
        Arrays.sort(elements);
        
        backtrackUniquePermutations(elements, current, used, result);
        return result;
    }
    
    /**
     * Backtracking for unique permutations (handles duplicates)
     */
    private static void backtrackUniquePermutations(int[] elements, List<Integer> current, 
                                                   boolean[] used, List<List<Integer>> result) {
        if (current.size() == elements.length) {
            result.add(new ArrayList<>(current));
            return;
        }
        
        for (int i = 0; i < elements.length; i++) {
            if (used[i]) continue;
            
            // Skip duplicates: if current element is same as previous and previous is not used
            if (i > 0 && elements[i] == elements[i - 1] && !used[i - 1]) {
                continue;
            }
            
            current.add(elements[i]);
            used[i] = true;
            
            backtrackUniquePermutations(elements, current, used, result);
            
            current.remove(current.size() - 1);
            used[i] = false;
        }
    }
    
    /**
     * Generate k-permutations (permutations of length k)
     * @param elements Array of elements
     * @param k Length of permutations
     * @return List of k-permutations
     */
    public static List<List<Integer>> generateKPermutations(int[] elements, int k) {
        List<List<Integer>> result = new ArrayList<>();
        List<Integer> current = new ArrayList<>();
        boolean[] used = new boolean[elements.length];
        
        backtrackKPermutations(elements, k, current, used, result);
        return result;
    }
    
    /**
     * Backtracking for k-permutations
     */
    private static void backtrackKPermutations(int[] elements, int k, List<Integer> current, 
                                             boolean[] used, List<List<Integer>> result) {
        if (current.size() == k) {
            result.add(new ArrayList<>(current));
            return;
        }
        
        for (int i = 0; i < elements.length; i++) {
            if (!used[i]) {
                current.add(elements[i]);
                used[i] = true;
                
                backtrackKPermutations(elements, k, current, used, result);
                
                current.remove(current.size() - 1);
                used[i] = false;
            }
        }
    }
    
    /**
     * Generate permutations with step tracking
     * @param elements Array of elements
     * @param steps List to store steps
     * @return List of permutations
     */
    public static List<List<Integer>> generateWithSteps(int[] elements, List<String> steps) {
        List<List<Integer>> result = new ArrayList<>();
        List<Integer> current = new ArrayList<>();
        boolean[] used = new boolean[elements.length];
        
        backtrackWithSteps(elements, current, used, result, steps, 1);
        return result;
    }
    
    /**
     * Backtracking with step tracking
     */
    private static void backtrackWithSteps(int[] elements, List<Integer> current, boolean[] used, 
                                         List<List<Integer>> result, List<String> steps, int stepNum) {
        steps.add(String.format("Step %d: Current permutation: %s, Used: %s", 
                                stepNum, current, Arrays.toString(used)));
        
        if (current.size() == elements.length) {
            steps.add("✓ Complete permutation found: " + current);
            result.add(new ArrayList<>(current));
            return;
        }
        
        steps.add(String.format("  Need %d more elements", elements.length - current.size()));
        
        for (int i = 0; i < elements.length; i++) {
            if (!used[i]) {
                steps.add(String.format("  Trying element %d at position %d", elements[i], current.size()));
                
                // Choose
                current.add(elements[i]);
                used[i] = true;
                
                // Explore
                backtrackWithSteps(elements, current, used, result, steps, stepNum + 1);
                
                // Unchoose
                steps.add(String.format("  Backtracking: removing %d from position %d", 
                                       elements[i], current.size() - 1));
                current.remove(current.size() - 1);
                used[i] = false;
            }
        }
    }
    
    /**
     * Generate permutations using Heap's algorithm (iterative, more efficient)
     * @param elements Array of elements
     * @return List of all permutations
     */
    public static List<List<Integer>> generatePermutationsHeap(int[] elements) {
        List<List<Integer>> result = new ArrayList<>();
        int n = elements.length;
        int[] arr = elements.clone();
        
        // Initialize control array
        int[] c = new int[n];
        Arrays.fill(c, 0);
        
        // Add initial permutation
        result.add(Arrays.stream(arr).boxed().collect(ArrayList::new, ArrayList::add, ArrayList::addAll));
        
        int i = 0;
        while (i < n) {
            if (c[i] < i) {
                if (i % 2 == 0) {
                    // Swap first and i-th element
                    swap(arr, 0, i);
                } else {
                    // Swap c[i]-th and i-th element
                    swap(arr, c[i], i);
                }
                
                result.add(Arrays.stream(arr).boxed().collect(ArrayList::new, ArrayList::add, ArrayList::addAll));
                c[i]++;
                i = 0;
            } else {
                c[i] = 0;
                i++;
            }
        }
        
        return result;
    }
    
    /**
     * Swap two elements in array
     */
    private static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    
    /**
     * Generate next lexicographic permutation
     * @param arr Array to permute (modified in place)
     * @return true if next permutation exists, false if this is the last permutation
     */
    public static boolean nextPermutation(int[] arr) {
        int n = arr.length;
        
        // Find largest index i such that arr[i] < arr[i + 1]
        int i = n - 2;
        while (i >= 0 && arr[i] >= arr[i + 1]) {
            i--;
        }
        
        if (i == -1) {
            return false; // No next permutation (this is the last one)
        }
        
        // Find largest index j such that arr[i] < arr[j]
        int j = n - 1;
        while (arr[j] <= arr[i]) {
            j--;
        }
        
        // Swap arr[i] and arr[j]
        swap(arr, i, j);
        
        // Reverse suffix starting at arr[i + 1]
        reverse(arr, i + 1, n - 1);
        
        return true;
    }
    
    /**
     * Reverse array elements from start to end
     */
    private static void reverse(int[] arr, int start, int end) {
        while (start < end) {
            swap(arr, start, end);
            start++;
            end--;
        }
    }
    
    /**
     * Generate all permutations using next permutation method
     * @param elements Array of elements (will be sorted first)
     * @return List of all permutations
     */
    public static List<List<Integer>> generatePermutationsIterative(int[] elements) {
        List<List<Integer>> result = new ArrayList<>();
        int[] arr = elements.clone();
        Arrays.sort(arr); // Start with smallest permutation
        
        do {
            result.add(Arrays.stream(arr).boxed().collect(ArrayList::new, ArrayList::add, ArrayList::addAll));
        } while (nextPermutation(arr));
        
        return result;
    }
    
    /**
     * Count total number of permutations without generating them
     * @param elements Array of elements
     * @return Number of permutations
     */
    public static long countPermutations(int[] elements) {
        int n = elements.length;
        long factorial = 1;
        for (int i = 1; i <= n; i++) {
            factorial *= i;
        }
        
        // Account for duplicates
        Map<Integer, Integer> frequency = new HashMap<>();
        for (int element : elements) {
            frequency.put(element, frequency.getOrDefault(element, 0) + 1);
        }
        
        for (int freq : frequency.values()) {
            if (freq > 1) {
                long freqFactorial = 1;
                for (int i = 1; i <= freq; i++) {
                    freqFactorial *= i;
                }
                factorial /= freqFactorial;
            }
        }
        
        return factorial;
    }
    
    /**
     * Generate permutations of a string
     * @param str Input string
     * @return List of all string permutations
     */
    public static List<String> generateStringPermutations(String str) {
        List<String> result = new ArrayList<>();
        char[] chars = str.toCharArray();
        Arrays.sort(chars);
        boolean[] used = new boolean[chars.length];
        
        backtrackStringPermutations(chars, new StringBuilder(), used, result);
        return result;
    }
    
    /**
     * Backtracking for string permutations
     */
    private static void backtrackStringPermutations(char[] chars, StringBuilder current, 
                                                   boolean[] used, List<String> result) {
        if (current.length() == chars.length) {
            result.add(current.toString());
            return;
        }
        
        for (int i = 0; i < chars.length; i++) {
            if (used[i]) continue;
            
            // Skip duplicates
            if (i > 0 && chars[i] == chars[i - 1] && !used[i - 1]) {
                continue;
            }
            
            current.append(chars[i]);
            used[i] = true;
            
            backtrackStringPermutations(chars, current, used, result);
            
            current.deleteCharAt(current.length() - 1);
            used[i] = false;
        }
    }
    
    /**
     * Print a list of permutations (limited output)
     */
    private static void printPermutations(List<List<Integer>> permutations, int limit) {
        System.out.println("Permutations (showing first " + Math.min(limit, permutations.size()) + "):");
        for (int i = 0; i < Math.min(limit, permutations.size()); i++) {
            System.out.println("  " + permutations.get(i));
        }
        if (permutations.size() > limit) {
            System.out.println("  ... (" + (permutations.size() - limit) + " more)");
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Permutation Generation - Backtracking ===\n");
        
        // Test Case 1: Basic permutations
        System.out.println("Test Case 1: Basic Permutations");
        int[] elements1 = {1, 2, 3};
        
        System.out.println("Elements: " + Arrays.toString(elements1));
        List<List<Integer>> perms1 = generatePermutations(elements1);
        printPermutations(perms1, 10);
        System.out.println("Total permutations: " + perms1.size());
        System.out.println("Expected: 3! = 6\n");
        
        // Test Case 2: Step-by-step demonstration
        System.out.println("Test Case 2: Step-by-step generation");
        int[] elements2 = {1, 2};
        
        System.out.println("Elements: " + Arrays.toString(elements2));
        List<String> steps = new ArrayList<>();
        List<List<Integer>> perms2 = generateWithSteps(elements2, steps);
        
        System.out.println("Generation steps (first 15):");
        for (int i = 0; i < Math.min(15, steps.size()); i++) {
            System.out.println(steps.get(i));
        }
        if (steps.size() > 15) {
            System.out.println("... (" + (steps.size() - 15) + " more steps)");
        }
        System.out.println();
        
        // Test Case 3: Permutations with duplicates
        System.out.println("Test Case 3: Permutations with duplicates");
        int[] elements3 = {1, 1, 2};
        
        System.out.println("Elements: " + Arrays.toString(elements3));
        List<List<Integer>> allPerms3 = generatePermutations(elements3);
        List<List<Integer>> uniquePerms3 = generateUniquePermutations(elements3);
        
        System.out.println("All permutations (including duplicates): " + allPerms3.size());
        printPermutations(allPerms3, 8);
        
        System.out.println("Unique permutations: " + uniquePerms3.size());
        printPermutations(uniquePerms3, 8);
        System.out.println();
        
        // Test Case 4: K-permutations
        System.out.println("Test Case 4: K-Permutations");
        int[] elements4 = {1, 2, 3, 4};
        int k = 2;
        
        System.out.println("Elements: " + Arrays.toString(elements4));
        System.out.println("K = " + k);
        
        List<List<Integer>> kPerms = generateKPermutations(elements4, k);
        printPermutations(kPerms, 15);
        System.out.println("Total " + k + "-permutations: " + kPerms.size());
        System.out.println("Expected: P(4,2) = 4!/(4-2)! = 12\n");
        
        // Test Case 5: Performance comparison
        System.out.println("Test Case 5: Performance Comparison");
        int[] elements5 = {1, 2, 3, 4, 5};
        
        System.out.println("Elements: " + Arrays.toString(elements5));
        
        long startTime = System.currentTimeMillis();
        List<List<Integer>> backtrackPerms = generatePermutations(elements5);
        long backtrackTime = System.currentTimeMillis() - startTime;
        
        startTime = System.currentTimeMillis();
        List<List<Integer>> heapPerms = generatePermutationsHeap(elements5);
        long heapTime = System.currentTimeMillis() - startTime;
        
        startTime = System.currentTimeMillis();
        List<List<Integer>> iterativePerms = generatePermutationsIterative(elements5);
        long iterativeTime = System.currentTimeMillis() - startTime;
        
        System.out.println("Backtracking: " + backtrackPerms.size() + " permutations (Time: " + backtrackTime + "ms)");
        System.out.println("Heap's algorithm: " + heapPerms.size() + " permutations (Time: " + heapTime + "ms)");
        System.out.println("Iterative: " + iterativePerms.size() + " permutations (Time: " + iterativeTime + "ms)");
        System.out.println();
        
        // Test Case 6: String permutations
        System.out.println("Test Case 6: String Permutations");
        String str = "abc";
        
        System.out.println("String: \"" + str + "\"");
        List<String> stringPerms = generateStringPermutations(str);
        
        System.out.println("String permutations:");
        for (String perm : stringPerms) {
            System.out.println("  \"" + perm + "\"");
        }
        System.out.println("Total: " + stringPerms.size() + "\n");
        
        // Test Case 7: Next permutation demonstration
        System.out.println("Test Case 7: Next Permutation Method");
        int[] arr = {1, 2, 3};
        
        System.out.println("Starting with: " + Arrays.toString(arr));
        System.out.println("All permutations in lexicographic order:");
        
        int count = 1;
        System.out.println(count + ": " + Arrays.toString(arr));
        
        while (nextPermutation(arr)) {
            count++;
            System.out.println(count + ": " + Arrays.toString(arr));
        }
        System.out.println();
        
        // Test Case 8: Large scale counting
        System.out.println("Test Case 8: Counting Permutations");
        
        for (int n = 1; n <= 10; n++) {
            int[] elements = new int[n];
            for (int i = 0; i < n; i++) {
                elements[i] = i + 1;
            }
            
            long count = countPermutations(elements);
            System.out.println("Elements [1.." + n + "]: " + count + " permutations");
        }
        System.out.println();
        
        // Test Case 9: Duplicates counting
        System.out.println("Test Case 9: Permutations with duplicates counting");
        
        int[] dup1 = {1, 1, 2};
        int[] dup2 = {1, 2, 2, 3};
        int[] dup3 = {1, 1, 1, 2, 2};
        
        System.out.println("Array " + Arrays.toString(dup1) + ": " + countPermutations(dup1) + " unique permutations");
        System.out.println("Array " + Arrays.toString(dup2) + ": " + countPermutations(dup2) + " unique permutations");
        System.out.println("Array " + Arrays.toString(dup3) + ": " + countPermutations(dup3) + " unique permutations");
        System.out.println();
        
        // Test Case 10: Edge cases
        System.out.println("Test Case 10: Edge Cases");
        
        // Empty array
        int[] empty = {};
        List<List<Integer>> emptyPerms = generatePermutations(empty);
        System.out.println("Empty array: " + emptyPerms.size() + " permutations");
        
        // Single element
        int[] single = {42};
        List<List<Integer>> singlePerms = generatePermutations(single);
        System.out.println("Single element: " + singlePerms.size() + " permutations");
        printPermutations(singlePerms, 5);
        
        // All same elements
        int[] allSame = {2, 2, 2};
        List<List<Integer>> allSamePerms = generateUniquePermutations(allSame);
        System.out.println("All same elements: " + allSamePerms.size() + " unique permutations");
        printPermutations(allSamePerms, 5);
        System.out.println();
        
        System.out.println("Applications:");
        System.out.println("- Traveling Salesman Problem: permutations of cities");
        System.out.println("- Anagram generation: permutations of letters");
        System.out.println("- Task scheduling: permutations of tasks");
        System.out.println("- Combinatorial optimization");
        System.out.println("- Cryptography: key generation");
        
        System.out.println("\nComplexity Analysis:");
        System.out.println("- Time: O(n! * n) for generating all permutations");
        System.out.println("- Space: O(n) for recursion stack");
        System.out.println("- Total permutations: n! for distinct elements");
        System.out.println("- With duplicates: n! / (n1! * n2! * ... * nk!)");
        System.out.println("- Optimizations: Heap's algorithm, iterative generation");
    }
}