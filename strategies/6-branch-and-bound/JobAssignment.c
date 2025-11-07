/**
 * Job Assignment Problem - Branch & Bound Implementation in C
 * 
 * This implementation demonstrates the Branch & Bound strategy for solving
 * the optimal assignment problem in C, showcasing systematic exploration
 * with efficient cost-based pruning.
 * 
 * Key Features:
 * - Priority queue-based best-first search
 * - Row and column reduction for bounds
 * - Comprehensive assignment tracking
 * - Performance comparison with brute force
 * 
 * Compilation: gcc -o assignment JobAssignment.c -lm
 * Usage: ./assignment
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define MAX_SIZE 10
#define INF INT_MAX

typedef struct {
    int level;                      // Current worker being assigned
    int cost;                       // Current assignment cost
    int bound;                      // Lower bound estimate
    int assignment[MAX_SIZE];       // job[i] = worker assigned to job i
    int worker_used[MAX_SIZE];      // Which workers are already assigned
} AssignmentNode;

typedef struct {
    AssignmentNode nodes[10000];    // Priority queue
    int size;                       // Current size
} PriorityQueue;

typedef struct {
    int assignment[MAX_SIZE];       // Best assignment found
    int min_cost;                   // Minimum cost
    int nodes_explored;             // Nodes explored
    int nodes_pruned;               // Nodes pruned
    double execution_time;          // Time in milliseconds
} AssignmentResult;

// Global variables
int cost_matrix[MAX_SIZE][MAX_SIZE];
int n;
int min_cost;
int best_assignment[MAX_SIZE];
int nodes_explored, nodes_pruned;
int verbose;

// Function prototypes
void solve_assignment();
int calculate_bound(AssignmentNode* node);
void pq_init(PriorityQueue* pq);
void pq_push(PriorityQueue* pq, AssignmentNode node);
AssignmentNode pq_pop(PriorityQueue* pq);
int pq_empty(PriorityQueue* pq);
AssignmentResult brute_force_assignment();
void print_matrix();
void print_assignment(int* assignment);
int next_permutation(int* arr, int n);
void demonstrate_scaling();

/**
 * Initialize priority queue
 */
void pq_init(PriorityQueue* pq) {
    pq->size = 0;
}

/**
 * Push node to priority queue (min-heap based on bound)
 */
void pq_push(PriorityQueue* pq, AssignmentNode node) {
    int i = pq->size++;
    pq->nodes[i] = node;
    
    // Bubble up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->nodes[i].bound >= pq->nodes[parent].bound) break;
        
        AssignmentNode temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[parent];
        pq->nodes[parent] = temp;
        i = parent;
    }
}

/**
 * Pop minimum bound node from priority queue
 */
AssignmentNode pq_pop(PriorityQueue* pq) {
    AssignmentNode min = pq->nodes[0];
    pq->nodes[0] = pq->nodes[--pq->size];
    
    // Bubble down
    int i = 0;
    while (2 * i + 1 < pq->size) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = left;
        
        if (right < pq->size && pq->nodes[right].bound < pq->nodes[left].bound) {
            smallest = right;
        }
        
        if (pq->nodes[i].bound <= pq->nodes[smallest].bound) break;
        
        AssignmentNode temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[smallest];
        pq->nodes[smallest] = temp;
        i = smallest;
    }
    
    return min;
}

/**
 * Check if priority queue is empty
 */
int pq_empty(PriorityQueue* pq) {
    return pq->size == 0;
}

/**
 * Calculate lower bound using row and column reduction
 */
int calculate_bound(AssignmentNode* node) {
    // Start with current cost
    int bound = node->cost;
    
    // For each unassigned worker, add minimum cost to any unassigned job
    for (int worker = node->level; worker < n; worker++) {
        int min_cost = INF;
        for (int job = 0; job < n; job++) {
            if (!node->worker_used[job]) {
                if (cost_matrix[worker][job] < min_cost) {
                    min_cost = cost_matrix[worker][job];
                }
            }
        }
        if (min_cost != INF) {
            bound += min_cost;
        }
    }
    
    return bound;
}

/**
 * Solve assignment problem using Branch & Bound
 */
void solve_assignment() {
    clock_t start = clock();
    
    if (verbose) {
        printf("=== Starting Job Assignment Branch & Bound Solution ===\n");
        printf("Workers: %d, Jobs: %d\n", n, n);
        printf("Cost Matrix:\n");
        print_matrix();
    }
    
    // Initialize
    min_cost = INF;
    for (int i = 0; i < n; i++) {
        best_assignment[i] = -1;
    }
    nodes_explored = 0;
    nodes_pruned = 0;
    
    // Initialize root node
    AssignmentNode root;
    root.level = 0;
    root.cost = 0;
    for (int i = 0; i < n; i++) {
        root.assignment[i] = -1;
        root.worker_used[i] = 0;
    }
    root.bound = calculate_bound(&root);
    
    if (verbose) {
        printf("Root node bound: %d\n", root.bound);
    }
    
    // Initialize priority queue
    PriorityQueue pq;
    pq_init(&pq);
    pq_push(&pq, root);
    
    while (!pq_empty(&pq)) {
        AssignmentNode current = pq_pop(&pq);
        nodes_explored++;
        
        if (verbose && nodes_explored <= 20) {
            printf("Exploring node at level %d, cost: %d, bound: %d\n",
                   current.level, current.cost, current.bound);
            printf("  Current assignment: [");
            for (int i = 0; i < n; i++) {
                if (i > 0) printf(", ");
                printf("%d", current.assignment[i]);
            }
            printf("]\n");
        }
        
        // Pruning check
        if (current.bound >= min_cost) {
            nodes_pruned++;
            if (verbose && nodes_explored <= 20) {
                printf("  Pruned: bound %d >= best %d\n", current.bound, min_cost);
            }
            continue;
        }
        
        // Check if we have a complete assignment
        if (current.level == n) {
            if (current.cost < min_cost) {
                min_cost = current.cost;
                for (int i = 0; i < n; i++) {
                    best_assignment[i] = current.assignment[i];
                }
                
                if (verbose) {
                    printf("*** New best solution found! ***\n");
                    printf("Assignment: ");
                    print_assignment(best_assignment);
                    printf("Cost: %d\n", min_cost);
                }
            }
            continue;
        }
        
        // Branch: Try assigning current worker to each available job
        for (int job = 0; job < n; job++) {
            if (!current.worker_used[job]) {
                AssignmentNode child = current;
                child.level = current.level + 1;
                child.assignment[current.level] = job;
                child.worker_used[job] = 1;
                child.cost = current.cost + cost_matrix[current.level][job];
                child.bound = calculate_bound(&child);
                
                // Only add to queue if bound is promising
                if (child.bound < min_cost) {
                    pq_push(&pq, child);
                } else {
                    nodes_pruned++;
                }
            }
        }
    }
    
    clock_t end = clock();
    double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    if (verbose) {
        printf("=== Final Results ===\n");
        printf("Optimal assignment: ");
        print_assignment(best_assignment);
        printf("Minimum cost: %d\n", min_cost);
        printf("Assignment details:\n");
        for (int i = 0; i < n; i++) {
            printf("  Worker %d -> Job %d (cost: %d)\n", 
                   i, best_assignment[i], cost_matrix[i][best_assignment[i]]);
        }
        printf("Nodes explored: %d\n", nodes_explored);
        printf("Nodes pruned: %d\n", nodes_pruned);
        printf("Execution time: %.2f ms\n", execution_time);
    }
}

/**
 * Brute force assignment solution for comparison
 */
AssignmentResult brute_force_assignment() {
    clock_t start = clock();
    
    int jobs[MAX_SIZE];
    for (int i = 0; i < n; i++) {
        jobs[i] = i;
    }
    
    int min_cost = INF;
    int best_assignment[MAX_SIZE];
    int permutations = 0;
    
    do {
        permutations++;
        int cost = 0;
        for (int worker = 0; worker < n; worker++) {
            cost += cost_matrix[worker][jobs[worker]];
        }
        
        if (cost < min_cost) {
            min_cost = cost;
            for (int i = 0; i < n; i++) {
                best_assignment[i] = jobs[i];
            }
        }
    } while (next_permutation(jobs, n));
    
    clock_t end = clock();
    double execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    AssignmentResult result;
    for (int i = 0; i < n; i++) {
        result.assignment[i] = best_assignment[i];
    }
    result.min_cost = min_cost;
    result.nodes_explored = permutations;
    result.nodes_pruned = 0;
    result.execution_time = execution_time;
    
    return result;
}

/**
 * Generate next permutation (lexicographic order)
 */
int next_permutation(int* arr, int n) {
    int i = n - 2;
    
    while (i >= 0 && arr[i] >= arr[i + 1]) {
        i--;
    }
    
    if (i < 0) return 0;
    
    int j = n - 1;
    while (arr[j] <= arr[i]) {
        j--;
    }
    
    // Swap
    int temp = arr[i];
    arr[i] = arr[j];
    arr[j] = temp;
    
    // Reverse suffix
    int left = i + 1, right = n - 1;
    while (left < right) {
        temp = arr[left];
        arr[left] = arr[right];
        arr[right] = temp;
        left++;
        right--;
    }
    
    return 1;
}

/**
 * Print cost matrix
 */
void print_matrix() {
    printf("     ");
    for (int j = 0; j < n; j++) {
        printf("J%-3d", j);
    }
    printf("\n");
    
    for (int i = 0; i < n; i++) {
        printf("W%-2d: ", i);
        for (int j = 0; j < n; j++) {
            printf("%4d", cost_matrix[i][j]);
        }
        printf("\n");
    }
}

/**
 * Print assignment array
 */
void print_assignment(int* assignment) {
    printf("[");
    for (int i = 0; i < n; i++) {
        if (i > 0) printf(", ");
        printf("%d", assignment[i]);
    }
    printf("]\n");
}

/**
 * Demonstrate scaling behavior
 */
void demonstrate_scaling() {
    printf("\n=== Scaling Analysis ===\n");
    
    srand(42);
    int sizes[] = {3, 4, 5, 6};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("Size | Nodes Explored | Nodes Pruned | Pruning %% | Time (ms)\n");
    printf("-------------------------------------------------------------\n");
    
    for (int s = 0; s < num_sizes; s++) {
        n = sizes[s];
        
        // Generate random cost matrix
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cost_matrix[i][j] = 1 + rand() % 30;
            }
        }
        
        verbose = 0;
        solve_assignment();
        
        double pruning_percent = (double)nodes_pruned / (nodes_explored + nodes_pruned) * 100.0;
        
        printf("%4d | %14d | %12d | %8.1f%% | %8.2f\n",
               n, nodes_explored, nodes_pruned, 
               pruning_percent, 0.0); // Time calculation simplified for this demo
    }
    
    printf("\nKey Observations:\n");
    printf("- Search space grows as n! but pruning is very effective\n");
    printf("- Good bounds eliminate most of the search space\n");
    printf("- Performance degrades rapidly beyond n=8\n");
    printf("- Hungarian algorithm is preferred for larger problems\n");
}

int main() {
    printf("=== Job Assignment Problem - Branch & Bound ===\n\n");
    
    // Test case 1: Small assignment problem
    n = 4;
    int cost1[4][4] = {
        {9, 2, 7, 8},
        {6, 4, 3, 7},
        {5, 8, 1, 8},
        {7, 6, 9, 4}
    };
    
    // Copy to global matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cost_matrix[i][j] = cost1[i][j];
        }
    }
    
    printf("Test Case 1: 4x4 Assignment Problem\n");
    printf("Cost Matrix:\n");
    print_matrix();
    
    verbose = 1;
    solve_assignment();
    
    // Compare with brute force
    printf("\nComparing with Brute Force:\n");
    AssignmentResult brute_result = brute_force_assignment();
    printf("=== Brute Force Results ===\n");
    printf("Permutations checked: %d\n", brute_result.nodes_explored);
    printf("Optimal assignment: ");
    print_assignment(brute_result.assignment);
    printf("Minimum cost: %d\n", brute_result.min_cost);
    printf("Execution time: %.2f ms\n", brute_result.execution_time);
    
    printf("\nEfficiency Comparison:\n");
    printf("Branch & Bound: %d nodes explored, %d pruned\n", nodes_explored, nodes_pruned);
    printf("Brute Force: %d permutations checked\n", brute_result.nodes_explored);
    
    // Test case 2: Larger assignment problem
    printf("\n============================================================\n");
    printf("Test Case 2: 5x5 Assignment Problem\n");
    
    n = 5;
    int cost2[5][5] = {
        {12, 9, 27, 10, 23},
        {7, 13, 13, 30, 19},
        {25, 18, 26, 15, 24},
        {6, 20, 14, 8, 17},
        {18, 24, 20, 21, 14}
    };
    
    // Copy to global matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            cost_matrix[i][j] = cost2[i][j];
        }
    }
    
    printf("Cost Matrix:\n");
    print_matrix();
    
    verbose = 0;
    solve_assignment();
    
    printf("\nBranch & Bound Results:\n");
    printf("Optimal assignment: ");
    print_assignment(best_assignment);
    printf("Minimum cost: %d\n", min_cost);
    printf("Assignment details:\n");
    for (int i = 0; i < n; i++) {
        printf("  Worker %d -> Job %d (cost: %d)\n", 
               i, best_assignment[i], cost_matrix[i][best_assignment[i]]);
    }
    printf("Nodes explored: %d\n", nodes_explored);
    printf("Nodes pruned: %d\n", nodes_pruned);
    
    // Performance analysis
    printf("\n=== Performance Analysis ===\n");
    printf("Branch & Bound Advantages:\n");
    printf("- Optimal solution guaranteed\n");
    printf("- Efficient pruning reduces search space significantly\n");
    printf("- Good bounds lead to early termination\n");
    printf("- Works well for medium-sized problems\n");
    
    printf("\nHungarian Algorithm Advantages:\n");
    printf("- Polynomial time complexity O(n³)\n");
    printf("- Consistent performance regardless of input\n");
    printf("- Best choice for large assignment problems\n");
    
    printf("\nWhen to use Branch & Bound for Assignment:\n");
    printf("- Medium-sized problems (n ≤ 15)\n");
    printf("- When you want to understand the search process\n");
    printf("- Educational purposes\n");
    printf("- When Hungarian implementation is not available\n");
    
    demonstrate_scaling();
    
    return 0;
}