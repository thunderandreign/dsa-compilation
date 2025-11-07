/**
 * Skip List - Advanced Probabilistic Data Structure C Implementation
 * 
 * This implementation demonstrates the Skip List, a probabilistic alternative to
 * balanced trees that achieves O(log n) expected performance for search, insertion,
 * and deletion through randomized level assignment.
 * 
 * Key Concepts:
 * - Probabilistic Balancing: Uses randomization instead of complex rebalancing
 * - Expected Performance: O(log n) search/insert/delete with high probability
 * - Space-Time Tradeoff: Additional pointers for better performance
 * - Simplicity: Easier to implement than balanced trees
 * 
 * Time Complexity:
 * - Search: O(log n) expected, O(n) worst case
 * - Insert: O(log n) expected, O(n) worst case  
 * - Delete: O(log n) expected, O(n) worst case
 * Space Complexity: O(n log n) expected
 * 
 * Applications:
 * - In-memory databases and key-value stores
 * - Concurrent data structures (lock-free implementations)
 * - Redis sorted sets implementation
 * - Alternative to balanced trees in systems programming
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <stdbool.h>

#define MAX_LEVEL 16
#define PROBABILITY 0.5
#define MAX_VALUE_LENGTH 256
#define MAX_OPERATIONS 1000

typedef struct SkipListNode {
    int key;
    char value[MAX_VALUE_LENGTH];
    struct SkipListNode** forward;
    int level;
} SkipListNode;

typedef struct {
    bool found;
    char value[MAX_VALUE_LENGTH];
    int comparisons;
    int search_path[MAX_LEVEL];
    int path_length;
} SearchResult;

typedef struct {
    int total_nodes;
    int max_level;
    double average_level;
    int total_forward_pointers;
    double space_overhead;
    int level_distribution[MAX_LEVEL + 1];
} PerformanceMetrics;

typedef struct {
    SkipListNode* header;
    int current_level;
    int size;
    bool verbose;
    char operations[MAX_OPERATIONS][200];
    int operation_count;
} SkipList;

// Global random state
static unsigned int random_seed = 1;

// Utility functions
void set_random_seed(unsigned int seed) {
    random_seed = seed;
    srand(seed);
}

double uniform_random() {
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fffffff;
    return (double)random_seed / 0x7fffffff;
}

void add_operation(SkipList* list, const char* operation) {
    if (list->verbose && list->operation_count < MAX_OPERATIONS) {
        strncpy(list->operations[list->operation_count], operation, 199);
        list->operations[list->operation_count][199] = '\0';
        list->operation_count++;
    }
}

void print_operations(const SkipList* list) {
    for (int i = 0; i < list->operation_count; i++) {
        printf("  %s\n", list->operations[i]);
    }
}

void clear_operations(SkipList* list) {
    list->operation_count = 0;
}

// Create a new skip list node
SkipListNode* create_node(int key, const char* value, int level) {
    SkipListNode* node = malloc(sizeof(SkipListNode));
    node->key = key;
    strncpy(node->value, value, MAX_VALUE_LENGTH - 1);
    node->value[MAX_VALUE_LENGTH - 1] = '\0';
    node->level = level;
    node->forward = malloc((level + 1) * sizeof(SkipListNode*));
    
    for (int i = 0; i <= level; i++) {
        node->forward[i] = NULL;
    }
    
    return node;
}

// Free a skip list node
void free_node(SkipListNode* node) {
    if (node) {
        free(node->forward);
        free(node);
    }
}

// Generate random level using geometric distribution
int random_level() {
    int level = 0;
    while (uniform_random() < PROBABILITY && level < MAX_LEVEL) {
        level++;
    }
    return level;
}

// Initialize skip list
SkipList* create_skip_list(bool verbose) {
    SkipList* list = malloc(sizeof(SkipList));
    list->header = create_node(INT_MIN, "", MAX_LEVEL);
    list->current_level = 0;
    list->size = 0;
    list->verbose = verbose;
    list->operation_count = 0;
    
    if (verbose) {
        add_operation(list, "=== Skip List Initialized ===");
        char msg[200];
        sprintf(msg, "Max Level: %d, Probability: %.2f", MAX_LEVEL, PROBABILITY);
        add_operation(list, msg);
    }
    
    return list;
}

// Search for a key in the skip list
SearchResult search(SkipList* list, int key) {
    SearchResult result;
    result.found = false;
    result.comparisons = 0;
    result.path_length = 0;
    memset(result.value, 0, MAX_VALUE_LENGTH);
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "=== Searching for key %d ===", key);
        add_operation(list, msg);
    }
    
    SkipListNode* current = list->header;
    
    // Start from the highest level and work downward
    for (int level = list->current_level; level >= 0; level--) {
        while (current->forward[level] != NULL && current->forward[level]->key < key) {
            current = current->forward[level];
            if (result.path_length < MAX_LEVEL) {
                result.search_path[result.path_length++] = current->key;
            }
            result.comparisons++;
            
            if (list->verbose) {
                char msg[200];
                sprintf(msg, "Level %d: Moving to node %d", level, current->key);
                add_operation(list, msg);
            }
        }
        result.comparisons++; // Count the comparison that failed the while condition
        
        if (list->verbose && level > 0) {
            char msg[200];
            sprintf(msg, "Level %d: Dropping down", level);
            add_operation(list, msg);
        }
    }
    
    // Move to the next node at level 0
    current = current->forward[0];
    if (current != NULL) {
        result.comparisons++;
    }
    
    result.found = current != NULL && current->key == key;
    if (result.found) {
        strcpy(result.value, current->value);
    }
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "Search result: %s, Comparisons: %d", 
                result.found ? "FOUND" : "NOT FOUND", result.comparisons);
        add_operation(list, msg);
    }
    
    return result;
}

// Insert a key-value pair into the skip list
bool insert(SkipList* list, int key, const char* value) {
    SkipListNode* update[MAX_LEVEL + 1];
    SkipListNode* current = list->header;
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "=== Inserting key %d, value %s ===", key, value);
        add_operation(list, msg);
    }
    
    // Find the insertion point and record update pointers
    for (int level = list->current_level; level >= 0; level--) {
        while (current->forward[level] != NULL && current->forward[level]->key < key) {
            current = current->forward[level];
        }
        update[level] = current;
        
        if (list->verbose) {
            char msg[200];
            sprintf(msg, "Level %d: Update pointer at node %d", level, current->key);
            add_operation(list, msg);
        }
    }
    
    current = current->forward[0];
    
    // If key already exists, update the value
    if (current != NULL && current->key == key) {
        strcpy(current->value, value);
        if (list->verbose) {
            add_operation(list, "Key already exists - updated value");
        }
        return false; // Indicate that no new node was inserted
    }
    
    // Generate random level for new node
    int new_level = random_level();
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "Generated random level: %d", new_level);
        add_operation(list, msg);
    }
    
    // If new level is higher than current level, update header pointers
    if (new_level > list->current_level) {
        for (int level = list->current_level + 1; level <= new_level; level++) {
            update[level] = list->header;
        }
        list->current_level = new_level;
        
        if (list->verbose) {
            char msg[200];
            sprintf(msg, "Increased current level to: %d", list->current_level);
            add_operation(list, msg);
        }
    }
    
    // Create new node and update pointers
    SkipListNode* new_node = create_node(key, value, new_level);
    
    for (int level = 0; level <= new_level; level++) {
        new_node->forward[level] = update[level]->forward[level];
        update[level]->forward[level] = new_node;
        
        if (list->verbose) {
            char msg[200];
            sprintf(msg, "Level %d: Linked new node", level);
            add_operation(list, msg);
        }
    }
    
    list->size++;
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "Insertion completed. Size: %d", list->size);
        add_operation(list, msg);
    }
    
    return true;
}

// Delete a key from the skip list
bool delete_key(SkipList* list, int key) {
    SkipListNode* update[MAX_LEVEL + 1];
    SkipListNode* current = list->header;
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "=== Deleting key %d ===", key);
        add_operation(list, msg);
    }
    
    // Find the node to delete and record update pointers
    for (int level = list->current_level; level >= 0; level--) {
        while (current->forward[level] != NULL && current->forward[level]->key < key) {
            current = current->forward[level];
        }
        update[level] = current;
    }
    
    current = current->forward[0];
    
    // If key doesn't exist, return false
    if (current == NULL || current->key != key) {
        if (list->verbose) {
            add_operation(list, "Key not found - deletion failed");
        }
        return false;
    }
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "Found node to delete: [key=%d, value=%s, level=%d]", 
                current->key, current->value, current->level);
        add_operation(list, msg);
    }
    
    // Update pointers to skip the deleted node
    for (int level = 0; level <= current->level; level++) {
        update[level]->forward[level] = current->forward[level];
        
        if (list->verbose) {
            char msg[200];
            sprintf(msg, "Level %d: Updated pointer to skip deleted node", level);
            add_operation(list, msg);
        }
    }
    
    // Free the deleted node
    free_node(current);
    
    // Reduce current level if necessary
    while (list->current_level > 0 && list->header->forward[list->current_level] == NULL) {
        list->current_level--;
        if (list->verbose) {
            char msg[200];
            sprintf(msg, "Reduced current level to: %d", list->current_level);
            add_operation(list, msg);
        }
    }
    
    list->size--;
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "Deletion completed. Size: %d", list->size);
        add_operation(list, msg);
    }
    
    return true;
}

// Display the skip list structure
void display(const SkipList* list) {
    printf("=== Skip List Structure ===\n");
    printf("Size: %d, Current Level: %d\n", list->size, list->current_level);
    
    for (int level = list->current_level; level >= 0; level--) {
        printf("Level %2d: ", level);
        SkipListNode* current = list->header->forward[level];
        
        while (current != NULL) {
            printf("[%d:%s] -> ", current->key, current->value);
            current = current->forward[level];
        }
        printf("NULL\n");
    }
    printf("\n");
}

// Range query: find all keys in the given range [min_key, max_key]
int range_query(SkipList* list, int min_key, int max_key, 
                int* result_keys, char result_values[][MAX_VALUE_LENGTH]) {
    int count = 0;
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "=== Range Query [%d, %d] ===", min_key, max_key);
        add_operation(list, msg);
    }
    
    // Find the first node >= min_key
    SkipListNode* current = list->header;
    for (int level = list->current_level; level >= 0; level--) {
        while (current->forward[level] != NULL && current->forward[level]->key < min_key) {
            current = current->forward[level];
        }
    }
    
    current = current->forward[0];
    
    // Collect all nodes in the range
    while (current != NULL && current->key <= max_key) {
        result_keys[count] = current->key;
        strcpy(result_values[count], current->value);
        count++;
        current = current->forward[0];
    }
    
    if (list->verbose) {
        char msg[200];
        sprintf(msg, "Range query found %d entries", count);
        add_operation(list, msg);
    }
    
    return count;
}

// Calculate performance metrics
PerformanceMetrics calculate_metrics(const SkipList* list) {
    PerformanceMetrics metrics;
    memset(&metrics, 0, sizeof(PerformanceMetrics));
    
    metrics.total_nodes = list->size;
    metrics.max_level = list->current_level;
    
    // Calculate level distribution and average level
    int total_levels = 0;
    int total_pointers = 0;
    
    SkipListNode* current = list->header->forward[0];
    while (current != NULL) {
        int node_level = current->level;
        total_levels += node_level + 1; // +1 because level is 0-indexed
        total_pointers += node_level + 1;
        
        metrics.level_distribution[node_level]++;
        current = current->forward[0];
    }
    
    if (list->size > 0) {
        metrics.average_level = (double) total_levels / list->size;
        metrics.space_overhead = (double) total_pointers / list->size;
    }
    
    metrics.total_forward_pointers = total_pointers;
    
    return metrics;
}

// Verify skip list invariants
bool verify_integrity(const SkipList* list) {
    // Check if keys are in sorted order at level 0
    SkipListNode* current = list->header->forward[0];
    int prev_key = INT_MIN;
    
    while (current != NULL) {
        if (current->key <= prev_key) {
            printf("Integrity violation: Key %d <= previous key %d\n", 
                   current->key, prev_key);
            return false;
        }
        prev_key = current->key;
        current = current->forward[0];
    }
    
    // Check if higher levels are subsets of lower levels
    for (int level = 1; level <= list->current_level; level++) {
        bool current_level_keys[100000] = {false}; // Simple bitmap for small ranges
        bool lower_level_keys[100000] = {false};
        
        // Collect keys at current level
        current = list->header->forward[level];
        while (current != NULL) {
            if (current->key >= 0 && current->key < 100000) {
                current_level_keys[current->key] = true;
            }
            current = current->forward[level];
        }
        
        // Collect keys at level below
        current = list->header->forward[level - 1];
        while (current != NULL) {
            if (current->key >= 0 && current->key < 100000) {
                lower_level_keys[current->key] = true;
            }
            current = current->forward[level - 1];
        }
        
        // Current level should be a subset of lower level
        for (int i = 0; i < 100000; i++) {
            if (current_level_keys[i] && !lower_level_keys[i]) {
                printf("Integrity violation: Level %d is not a subset of level %d\n", 
                       level, level - 1);
                return false;
            }
        }
    }
    
    return true;
}

// Free the entire skip list
void free_skip_list(SkipList* list) {
    SkipListNode* current = list->header;
    
    while (current != NULL) {
        SkipListNode* next = current->forward[0];
        free_node(current);
        current = next;
    }
    
    free(list);
}

void print_search_result(const SearchResult* result, int key) {
    printf("Search(%d): Found: %s, Value: %s, Comparisons: %d\n",
           key, result->found ? "true" : "false", 
           result->found ? result->value : "null", result->comparisons);
}

void print_performance_metrics(const PerformanceMetrics* metrics) {
    printf("Nodes: %d, Max Level: %d, Avg Level: %.2f, "
           "Total Pointers: %d, Space Overhead: %.2fx\n",
           metrics->total_nodes, metrics->max_level, metrics->average_level, 
           metrics->total_forward_pointers, metrics->space_overhead);
}

void demonstrate_real_world_applications() {
    printf("\n=== Real-World Applications ===\n");
    
    // Application 1: In-memory database index
    printf("\n1. In-Memory Database Index Simulation\n");
    SkipList* db_index = create_skip_list(false);
    
    // Simulate database records
    const char* records[][3] = {
        {"1", "emp_001", "John Doe:Software Engineer"},
        {"3", "emp_003", "Alice Smith:Product Manager"}, 
        {"7", "emp_007", "Bob Johnson:Data Scientist"},
        {"12", "emp_012", "Carol Brown:UX Designer"},
        {"19", "emp_019", "David Wilson:DevOps Engineer"}
    };
    
    // Index by employee ID
    for (int i = 0; i < 5; i++) {
        int emp_id = atoi(records[i][0]);
        insert(db_index, emp_id, records[i][2]);
    }
    
    printf("Database Index Contents:\n");
    SkipListNode* current = db_index->header->forward[0];
    while (current != NULL) {
        printf("ID %03d: %s\n", current->key, current->value);
        current = current->forward[0];
    }
    
    // Demonstrate range queries
    printf("\nEmployees with IDs 5-15:\n");
    int range_keys[10];
    char range_values[10][MAX_VALUE_LENGTH];
    int range_count = range_query(db_index, 5, 15, range_keys, range_values);
    
    for (int i = 0; i < range_count; i++) {
        printf("  %s\n", range_values[i]);
    }
    
    free_skip_list(db_index);
    
    // Application 2: Priority queue simulation
    printf("\n2. Task Scheduling Priority Queue\n");
    SkipList* task_queue = create_skip_list(false);
    
    // Simulate tasks with priorities (lower number = higher priority)
    const char* tasks[][2] = {
        {"1", "Critical Security Patch"},
        {"3", "Database Backup"},
        {"5", "Code Review"},
        {"7", "Update Documentation"},
        {"2", "System Monitoring"}
    };
    
    for (int i = 0; i < 5; i++) {
        int priority = atoi(tasks[i][0]);
        insert(task_queue, priority, tasks[i][1]);
    }
    
    printf("Task Queue (by priority):\n");
    current = task_queue->header->forward[0];
    while (current != NULL) {
        printf("Priority %d: %s\n", current->key, current->value);
        current = current->forward[0];
    }
    
    // Process high-priority tasks (priority <= 3)
    printf("\nHigh-priority tasks to process first:\n");
    int high_priority_keys[10];
    char high_priority_values[10][MAX_VALUE_LENGTH];
    int high_priority_count = range_query(task_queue, 1, 3, high_priority_keys, high_priority_values);
    
    for (int i = 0; i < high_priority_count; i++) {
        printf("  Processing: %s (Priority %d)\n", high_priority_values[i], high_priority_keys[i]);
    }
    
    free_skip_list(task_queue);
    
    printf("\nSkip List Applications Summary:\n");
    printf("- Redis Sorted Sets (ZSET commands)\n");
    printf("- Concurrent data structures in systems programming\n");
    printf("- Database indexing systems\n");
    printf("- Priority queues and schedulers\n");
    printf("- Time-series data storage\n");
    printf("- Distributed systems coordination\n");
}

int main() {
    printf("=== Skip List - Comprehensive Analysis ===\n\n");
    
    set_random_seed(42); // Fixed seed for reproducible results
    
    // Test case 1: Basic operations with step-by-step demonstration
    printf("Test Case 1: Basic Operations\n");
    SkipList* skip_list = create_skip_list(true);
    
    // Insert some key-value pairs
    int keys[] = {3, 6, 7, 9, 12, 19, 17, 26, 21, 25};
    const char* values[] = {"three", "six", "seven", "nine", "twelve", 
                           "nineteen", "seventeen", "twenty-six", "twenty-one", "twenty-five"};
    int num_keys = sizeof(keys) / sizeof(keys[0]);
    
    printf("Inserting key-value pairs:\n");
    for (int i = 0; i < num_keys; i++) {
        insert(skip_list, keys[i], values[i]);
        if (i < 5) { // Show details for first few insertions
            printf("\nOperations for insert(%d, %s):\n", keys[i], values[i]);
            print_operations(skip_list);
            clear_operations(skip_list);
        }
    }
    
    display(skip_list);
    
    // Search operations
    printf("Search Operations:\n");
    int search_keys[] = {7, 15, 21, 30};
    int num_search_keys = sizeof(search_keys) / sizeof(search_keys[0]);
    
    for (int i = 0; i < num_search_keys; i++) {
        SearchResult result = search(skip_list, search_keys[i]);
        print_search_result(&result, search_keys[i]);
        
        if (skip_list->verbose && search_keys[i] == 7) {
            printf("Detailed search path:\n");
            print_operations(skip_list);
            clear_operations(skip_list);
        }
    }
    
    // Test case 2: Performance comparison with different data sizes
    printf("\n%s\n", "============================================================");
    printf("Test Case 2: Performance Analysis\n");
    
    int data_sizes[] = {100, 1000, 10000};
    int num_sizes = sizeof(data_sizes) / sizeof(data_sizes[0]);
    
    for (int s = 0; s < num_sizes; s++) {
        int data_size = data_sizes[s];
        printf("\nAnalyzing Skip List with %d elements:\n", data_size);
        
        SkipList* perf_list = create_skip_list(false);
        set_random_seed(42);
        
        // Insert random data
        clock_t insert_start = clock();
        bool inserted_keys[data_size * 2];
        memset(inserted_keys, false, sizeof(inserted_keys));
        int actual_size = 0;
        
        for (int i = 0; i < data_size; i++) {
            int key = rand() % (data_size * 2);
            if (!inserted_keys[key]) {
                inserted_keys[key] = true;
                actual_size++;
            }
            char value[50];
            sprintf(value, "value%d", key);
            insert(perf_list, key, value);
        }
        
        clock_t insert_end = clock();
        double insert_time = ((double)(insert_end - insert_start)) / CLOCKS_PER_SEC * 1000;
        
        // Create array of inserted keys for searching
        int* key_array = malloc(actual_size * sizeof(int));
        int key_count = 0;
        for (int i = 0; i < data_size * 2; i++) {
            if (inserted_keys[i]) {
                key_array[key_count++] = i;
            }
        }
        
        // Perform random searches
        int num_searches = (actual_size < 1000) ? actual_size : 1000;
        int total_comparisons = 0;
        
        clock_t search_start = clock();
        for (int i = 0; i < num_searches; i++) {
            int random_key = key_array[rand() % actual_size];
            SearchResult result = search(perf_list, random_key);
            total_comparisons += result.comparisons;
        }
        clock_t search_end = clock();
        double search_time = ((double)(search_end - search_start)) / CLOCKS_PER_SEC * 1000;
        
        // Calculate metrics
        PerformanceMetrics metrics = calculate_metrics(perf_list);
        double avg_comparisons = (double) total_comparisons / num_searches;
        double theoretical_comparisons = log2(perf_list->size);
        
        printf("Size: %d\n", perf_list->size);
        printf("Insert Time: %.2f ms\n", insert_time);
        printf("Search Time: %.2f ms (%d searches)\n", search_time, num_searches);
        printf("Average Comparisons: %.2f (theoretical: %.2f)\n", 
               avg_comparisons, theoretical_comparisons);
        printf("Performance Metrics: ");
        print_performance_metrics(&metrics);
        
        // Verify integrity
        bool is_valid = verify_integrity(perf_list);
        printf("Integrity Check: %s\n", is_valid ? "PASSED" : "FAILED");
        
        free(key_array);
        free_skip_list(perf_list);
    }
    
    // Test case 3: Level distribution analysis
    printf("\n%s\n", "============================================================");
    printf("Test Case 3: Level Distribution Analysis\n");
    
    SkipList* level_analysis = create_skip_list(false);
    set_random_seed(42);
    
    // Insert 10000 elements and analyze level distribution
    for (int i = 0; i < 10000; i++) {
        int key = rand() % 50000;
        char value[50];
        sprintf(value, "value%d", i);
        insert(level_analysis, key, value);
    }
    
    PerformanceMetrics level_metrics = calculate_metrics(level_analysis);
    
    printf("Level Distribution:\n");
    printf("%-8s | %-8s | %-12s | %-12s\n", "Level", "Count", "Percentage", "Expected %");
    printf("--------------------------------------------------\n");
    
    for (int level = 0; level <= level_metrics.max_level; level++) {
        int count = level_metrics.level_distribution[level];
        double percentage = (count * 100.0) / level_metrics.total_nodes;
        double expected = 50.0 * pow(0.5, level); // Expected percentage
        
        printf("%-8d | %-8d | %-12.2f | %-12.2f\n", 
               level, count, percentage, expected);
    }
    
    printf("\nTheoretical vs Actual Analysis:\n");
    printf("Expected Average Level: %.2f\n", 1.0 / PROBABILITY);
    printf("Actual Average Level: %.2f\n", level_metrics.average_level);
    printf("Expected Space Overhead: %.2fx\n", 1.0 / (1.0 - PROBABILITY));
    printf("Actual Space Overhead: %.2fx\n", level_metrics.space_overhead);
    
    free_skip_list(level_analysis);
    
    // Test case 4: Range query demonstration
    printf("\n%s\n", "============================================================");
    printf("Test Case 4: Range Query Operations\n");
    
    SkipList* range_list = create_skip_list(true);
    
    // Insert ordered data
    for (int i = 1; i <= 20; i += 2) {
        char value[50];
        sprintf(value, "value%d", i);
        insert(range_list, i, value);
    }
    
    display(range_list);
    
    // Perform range queries
    int ranges[][2] = {{3, 9}, {10, 15}, {5, 25}};
    int num_ranges = sizeof(ranges) / sizeof(ranges[0]);
    
    for (int r = 0; r < num_ranges; r++) {
        int result_keys[50];
        char result_values[50][MAX_VALUE_LENGTH];
        int count = range_query(range_list, ranges[r][0], ranges[r][1], result_keys, result_values);
        
        printf("Range [%d, %d]: ", ranges[r][0], ranges[r][1]);
        for (int i = 0; i < count; i++) {
            printf("(%d:%s) ", result_keys[i], result_values[i]);
        }
        printf("\n");
    }
    
    free_skip_list(range_list);
    free_skip_list(skip_list);
    
    printf("\n=== Skip List Analysis Summary ===\n");
    printf("Key Advantages:\n");
    printf("- Simple probabilistic balancing (no complex rotations)\n");
    printf("- Expected O(log n) performance for all operations\n");
    printf("- Excellent for concurrent implementations\n");
    printf("- Natural range query support\n");
    printf("- Predictable memory access patterns\n");
    
    printf("\nComparison with Balanced Trees:\n");
    printf("- Simpler implementation than AVL/Red-Black trees\n");
    printf("- Better constant factors in practice\n");
    printf("- More space overhead but better cache performance\n");
    printf("- Probabilistic guarantees vs deterministic guarantees\n");
    
    demonstrate_real_world_applications();
    
    return 0;
}