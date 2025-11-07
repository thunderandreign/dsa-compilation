/**
 * Randomized QuickSort - Advanced Implementation
 * 
 * This implementation demonstrates the power of randomization in the QuickSort
 * algorithm, showing how random pivot selection transforms worst-case O(n²)
 * performance into expected O(n log n) with high probability.
 * 
 * Key Concepts:
 * - Randomized Pivot Selection: Avoids worst-case scenarios
 * - Expected Time Complexity: O(n log n) with high probability
 * - In-place Sorting: O(log n) space complexity
 * - Probabilistic Analysis: Understanding expected vs worst-case performance
 * 
 * Time Complexity: 
 * - Best Case: O(n log n)
 * - Average Case: O(n log n) 
 * - Worst Case: O(n²) but extremely unlikely with randomization
 * Space Complexity: O(log n) average, O(n) worst case
 * 
 * Real-world Applications:
 * - Standard library sorting implementations
 * - Database query optimization
 * - Computational geometry algorithms
 * - Statistical sampling and analysis
 */

import java.util.*;

public class RandomizedQuickSort {
    
    static class SortingMetrics {
        int comparisons;
        int swaps;
        int pivotSelections;
        int recursionDepth;
        int maxDepth;
        long executionTime;
        
        SortingMetrics() {
            comparisons = 0;
            swaps = 0;
            pivotSelections = 0;
            recursionDepth = 0;
            maxDepth = 0;
            executionTime = 0;
        }
        
        void reset() {
            comparisons = 0;
            swaps = 0;
            pivotSelections = 0;
            recursionDepth = 0;
            maxDepth = 0;
            executionTime = 0;
        }
        
        @Override
        public String toString() {
            return String.format("Comparisons: %d, Swaps: %d, Pivots: %d, Max Depth: %d, Time: %dms",
                               comparisons, swaps, pivotSelections, maxDepth, executionTime);
        }
    }
    
    static class QuickSortVariant {
        String name;
        SortingMetrics metrics;
        
        QuickSortVariant(String name) {
            this.name = name;
            this.metrics = new SortingMetrics();
        }
    }
    
    private Random random;
    private boolean verbose;
    private List<String> steps;
    
    public RandomizedQuickSort(boolean verbose) {
        this.random = new Random();
        this.verbose = verbose;
        this.steps = new ArrayList<>();
    }
    
    public RandomizedQuickSort(long seed, boolean verbose) {
        this.random = new Random(seed);
        this.verbose = verbose;
        this.steps = new ArrayList<>();
    }
    
    /**
     * Randomized QuickSort with detailed metrics
     */
    public void randomizedQuickSort(int[] arr, SortingMetrics metrics) {
        if (verbose) {
            steps.add("=== Starting Randomized QuickSort ===");
            steps.add("Initial array: " + Arrays.toString(arr));
        }
        
        long startTime = System.nanoTime();
        quickSortRecursive(arr, 0, arr.length - 1, metrics, 0);
        long endTime = System.nanoTime();
        
        metrics.executionTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add("Final sorted array: " + Arrays.toString(arr));
            steps.add("Sorting completed with metrics: " + metrics);
        }
    }
    
    private void quickSortRecursive(int[] arr, int low, int high, SortingMetrics metrics, int depth) {
        if (low < high) {
            metrics.recursionDepth = depth;
            metrics.maxDepth = Math.max(metrics.maxDepth, depth);
            
            if (verbose && depth <= 5 && high - low <= 20) {
                steps.add("Depth " + depth + ": Sorting subarray [" + low + ".." + high + "]: " +
                         Arrays.toString(Arrays.copyOfRange(arr, low, high + 1)));
            }
            
            // Randomized pivot selection
            int pivotIndex = randomizedPartition(arr, low, high, metrics);
            
            if (verbose && depth <= 5 && high - low <= 20) {
                steps.add("Pivot " + arr[pivotIndex] + " placed at index " + pivotIndex);
                steps.add("After partition: " + Arrays.toString(arr));
            }
            
            // Recursively sort subarrays
            quickSortRecursive(arr, low, pivotIndex - 1, metrics, depth + 1);
            quickSortRecursive(arr, pivotIndex + 1, high, metrics, depth + 1);
        }
    }
    
    /**
     * Randomized partitioning with random pivot selection
     */
    private int randomizedPartition(int[] arr, int low, int high, SortingMetrics metrics) {
        // Random pivot selection - key to avoiding worst-case scenarios
        int randomIndex = low + random.nextInt(high - low + 1);
        metrics.pivotSelections++;
        
        // Swap random element with last element
        swap(arr, randomIndex, high, metrics);
        
        // Standard Lomuto partition scheme
        return partition(arr, low, high, metrics);
    }
    
    /**
     * Standard Lomuto partition scheme
     */
    private int partition(int[] arr, int low, int high, SortingMetrics metrics) {
        int pivot = arr[high]; // Pivot element
        int i = low - 1; // Index of smaller element
        
        for (int j = low; j < high; j++) {
            metrics.comparisons++;
            
            if (arr[j] <= pivot) {
                i++;
                swap(arr, i, j, metrics);
            }
        }
        
        swap(arr, i + 1, high, metrics);
        return i + 1;
    }
    
    /**
     * Three-way partitioning for handling duplicates efficiently
     */
    public void threeWayQuickSort(int[] arr, SortingMetrics metrics) {
        if (verbose) {
            steps.add("=== Starting Three-Way Randomized QuickSort ===");
            steps.add("Initial array: " + Arrays.toString(arr));
        }
        
        long startTime = System.nanoTime();
        threeWayQuickSortRecursive(arr, 0, arr.length - 1, metrics, 0);
        long endTime = System.nanoTime();
        
        metrics.executionTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add("Final sorted array: " + Arrays.toString(arr));
            steps.add("Three-way sorting completed with metrics: " + metrics);
        }
    }
    
    private void threeWayQuickSortRecursive(int[] arr, int low, int high, SortingMetrics metrics, int depth) {
        if (low < high) {
            metrics.recursionDepth = depth;
            metrics.maxDepth = Math.max(metrics.maxDepth, depth);
            
            // Randomized pivot selection
            int randomIndex = low + random.nextInt(high - low + 1);
            swap(arr, randomIndex, low, metrics);
            metrics.pivotSelections++;
            
            int[] partitionResult = threeWayPartition(arr, low, high, metrics);
            int lt = partitionResult[0]; // Less than pivot
            int gt = partitionResult[1]; // Greater than pivot
            
            if (verbose && depth <= 3 && high - low <= 15) {
                steps.add("Depth " + depth + ": Three-way partition of [" + low + ".." + high + "]");
                steps.add("  < pivot: [" + low + ".." + (lt-1) + "], = pivot: [" + lt + ".." + gt + "], > pivot: [" + (gt+1) + ".." + high + "]");
            }
            
            // Recursively sort the < and > parts (= part is already sorted)
            threeWayQuickSortRecursive(arr, low, lt - 1, metrics, depth + 1);
            threeWayQuickSortRecursive(arr, gt + 1, high, metrics, depth + 1);
        }
    }
    
    /**
     * Three-way partitioning (Dutch National Flag algorithm)
     */
    private int[] threeWayPartition(int[] arr, int low, int high, SortingMetrics metrics) {
        int pivot = arr[low];
        int lt = low;      // arr[low..lt-1] < pivot
        int i = low + 1;   // arr[lt..i-1] = pivot
        int gt = high;     // arr[gt+1..high] > pivot
        
        while (i <= gt) {
            metrics.comparisons++;
            
            if (arr[i] < pivot) {
                swap(arr, lt, i, metrics);
                lt++;
                i++;
            } else if (arr[i] > pivot) {
                swap(arr, i, gt, metrics);
                gt--;
                // Don't increment i as we need to check swapped element
            } else {
                i++;
            }
        }
        
        return new int[]{lt, gt};
    }
    
    /**
     * Hybrid QuickSort with insertion sort for small arrays
     */
    public void hybridQuickSort(int[] arr, SortingMetrics metrics) {
        if (verbose) {
            steps.add("=== Starting Hybrid Randomized QuickSort ===");
            steps.add("Initial array: " + Arrays.toString(arr));
        }
        
        long startTime = System.nanoTime();
        hybridQuickSortRecursive(arr, 0, arr.length - 1, metrics, 0);
        long endTime = System.nanoTime();
        
        metrics.executionTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add("Final sorted array: " + Arrays.toString(arr));
            steps.add("Hybrid sorting completed with metrics: " + metrics);
        }
    }
    
    private void hybridQuickSortRecursive(int[] arr, int low, int high, SortingMetrics metrics, int depth) {
        final int INSERTION_SORT_THRESHOLD = 10;
        
        if (high - low + 1 <= INSERTION_SORT_THRESHOLD) {
            // Use insertion sort for small subarrays
            insertionSort(arr, low, high, metrics);
            return;
        }
        
        if (low < high) {
            metrics.recursionDepth = depth;
            metrics.maxDepth = Math.max(metrics.maxDepth, depth);
            
            int pivotIndex = randomizedPartition(arr, low, high, metrics);
            
            hybridQuickSortRecursive(arr, low, pivotIndex - 1, metrics, depth + 1);
            hybridQuickSortRecursive(arr, pivotIndex + 1, high, metrics, depth + 1);
        }
    }
    
    /**
     * Insertion sort for small subarrays
     */
    private void insertionSort(int[] arr, int low, int high, SortingMetrics metrics) {
        for (int i = low + 1; i <= high; i++) {
            int key = arr[i];
            int j = i - 1;
            
            while (j >= low && arr[j] > key) {
                metrics.comparisons++;
                arr[j + 1] = arr[j];
                metrics.swaps++;
                j--;
            }
            if (j >= low) metrics.comparisons++; // Count the final comparison
            
            arr[j + 1] = key;
        }
    }
    
    /**
     * Deterministic QuickSort for comparison (always picks last element as pivot)
     */
    public void deterministicQuickSort(int[] arr, SortingMetrics metrics) {
        if (verbose) {
            steps.add("=== Starting Deterministic QuickSort ===");
            steps.add("Initial array: " + Arrays.toString(arr));
        }
        
        long startTime = System.nanoTime();
        deterministicQuickSortRecursive(arr, 0, arr.length - 1, metrics, 0);
        long endTime = System.nanoTime();
        
        metrics.executionTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add("Final sorted array: " + Arrays.toString(arr));
            steps.add("Deterministic sorting completed with metrics: " + metrics);
        }
    }
    
    private void deterministicQuickSortRecursive(int[] arr, int low, int high, SortingMetrics metrics, int depth) {
        if (low < high) {
            metrics.recursionDepth = depth;
            metrics.maxDepth = Math.max(metrics.maxDepth, depth);
            metrics.pivotSelections++;
            
            int pivotIndex = partition(arr, low, high, metrics);
            
            deterministicQuickSortRecursive(arr, low, pivotIndex - 1, metrics, depth + 1);
            deterministicQuickSortRecursive(arr, pivotIndex + 1, high, metrics, depth + 1);
        }
    }
    
    private void swap(int[] arr, int i, int j, SortingMetrics metrics) {
        if (i != j) {
            metrics.swaps++;
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
        }
    }
    
    /**
     * Generate different types of test arrays
     */
    public static int[] generateRandomArray(int size, int maxValue, Random random) {
        int[] arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = random.nextInt(maxValue);
        }
        return arr;
    }
    
    public static int[] generateSortedArray(int size) {
        int[] arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = i;
        }
        return arr;
    }
    
    public static int[] generateReverseSortedArray(int size) {
        int[] arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = size - 1 - i;
        }
        return arr;
    }
    
    public static int[] generateMostlyDuplicatesArray(int size, int numUniqueValues, Random random) {
        int[] arr = new int[size];
        for (int i = 0; i < size; i++) {
            arr[i] = random.nextInt(numUniqueValues);
        }
        return arr;
    }
    
    public List<String> getSteps() {
        return new ArrayList<>(steps);
    }
    
    public void clearSteps() {
        steps.clear();
    }
    
    public static void main(String[] args) {
        System.out.println("=== Randomized QuickSort - Comprehensive Analysis ===\n");
        
        Random random = new Random(42); // Fixed seed for reproducible results
        
        // Test case 1: Small array with step-by-step demonstration
        System.out.println("Test Case 1: Small Array Step-by-Step");
        int[] smallArray = {64, 34, 25, 12, 22, 11, 90, 5};
        System.out.println("Original array: " + Arrays.toString(smallArray));
        
        RandomizedQuickSort sorter = new RandomizedQuickSort(42, true);
        QuickSortVariant randomized = new QuickSortVariant("Randomized QuickSort");
        
        int[] testArray = smallArray.clone();
        sorter.randomizedQuickSort(testArray, randomized.metrics);
        
        System.out.println("\nStep-by-step execution:");
        for (String step : sorter.getSteps()) {
            System.out.println(step);
        }
        
        // Test case 2: Performance comparison on different array types
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: Performance Comparison");
        
        int[] sizes = {100, 1000, 10000};
        String[] arrayTypes = {"Random", "Sorted", "Reverse", "Duplicates"};
        
        System.out.printf("%-12s | %-8s | %-15s | %-15s | %-15s | %-15s%n", 
                         "Array Type", "Size", "Randomized", "Three-Way", "Hybrid", "Deterministic");
        System.out.println("-".repeat(100));
        
        for (int size : sizes) {
            for (String arrayType : arrayTypes) {
                // Generate test array
                int[] baseArray = switch (arrayType) {
                    case "Random" -> generateRandomArray(size, size, random);
                    case "Sorted" -> generateSortedArray(size);
                    case "Reverse" -> generateReverseSortedArray(size);
                    case "Duplicates" -> generateMostlyDuplicatesArray(size, size / 10, random);
                    default -> generateRandomArray(size, size, random);
                };
                
                // Test different variants
                RandomizedQuickSort tester = new RandomizedQuickSort(42, false);
                
                QuickSortVariant[] variants = {
                    new QuickSortVariant("Randomized"),
                    new QuickSortVariant("Three-Way"),
                    new QuickSortVariant("Hybrid"),
                    new QuickSortVariant("Deterministic")
                };
                
                // Test each variant
                int[] testArr1 = baseArray.clone();
                tester.randomizedQuickSort(testArr1, variants[0].metrics);
                
                int[] testArr2 = baseArray.clone();
                tester.threeWayQuickSort(testArr2, variants[1].metrics);
                
                int[] testArr3 = baseArray.clone();
                tester.hybridQuickSort(testArr3, variants[2].metrics);
                
                int[] testArr4 = baseArray.clone();
                tester.deterministicQuickSort(testArr4, variants[3].metrics);
                
                // Print results
                System.out.printf("%-12s | %-8d | %-15s | %-15s | %-15s | %-15s%n",
                    arrayType, size,
                    String.format("%dms/%dd", variants[0].metrics.executionTime, variants[0].metrics.maxDepth),
                    String.format("%dms/%dd", variants[1].metrics.executionTime, variants[1].metrics.maxDepth),
                    String.format("%dms/%dd", variants[2].metrics.executionTime, variants[2].metrics.maxDepth),
                    String.format("%dms/%dd", variants[3].metrics.executionTime, variants[3].metrics.maxDepth));
            }
        }
        
        // Test case 3: Demonstrate randomization benefits
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 3: Randomization Benefits Analysis");
        
        // Create worst-case scenario for deterministic QuickSort
        int[] worstCase = generateSortedArray(1000);
        System.out.println("Testing on sorted array (worst case for deterministic QuickSort):");
        
        RandomizedQuickSort analyzer = new RandomizedQuickSort(false);
        
        QuickSortVariant deterministicWorst = new QuickSortVariant("Deterministic");
        QuickSortVariant randomizedWorst = new QuickSortVariant("Randomized");
        
        int[] deterministicArray = worstCase.clone();
        analyzer.deterministicQuickSort(deterministicArray, deterministicWorst.metrics);
        
        int[] randomizedArray = worstCase.clone();
        analyzer.randomizedQuickSort(randomizedArray, randomizedWorst.metrics);
        
        System.out.println("Deterministic QuickSort: " + deterministicWorst.metrics);
        System.out.println("Randomized QuickSort: " + randomizedWorst.metrics);
        System.out.printf("Improvement: %.2fx faster, %.2fx less depth%n",
            (double) deterministicWorst.metrics.executionTime / randomizedWorst.metrics.executionTime,
            (double) deterministicWorst.metrics.maxDepth / randomizedWorst.metrics.maxDepth);
        
        // Performance analysis
        System.out.println("\n=== Performance Analysis ===");
        System.out.println("Randomized QuickSort Advantages:");
        System.out.println("- Avoids worst-case O(n²) behavior with high probability");
        System.out.println("- Expected O(n log n) performance regardless of input");
        System.out.println("- Better cache performance due to balanced partitions");
        System.out.println("- Robust against adversarial inputs");
        
        System.out.println("\nVariant Comparisons:");
        System.out.println("- Three-Way: Excellent for arrays with many duplicates");
        System.out.println("- Hybrid: Best overall performance with insertion sort optimization");
        System.out.println("- Standard: Good general-purpose randomized sorting");
        
        demonstrateRandomizationTheory();
    }
    
    private static void demonstrateRandomizationTheory() {
        System.out.println("\n=== Randomization Theory Demonstration ===");
        
        Random random = new Random(42);
        int numTrials = 1000;
        int arraySize = 100;
        
        // Collect depth statistics
        List<Integer> randomizedDepths = new ArrayList<>();
        List<Integer> deterministicDepths = new ArrayList<>();
        
        RandomizedQuickSort analyzer = new RandomizedQuickSort(false);
        
        for (int trial = 0; trial < numTrials; trial++) {
            // Generate random array
            int[] testArray = generateRandomArray(arraySize, arraySize, random);
            
            // Test randomized version
            QuickSortVariant randomizedTest = new QuickSortVariant("Randomized");
            int[] randomizedArray = testArray.clone();
            analyzer = new RandomizedQuickSort(random.nextLong(), false);
            analyzer.randomizedQuickSort(randomizedArray, randomizedTest.metrics);
            randomizedDepths.add(randomizedTest.metrics.maxDepth);
            
            // Test deterministic version
            QuickSortVariant deterministicTest = new QuickSortVariant("Deterministic");
            int[] deterministicArray = testArray.clone();
            analyzer.deterministicQuickSort(deterministicArray, deterministicTest.metrics);
            deterministicDepths.add(deterministicTest.metrics.maxDepth);
        }
        
        // Calculate statistics
        double avgRandomizedDepth = randomizedDepths.stream().mapToInt(Integer::intValue).average().orElse(0);
        double avgDeterministicDepth = deterministicDepths.stream().mapToInt(Integer::intValue).average().orElse(0);
        
        int maxRandomizedDepth = randomizedDepths.stream().mapToInt(Integer::intValue).max().orElse(0);
        int maxDeterministicDepth = deterministicDepths.stream().mapToInt(Integer::intValue).max().orElse(0);
        
        double expectedDepth = Math.log(arraySize) / Math.log(2); // log₂(n)
        
        System.out.printf("Statistical Analysis over %d trials:%n", numTrials);
        System.out.printf("Array size: %d, Expected optimal depth: %.2f%n", arraySize, expectedDepth);
        System.out.printf("Randomized - Average depth: %.2f, Max depth: %d%n", 
                         avgRandomizedDepth, maxRandomizedDepth);
        System.out.printf("Deterministic - Average depth: %.2f, Max depth: %d%n", 
                         avgDeterministicDepth, maxDeterministicDepth);
        System.out.printf("Randomization improvement: %.2fx average, %.2fx worst-case%n",
                         avgDeterministicDepth / avgRandomizedDepth,
                         (double) maxDeterministicDepth / maxRandomizedDepth);
        
        System.out.println("\nKey Insights:");
        System.out.println("- Randomization keeps performance close to theoretical optimum");
        System.out.println("- Worst-case scenarios become extremely rare");
        System.out.println("- Performance is more predictable and consistent");
        System.out.println("- Small constant factors make randomized version practical");
    }
}