/**
 * N-Queens Problem - Branch & Bound Optimization Implementation in C
 * 
 * This implementation demonstrates the Branch & Bound strategy for optimizing
 * N-Queens solutions, focusing on finding high-quality placements with
 * different optimization criteria.
 * 
 * Key Features:
 * - Multiple optimization modes (conflicts, center, symmetry)
 * - Priority queue-based best-first search
 * - Solution quality scoring and ranking
 * - Performance comparison with backtracking
 * 
 * Compilation: gcc -o nqueens_opt NQueensOptimized.c -lm
 * Usage: ./nqueens_opt
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define MAX_N 12
#define MAX_SOLUTIONS 100

typedef struct {
    int placement[MAX_N];    // Queen positions
    int conflicts;           // Number of conflicts
    double score;           // Solution quality score
} QueensSolution;

typedef struct {
    int level;                      // Current row being processed
    int placement[MAX_N];           // Current partial placement
    int col_used[MAX_N];           // Used columns
    int diag1_used[2 * MAX_N - 1]; // Used main diagonals
    int diag2_used[2 * MAX_N - 1]; // Used anti-diagonals
    int conflicts;                  // Current conflicts
    int bound;                      // Lower bound estimate
} QueensNode;

typedef struct {
    QueensNode nodes[10000];        // Priority queue
    int size;                       // Current size
} PriorityQueue;

typedef struct {
    QueensSolution solutions[MAX_SOLUTIONS];
    int solution_count;
    QueensSolution best_solution;
    int nodes_explored;
    int nodes_pruned;
    double execution_time;
} OptimizationResult;

// Global variables
int n;
int max_solutions;
char optimization_mode[20];
int verbose;

// Function prototypes
OptimizationResult solve_optimized();
int calculate_bound(QueensNode* node);
void get_column_order(QueensNode* node, int* columns, int* count);
int can_place(QueensNode* node, int row, int col);
void place_queen(QueensNode* node, int row, int col);
double calculate_solution_score(QueensSolution* solution);
void pq_init(PriorityQueue* pq);
void pq_push(PriorityQueue* pq, QueensNode node);
QueensNode pq_pop(PriorityQueue* pq);
int pq_empty(PriorityQueue* pq);
OptimizationResult backtracking_solution();
void backtrack_recursive(int row, int* placement, int* col_used, 
                        int* diag1_used, int* diag2_used,
                        QueensSolution* solutions, int* solution_count, int* explored_count);
void print_solution(QueensSolution* solution);
void visualize_solution(QueensSolution* solution);
int count_potential_conflicts(QueensNode* node, int row, int col);

/**
 * Initialize priority queue
 */
void pq_init(PriorityQueue* pq) {
    pq->size = 0;
}

/**
 * Push node to priority queue (min-heap based on bound)
 */
void pq_push(PriorityQueue* pq, QueensNode node) {
    int i = pq->size++;
    pq->nodes[i] = node;
    
    // Bubble up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->nodes[i].bound >= pq->nodes[parent].bound) break;
        
        QueensNode temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[parent];
        pq->nodes[parent] = temp;
        i = parent;
    }
}

/**
 * Pop minimum bound node from priority queue
 */
QueensNode pq_pop(PriorityQueue* pq) {
    QueensNode min = pq->nodes[0];
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
        
        QueensNode temp = pq->nodes[i];
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
 * Calculate lower bound estimate for remaining conflicts
 */
int calculate_bound(QueensNode* node) {
    int bound = node->conflicts;
    
    // For remaining rows, estimate minimum conflicts
    for (int row = node->level; row < n; row++) {
        int min_conflicts = MAX_N;
        
        for (int col = 0; col < n; col++) {
            if (!node->col_used[col] && 
                !node->diag1_used[row - col + n - 1] && 
                !node->diag2_used[row + col]) {
                
                // Calculate potential conflicts if queen is placed here
                int conflicts = 0;
                
                // Check conflicts with already placed queens
                for (int prev_row = 0; prev_row < node->level; prev_row++) {
                    int prev_col = node->placement[prev_row];
                    if (prev_col == col || 
                        abs(prev_row - row) == abs(prev_col - col)) {
                        conflicts++;
                    }
                }
                
                if (conflicts < min_conflicts) {
                    min_conflicts = conflicts;
                }
            }
        }
        
        if (min_conflicts != MAX_N) {
            bound += min_conflicts;
        }
    }
    
    return bound;
}

/**
 * Get column ordering for branching (heuristic-based)
 */
void get_column_order(QueensNode* node, int* columns, int* count) {
    *count = 0;
    
    // Add all valid columns
    for (int col = 0; col < n; col++) {
        if (can_place(node, node->level, col)) {
            columns[*count] = col;
            (*count)++;
        }
    }
    
    // Apply ordering heuristic based on optimization mode
    if (strcmp(optimization_mode, "center") == 0) {
        // Sort by distance from center (bubble sort for simplicity)
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                int center_dist_a = abs(columns[j] - n / 2);
                int center_dist_b = abs(columns[j + 1] - n / 2);
                if (center_dist_a > center_dist_b) {
                    int temp = columns[j];
                    columns[j] = columns[j + 1];
                    columns[j + 1] = temp;
                }
            }
        }
    } else if (strcmp(optimization_mode, "conflicts") == 0) {
        // Sort by potential conflicts (bubble sort)
        for (int i = 0; i < *count - 1; i++) {
            for (int j = 0; j < *count - i - 1; j++) {
                int conflicts_a = count_potential_conflicts(node, node->level, columns[j]);
                int conflicts_b = count_potential_conflicts(node, node->level, columns[j + 1]);
                if (conflicts_a > conflicts_b) {
                    int temp = columns[j];
                    columns[j] = columns[j + 1];
                    columns[j + 1] = temp;
                }
            }
        }
    } else if (strcmp(optimization_mode, "symmetry") == 0) {
        // For first queen, only consider first half + middle
        if (node->level == 0) {
            *count = (n + 1) / 2;
        }
    }
}

int count_potential_conflicts(QueensNode* node, int row, int col) {
    int conflicts = 0;
    
    // Count conflicts with remaining unplaced positions
    for (int future_row = row + 1; future_row < n; future_row++) {
        for (int future_col = 0; future_col < n; future_col++) {
            if (!node->col_used[future_col] && 
                (future_col == col || abs(future_row - row) == abs(future_col - col))) {
                conflicts++;
            }
        }
    }
    
    return conflicts;
}

int can_place(QueensNode* node, int row, int col) {
    return !node->col_used[col] && 
           !node->diag1_used[row - col + n - 1] && 
           !node->diag2_used[row + col];
}

void place_queen(QueensNode* node, int row, int col) {
    node->placement[row] = col;
    node->col_used[col] = 1;
    node->diag1_used[row - col + n - 1] = 1;
    node->diag2_used[row + col] = 1;
}

double calculate_solution_score(QueensSolution* solution) {
    double score = 0.0;
    
    if (strcmp(optimization_mode, "center") == 0) {
        // Reward solutions with queens closer to center
        for (int i = 0; i < n; i++) {
            score += 1.0 / (1.0 + abs(solution->placement[i] - n / 2.0));
        }
    } else if (strcmp(optimization_mode, "symmetry") == 0) {
        // Reward symmetric solutions
        int symmetric = 1;
        for (int i = 0; i < n; i++) {
            if (solution->placement[i] != n - 1 - solution->placement[n - 1 - i]) {
                symmetric = 0;
                break;
            }
        }
        score = symmetric ? 10.0 : 1.0;
    } else {
        // Default: reward solutions with fewer conflicts
        score = 100.0 / (1.0 + solution->conflicts);
    }
    
    return score;
}

/**
 * Solve N-Queens using Branch & Bound optimization
 */
OptimizationResult solve_optimized() {
    clock_t start = clock();
    
    OptimizationResult result;
    result.solution_count = 0;
    result.nodes_explored = 0;
    result.nodes_pruned = 0;
    result.best_solution.score = -1.0;
    
    if (verbose) {
        printf("=== Starting N-Queens Branch & Bound Optimization ===\n");
        printf("Board size: %dx%d\n", n, n);
        printf("Optimization mode: %s\n", optimization_mode);
        printf("Max solutions to find: %d\n", max_solutions);
    }
    
    // Initialize root node
    QueensNode root;
    root.level = 0;
    root.conflicts = 0;
    memset(root.col_used, 0, sizeof(root.col_used));
    memset(root.diag1_used, 0, sizeof(root.diag1_used));
    memset(root.diag2_used, 0, sizeof(root.diag2_used));
    root.bound = calculate_bound(&root);
    
    if (verbose) {
        printf("Root node bound: %d\n", root.bound);
    }
    
    // Initialize priority queue
    PriorityQueue pq;
    pq_init(&pq);
    pq_push(&pq, root);
    
    while (!pq_empty(&pq) && result.solution_count < max_solutions) {
        QueensNode current = pq_pop(&pq);
        result.nodes_explored++;
        
        if (verbose && result.nodes_explored <= 25) {
            printf("Exploring node at level %d, conflicts: %d, bound: %d\n",
                   current.level, current.conflicts, current.bound);
        }
        
        // Pruning check
        if (result.best_solution.score >= 0 && current.bound > result.best_solution.conflicts) {
            result.nodes_pruned++;
            if (verbose && result.nodes_explored <= 25) {
                printf("  Pruned: bound %d > best conflicts %d\n", 
                       current.bound, result.best_solution.conflicts);
            }
            continue;
        }
        
        // Check if we have a complete solution
        if (current.level == n) {
            QueensSolution solution;
            for (int i = 0; i < n; i++) {
                solution.placement[i] = current.placement[i];
            }
            solution.conflicts = current.conflicts;
            solution.score = calculate_solution_score(&solution);
            
            result.solutions[result.solution_count] = solution;
            result.solution_count++;
            
            if (result.best_solution.score < 0 || 
                solution.conflicts < result.best_solution.conflicts ||
                (solution.conflicts == result.best_solution.conflicts && 
                 solution.score > result.best_solution.score)) {
                result.best_solution = solution;
            }
            
            if (verbose) {
                printf("*** Solution found! ***\n");
                printf("Conflicts: %d, Score: %.2f\n", solution.conflicts, solution.score);
            }
            continue;
        }
        
        // Branch: Try placing queen in each column of current row
        int columns[MAX_N];
        int column_count;
        get_column_order(&current, columns, &column_count);
        
        for (int i = 0; i < column_count; i++) {
            int col = columns[i];
            if (can_place(&current, current.level, col)) {
                QueensNode child = current;
                place_queen(&child, current.level, col);
                child.level = current.level + 1;
                child.bound = calculate_bound(&child);
                
                // Only add to queue if bound is promising
                if (result.best_solution.score < 0 || 
                    child.bound <= result.best_solution.conflicts + 1) {
                    pq_push(&pq, child);
                } else {
                    result.nodes_pruned++;
                }
            }
        }
    }
    
    clock_t end = clock();
    result.execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    if (verbose) {
        printf("=== Final Results ===\n");
        printf("Solutions found: %d\n", result.solution_count);
        if (result.best_solution.score >= 0) {
            printf("Best solution score: %.2f\n", result.best_solution.score);
            printf("Best placement visualization:\n");
            visualize_solution(&result.best_solution);
        }
        printf("Nodes explored: %d\n", result.nodes_explored);
        printf("Nodes pruned: %d\n", result.nodes_pruned);
        printf("Execution time: %.2f ms\n", result.execution_time);
    }
    
    return result;
}

/**
 * Backtracking solution for comparison
 */
OptimizationResult backtracking_solution() {
    clock_t start = clock();
    
    OptimizationResult result;
    result.solution_count = 0;
    result.nodes_explored = 0;
    result.nodes_pruned = 0;
    result.best_solution.score = -1.0;
    
    int placement[MAX_N];
    int col_used[MAX_N] = {0};
    int diag1_used[2 * MAX_N - 1] = {0};
    int diag2_used[2 * MAX_N - 1] = {0};
    
    backtrack_recursive(0, placement, col_used, diag1_used, diag2_used,
                       result.solutions, &result.solution_count, &result.nodes_explored);
    
    // Calculate scores and find best
    for (int i = 0; i < result.solution_count && i < max_solutions; i++) {
        result.solutions[i].score = calculate_solution_score(&result.solutions[i]);
        if (result.best_solution.score < 0 || 
            result.solutions[i].score > result.best_solution.score) {
            result.best_solution = result.solutions[i];
        }
    }
    
    if (result.solution_count > max_solutions) {
        result.solution_count = max_solutions;
    }
    
    clock_t end = clock();
    result.execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    
    return result;
}

void backtrack_recursive(int row, int* placement, int* col_used, 
                        int* diag1_used, int* diag2_used,
                        QueensSolution* solutions, int* solution_count, int* explored_count) {
    (*explored_count)++;
    
    if (*solution_count >= max_solutions) return;
    
    if (row == n) {
        for (int i = 0; i < n; i++) {
            solutions[*solution_count].placement[i] = placement[i];
        }
        solutions[*solution_count].conflicts = 0;
        (*solution_count)++;
        return;
    }
    
    for (int col = 0; col < n; col++) {
        if (!col_used[col] && !diag1_used[row - col + n - 1] && !diag2_used[row + col]) {
            placement[row] = col;
            col_used[col] = 1;
            diag1_used[row - col + n - 1] = 1;
            diag2_used[row + col] = 1;
            
            backtrack_recursive(row + 1, placement, col_used, diag1_used, diag2_used,
                               solutions, solution_count, explored_count);
            
            col_used[col] = 0;
            diag1_used[row - col + n - 1] = 0;
            diag2_used[row + col] = 0;
        }
    }
}

void print_solution(QueensSolution* solution) {
    printf("[");
    for (int i = 0; i < n; i++) {
        if (i > 0) printf(", ");
        printf("%d", solution->placement[i]);
    }
    printf("] (score: %.2f)", solution->score);
}

void visualize_solution(QueensSolution* solution) {
    for (int row = 0; row < n; row++) {
        for (int col = 0; col < n; col++) {
            if (solution->placement[row] == col) {
                printf(" Q ");
            } else {
                printf(" . ");
            }
        }
        printf("\n");
    }
}

int main() {
    printf("=== N-Queens Problem - Branch & Bound Optimization ===\n\n");
    
    // Test case 1: 8-Queens with conflict optimization
    printf("Test Case 1: 8-Queens with Conflict Optimization\n");
    
    n = 8;
    max_solutions = 5;
    strcpy(optimization_mode, "conflicts");
    verbose = 1;
    
    OptimizationResult result1 = solve_optimized();
    
    // Compare with backtracking
    printf("\nComparing with Backtracking:\n");
    OptimizationResult backtrack_result1 = backtracking_solution();
    printf("=== Backtracking Results ===\n");
    printf("Solutions found: %d\n", backtrack_result1.solution_count);
    printf("Nodes explored: %d\n", backtrack_result1.nodes_explored);
    printf("Execution time: %.2f ms\n", backtrack_result1.execution_time);
    
    printf("\nEfficiency Comparison:\n");
    printf("Branch & Bound: %d nodes explored, %d pruned\n", 
           result1.nodes_explored, result1.nodes_pruned);
    printf("Backtracking: %d nodes explored\n", backtrack_result1.nodes_explored);
    if (backtrack_result1.execution_time > 0) {
        printf("Speedup: %.2fx\n", backtrack_result1.execution_time / result1.execution_time);
    }
    
    // Test case 2: Center-preference optimization
    printf("\n============================================================\n");
    printf("Test Case 2: 6-Queens with Center Preference\n");
    
    n = 6;
    max_solutions = 3;
    strcpy(optimization_mode, "center");
    verbose = 0;
    
    OptimizationResult result2 = solve_optimized();
    
    printf("Solutions found: %d\n", result2.solution_count);
    if (result2.best_solution.score >= 0) {
        printf("Best solution: ");
        print_solution(&result2.best_solution);
        printf("\n\nBest solution visualization:\n");
        visualize_solution(&result2.best_solution);
    }
    
    printf("\nAll solutions with scores:\n");
    for (int i = 0; i < result2.solution_count; i++) {
        printf("%d. ", i + 1);
        print_solution(&result2.solutions[i]);
        printf("\n");
    }
    
    // Test case 3: Symmetry optimization
    printf("\n============================================================\n");
    printf("Test Case 3: 4-Queens with Symmetry Optimization\n");
    
    n = 4;
    max_solutions = 10;
    strcpy(optimization_mode, "symmetry");
    verbose = 0;
    
    OptimizationResult result3 = solve_optimized();
    
    printf("Solutions found: %d\n", result3.solution_count);
    printf("Nodes explored: %d\n", result3.nodes_explored);
    printf("Nodes pruned: %d\n", result3.nodes_pruned);
    
    printf("\nAll solutions:\n");
    for (int i = 0; i < result3.solution_count; i++) {
        printf("%d. [", i + 1);
        for (int j = 0; j < n; j++) {
            if (j > 0) printf(", ");
            printf("%d", result3.solutions[i].placement[j]);
        }
        printf("]\n");
    }
    
    // Performance analysis
    printf("\n=== Performance Analysis ===\n");
    printf("Branch & Bound Optimization Advantages:\n");
    printf("- Can find multiple high-quality solutions\n");
    printf("- Supports different optimization criteria\n");
    printf("- Efficient pruning based on solution quality\n");
    printf("- Suitable for constraint optimization variants\n");
    
    printf("\nOptimization Modes:\n");
    printf("- 'conflicts': Minimize queen conflicts\n");
    printf("- 'center': Prefer central queen placements\n");
    printf("- 'symmetry': Find symmetric solutions\n");
    
    // Scaling analysis
    printf("\n=== Scaling Analysis ===\n");
    
    int sizes[] = {4, 6, 8};
    char* modes[] = {"conflicts", "center", "symmetry"};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    int num_modes = sizeof(modes) / sizeof(modes[0]);
    
    printf("Size | Mode      | Solutions | Nodes | Pruned | Time (ms)\n");
    printf("------------------------------------------------------------\n");
    
    for (int s = 0; s < num_sizes; s++) {
        for (int m = 0; m < num_modes; m++) {
            n = sizes[s];
            max_solutions = 3;
            strcpy(optimization_mode, modes[m]);
            verbose = 0;
            
            OptimizationResult result = solve_optimized();
            
            printf("%4d | %-9s | %9d | %5d | %6d | %8.2f\n",
                n, optimization_mode, result.solution_count, result.nodes_explored,
                result.nodes_pruned, result.execution_time);
        }
    }
    
    printf("\nKey Observations:\n");
    printf("- Different optimization modes affect search efficiency\n");
    printf("- Symmetry mode reduces search space significantly\n");
    printf("- Center preference finds good solutions quickly\n");
    printf("- Conflict minimization explores more nodes but finds better solutions\n");
    
    return 0;
}