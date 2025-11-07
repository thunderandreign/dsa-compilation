/**
 * 0/1 Knapsack Problem - Branch & Bound Implementation in C
 * 
 * This implementation demonstrates the Branch & Bound strategy for solving
 * the 0/1 Knapsack problem, showcasing systematic exploration with pruning.
 * 
 * Key Features:
 * - Priority queue-based best-first search
 * - Fractional knapsack upper bound calculation
 * - Comprehensive pruning strategies
 * - Performance comparison with brute force and DP
 * 
 * Compilation: gcc -o knapsack KnapsackBranchBound.c -lm
 * Usage: ./knapsack
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>
#include <math.h>

#define MAX_ITEMS 20
#define MAX_CAPACITY 1000

typedef struct {
    int weight, value, index;
    double ratio; // value-to-weight ratio
} Item;

typedef struct {
    int level;              // Current item being considered
    int profit;             // Current profit
    int weight;             // Current weight
    double bound;           // Upper bound estimate
    int included[MAX_ITEMS]; // Items included (1) or not (0)
} KnapsackNode;

typedef struct {
    KnapsackNode nodes[10000]; // Priority queue
    int size;                  // Current size
} PriorityQueue;

typedef struct {
    int solution[MAX_ITEMS];   // Best solution found
    int max_profit;            // Maximum profit
    int total_weight;          // Total weight of solution
    int nodes_explored;        // Nodes explored
    int nodes_pruned;          // Nodes pruned
    double execution_time;     // Time in milliseconds
} KnapsackResult;

// Global variables
Item items[MAX_ITEMS];
int n, capacity;
int max_profit;
int best_solution[MAX_ITEMS];
int nodes_explored, nodes_pruned;
int verbose;

// Function prototypes
void solve_knapsack();
double calculate_bound(KnapsackNode* node);
void pq_init(PriorityQueue* pq);
void pq_push(PriorityQueue* pq, KnapsackNode node);
KnapsackNode pq_pop(PriorityQueue* pq);
int pq_empty(PriorityQueue* pq);
KnapsackResult brute_force_knapsack();
KnapsackResult dynamic_programming_knapsack();
void print_items();
void print_solution(int* solution);
int compare_items(const void* a, const void* b);
void demonstrate_scaling();

/**
 * Compare items by value-to-weight ratio (descending)
 */
int compare_items(const void* a, const void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    
    if (itemB->ratio > itemA->ratio) return 1;
    if (itemB->ratio < itemA->ratio) return -1;
    return 0;
}

/**
 * Initialize priority queue
 */
void pq_init(PriorityQueue* pq) {
    pq->size = 0;
}

/**
 * Push node to priority queue (max-heap based on bound)
 */
void pq_push(PriorityQueue* pq, KnapsackNode node) {
    int i = pq->size++;
    pq->nodes[i] = node;
    
    // Bubble up (max-heap)
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->nodes[i].bound <= pq->nodes[parent].bound) break;
        
        KnapsackNode temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[parent];
        pq->nodes[parent] = temp;
        i = parent;
    }
}

/**
 * Pop maximum bound node from priority queue
 */
KnapsackNode pq_pop(PriorityQueue* pq) {
    KnapsackNode max = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];
    
    // Bubble down
    int i = 0;
    while (2 * i + 1 < pq->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int largest = left;
        
        if (right < pq->size && pq->nodes[right].bound > pq->nodes[left].bound) {
            largest = right;
        }
        
        if (pq->nodes[i].bound >= pq->nodes[largest].bound) break;
        
        KnapsackNode temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[largest];
        pq->nodes[largest] = temp;
        i = largest;
    }
    
    return max;
}

/**
 * Check if priority queue is empty
 */
int pq_empty(PriorityQueue* pq) {
    return pq->size == 0;
}

/**
 * Calculate upper bound using fractional knapsack relaxation
 */
double calculate_bound(KnapsackNode* node) {
    if (node->weight >= capacity) return 0.0;
    
    double bound = node->profit;
    int remaining_capacity = capacity - node->weight;
    
    // Add items greedily (fractionally if necessary)
    for (int i = node->level; i < n; i++) {
        if (items[i].weight <= remaining_capacity) {
            // Include entire item
            bound += items[i].value;
            remaining_capacity -= items[i].weight;
        } else {
            // Include fraction of item
            bound += items[i].ratio * remaining_capacity;
            break;
        }
    }
    
    return bound;
}

/**
 * Solve knapsack using Branch & Bound
 */
void solve_knapsack() {
    clock_t start = clock();
    
    if (verbose) {
        printf("=== Starting Knapsack Branch & Bound Solution ===\n");
        printf("Capacity: %d, Items: %d\n", capacity, n);
    }
    
    // Initialize
    max_profit = 0;
    memset(best_solution, 0, sizeof(best_solution));
    nodes_explored = 0;
    nodes_pruned = 0;
    
    // Sort items by value-to-weight ratio
    qsort(items, n, sizeof(Item), compare_items);
    
    if (verbose) {
        printf("Items sorted by value-to-weight ratio:\n");
        for (int i = 0; i < n; i++) {
            printf("  Item%d: w=%d, v=%d, r=%.2f\n", 
                   items[i].index, items[i].weight, items[i].value, items[i].ratio);
        }
    }
    
    // Initialize root node
    KnapsackNode root;
    root.level = 0;
    root.profit = 0;
    root.weight = 0;
    memset(root.included, 0, sizeof(root.included));
    root.bound = calculate_bound(&root);
    
    if (verbose) {
        printf("Root node bound: %.2f\n", root.bound);
    }
    
    // Initialize priority queue
    PriorityQueue pq;
    pq_init(&pq);
    pq_push(&pq, root);
    
    while (!pq_empty(&pq)) {
        KnapsackNode current = pq_pop(&pq);
        nodes_explored++;
        
        if (verbose && nodes_explored <= 15) {
            printf("Exploring node at level %d, profit: %d, weight: %d, bound: %.2f\n",
                   current.level, current.profit, current.weight, current.bound);
        }
        
        // Pruning check
        if (current.bound <= max_profit) {
            nodes_pruned++;
            if (verbose && nodes_explored <= 15) {
                printf("  Pruned: bound %.2f <= best %d\n", current.bound, max_profit);
            }
            continue;
        }
        
        // Check if we've reached a leaf node
        if (current.level == n) {
            if (current.profit > max_profit) {
                max_profit = current.profit;
                memcpy(best_solution, current.included, sizeof(current.included));
                
                if (verbose) {
                    printf("*** New best solution found! ***\n");
                    printf("Profit: %d, Weight: %d\n", max_profit, current.weight);
                }
            }
            continue;
        }
        
        Item current_item = items[current.level];
        
        // Branch 1: Include current item (if it fits)
        if (current.weight + current_item.weight <= capacity) {
            KnapsackNode include_node = current;
            include_node.level = current.level + 1;
            include_node.profit = current.profit + current_item.value;
            include_node.weight = current.weight + current_item.weight;
            include_node.included[current.level] = 1;
            include_node.bound = calculate_bound(&include_node);
            
            if (include_node.bound > max_profit) {
                pq_push(&pq, include_node);
            } else {
                nodes_pruned++;
            }
        }
        
        // Branch 2: Exclude current item
        KnapsackNode exclude_node = current;
        exclude_node.level = current.level + 1;
        exclude_node.bound = calculate_bound(&exclude_node);
        
        if (exclude_node.bound > max_profit) {
            pq_push(&pq, exclude_node);
        } else {
            nodes_pruned++;
        }
    }
    
    clock_t end = clock();
    double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    if (verbose) {
        printf("=== Final Results ===\n");
        printf("Maximum profit: %d\n", max_profit);
        
        int total_weight = 0;
        printf("Selected items: ");
        for (int i = 0; i < n; i++) {
            if (best_solution[i]) {
                printf("Item%d ", items[i].index);
                total_weight += items[i].weight;
            }
        }
        printf("\nTotal weight: %d\n", total_weight);
        printf("Nodes explored: %d\n", nodes_explored);
        printf("Nodes pruned: %d\n", nodes_pruned);
        printf("Execution time: %.2f ms\n", execution_time);
    }
}

/**
 * Brute force knapsack solution for comparison
 */
KnapsackResult brute_force_knapsack() {
    clock_t start = clock();
    
    int max_profit = 0;
    int best_solution[MAX_ITEMS] = {0};
    int combinations = 1 << n; // 2^n combinations
    
    for (int mask = 0; mask < combinations; mask++) {
        int current_weight = 0, current_profit = 0;
        int current_solution[MAX_ITEMS] = {0};
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                current_weight += items[i].weight;
                current_profit += items[i].value;
                current_solution[i] = 1;
            }
        }
        
        if (current_weight <= capacity && current_profit > max_profit) {
            max_profit = current_profit;
            memcpy(best_solution, current_solution, sizeof(current_solution));
        }
    }
    
    clock_t end = clock();
    double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    KnapsackResult result;
    memcpy(result.solution, best_solution, sizeof(best_solution));
    result.max_profit = max_profit;
    result.nodes_explored = combinations;
    result.nodes_pruned = 0;
    result.execution_time = execution_time;
    
    // Calculate total weight
    result.total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (best_solution[i]) {
            result.total_weight += items[i].weight;
        }
    }
    
    return result;
}

/**
 * Dynamic programming knapsack solution for comparison
 */
KnapsackResult dynamic_programming_knapsack() {
    clock_t start = clock();
    
    int dp[MAX_ITEMS + 1][MAX_CAPACITY + 1];
    
    // Initialize DP table
    for (int i = 0; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            dp[i][w] = 0;
        }
    }
    
    // Fill DP table
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= capacity; w++) {
            if (items[i-1].weight <= w) {
                int include = dp[i-1][w - items[i-1].weight] + items[i-1].value;
                dp[i][w] = (dp[i-1][w] > include) ? dp[i-1][w] : include;
            } else {
                dp[i][w] = dp[i-1][w];
            }
        }
    }
    
    // Backtrack to find solution
    int solution[MAX_ITEMS] = {0};
    int w = capacity;
    for (int i = n; i > 0 && dp[i][w] > 0; i--) {
        if (dp[i][w] != dp[i-1][w]) {
            solution[i-1] = 1;
            w -= items[i-1].weight;
        }
    }
    
    clock_t end = clock();
    double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    KnapsackResult result;
    memcpy(result.solution, solution, sizeof(solution));
    result.max_profit = dp[n][capacity];
    result.nodes_explored = (n + 1) * (capacity + 1);
    result.nodes_pruned = 0;
    result.execution_time = execution_time;
    
    // Calculate total weight
    result.total_weight = 0;
    for (int i = 0; i < n; i++) {
        if (solution[i]) {
            result.total_weight += items[i].weight;
        }
    }
    
    return result;
}

/**
 * Print items
 */
void print_items() {
    printf("Items:\n");
    for (int i = 0; i < n; i++) {
        printf("  Item%d: weight=%d, value=%d, ratio=%.2f\n", 
               i, items[i].weight, items[i].value, items[i].ratio);
    }
}

/**
 * Print solution
 */
void print_solution(int* solution) {
    printf("Selected items: ");
    for (int i = 0; i < n; i++) {
        if (solution[i]) {
            printf("Item%d ", i);
        }
    }
    printf("\n");
}

/**
 * Demonstrate scaling behavior
 */
void demonstrate_scaling() {
    printf("\n=== Scaling Analysis ===\n");
    
    srand(42);
    int sizes[] = {5, 7, 10};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int base_capacity = 20;
    
    printf("Items | Capacity | Nodes Explored | Nodes Pruned | Pruning %% | Time (ms)\n");
    printf("-----------------------------------------------------------------------\n");
    
    for (int s = 0; s < num_sizes; s++) {
        n = sizes[s];
        capacity = base_capacity + n * 2;
        
        // Generate random items
        for (int i = 0; i < n; i++) {
            items[i].weight = 1 + rand() % 10;
            items[i].value = 5 + rand() % 45;
            items[i].index = i;
            items[i].ratio = (double)items[i].value / items[i].weight;
        }
        
        verbose = 0;
        solve_knapsack();
        
        double pruning_percent = (double)nodes_pruned / (nodes_explored + nodes_pruned) * 100.0;
        
        printf("%5d | %8d | %14d | %12d | %8.1f%% | %8.2f\n",
               n, capacity, nodes_explored, nodes_pruned, 
               pruning_percent, 0.0); // Time calculation simplified for this demo
    }
    
    printf("\nKey Observations:\n");
    printf("- Pruning becomes more effective with larger problems\n");
    printf("- Good bounds are essential for performance\n");
    printf("- Best-first search finds optimal solutions quickly\n");
}

int main() {
    printf("=== 0/1 Knapsack Problem - Branch & Bound ===\n\n");
    
    // Test case 1: Classic example
    n = 3;
    capacity = 50;
    
    items[0] = (Item){10, 60, 0, 6.0};
    items[1] = (Item){20, 100, 1, 5.0};
    items[2] = (Item){30, 120, 2, 4.0};
    
    printf("Test Case 1: Classic 3-item knapsack\n");
    print_items();
    printf("Capacity: %d\n", capacity);
    
    verbose = 1;
    solve_knapsack();
    
    // Compare with other methods
    printf("\nComparing with other methods:\n");
    
    KnapsackResult brute_result = brute_force_knapsack();
    printf("=== Brute Force Results ===\n");
    printf("Combinations checked: %d\n", brute_result.nodes_explored);
    printf("Maximum profit: %d\n", brute_result.max_profit);
    printf("Execution time: %.2f ms\n", brute_result.execution_time);
    
    KnapsackResult dp_result = dynamic_programming_knapsack();
    printf("=== Dynamic Programming Results ===\n");
    printf("DP table size: %dx%d\n", n + 1, capacity + 1);
    printf("Maximum profit: %d\n", dp_result.max_profit);
    printf("Execution time: %.2f ms\n", dp_result.execution_time);
    
    // Test case 2: Larger problem
    printf("\n============================================================\n");
    printf("Test Case 2: Larger knapsack problem\n");
    
    n = 7;
    capacity = 15;
    
    int weights[] = {5, 4, 6, 3, 7, 2, 8};
    int values[] = {10, 40, 30, 50, 20, 60, 25};
    
    for (int i = 0; i < n; i++) {
        items[i].weight = weights[i];
        items[i].value = values[i];
        items[i].index = i;
        items[i].ratio = (double)values[i] / weights[i];
    }
    
    print_items();
    printf("Capacity: %d\n", capacity);
    
    verbose = 0;
    solve_knapsack();
    
    printf("\nBranch & Bound Results:\n");
    printf("Maximum profit: %d\n", max_profit);
    
    int total_weight = 0;
    printf("Selected items: ");
    for (int i = 0; i < n; i++) {
        if (best_solution[i]) {
            printf("Item%d ", items[i].index);
            total_weight += items[i].weight;
        }
    }
    printf("\nTotal weight: %d\n", total_weight);
    printf("Nodes explored: %d\n", nodes_explored);
    printf("Nodes pruned: %d\n", nodes_pruned);
    
    // Performance comparison
    KnapsackResult brute_result2 = brute_force_knapsack();
    KnapsackResult dp_result2 = dynamic_programming_knapsack();
    
    printf("\n=== Performance Comparison ===\n");
    printf("%-15s | %12s | %12s | %10s\n", "Method", "Operations", "Space", "Time (ms)");
    printf("-------------------------------------------------------\n");
    printf("%-15s | %12d | %12s | %10.2f\n", "Branch & Bound", 
           nodes_explored, "O(n)", 0.0);
    printf("%-15s | %12d | %12s | %10.2f\n", "Brute Force", 
           brute_result2.nodes_explored, "O(1)", brute_result2.execution_time);
    printf("%-15s | %12d | %12s | %10.2f\n", "Dynamic Prog", 
           dp_result2.nodes_explored, "O(nW)", dp_result2.execution_time);
    
    printf("\n=== When to use each approach ===\n");
    printf("Branch & Bound:\n");
    printf("- When you need optimal solution\n");
    printf("- Items have good value-to-weight ratios\n");
    printf("- Memory is limited\n");
    printf("- Can afford variable execution time\n");
    
    printf("\nDynamic Programming:\n");
    printf("- When capacity is not too large\n");
    printf("- Consistent execution time needed\n");
    printf("- Plenty of memory available\n");
    
    printf("\nBrute Force:\n");
    printf("- Very small problem sizes only\n");
    printf("- When implementation simplicity matters\n");
    
    demonstrate_scaling();
    
    return 0;
}