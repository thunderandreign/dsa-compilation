/**
 * Divide and Conquer Strategy: Binary Search
 * Core Idea: Divide search space in half by comparing with middle element
 * Time Complexity: O(log n) where n is the number of elements
 * Space Complexity: O(1) iterative, O(log n) recursive
 */
public class BinarySearch {
    
    /**
     * Iterative binary search implementation
     * @param arr Sorted array to search in
     * @param target Element to search for
     * @return Index of target element if found, -1 otherwise
     */
    public static int binarySearchIterative(int[] arr, int target) {
        if (arr == null || arr.length == 0) {
            return -1;
        }
        
        int left = 0;
        int right = arr.length - 1;
        
        while (left <= right) {
            // Find middle index (avoid overflow)
            int mid = left + (right - left) / 2;
            
            // Check if target is at mid
            if (arr[mid] == target) {
                return mid;
            }
            
            // If target is greater, ignore left half
            if (arr[mid] < target) {
                left = mid + 1;
            }
            // If target is smaller, ignore right half
            else {
                right = mid - 1;
            }
        }
        
        // Target not found
        return -1;
    }
    
    /**
     * Recursive binary search implementation
     * @param arr Sorted array to search in
     * @param target Element to search for
     * @return Index of target element if found, -1 otherwise
     */
    public static int binarySearchRecursive(int[] arr, int target) {
        if (arr == null || arr.length == 0) {
            return -1;
        }
        
        return binarySearchRecursiveHelper(arr, target, 0, arr.length - 1);
    }
    
    /**
     * Recursive helper method for binary search
     * @param arr Sorted array
     * @param target Target element
     * @param left Left boundary
     * @param right Right boundary
     * @return Index of target if found, -1 otherwise
     */
    private static int binarySearchRecursiveHelper(int[] arr, int target, int left, int right) {
        // Base case: element not found
        if (left > right) {
            return -1;
        }
        
        // Divide: find middle point
        int mid = left + (right - left) / 2;
        
        // Base case: element found
        if (arr[mid] == target) {
            return mid;
        }
        
        // Conquer: search in appropriate half
        if (arr[mid] > target) {
            // Search in left half
            return binarySearchRecursiveHelper(arr, target, left, mid - 1);
        } else {
            // Search in right half
            return binarySearchRecursiveHelper(arr, target, mid + 1, right);
        }
    }
    
    /**
     * Find the first occurrence of target in array with duplicates
     * @param arr Sorted array (may contain duplicates)
     * @param target Element to find first occurrence of
     * @return Index of first occurrence, -1 if not found
     */
    public static int findFirstOccurrence(int[] arr, int target) {
        if (arr == null || arr.length == 0) {
            return -1;
        }
        
        int left = 0;
        int right = arr.length - 1;
        int result = -1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                result = mid; // Store potential result
                right = mid - 1; // Continue searching in left half
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
    }
    
    /**
     * Find the last occurrence of target in array with duplicates
     * @param arr Sorted array (may contain duplicates)
     * @param target Element to find last occurrence of
     * @return Index of last occurrence, -1 if not found
     */
    public static int findLastOccurrence(int[] arr, int target) {
        if (arr == null || arr.length == 0) {
            return -1;
        }
        
        int left = 0;
        int right = arr.length - 1;
        int result = -1;
        
        while (left <= right) {
            int mid = left + (right - left) / 2;
            
            if (arr[mid] == target) {
                result = mid; // Store potential result
                left = mid + 1; // Continue searching in right half
            } else if (arr[mid] < target) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        
        return result;
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
        System.out.println("=== Binary Search - Divide and Conquer ===");
        
        // Test Case 1: Standard binary search
        int[] arr1 = {2, 3, 4, 10, 40, 50, 80};
        int target1 = 10;
        System.out.print("Array: ");
        printArray(arr1);
        System.out.println("\nTarget: " + target1);
        
        int resultIter = binarySearchIterative(arr1, target1);
        int resultRec = binarySearchRecursive(arr1, target1);
        
        System.out.println("Iterative result: " + (resultIter != -1 ? "Found at index " + resultIter : "Not found"));
        System.out.println("Recursive result: " + (resultRec != -1 ? "Found at index " + resultRec : "Not found"));
        System.out.println();
        
        // Test Case 2: Target not found
        int target2 = 25;
        System.out.print("Array: ");
        printArray(arr1);
        System.out.println("\nTarget: " + target2);
        
        int result2 = binarySearchIterative(arr1, target2);
        System.out.println("Result: " + (result2 != -1 ? "Found at index " + result2 : "Not found"));
        System.out.println();
        
        // Test Case 3: Target at boundaries
        System.out.println("Test Case 3: Boundary elements");
        System.out.print("Array: ");
        printArray(arr1);
        
        System.out.println("\nSearching for first element (2): " + 
                          (binarySearchIterative(arr1, 2) != -1 ? "Found at index " + binarySearchIterative(arr1, 2) : "Not found"));
        System.out.println("Searching for last element (80): " + 
                          (binarySearchIterative(arr1, 80) != -1 ? "Found at index " + binarySearchIterative(arr1, 80) : "Not found"));
        System.out.println();
        
        // Test Case 4: Array with duplicates
        int[] arr4 = {1, 2, 2, 2, 3, 4, 4, 5};
        int target4 = 2;
        System.out.println("Test Case 4: Array with duplicates");
        System.out.print("Array: ");
        printArray(arr4);
        System.out.println("\nTarget: " + target4);
        
        int anyOccurrence = binarySearchIterative(arr4, target4);
        int firstOccurrence = findFirstOccurrence(arr4, target4);
        int lastOccurrence = findLastOccurrence(arr4, target4);
        
        System.out.println("Any occurrence: " + (anyOccurrence != -1 ? "Index " + anyOccurrence : "Not found"));
        System.out.println("First occurrence: " + (firstOccurrence != -1 ? "Index " + firstOccurrence : "Not found"));
        System.out.println("Last occurrence: " + (lastOccurrence != -1 ? "Index " + lastOccurrence : "Not found"));
        System.out.println();
        
        // Test Case 5: Edge cases
        System.out.println("Test Case 5: Edge cases");
        
        // Single element array
        int[] singleArr = {5};
        System.out.println("Single element array [5], searching for 5: " + 
                          (binarySearchIterative(singleArr, 5) != -1 ? "Found" : "Not found"));
        System.out.println("Single element array [5], searching for 3: " + 
                          (binarySearchIterative(singleArr, 3) != -1 ? "Found" : "Not found"));
        
        // Empty array
        int[] emptyArr = {};
        System.out.println("Empty array, searching for 1: " + 
                          (binarySearchIterative(emptyArr, 1) != -1 ? "Found" : "Not found"));
        
        // Two element array
        int[] twoArr = {1, 3};
        System.out.println("Two element array [1, 3], searching for 1: " + 
                          (binarySearchIterative(twoArr, 1) != -1 ? "Found at index " + binarySearchIterative(twoArr, 1) : "Not found"));
        System.out.println("Two element array [1, 3], searching for 3: " + 
                          (binarySearchIterative(twoArr, 3) != -1 ? "Found at index " + binarySearchIterative(twoArr, 3) : "Not found"));
    }
}