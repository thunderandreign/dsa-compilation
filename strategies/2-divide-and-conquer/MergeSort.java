import java.util.Arrays;

/**
 * Divide and Conquer Strategy: Merge Sort
 * Core Idea: Divide array into halves, sort each half, then merge sorted halves
 * Time Complexity: O(n log n) in all cases
 * Space Complexity: O(n) for temporary arrays during merge
 */
public class MergeSort {
    
    /**
     * Sort an array using merge sort algorithm
     * @param arr Array to be sorted
     */
    public static void mergeSort(int[] arr) {
        if (arr == null || arr.length <= 1) {
            return; // Base case: array is already sorted
        }
        
        // Create a temporary array for merging
        int[] temp = new int[arr.length];
        mergeSortHelper(arr, temp, 0, arr.length - 1);
    }
    
    /**
     * Recursive helper method for merge sort
     * @param arr Original array
     * @param temp Temporary array for merging
     * @param left Left boundary of current subarray
     * @param right Right boundary of current subarray
     */
    private static void mergeSortHelper(int[] arr, int[] temp, int left, int right) {
        // Base case: single element or empty subarray
        if (left >= right) {
            return;
        }
        
        // Divide: find the middle point
        int mid = left + (right - left) / 2;
        
        // Conquer: recursively sort both halves
        mergeSortHelper(arr, temp, left, mid);      // Sort left half
        mergeSortHelper(arr, temp, mid + 1, right); // Sort right half
        
        // Combine: merge the sorted halves
        merge(arr, temp, left, mid, right);
    }
    
    /**
     * Merge two sorted subarrays into one sorted array
     * @param arr Original array
     * @param temp Temporary array
     * @param left Start of left subarray
     * @param mid End of left subarray (start of right is mid+1)
     * @param right End of right subarray
     */
    private static void merge(int[] arr, int[] temp, int left, int mid, int right) {
        // Copy data to temporary array
        for (int i = left; i <= right; i++) {
            temp[i] = arr[i];
        }
        
        // Merge the two sorted subarrays back into arr[left..right]
        int i = left;      // Initial index of left subarray
        int j = mid + 1;   // Initial index of right subarray
        int k = left;      // Initial index of merged subarray
        
        // Compare elements from both subarrays and merge in sorted order
        while (i <= mid && j <= right) {
            if (temp[i] <= temp[j]) {
                arr[k] = temp[i];
                i++;
            } else {
                arr[k] = temp[j];
                j++;
            }
            k++;
        }
        
        // Copy remaining elements of left subarray, if any
        while (i <= mid) {
            arr[k] = temp[i];
            i++;
            k++;
        }
        
        // Copy remaining elements of right subarray, if any
        while (j <= right) {
            arr[k] = temp[j];
            j++;
            k++;
        }
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
        System.out.println("]");
    }
    
    /**
     * Helper method to copy array
     */
    public static int[] copyArray(int[] arr) {
        return Arrays.copyOf(arr, arr.length);
    }
    
    public static void main(String[] args) {
        System.out.println("=== Merge Sort - Divide and Conquer ===");
        
        // Test Case 1: Random unsorted array
        int[] arr1 = {38, 27, 43, 3, 9, 82, 10};
        System.out.println("Test Case 1: Random array");
        System.out.print("Before: ");
        printArray(arr1);
        
        int[] sorted1 = copyArray(arr1);
        mergeSort(sorted1);
        System.out.print("After:  ");
        printArray(sorted1);
        System.out.println();
        
        // Test Case 2: Already sorted array
        int[] arr2 = {1, 2, 3, 4, 5, 6};
        System.out.println("Test Case 2: Already sorted");
        System.out.print("Before: ");
        printArray(arr2);
        
        int[] sorted2 = copyArray(arr2);
        mergeSort(sorted2);
        System.out.print("After:  ");
        printArray(sorted2);
        System.out.println();
        
        // Test Case 3: Reverse sorted array
        int[] arr3 = {6, 5, 4, 3, 2, 1};
        System.out.println("Test Case 3: Reverse sorted");
        System.out.print("Before: ");
        printArray(arr3);
        
        int[] sorted3 = copyArray(arr3);
        mergeSort(sorted3);
        System.out.print("After:  ");
        printArray(sorted3);
        System.out.println();
        
        // Test Case 4: Array with duplicates
        int[] arr4 = {5, 2, 8, 2, 9, 5, 1, 8};
        System.out.println("Test Case 4: With duplicates");
        System.out.print("Before: ");
        printArray(arr4);
        
        int[] sorted4 = copyArray(arr4);
        mergeSort(sorted4);
        System.out.print("After:  ");
        printArray(sorted4);
        System.out.println();
        
        // Test Case 5: Single element and empty array
        int[] arr5 = {42};
        int[] arr6 = {};
        
        System.out.println("Test Case 5: Edge cases");
        System.out.print("Single element before: ");
        printArray(arr5);
        mergeSort(arr5);
        System.out.print("Single element after:  ");
        printArray(arr5);
        
        System.out.print("Empty array before: ");
        printArray(arr6);
        mergeSort(arr6);
        System.out.print("Empty array after:  ");
        printArray(arr6);
    }
}