/**
 * Randomized QuickSort - Advanced C Implementation
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
 * - Standard library sorting implementations (glibc qsort)
 * - Database query optimization
 * - Computational geometry algorithms
 * - Statistical sampling and analysis
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define MAX_ARRAY_SIZE 100000
#define MAX_STEPS 1000
#define INSERTION_SORT_THRESHOLD 10

typedef struct {
    int comparisons;
    int swaps;
    int pivot_selections;
    int recursion_depth;
    int max_depth;
    long execution_time_ms;
} SortingMetrics;

typedef struct {
    char name[50];
    SortingMetrics metrics;
} QuickSortVariant;

typedef struct {
    char steps[MAX_STEPS][200];
    int step_count;
    bool verbose;
} StepTracker;

// Global random state
static unsigned int random_seed = 1;

// Utility functions
void reset_metrics(SortingMetrics* metrics) {
    memset(metrics, 0, sizeof(SortingMetrics));
}

void init_step_tracker(StepTracker* tracker, bool verbose) {
    tracker->step_count = 0;
    tracker->verbose = verbose;
}

void add_step(StepTracker* tracker, const char* step) {
    if (tracker->verbose && tracker->step_count < MAX_STEPS) {
        strncpy(tracker->steps[tracker->step_count], step, 199);
        tracker->steps[tracker->step_count][199] = '\0';
        tracker->step_count++;
    }
}

void print_steps(const StepTracker* tracker) {
    for (int i = 0; i < tracker->step_count; i++) {
        printf("%s\n", tracker->steps[i]);
    }
}

// Simple linear congruential generator for consistent randomization
unsigned int simple_random() {
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fffffff;
    return random_seed;
}

void set_random_seed(unsigned int seed) {
    random_seed = seed;
}

int random_range(int min, int max) {
    if (min >= max) return min;
    return min + (simple_random() % (max - min + 1));
}

void swap(int* arr, int i, int j, SortingMetrics* metrics) {
    if (i != j) {
        metrics->swaps++;
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
}

void print_array_range(const int* arr, int start, int end, char* buffer) {
    char temp[20];
    strcpy(buffer, "[");
    for (int i = start; i <= end; i++) {
        sprintf(temp, "%d", arr[i]);
        strcat(buffer, temp);
        if (i < end) strcat(buffer, ", ");
    }
    strcat(buffer, "]");
}

void print_array(const int* arr, int size) {
    printf("[");
    for (int i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(", ");
    }
    printf("]");
}

// Standard Lomuto partition scheme
int partition(int* arr, int low, int high, SortingMetrics* metrics) {
    int pivot = arr[high]; // Pivot element
    int i = low - 1; // Index of smaller element
    
    for (int j = low; j < high; j++) {
        metrics->comparisons++;
        
        if (arr[j] <= pivot) {
            i++;
            swap(arr, i, j, metrics);
        }
    }
    
    swap(arr, i + 1, high, metrics);
    return i + 1;
}

// Randomized partitioning with random pivot selection
int randomized_partition(int* arr, int low, int high, SortingMetrics* metrics) {
    // Random pivot selection - key to avoiding worst-case scenarios
    int random_index = random_range(low, high);
    metrics->pivot_selections++;
    
    // Swap random element with last element
    swap(arr, random_index, high, metrics);
    
    // Standard Lomuto partition scheme
    return partition(arr, low, high, metrics);
}

// Randomized QuickSort recursive implementation
void randomized_quicksort_recursive(int* arr, int low, int high, SortingMetrics* metrics, 
                                   StepTracker* tracker, int depth) {
    if (low < high) {
        metrics->recursion_depth = depth;
        if (depth > metrics->max_depth) {
            metrics->max_depth = depth;
        }
        
        if (tracker->verbose && depth <= 5 && high - low <= 20) {
            char step[200];
            char subarray[100];
            print_array_range(arr, low, high, subarray);
            sprintf(step, "Depth %d: Sorting subarray [%d..%d]: %s", depth, low, high, subarray);
            add_step(tracker, step);
        }
        
        // Randomized pivot selection
        int pivot_index = randomized_partition(arr, low, high, metrics);
        
        if (tracker->verbose && depth <= 5 && high - low <= 20) {
            char step[200];
            sprintf(step, "Pivot %d placed at index %d", arr[pivot_index], pivot_index);
            add_step(tracker, step);
        }
        
        // Recursively sort subarrays
        randomized_quicksort_recursive(arr, low, pivot_index - 1, metrics, tracker, depth + 1);
        randomized_quicksort_recursive(arr, pivot_index + 1, high, metrics, tracker, depth + 1);
    }
}

// Main randomized QuickSort function
void randomized_quicksort(int* arr, int size, SortingMetrics* metrics, StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Starting Randomized QuickSort ===");
        char step[200];
        sprintf(step, "Initial array size: %d", size);
        add_step(tracker, step);
    }
    
    clock_t start_time = clock();
    randomized_quicksort_recursive(arr, 0, size - 1, metrics, tracker, 0);
    clock_t end_time = clock();
    
    metrics->execution_time_ms = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    if (tracker->verbose) {
        add_step(tracker, "Randomized QuickSort completed");
    }
}

// Three-way partitioning (Dutch National Flag algorithm)
void three_way_partition(int* arr, int low, int high, int* lt, int* gt, SortingMetrics* metrics) {
    int pivot = arr[low];
    *lt = low;      // arr[low..lt-1] < pivot
    int i = low + 1; // arr[lt..i-1] = pivot
    *gt = high;     // arr[gt+1..high] > pivot
    
    while (i <= *gt) {
        metrics->comparisons++;
        
        if (arr[i] < pivot) {
            swap(arr, *lt, i, metrics);
            (*lt)++;
            i++;
        } else if (arr[i] > pivot) {
            swap(arr, i, *gt, metrics);
            (*gt)--;
            // Don't increment i as we need to check swapped element
        } else {
            i++;
        }
    }
}

// Three-way QuickSort recursive implementation
void three_way_quicksort_recursive(int* arr, int low, int high, SortingMetrics* metrics, 
                                  StepTracker* tracker, int depth) {
    if (low < high) {
        metrics->recursion_depth = depth;
        if (depth > metrics->max_depth) {
            metrics->max_depth = depth;
        }
        
        // Randomized pivot selection
        int random_index = random_range(low, high);
        swap(arr, random_index, low, metrics);
        metrics->pivot_selections++;
        
        int lt, gt;
        three_way_partition(arr, low, high, &lt, &gt, metrics);
        
        if (tracker->verbose && depth <= 3 && high - low <= 15) {
            char step[200];
            sprintf(step, "Depth %d: Three-way partition of [%d..%d]", depth, low, high);
            add_step(tracker, step);
            sprintf(step, "  < pivot: [%d..%d], = pivot: [%d..%d], > pivot: [%d..%d]", 
                   low, lt-1, lt, gt, gt+1, high);
            add_step(tracker, step);
        }
        
        // Recursively sort the < and > parts (= part is already sorted)
        three_way_quicksort_recursive(arr, low, lt - 1, metrics, tracker, depth + 1);
        three_way_quicksort_recursive(arr, gt + 1, high, metrics, tracker, depth + 1);
    }
}

// Three-way QuickSort main function
void three_way_quicksort(int* arr, int size, SortingMetrics* metrics, StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Starting Three-Way Randomized QuickSort ===");
    }
    
    clock_t start_time = clock();
    three_way_quicksort_recursive(arr, 0, size - 1, metrics, tracker, 0);
    clock_t end_time = clock();
    
    metrics->execution_time_ms = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    if (tracker->verbose) {
        add_step(tracker, "Three-way QuickSort completed");
    }
}

// Insertion sort for small subarrays
void insertion_sort(int* arr, int low, int high, SortingMetrics* metrics) {
    for (int i = low + 1; i <= high; i++) {
        int key = arr[i];
        int j = i - 1;
        
        while (j >= low && arr[j] > key) {
            metrics->comparisons++;
            arr[j + 1] = arr[j];
            metrics->swaps++;
            j--;
        }
        if (j >= low) metrics->comparisons++; // Count the final comparison
        
        arr[j + 1] = key;
    }
}

// Hybrid QuickSort recursive implementation
void hybrid_quicksort_recursive(int* arr, int low, int high, SortingMetrics* metrics, 
                               StepTracker* tracker, int depth) {
    if (high - low + 1 <= INSERTION_SORT_THRESHOLD) {
        // Use insertion sort for small subarrays
        insertion_sort(arr, low, high, metrics);
        return;
    }
    
    if (low < high) {
        metrics->recursion_depth = depth;
        if (depth > metrics->max_depth) {
            metrics->max_depth = depth;
        }
        
        int pivot_index = randomized_partition(arr, low, high, metrics);
        
        hybrid_quicksort_recursive(arr, low, pivot_index - 1, metrics, tracker, depth + 1);
        hybrid_quicksort_recursive(arr, pivot_index + 1, high, metrics, tracker, depth + 1);
    }
}

// Hybrid QuickSort main function
void hybrid_quicksort(int* arr, int size, SortingMetrics* metrics, StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Starting Hybrid Randomized QuickSort ===");
    }
    
    clock_t start_time = clock();
    hybrid_quicksort_recursive(arr, 0, size - 1, metrics, tracker, 0);
    clock_t end_time = clock();
    
    metrics->execution_time_ms = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    if (tracker->verbose) {
        add_step(tracker, "Hybrid QuickSort completed");
    }
}

// Deterministic QuickSort recursive implementation (for comparison)
void deterministic_quicksort_recursive(int* arr, int low, int high, SortingMetrics* metrics, 
                                     StepTracker* tracker, int depth) {
    if (low < high) {
        metrics->recursion_depth = depth;
        if (depth > metrics->max_depth) {
            metrics->max_depth = depth;
        }
        metrics->pivot_selections++;
        
        int pivot_index = partition(arr, low, high, metrics);
        
        deterministic_quicksort_recursive(arr, low, pivot_index - 1, metrics, tracker, depth + 1);
        deterministic_quicksort_recursive(arr, pivot_index + 1, high, metrics, tracker, depth + 1);
    }
}

// Deterministic QuickSort main function
void deterministic_quicksort(int* arr, int size, SortingMetrics* metrics, StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Starting Deterministic QuickSort ===");
    }
    
    clock_t start_time = clock();
    deterministic_quicksort_recursive(arr, 0, size - 1, metrics, tracker, 0);
    clock_t end_time = clock();
    
    metrics->execution_time_ms = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    if (tracker->verbose) {
        add_step(tracker, "Deterministic QuickSort completed");
    }
}

// Array generation functions
void generate_random_array(int* arr, int size, int max_value) {
    for (int i = 0; i < size; i++) {
        arr[i] = simple_random() % max_value;
    }
}

void generate_sorted_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = i;
    }
}

void generate_reverse_sorted_array(int* arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = size - 1 - i;
    }
}

void generate_mostly_duplicates_array(int* arr, int size, int num_unique_values) {
    for (int i = 0; i < size; i++) {
        arr[i] = simple_random() % num_unique_values;
    }
}

// Verification function
bool is_sorted(const int* arr, int size) {
    for (int i = 1; i < size; i++) {
        if (arr[i] < arr[i-1]) {
            return false;
        }
    }
    return true;
}

void copy_array(const int* source, int* dest, int size) {
    memcpy(dest, source, size * sizeof(int));
}

void print_metrics(const char* name, const SortingMetrics* metrics) {
    printf("%s: Comparisons: %d, Swaps: %d, Pivots: %d, Max Depth: %d, Time: %ldms\n",
           name, metrics->comparisons, metrics->swaps, metrics->pivot_selections, 
           metrics->max_depth, metrics->execution_time_ms);
}

void demonstrate_randomization_theory() {
    printf("\n=== Randomization Theory Demonstration ===\n");
    
    const int num_trials = 1000;
    const int array_size = 100;
    
    int* randomized_depths = malloc(num_trials * sizeof(int));
    int* deterministic_depths = malloc(num_trials * sizeof(int));
    int* test_array = malloc(array_size * sizeof(int));
    
    StepTracker tracker;
    init_step_tracker(&tracker, false);
    
    for (int trial = 0; trial < num_trials; trial++) {
        // Generate random array
        set_random_seed(trial + 42);
        generate_random_array(test_array, array_size, array_size);
        
        // Test randomized version
        SortingMetrics randomized_metrics;
        reset_metrics(&randomized_metrics);
        int* randomized_array = malloc(array_size * sizeof(int));
        copy_array(test_array, randomized_array, array_size);
        
        set_random_seed(trial * 17 + 123); // Different seed for randomization
        randomized_quicksort(randomized_array, array_size, &randomized_metrics, &tracker);
        randomized_depths[trial] = randomized_metrics.max_depth;
        free(randomized_array);
        
        // Test deterministic version
        SortingMetrics deterministic_metrics;
        reset_metrics(&deterministic_metrics);
        int* deterministic_array = malloc(array_size * sizeof(int));
        copy_array(test_array, deterministic_array, array_size);
        
        deterministic_quicksort(deterministic_array, array_size, &deterministic_metrics, &tracker);
        deterministic_depths[trial] = deterministic_metrics.max_depth;
        free(deterministic_array);
    }
    
    // Calculate statistics
    double avg_randomized_depth = 0;
    double avg_deterministic_depth = 0;
    int max_randomized_depth = 0;
    int max_deterministic_depth = 0;
    
    for (int i = 0; i < num_trials; i++) {
        avg_randomized_depth += randomized_depths[i];
        avg_deterministic_depth += deterministic_depths[i];
        if (randomized_depths[i] > max_randomized_depth) {
            max_randomized_depth = randomized_depths[i];
        }
        if (deterministic_depths[i] > max_deterministic_depth) {
            max_deterministic_depth = deterministic_depths[i];
        }
    }
    
    avg_randomized_depth /= num_trials;
    avg_deterministic_depth /= num_trials;
    
    double expected_depth = log2(array_size);
    
    printf("Statistical Analysis over %d trials:\n", num_trials);
    printf("Array size: %d, Expected optimal depth: %.2f\n", array_size, expected_depth);
    printf("Randomized - Average depth: %.2f, Max depth: %d\n", 
           avg_randomized_depth, max_randomized_depth);
    printf("Deterministic - Average depth: %.2f, Max depth: %d\n", 
           avg_deterministic_depth, max_deterministic_depth);
    printf("Randomization improvement: %.2fx average, %.2fx worst-case\n",
           avg_deterministic_depth / avg_randomized_depth,
           (double) max_deterministic_depth / max_randomized_depth);
    
    printf("\nKey Insights:\n");
    printf("- Randomization keeps performance close to theoretical optimum\n");
    printf("- Worst-case scenarios become extremely rare\n");
    printf("- Performance is more predictable and consistent\n");
    printf("- Small constant factors make randomized version practical\n");
    
    free(randomized_depths);
    free(deterministic_depths);
    free(test_array);
}

int main() {
    printf("=== Randomized QuickSort - Comprehensive Analysis ===\n\n");
    
    set_random_seed(42); // Fixed seed for reproducible results
    
    // Test case 1: Small array with step-by-step demonstration
    printf("Test Case 1: Small Array Step-by-Step\n");
    int small_array[] = {64, 34, 25, 12, 22, 11, 90, 5};
    int small_size = sizeof(small_array) / sizeof(small_array[0]);
    
    printf("Original array: ");
    print_array(small_array, small_size);
    printf("\n");
    
    StepTracker tracker;
    init_step_tracker(&tracker, true);
    SortingMetrics metrics;
    reset_metrics(&metrics);
    
    int* test_array = malloc(small_size * sizeof(int));
    copy_array(small_array, test_array, small_size);
    
    randomized_quicksort(test_array, small_size, &metrics, &tracker);
    
    printf("\nStep-by-step execution:\n");
    print_steps(&tracker);
    
    printf("Final sorted array: ");
    print_array(test_array, small_size);
    printf("\n");
    print_metrics("Randomized QuickSort", &metrics);
    
    free(test_array);
    
    // Test case 2: Performance comparison on different array types
    printf("\n%s\n", "============================================================");
    printf("Test Case 2: Performance Comparison\n");
    
    int sizes[] = {100, 1000, 10000};
    const char* array_types[] = {"Random", "Sorted", "Reverse", "Duplicates"};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_types = sizeof(array_types) / sizeof(array_types[0]);
    
    printf("%-12s | %-8s | %-15s | %-15s | %-15s | %-15s\n", 
           "Array Type", "Size", "Randomized", "Three-Way", "Hybrid", "Deterministic");
    printf("----------------------------------------------------------------------------------------------------\n");
    
    for (int s = 0; s < num_sizes; s++) {
        int size = sizes[s];
        int* base_array = malloc(size * sizeof(int));
        
        for (int t = 0; t < num_types; t++) {
            const char* array_type = array_types[t];
            
            // Generate test array
            set_random_seed(42);
            if (strcmp(array_type, "Random") == 0) {
                generate_random_array(base_array, size, size);
            } else if (strcmp(array_type, "Sorted") == 0) {
                generate_sorted_array(base_array, size);
            } else if (strcmp(array_type, "Reverse") == 0) {
                generate_reverse_sorted_array(base_array, size);
            } else if (strcmp(array_type, "Duplicates") == 0) {
                generate_mostly_duplicates_array(base_array, size, size / 10);
            }
            
            // Test different variants
            StepTracker test_tracker;
            init_step_tracker(&test_tracker, false);
            
            SortingMetrics variants[4];
            int* test_arrays[4];
            
            for (int i = 0; i < 4; i++) {
                reset_metrics(&variants[i]);
                test_arrays[i] = malloc(size * sizeof(int));
                copy_array(base_array, test_arrays[i], size);
            }
            
            // Test each variant
            set_random_seed(42);
            randomized_quicksort(test_arrays[0], size, &variants[0], &test_tracker);
            
            set_random_seed(42);
            three_way_quicksort(test_arrays[1], size, &variants[1], &test_tracker);
            
            set_random_seed(42);
            hybrid_quicksort(test_arrays[2], size, &variants[2], &test_tracker);
            
            deterministic_quicksort(test_arrays[3], size, &variants[3], &test_tracker);
            
            // Verify all arrays are sorted
            for (int i = 0; i < 4; i++) {
                if (!is_sorted(test_arrays[i], size)) {
                    printf("ERROR: Array %d not sorted properly!\n", i);
                }
            }
            
            // Print results
            printf("%-12s | %-8d | %-15s | %-15s | %-15s | %-15s\n",
                array_type, size,
                (char[16]){0}, (char[16]){0}, (char[16]){0}, (char[16]){0});
            
            // Format the results manually to avoid buffer issues
            printf("%-12s | %-8d | %3ldms/%2dd      | %3ldms/%2dd      | %3ldms/%2dd      | %3ldms/%2dd     \n",
                array_type, size,
                variants[0].execution_time_ms, variants[0].max_depth,
                variants[1].execution_time_ms, variants[1].max_depth,
                variants[2].execution_time_ms, variants[2].max_depth,
                variants[3].execution_time_ms, variants[3].max_depth);
            
            for (int i = 0; i < 4; i++) {
                free(test_arrays[i]);
            }
        }
        
        free(base_array);
    }
    
    // Test case 3: Demonstrate randomization benefits
    printf("\n%s\n", "============================================================");
    printf("Test Case 3: Randomization Benefits Analysis\n");
    
    // Create worst-case scenario for deterministic QuickSort
    const int worst_case_size = 1000;
    int* worst_case = malloc(worst_case_size * sizeof(int));
    generate_sorted_array(worst_case, worst_case_size);
    
    printf("Testing on sorted array (worst case for deterministic QuickSort):\n");
    
    StepTracker analyzer_tracker;
    init_step_tracker(&analyzer_tracker, false);
    
    SortingMetrics deterministic_worst, randomized_worst;
    reset_metrics(&deterministic_worst);
    reset_metrics(&randomized_worst);
    
    int* deterministic_array = malloc(worst_case_size * sizeof(int));
    int* randomized_array = malloc(worst_case_size * sizeof(int));
    
    copy_array(worst_case, deterministic_array, worst_case_size);
    copy_array(worst_case, randomized_array, worst_case_size);
    
    deterministic_quicksort(deterministic_array, worst_case_size, &deterministic_worst, &analyzer_tracker);
    
    set_random_seed(42);
    randomized_quicksort(randomized_array, worst_case_size, &randomized_worst, &analyzer_tracker);
    
    print_metrics("Deterministic QuickSort", &deterministic_worst);
    print_metrics("Randomized QuickSort", &randomized_worst);
    
    printf("Improvement: %.2fx faster, %.2fx less depth\n",
        (double) deterministic_worst.execution_time_ms / randomized_worst.execution_time_ms,
        (double) deterministic_worst.max_depth / randomized_worst.max_depth);
    
    // Performance analysis
    printf("\n=== Performance Analysis ===\n");
    printf("Randomized QuickSort Advantages:\n");
    printf("- Avoids worst-case O(n²) behavior with high probability\n");
    printf("- Expected O(n log n) performance regardless of input\n");
    printf("- Better cache performance due to balanced partitions\n");
    printf("- Robust against adversarial inputs\n");
    
    printf("\nVariant Comparisons:\n");
    printf("- Three-Way: Excellent for arrays with many duplicates\n");
    printf("- Hybrid: Best overall performance with insertion sort optimization\n");
    printf("- Standard: Good general-purpose randomized sorting\n");
    
    free(worst_case);
    free(deterministic_array);
    free(randomized_array);
    
    demonstrate_randomization_theory();
    
    return 0;
}