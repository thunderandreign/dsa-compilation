/**
 * Divide and Conquer Strategy: Maximum Subarray Sum (Kadane's Algorithm using D&C)
 * Core Idea: Divide array into halves and find max sum in left, right, or crossing middle
 * Time Complexity: O(n log n) using divide and conquer, O(n) with Kadane's algorithm
 * Space Complexity: O(log n) for recursion stack
 */
public class MaximumSubarray {
    
    /**
     * Result class to store subarray information
     */
    static class SubarrayResult {
        int maxSum;
        int startIndex;
        int endIndex;
        
        SubarrayResult(int sum, int start, int end) {
            this.maxSum = sum;
            this.startIndex = start;
            this.endIndex = end;
        }
        
        @Override
        public String toString() {
            return String.format("Sum: %d, Indices: [%d, %d]", maxSum, startIndex, endIndex);
        }
    }
    
    /**
     * Find maximum subarray sum using divide and conquer approach
     * @param arr Input array
     * @return SubarrayResult containing max sum and indices
     */
    public static SubarrayResult maxSubarrayDivideConquer(int[] arr) {
        if (arr == null || arr.length == 0) {
            return new SubarrayResult(0, -1, -1);
        }
        
        return maxSubarrayHelper(arr, 0, arr.length - 1);
    }
    
    /**
     * Recursive helper for divide and conquer approach
     * @param arr Input array
     * @param low Left boundary
     * @param high Right boundary
     * @return SubarrayResult for the range [low, high]
     */
    private static SubarrayResult maxSubarrayHelper(int[] arr, int low, int high) {
        // Base case: single element
        if (low == high) {
            return new SubarrayResult(arr[low], low, high);
        }
        
        // Divide: find middle point
        int mid = low + (high - low) / 2;
        
        // Conquer: find maximum subarray in left and right halves
        SubarrayResult leftResult = maxSubarrayHelper(arr, low, mid);
        SubarrayResult rightResult = maxSubarrayHelper(arr, mid + 1, high);
        
        // Combine: find maximum subarray crossing the middle
        SubarrayResult crossResult = maxCrossingSubarray(arr, low, mid, high);
        
        // Return the maximum of the three
        if (leftResult.maxSum >= rightResult.maxSum && leftResult.maxSum >= crossResult.maxSum) {
            return leftResult;
        } else if (rightResult.maxSum >= leftResult.maxSum && rightResult.maxSum >= crossResult.maxSum) {
            return rightResult;
        } else {
            return crossResult;
        }
    }
    
    /**
     * Find maximum subarray that crosses the middle point
     * @param arr Input array
     * @param low Left boundary
     * @param mid Middle point
     * @param high Right boundary
     * @return SubarrayResult for crossing subarray
     */
    private static SubarrayResult maxCrossingSubarray(int[] arr, int low, int mid, int high) {
        // Find max sum for left half ending at mid
        int leftSum = Integer.MIN_VALUE;
        int sum = 0;
        int maxLeft = mid;
        
        for (int i = mid; i >= low; i--) {
            sum += arr[i];
            if (sum > leftSum) {
                leftSum = sum;
                maxLeft = i;
            }
        }
        
        // Find max sum for right half starting at mid+1
        int rightSum = Integer.MIN_VALUE;
        sum = 0;
        int maxRight = mid + 1;
        
        for (int i = mid + 1; i <= high; i++) {
            sum += arr[i];
            if (sum > rightSum) {
                rightSum = sum;
                maxRight = i;
            }
        }
        
        return new SubarrayResult(leftSum + rightSum, maxLeft, maxRight);
    }
    
    /**
     * Find maximum subarray sum using Kadane's algorithm (more efficient)
     * @param arr Input array
     * @return SubarrayResult containing max sum and indices
     */
    public static SubarrayResult maxSubarrayKadane(int[] arr) {
        if (arr == null || arr.length == 0) {
            return new SubarrayResult(0, -1, -1);
        }
        
        int maxSum = arr[0];
        int currentSum = arr[0];
        int start = 0, end = 0, tempStart = 0;
        
        for (int i = 1; i < arr.length; i++) {
            // If current sum becomes negative, start new subarray
            if (currentSum < 0) {
                currentSum = arr[i];
                tempStart = i;
            } else {
                currentSum += arr[i];
            }
            
            // Update maximum sum and indices if current sum is greater
            if (currentSum > maxSum) {
                maxSum = currentSum;
                start = tempStart;
                end = i;
            }
        }
        
        return new SubarrayResult(maxSum, start, end);
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
    
    /**
     * Helper method to print subarray
     */
    public static void printSubarray(int[] arr, int start, int end) {
        if (start == -1 || end == -1) {
            System.out.print("[]");
            return;
        }
        
        System.out.print("[");
        for (int i = start; i <= end; i++) {
            System.out.print(arr[i]);
            if (i < end) System.out.print(", ");
        }
        System.out.print("]");
    }
    
    public static void main(String[] args) {
        System.out.println("=== Maximum Subarray Sum - Divide and Conquer ===");
        
        // Test Case 1: Mixed positive and negative numbers
        int[] arr1 = {-2, -3, 4, -1, -2, 1, 5, -3};
        System.out.println("Test Case 1: Mixed numbers");
        System.out.print("Array: ");
        printArray(arr1);
        System.out.println();
        
        SubarrayResult result1DC = maxSubarrayDivideConquer(arr1);
        SubarrayResult result1K = maxSubarrayKadane(arr1);
        
        System.out.println("Divide & Conquer: " + result1DC);
        System.out.print("Subarray: ");
        printSubarray(arr1, result1DC.startIndex, result1DC.endIndex);
        System.out.println();
        
        System.out.println("Kadane's Algorithm: " + result1K);
        System.out.print("Subarray: ");
        printSubarray(arr1, result1K.startIndex, result1K.endIndex);
        System.out.println("\n");
        
        // Test Case 2: All negative numbers
        int[] arr2 = {-5, -2, -8, -1, -4};
        System.out.println("Test Case 2: All negative numbers");
        System.out.print("Array: ");
        printArray(arr2);
        System.out.println();
        
        SubarrayResult result2DC = maxSubarrayDivideConquer(arr2);
        SubarrayResult result2K = maxSubarrayKadane(arr2);
        
        System.out.println("Divide & Conquer: " + result2DC);
        System.out.print("Subarray: ");
        printSubarray(arr2, result2DC.startIndex, result2DC.endIndex);
        System.out.println();
        
        System.out.println("Kadane's Algorithm: " + result2K);
        System.out.print("Subarray: ");
        printSubarray(arr2, result2K.startIndex, result2K.endIndex);
        System.out.println("\n");
        
        // Test Case 3: All positive numbers
        int[] arr3 = {1, 2, 3, 4, 5};
        System.out.println("Test Case 3: All positive numbers");
        System.out.print("Array: ");
        printArray(arr3);
        System.out.println();
        
        SubarrayResult result3DC = maxSubarrayDivideConquer(arr3);
        System.out.println("Divide & Conquer: " + result3DC);
        System.out.print("Subarray: ");
        printSubarray(arr3, result3DC.startIndex, result3DC.endIndex);
        System.out.println("\n");
        
        // Test Case 4: Single element
        int[] arr4 = {-7};
        System.out.println("Test Case 4: Single element");
        System.out.print("Array: ");
        printArray(arr4);
        System.out.println();
        
        SubarrayResult result4DC = maxSubarrayDivideConquer(arr4);
        System.out.println("Divide & Conquer: " + result4DC);
        System.out.print("Subarray: ");
        printSubarray(arr4, result4DC.startIndex, result4DC.endIndex);
        System.out.println("\n");
        
        // Test Case 5: Performance comparison
        System.out.println("Test Case 5: Performance comparison");
        int[] largeArr = {1, -3, 2, 1, -1, 3, -2, 4, -1, 2, -5, 1};
        System.out.print("Array: ");
        printArray(largeArr);
        System.out.println();
        
        // Time divide and conquer approach
        long start1 = System.nanoTime();
        SubarrayResult resultDC = maxSubarrayDivideConquer(largeArr);
        long end1 = System.nanoTime();
        
        // Time Kadane's algorithm
        long start2 = System.nanoTime();
        SubarrayResult resultK = maxSubarrayKadane(largeArr);
        long end2 = System.nanoTime();
        
        System.out.println("Divide & Conquer: " + resultDC);
        System.out.println("Time: " + (end1 - start1) + " nanoseconds");
        
        System.out.println("Kadane's Algorithm: " + resultK);
        System.out.println("Time: " + (end2 - start2) + " nanoseconds");
        
        System.out.println("Results match: " + (resultDC.maxSum == resultK.maxSum));
    }
}