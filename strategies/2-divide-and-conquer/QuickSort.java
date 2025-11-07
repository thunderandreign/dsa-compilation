import java.util.Arrays;
import java.util.Random;

/**
 * Divide and Conquer Strategy: Quick Sort
 * Core Idea: Choose a pivot, partition array around pivot, then sort partitions
 * Time Complexity: O(n log n) average case, O(n²) worst case
 * Space Complexity: O(log n) average case for recursion stack
 */
public class QuickSort {
    
    private static Random random = new Random();
    
    /**
     * Sort an array using quick sort algorithm
     * @param arr Array to be sorted
     */
    public static void quickSort(int[] arr) {
        if (arr == null || arr.length <= 1) {
            return; // Base case: array is already sorted
        }
        
        quickSortHelper(arr, 0, arr.length - 1);
    }
    
    /**
     * Recursive helper method for quick sort
     * @param arr Array to sort
     * @param low Starting index
     * @param high Ending index
     */
    private static void quickSortHelper(int[] arr, int low, int high) {
        // Base case: if low >= high, subarray has 0 or 1 element
        if (low < high) {
            // Divide: partition the array and get pivot index
            int pivotIndex = partition(arr, low, high);
            
            // Conquer: recursively sort elements before and after partition
            quickSortHelper(arr, low, pivotIndex - 1);     // Sort left partition
            quickSortHelper(arr, pivotIndex + 1, high);    // Sort right partition
        }
    }
    
    /**
     * Partition the array around a pivot element
     * @param arr Array to partition
     * @param low Starting index
     * @param high Ending index
     * @return Index of the pivot after partitioning
     */
    private static int partition(int[] arr, int low, int high) {
        // Choose the rightmost element as pivot
        int pivot = arr[high];
        
        // Index of smaller element, indicates right position of pivot
        int i = low - 1;
        
        for (int j = low; j < high; j++) {
            // If current element is smaller than or equal to pivot
            if (arr[j] <= pivot) {
                i++; // Increment index of smaller element
                swap(arr, i, j);
            }
        }
        
        // Place pivot in correct position
        swap(arr, i + 1, high);
        return i + 1;
    }
    
    /**
     * Randomized version of quick sort (better average performance)
     * @param arr Array to be sorted
     */
    public static void randomizedQuickSort(int[] arr) {
        if (arr == null || arr.length <= 1) {
            return;
        }
        
        randomizedQuickSortHelper(arr, 0, arr.length - 1);
    }
    
    /**
     * Helper for randomized quick sort
     */
    private static void randomizedQuickSortHelper(int[] arr, int low, int high) {
        if (low < high) {
            // Randomly choose pivot and swap with last element
            int randomIndex = low + random.nextInt(high - low + 1);
            swap(arr, randomIndex, high);
            
            // Partition and recurse
            int pivotIndex = partition(arr, low, high);
            randomizedQuickSortHelper(arr, low, pivotIndex - 1);
            randomizedQuickSortHelper(arr, pivotIndex + 1, high);
        }
    }
    
    /**
     * Swap two elements in an array
     */
    private static void swap(int[] arr, int i, int j) {
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
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
        System.out.println("=== Quick Sort - Divide and Conquer ===");
        
        // Test Case 1: Random unsorted array
        int[] arr1 = {10, 7, 8, 9, 1, 5};
        System.out.println("Test Case 1: Random array (standard quicksort)");
        System.out.print("Before: ");
        printArray(arr1);
        
        int[] sorted1 = copyArray(arr1);
        quickSort(sorted1);
        System.out.print("After:  ");
        printArray(sorted1);
        System.out.println();
        
        // Test Case 2: Same array with randomized quicksort
        System.out.println("Test Case 2: Same array (randomized quicksort)");
        System.out.print("Before: ");
        printArray(arr1);
        
        int[] sorted2 = copyArray(arr1);
        randomizedQuickSort(sorted2);
        System.out.print("After:  ");
        printArray(sorted2);
        System.out.println();
        
        // Test Case 3: Already sorted array (worst case for standard quicksort)
        int[] arr3 = {1, 2, 3, 4, 5, 6};
        System.out.println("Test Case 3: Already sorted (worst case)");
        System.out.print("Before: ");
        printArray(arr3);
        
        int[] sorted3 = copyArray(arr3);
        quickSort(sorted3);
        System.out.print("After:  ");
        printArray(sorted3);
        System.out.println();
        
        // Test Case 4: Array with duplicates
        int[] arr4 = {4, 1, 3, 4, 2, 4, 1};
        System.out.println("Test Case 4: With duplicates");
        System.out.print("Before: ");
        printArray(arr4);
        
        int[] sorted4 = copyArray(arr4);
        quickSort(sorted4);
        System.out.print("After:  ");
        printArray(sorted4);
        System.out.println();
        
        // Test Case 5: Large array performance comparison
        System.out.println("Test Case 5: Performance comparison");
        int[] largeArr = new int[1000];
        for (int i = 0; i < largeArr.length; i++) {
            largeArr[i] = random.nextInt(1000);
        }
        
        // Time standard quicksort
        int[] copy1 = copyArray(largeArr);
        long start1 = System.nanoTime();
        quickSort(copy1);
        long end1 = System.nanoTime();
        
        // Time randomized quicksort
        int[] copy2 = copyArray(largeArr);
        long start2 = System.nanoTime();
        randomizedQuickSort(copy2);
        long end2 = System.nanoTime();
        
        System.out.println("Array size: " + largeArr.length);
        System.out.println("Standard quicksort time: " + (end1 - start1) / 1000 + " microseconds");
        System.out.println("Randomized quicksort time: " + (end2 - start2) / 1000 + " microseconds");
        
        // Verify both are sorted correctly
        Arrays.sort(largeArr); // Use built-in sort for verification
        boolean correct1 = Arrays.equals(copy1, largeArr);
        boolean correct2 = Arrays.equals(copy2, largeArr);
        System.out.println("Standard quicksort correct: " + correct1);
        System.out.println("Randomized quicksort correct: " + correct2);
    }
}