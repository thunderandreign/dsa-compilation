// Java Program demonstrating major Array Operations
// Including: creation, insertion, deletion, merging, traversal,
// updating, sorting (bubble, shell, quick, selection, insertion),
// and searching (sequential, binary, interpolation)

import java.util.Arrays;
import java.util.Scanner;

public class ArrayOperations {

    // Scanner for input
    static Scanner sc = new Scanner(System.in);

    // CREATE ARRAY
    public static int[] createArray(int size) {
        // Create an integer array with the given size
        int[] arr = new int[size];
        System.out.println("Enter " + size + " elements:");
        for (int i = 0; i < size; i++) {
            arr[i] = sc.nextInt(); // Fill array with user input
        }
        return arr;
    }

    // TRAVERSE ARRAY
    public static void traverseArray(int[] arr) {
        // Display all elements of the array
        System.out.println("Array elements:");
        for (int val : arr) {
            System.out.print(val + " ");
        }
        System.out.println();
    }

    // 3️⃣ INSERTION
    public static int[] insertElement(int[] arr, int index, int value) {
        // Make new array with one extra slot
        int[] newArr = new int[arr.length + 1];
        for (int i = 0, j = 0; i < newArr.length; i++) {
            if (i == index) newArr[i] = value; // Insert at position
            else newArr[i] = arr[j++];         // Copy other elements
        }
        return newArr;
    }

    // 4️⃣ DELETION
    public static int[] deleteElement(int[] arr, int index) {
        // Create smaller array skipping deleted element
        int[] newArr = new int[arr.length - 1];
        for (int i = 0, j = 0; i < arr.length; i++) {
            if (i == index) continue;  // Skip this index
            newArr[j++] = arr[i];
        }
        return newArr;
    }

    // 5️⃣ UPDATE
    public static void updateElement(int[] arr, int index, int newValue) {
        arr[index] = newValue; // Replace old value
    }

    // 6️⃣ MERGE
    public static int[] mergeArrays(int[] a, int[] b) {
        // Join two arrays into one
        int[] merged = new int[a.length + b.length];
        int i = 0;
        for (int val : a) merged[i++] = val;
        for (int val : b) merged[i++] = val;
        return merged;
    }

    // 7️⃣ SORTING ALGORITHMS
    // Bubble Sort (Sort by Exchange)
    public static void bubbleSort(int[] arr) {
        for (int i = 0; i < arr.length - 1; i++)
            for (int j = 0; j < arr.length - i - 1; j++)
                if (arr[j] > arr[j + 1]) {
                    // Swap
                    int temp = arr[j];
                    arr[j] = arr[j + 1];
                    arr[j + 1] = temp;
                }
    }

    // Selection Sort
    public static void selectionSort(int[] arr) {
        for (int i = 0; i < arr.length - 1; i++) {
            int min = i;
            for (int j = i + 1; j < arr.length; j++)
                if (arr[j] < arr[min]) min = j;
            // Swap smallest into place
            int temp = arr[i];
            arr[i] = arr[min];
            arr[min] = temp;
        }
    }

    // Insertion Sort
    public static void insertionSort(int[] arr) {
        for (int i = 1; i < arr.length; i++) {
            int key = arr[i];
            int j = i - 1;
            while (j >= 0 && arr[j] > key) {
                arr[j + 1] = arr[j];
                j--;
            }
            arr[j + 1] = key;
        }
    }

    // Shell Sort (Gap-based Insertion)
    public static void shellSort(int[] arr) {
        for (int gap = arr.length / 2; gap > 0; gap /= 2) {
            for (int i = gap; i < arr.length; i++) {
                int temp = arr[i];
                int j = i;
                while (j >= gap && arr[j - gap] > temp) {
                    arr[j] = arr[j - gap];
                    j -= gap;
                }
                arr[j] = temp;
            }
        }
    }

    // Quick Sort (Divide and Conquer)
    public static void quickSort(int[] arr, int low, int high) {
        if (low < high) {
            int pivot = partition(arr, low, high);
            quickSort(arr, low, pivot - 1);
            quickSort(arr, pivot + 1, high);
        }
    }

    public static int partition(int[] arr, int low, int high) {
        int pivot = arr[high];
        int i = (low - 1);
        for (int j = low; j < high; j++) {
            if (arr[j] <= pivot) {
                i++;
                int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
            }
        }
        int temp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = temp;
        return i + 1;
    }

    // 8️⃣ SEARCHING
    public static int sequentialSearch(int[] arr, int key) {
        for (int i = 0; i < arr.length; i++)
            if (arr[i] == key) return i;
        return -1;
    }

    public static int binarySearch(int[] arr, int key) {
        int low = 0, high = arr.length - 1;
        while (low <= high) {
            int mid = (low + high) / 2;
            if (arr[mid] == key) return mid;
            else if (arr[mid] < key) low = mid + 1;
            else high = mid - 1;
        }
        return -1;
    }

    public static int interpolationSearch(int[] arr, int key) {
        int low = 0, high = arr.length - 1;
        while (low <= high && key >= arr[low] && key <= arr[high]) {
            if (low == high) return arr[low] == key ? low : -1;
            int pos = low + (((key - arr[low]) * (high - low)) /
                    (arr[high] - arr[low]));
            if (arr[pos] == key) return pos;
            if (arr[pos] < key) low = pos + 1;
            else high = pos - 1;
        }
        return -1;
    }

    // MAIN METHOD
    public static void main(String[] args) {
        int[] arr = createArray(5);
        traverseArray(arr);
        arr = insertElement(arr, 2, 99);
        arr = deleteElement(arr, 3);
        updateElement(arr, 1, 77);
        traverseArray(arr);

        int[] another = {10, 20, 30};
        int[] merged = mergeArrays(arr, another);
        System.out.println("Merged:");
        traverseArray(merged);

        bubbleSort(merged);
        System.out.println("Sorted:");
        traverseArray(merged);

        System.out.println("Binary search for 20: index " + binarySearch(merged, 20));
    }
}
