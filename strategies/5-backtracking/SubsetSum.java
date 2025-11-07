/**
 * Backtracking Strategy: Subset Sum Problem
 * Core Idea: Find if there exists a subset of given numbers that sum to a target value
 * Time Complexity: O(2^n) where n is the number of elements (worst case)
 * Space Complexity: O(n) for recursion stack and subset tracking
 */

import java.util.*;

public class SubsetSum {
    
    /**
     * Check if there exists a subset with given sum using backtracking
     * @param numbers Array of numbers
     * @param targetSum Target sum to achieve
     * @return true if subset exists, false otherwise
     */
    public static boolean hasSubsetSum(int[] numbers, int targetSum) {
        List<Integer> currentSubset = new ArrayList<>();
        return backtrackSubsetSum(numbers, 0, targetSum, 0, currentSubset);
    }
    
    /**
     * Backtracking helper function for subset sum
     * @param numbers Array of numbers
     * @param index Current index in the array
     * @param targetSum Target sum
     * @param currentSum Current sum of selected elements
     * @param currentSubset Current subset being built
     * @return true if target sum can be achieved, false otherwise
     */
    private static boolean backtrackSubsetSum(int[] numbers, int index, int targetSum, 
                                            int currentSum, List<Integer> currentSubset) {
        // Base case: reached target sum
        if (currentSum == targetSum) {
            return true;
        }
        
        // Base case: exceeded target or no more elements
        if (currentSum > targetSum || index >= numbers.length) {
            return false;
        }
        
        // Include current element
        currentSubset.add(numbers[index]);
        if (backtrackSubsetSum(numbers, index + 1, targetSum, currentSum + numbers[index], currentSubset)) {
            return true;
        }
        
        // Backtrack: exclude current element
        currentSubset.remove(currentSubset.size() - 1);
        if (backtrackSubsetSum(numbers, index + 1, targetSum, currentSum, currentSubset)) {
            return true;
        }
        
        return false;
    }
    
    /**
     * Find one subset that sums to target value
     * @param numbers Array of numbers
     * @param targetSum Target sum
     * @return List representing the subset, or null if no subset exists
     */
    public static List<Integer> findSubsetSum(int[] numbers, int targetSum) {
        List<Integer> result = new ArrayList<>();
        if (findSubsetSumHelper(numbers, 0, targetSum, 0, new ArrayList<>(), result)) {
            return result;
        }
        return null;
    }
    
    /**
     * Helper method to find and store one valid subset
     */
    private static boolean findSubsetSumHelper(int[] numbers, int index, int targetSum, 
                                             int currentSum, List<Integer> currentSubset, 
                                             List<Integer> result) {
        if (currentSum == targetSum) {
            result.addAll(currentSubset);
            return true;
        }
        
        if (currentSum > targetSum || index >= numbers.length) {
            return false;
        }
        
        // Include current element
        currentSubset.add(numbers[index]);
        if (findSubsetSumHelper(numbers, index + 1, targetSum, currentSum + numbers[index], currentSubset, result)) {
            return true;
        }
        
        // Backtrack: exclude current element
        currentSubset.remove(currentSubset.size() - 1);
        if (findSubsetSumHelper(numbers, index + 1, targetSum, currentSum, currentSubset, result)) {
            return true;
        }
        
        return false;
    }
    
    /**
     * Find all subsets that sum to target value
     * @param numbers Array of numbers
     * @param targetSum Target sum
     * @return List of all valid subsets
     */
    public static List<List<Integer>> findAllSubsetSums(int[] numbers, int targetSum) {
        List<List<Integer>> allSubsets = new ArrayList<>();
        List<Integer> currentSubset = new ArrayList<>();
        findAllSubsetsHelper(numbers, 0, targetSum, 0, currentSubset, allSubsets);
        return allSubsets;
    }
    
    /**
     * Helper method to find all valid subsets
     */
    private static void findAllSubsetsHelper(int[] numbers, int index, int targetSum, 
                                           int currentSum, List<Integer> currentSubset, 
                                           List<List<Integer>> allSubsets) {
        if (currentSum == targetSum) {
            allSubsets.add(new ArrayList<>(currentSubset));
            return;
        }
        
        if (currentSum > targetSum || index >= numbers.length) {
            return;
        }
        
        // Include current element
        currentSubset.add(numbers[index]);
        findAllSubsetsHelper(numbers, index + 1, targetSum, currentSum + numbers[index], currentSubset, allSubsets);
        
        // Backtrack: exclude current element
        currentSubset.remove(currentSubset.size() - 1);
        findAllSubsetsHelper(numbers, index + 1, targetSum, currentSum, currentSubset, allSubsets);
    }
    
    /**
     * Solve with step tracking for demonstration
     * @param numbers Array of numbers
     * @param targetSum Target sum
     * @param steps List to store solution steps
     * @return true if solution exists, false otherwise
     */
    public static boolean solveWithSteps(int[] numbers, int targetSum, List<String> steps) {
        List<Integer> currentSubset = new ArrayList<>();
        return backtrackWithSteps(numbers, 0, targetSum, 0, currentSubset, steps, 1);
    }
    
    /**
     * Backtracking with step tracking
     */
    private static boolean backtrackWithSteps(int[] numbers, int index, int targetSum, 
                                            int currentSum, List<Integer> currentSubset, 
                                            List<String> steps, int stepNum) {
        steps.add(String.format("Step %d: Index=%d, CurrentSum=%d, Target=%d, Subset=%s", 
                                stepNum, index, currentSum, targetSum, currentSubset));
        
        if (currentSum == targetSum) {
            steps.add("✓ Found target sum! Subset: " + currentSubset);
            return true;
        }
        
        if (currentSum > targetSum) {
            steps.add("✗ Sum exceeded target, backtracking");
            return false;
        }
        
        if (index >= numbers.length) {
            steps.add("✗ No more elements, backtracking");
            return false;
        }
        
        // Try including current element
        steps.add(String.format("  Trying to include %d", numbers[index]));
        currentSubset.add(numbers[index]);
        if (backtrackWithSteps(numbers, index + 1, targetSum, currentSum + numbers[index], 
                              currentSubset, steps, stepNum + 1)) {
            return true;
        }
        
        // Backtrack: exclude current element
        currentSubset.remove(currentSubset.size() - 1);
        steps.add(String.format("  Trying to exclude %d", numbers[index]));
        if (backtrackWithSteps(numbers, index + 1, targetSum, currentSum, 
                              currentSubset, steps, stepNum + 1)) {
            return true;
        }
        
        return false;
    }
    
    /**
     * Optimized version with pruning
     * @param numbers Sorted array of numbers
     * @param targetSum Target sum
     * @return true if subset exists, false otherwise
     */
    public static boolean hasSubsetSumOptimized(int[] numbers, int targetSum) {
        // Sort for better pruning
        int[] sortedNumbers = numbers.clone();
        Arrays.sort(sortedNumbers);
        
        return backtrackOptimized(sortedNumbers, 0, targetSum, 0);
    }
    
    /**
     * Optimized backtracking with pruning
     */
    private static boolean backtrackOptimized(int[] numbers, int index, int targetSum, int currentSum) {
        if (currentSum == targetSum) {
            return true;
        }
        
        if (currentSum > targetSum || index >= numbers.length) {
            return false;
        }
        
        // Pruning: if remaining sum cannot reach target
        int remainingSum = 0;
        for (int i = index; i < numbers.length; i++) {
            remainingSum += numbers[i];
        }
        if (currentSum + remainingSum < targetSum) {
            return false;
        }
        
        // Include current element
        if (backtrackOptimized(numbers, index + 1, targetSum, currentSum + numbers[index])) {
            return true;
        }
        
        // Skip duplicates when excluding
        int currentValue = numbers[index];
        while (index < numbers.length && numbers[index] == currentValue) {
            index++;
        }
        
        // Exclude current element
        return backtrackOptimized(numbers, index, targetSum, currentSum);
    }
    
    /**
     * Count number of subsets with given sum
     * @param numbers Array of numbers
     * @param targetSum Target sum
     * @return Number of valid subsets
     */
    public static int countSubsetSums(int[] numbers, int targetSum) {
        return countSubsetsHelper(numbers, 0, targetSum, 0);
    }
    
    /**
     * Helper method to count subsets
     */
    private static int countSubsetsHelper(int[] numbers, int index, int targetSum, int currentSum) {
        if (currentSum == targetSum) {
            return 1;
        }
        
        if (currentSum > targetSum || index >= numbers.length) {
            return 0;
        }
        
        // Include + exclude current element
        return countSubsetsHelper(numbers, index + 1, targetSum, currentSum + numbers[index]) +
               countSubsetsHelper(numbers, index + 1, targetSum, currentSum);
    }
    
    /**
     * Check if array can be partitioned into two equal sum subsets
     * @param numbers Array of numbers
     * @return true if can be partitioned, false otherwise
     */
    public static boolean canPartition(int[] numbers) {
        int totalSum = Arrays.stream(numbers).sum();
        
        // If total sum is odd, cannot partition into equal sums
        if (totalSum % 2 != 0) {
            return false;
        }
        
        return hasSubsetSum(numbers, totalSum / 2);
    }
    
    /**
     * Find subset closest to target sum
     * @param numbers Array of numbers
     * @param targetSum Target sum
     * @return Subset closest to target sum
     */
    public static List<Integer> findClosestSubset(int[] numbers, int targetSum) {
        List<Integer> bestSubset = new ArrayList<>();
        int[] bestDifference = {Integer.MAX_VALUE};
        
        findClosestHelper(numbers, 0, targetSum, 0, new ArrayList<>(), bestSubset, bestDifference);
        return bestSubset;
    }
    
    /**
     * Helper method to find closest subset
     */
    private static void findClosestHelper(int[] numbers, int index, int targetSum, 
                                        int currentSum, List<Integer> currentSubset, 
                                        List<Integer> bestSubset, int[] bestDifference) {
        // Update best if current is closer
        int currentDifference = Math.abs(targetSum - currentSum);
        if (currentDifference < bestDifference[0]) {
            bestDifference[0] = currentDifference;
            bestSubset.clear();
            bestSubset.addAll(currentSubset);
        }
        
        if (index >= numbers.length) {
            return;
        }
        
        // Include current element
        currentSubset.add(numbers[index]);
        findClosestHelper(numbers, index + 1, targetSum, currentSum + numbers[index], 
                         currentSubset, bestSubset, bestDifference);
        
        // Exclude current element
        currentSubset.remove(currentSubset.size() - 1);
        findClosestHelper(numbers, index + 1, targetSum, currentSum, 
                         currentSubset, bestSubset, bestDifference);
    }
    
    /**
     * Print array
     */
    private static void printArray(int[] arr) {
        System.out.print("[");
        for (int i = 0; i < arr.length; i++) {
            System.out.print(arr[i]);
            if (i < arr.length - 1) System.out.print(", ");
        }
        System.out.print("]");
    }
    
    public static void main(String[] args) {
        System.out.println("=== Subset Sum Problem - Backtracking ===\n");
        
        // Test Case 1: Basic subset sum
        System.out.println("Test Case 1: Basic Subset Sum");
        int[] numbers1 = {3, 34, 4, 12, 5, 2};
        int target1 = 9;
        
        System.out.print("Numbers: ");
        printArray(numbers1);
        System.out.println("\nTarget sum: " + target1);
        
        boolean exists1 = hasSubsetSum(numbers1, target1);
        System.out.println("Subset exists: " + exists1);
        
        List<Integer> subset1 = findSubsetSum(numbers1, target1);
        System.out.println("One valid subset: " + subset1);
        
        List<List<Integer>> allSubsets1 = findAllSubsetSums(numbers1, target1);
        System.out.println("All valid subsets: " + allSubsets1);
        System.out.println("Number of subsets: " + countSubsetSums(numbers1, target1));
        System.out.println();
        
        // Test Case 2: Step-by-step demonstration
        System.out.println("Test Case 2: Step-by-step solution");
        int[] numbers2 = {2, 3, 7, 8};
        int target2 = 11;
        
        System.out.print("Numbers: ");
        printArray(numbers2);
        System.out.println("\nTarget sum: " + target2);
        
        List<String> steps = new ArrayList<>();
        boolean solved = solveWithSteps(numbers2, target2, steps);
        
        System.out.println("Solution steps (first 20):");
        for (int i = 0; i < Math.min(20, steps.size()); i++) {
            System.out.println(steps.get(i));
        }
        if (steps.size() > 20) {
            System.out.println("... (" + (steps.size() - 20) + " more steps)");
        }
        System.out.println("Solution found: " + solved);
        System.out.println();
        
        // Test Case 3: No solution case
        System.out.println("Test Case 3: No solution case");
        int[] numbers3 = {1, 3, 5, 7};
        int target3 = 12;
        
        System.out.print("Numbers: ");
        printArray(numbers3);
        System.out.println("\nTarget sum: " + target3);
        
        boolean exists3 = hasSubsetSum(numbers3, target3);
        List<Integer> subset3 = findSubsetSum(numbers3, target3);
        
        System.out.println("Subset exists: " + exists3);
        System.out.println("Subset found: " + subset3);
        System.out.println();
        
        // Test Case 4: Performance comparison
        System.out.println("Test Case 4: Performance comparison");
        int[] numbers4 = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        int target4 = 15;
        
        System.out.print("Numbers: ");
        printArray(numbers4);
        System.out.println("\nTarget sum: " + target4);
        
        long startTime = System.currentTimeMillis();
        boolean basicResult = hasSubsetSum(numbers4, target4);
        long basicTime = System.currentTimeMillis() - startTime;
        
        startTime = System.currentTimeMillis();
        boolean optimizedResult = hasSubsetSumOptimized(numbers4, target4);
        long optimizedTime = System.currentTimeMillis() - startTime;
        
        System.out.println("Basic backtracking: " + basicResult + " (Time: " + basicTime + "ms)");
        System.out.println("Optimized backtracking: " + optimizedResult + " (Time: " + optimizedTime + "ms)");
        System.out.println();
        
        // Test Case 5: Partition problem
        System.out.println("Test Case 5: Equal Partition Problem");
        int[] numbers5 = {1, 5, 11, 5};
        
        System.out.print("Numbers: ");
        printArray(numbers5);
        System.out.println();
        
        boolean canPartition = canPartition(numbers5);
        System.out.println("Can partition into equal sums: " + canPartition);
        
        if (canPartition) {
            int totalSum = Arrays.stream(numbers5).sum();
            List<Integer> partition1 = findSubsetSum(numbers5, totalSum / 2);
            System.out.println("First partition: " + partition1 + " (sum: " + partition1.stream().mapToInt(i -> i).sum() + ")");
        }
        System.out.println();
        
        // Test Case 6: Closest subset
        System.out.println("Test Case 6: Closest Subset");
        int[] numbers6 = {2, 7, 1, 4, 9, 3};
        int target6 = 15;
        
        System.out.print("Numbers: ");
        printArray(numbers6);
        System.out.println("\nTarget sum: " + target6);
        
        List<Integer> closest = findClosestSubset(numbers6, target6);
        int closestSum = closest.stream().mapToInt(i -> i).sum();
        
        System.out.println("Closest subset: " + closest);
        System.out.println("Closest sum: " + closestSum + " (difference: " + Math.abs(target6 - closestSum) + ")");
        System.out.println();
        
        // Test Case 7: Large numbers
        System.out.println("Test Case 7: Large numbers");
        int[] numbers7 = {15, 22, 14, 26, 32, 9, 16, 8};
        int target7 = 53;
        
        System.out.print("Numbers: ");
        printArray(numbers7);
        System.out.println("\nTarget sum: " + target7);
        
        startTime = System.currentTimeMillis();
        List<List<Integer>> allLarge = findAllSubsetSums(numbers7, target7);
        long endTime = System.currentTimeMillis();
        
        System.out.println("Number of valid subsets: " + allLarge.size());
        System.out.println("Time taken: " + (endTime - startTime) + "ms");
        
        if (!allLarge.isEmpty()) {
            System.out.println("First few subsets:");
            for (int i = 0; i < Math.min(5, allLarge.size()); i++) {
                System.out.println("  " + allLarge.get(i) + " (sum: " + allLarge.get(i).stream().mapToInt(x -> x).sum() + ")");
            }
        }
        System.out.println();
        
        // Test Case 8: Edge cases
        System.out.println("Test Case 8: Edge Cases");
        
        // Empty array
        int[] empty = {};
        System.out.println("Empty array, target 0: " + hasSubsetSum(empty, 0));
        System.out.println("Empty array, target 5: " + hasSubsetSum(empty, 5));
        
        // Single element
        int[] single = {5};
        System.out.println("Single element [5], target 5: " + hasSubsetSum(single, 5));
        System.out.println("Single element [5], target 3: " + hasSubsetSum(single, 3));
        
        // Target is 0
        int[] numbers8 = {1, 2, 3};
        System.out.println("Array [1,2,3], target 0: " + hasSubsetSum(numbers8, 0));
        System.out.println("Count of subsets with sum 0: " + countSubsetSums(numbers8, 0));
        System.out.println();
        
        // Test Case 9: Duplicates
        System.out.println("Test Case 9: Array with duplicates");
        int[] duplicates = {2, 2, 2, 2};
        int targetDup = 4;
        
        System.out.print("Numbers: ");
        printArray(duplicates);
        System.out.println("\nTarget sum: " + targetDup);
        
        List<List<Integer>> allDuplicates = findAllSubsetSums(duplicates, targetDup);
        System.out.println("All subsets with sum " + targetDup + ": " + allDuplicates);
        System.out.println("Count: " + countSubsetSums(duplicates, targetDup));
        System.out.println();
        
        System.out.println("Complexity Analysis:");
        System.out.println("- Time: O(2^n) where n is number of elements (worst case)");
        System.out.println("- Space: O(n) for recursion stack");
        System.out.println("- Optimizations: pruning, sorting, duplicate skipping");
        System.out.println("- Applications: partition problem, knapsack, change making");
        System.out.println("- Related: subset sum is NP-Complete problem");
    }
}