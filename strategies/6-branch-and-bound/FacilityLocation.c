/**
 * Facility Location Problem - Branch & Bound Implementation in C
 * 
 * This implementation demonstrates the Branch & Bound strategy for solving
 * the uncapacitated facility location problem in C, showcasing systematic
 * optimization with effective cost-based pruning.
 * 
 * Key Features:
 * - Priority queue-based best-first search
 * - Lagrangian relaxation bounds
 * - Greedy heuristic for initial bounds
 * - Comprehensive solution tracking
 * 
 * Compilation: gcc -o facility FacilityLocation.c -lm
 * Usage: ./facility
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <float.h>

#define MAX_FACILITIES 10
#define MAX_CUSTOMERS 15
#define MAX_NAME_LEN 20

typedef struct {
    int id;
    double x, y;
    char name[MAX_NAME_LEN];
} Location;

typedef struct {
    int level;                          // Current facility being considered
    int facility_open[MAX_FACILITIES];  // Which facilities are open
    double fixed_cost;                  // Total facility opening costs
    double service_cost;                // Total service costs
    double total_cost;                  // Total cost so far
    double bound;                       // Lower bound estimate
} FacilityNode;

typedef struct {
    int facility_open[MAX_FACILITIES];
    int customer_assignment[MAX_CUSTOMERS]; // Which facility serves each customer
    double fixed_cost;
    double service_cost;
    double total_cost;
} FacilitySolution;

typedef struct {
    FacilityNode nodes[5000];           // Priority queue
    int size;                           // Current size
} PriorityQueue;

typedef struct {
    FacilitySolution best_solution;
    int nodes_explored;
    int nodes_pruned;
    double execution_time;
} LocationResult;

// Global variables
Location facilities[MAX_FACILITIES];
Location customers[MAX_CUSTOMERS];
double facility_costs[MAX_FACILITIES];
double service_costs[MAX_FACILITIES][MAX_CUSTOMERS];
int num_facilities, num_customers;
FacilitySolution best_solution;
double best_cost;
int nodes_explored, nodes_pruned;
int verbose;

// Function prototypes
LocationResult solve_facility_location();
double calculate_bound(FacilityNode* node);
double calculate_service_cost(FacilityNode* node);
FacilitySolution construct_solution(FacilityNode* node);
FacilitySolution greedy_heuristic();
double calculate_improvement(int facility, int* facility_open, int* customer_served);
double get_current_service_cost(int customer, int* facility_open);
void pq_init(PriorityQueue* pq);
void pq_push(PriorityQueue* pq, FacilityNode node);
FacilityNode pq_pop(PriorityQueue* pq);
int pq_empty(PriorityQueue* pq);
double distance(Location* a, Location* b);
void print_solution(FacilitySolution* solution);
void print_open_facilities(int* facility_open);

/**
 * Calculate distance between two locations
 */
double distance(Location* a, Location* b) {
    return sqrt((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
}

/**
 * Initialize priority queue
 */
void pq_init(PriorityQueue* pq) {
    pq->size = 0;
}

/**
 * Push node to priority queue (min-heap based on bound)
 */
void pq_push(PriorityQueue* pq, FacilityNode node) {
    int i = pq->size++;
    pq->nodes[i] = node;
    
    // Bubble up
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (pq->nodes[i].bound >= pq->nodes[parent].bound) break;
        
        FacilityNode temp = pq->nodes[i];
        pq->nodes[i] = pq->nodes[parent];
        pq->nodes[parent] = temp;
        i = parent;
    }
}

/**
 * Pop minimum bound node from priority queue
 */
FacilityNode pq_pop(PriorityQueue* pq) {
    FacilityNode min = pq->nodes[0];
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
        
        FacilityNode temp = pq->nodes[i];
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
 * Calculate lower bound using Lagrangian relaxation approach
 */
double calculate_bound(FacilityNode* node) {
    double bound = node->fixed_cost;
    
    // For each customer, add minimum service cost from available facilities
    for (int j = 0; j < num_customers; j++) {
        double min_service_cost = DBL_MAX;
        int has_open_facility = 0;
        
        // Check already opened facilities
        for (int i = 0; i < node->level; i++) {
            if (node->facility_open[i]) {
                has_open_facility = 1;
                if (service_costs[i][j] < min_service_cost) {
                    min_service_cost = service_costs[i][j];
                }
            }
        }
        
        // If no facility is open yet, consider minimum from remaining facilities
        if (!has_open_facility || node->level < num_facilities) {
            for (int i = node->level; i < num_facilities; i++) {
                // Consider cost of opening facility i plus service cost
                double cost_with_facility = service_costs[i][j];
                if (!has_open_facility) {
                    // If no facility is open, we must open at least one
                    cost_with_facility += facility_costs[i] / num_customers; // Amortized cost
                }
                if (cost_with_facility < min_service_cost) {
                    min_service_cost = cost_with_facility;
                }
            }
        }
        
        if (min_service_cost != DBL_MAX) {
            bound += min_service_cost;
        }
    }
    
    return bound;
}

/**
 * Calculate service cost for current facility configuration
 */
double calculate_service_cost(FacilityNode* node) {
    double service_cost = 0.0;
    
    for (int j = 0; j < num_customers; j++) {
        double min_cost = DBL_MAX;
        
        // Find cheapest open facility to serve customer j
        for (int i = 0; i < num_facilities; i++) {
            if (node->facility_open[i]) {
                if (service_costs[i][j] < min_cost) {
                    min_cost = service_costs[i][j];
                }
            }
        }
        
        if (min_cost != DBL_MAX) {
            service_cost += min_cost;
        } else {
            // No facility is open - return high cost to indicate infeasibility
            return DBL_MAX;
        }
    }
    
    return service_cost;
}

/**
 * Construct complete solution from facility node
 */
FacilitySolution construct_solution(FacilityNode* node) {
    FacilitySolution solution;
    solution.fixed_cost = 0.0;
    solution.service_cost = 0.0;
    solution.total_cost = 0.0;
    
    // Check if at least one facility is open
    int has_facility = 0;
    for (int i = 0; i < num_facilities; i++) {
        solution.facility_open[i] = node->facility_open[i];
        if (node->facility_open[i]) {
            has_facility = 1;
            solution.fixed_cost += facility_costs[i];
        }
    }
    
    if (!has_facility) {
        solution.total_cost = DBL_MAX;
        return solution; // Infeasible solution
    }
    
    // Assign each customer to cheapest open facility
    for (int j = 0; j < num_customers; j++) {
        double min_cost = DBL_MAX;
        int best_facility = -1;
        
        for (int i = 0; i < num_facilities; i++) {
            if (solution.facility_open[i] && service_costs[i][j] < min_cost) {
                min_cost = service_costs[i][j];
                best_facility = i;
            }
        }
        
        solution.customer_assignment[j] = best_facility;
        solution.service_cost += min_cost;
    }
    
    solution.total_cost = solution.fixed_cost + solution.service_cost;
    return solution;
}

/**
 * Greedy heuristic for initial upper bound
 */
FacilitySolution greedy_heuristic() {
    int facility_open[MAX_FACILITIES] = {0};
    int customer_served[MAX_CUSTOMERS] = {0};
    double total_cost = 0.0;
    
    // Greedy: repeatedly open facility that gives best cost improvement
    while (1) {
        double best_improvement = 0.0;
        int best_facility = -1;
        
        for (int i = 0; i < num_facilities; i++) {
            if (!facility_open[i]) {
                double improvement = calculate_improvement(i, facility_open, customer_served);
                if (improvement > best_improvement) {
                    best_improvement = improvement;
                    best_facility = i;
                }
            }
        }
        
        if (best_facility == -1 || best_improvement <= 0.0) break;
        
        facility_open[best_facility] = 1;
        total_cost += facility_costs[best_facility];
        
        // Update customer service status
        for (int j = 0; j < num_customers; j++) {
            if (!customer_served[j]) {
                customer_served[j] = 1;
                total_cost += service_costs[best_facility][j];
            } else {
                // Check if new facility is cheaper
                double current_cost = get_current_service_cost(j, facility_open);
                if (service_costs[best_facility][j] < current_cost) {
                    total_cost = total_cost - current_cost + service_costs[best_facility][j];
                }
            }
        }
    }
    
    // Ensure all customers are served
    for (int j = 0; j < num_customers; j++) {
        if (!customer_served[j]) {
            // Open cheapest facility for unserved customers
            double min_cost = DBL_MAX;
            int cheapest_facility = -1;
            
            for (int i = 0; i < num_facilities; i++) {
                double cost = facility_costs[i] + service_costs[i][j];
                if (cost < min_cost) {
                    min_cost = cost;
                    cheapest_facility = i;
                }
            }
            
            if (cheapest_facility != -1 && !facility_open[cheapest_facility]) {
                facility_open[cheapest_facility] = 1;
                total_cost += facility_costs[cheapest_facility];
            }
        }
    }
    
    // Construct solution
    FacilityNode greedy_node;
    greedy_node.level = num_facilities;
    greedy_node.fixed_cost = 0.0;
    
    for (int i = 0; i < num_facilities; i++) {
        greedy_node.facility_open[i] = facility_open[i];
        if (facility_open[i]) {
            greedy_node.fixed_cost += facility_costs[i];
        }
    }
    
    return construct_solution(&greedy_node);
}

double calculate_improvement(int facility, int* facility_open, int* customer_served) {
    double improvement = -facility_costs[facility]; // Cost of opening facility
    
    for (int j = 0; j < num_customers; j++) {
        if (!customer_served[j]) {
            improvement += DBL_MAX / num_customers; // High value for serving new customer
        } else {
            double current_cost = get_current_service_cost(j, facility_open);
            double new_cost = service_costs[facility][j];
            if (new_cost < current_cost) {
                improvement += current_cost - new_cost;
            }
        }
    }
    
    return improvement;
}

double get_current_service_cost(int customer, int* facility_open) {
    double min_cost = DBL_MAX;
    
    for (int i = 0; i < num_facilities; i++) {
        if (facility_open[i]) {
            if (service_costs[i][customer] < min_cost) {
                min_cost = service_costs[i][customer];
            }
        }
    }
    
    return min_cost;
}

/**
 * Solve facility location using Branch & Bound
 */
LocationResult solve_facility_location() {
    clock_t start = clock();
    
    LocationResult result;
    result.nodes_explored = 0;
    result.nodes_pruned = 0;
    
    if (verbose) {
        printf("=== Starting Facility Location Branch & Bound Solution ===\n");
        printf("Facilities: %d, Customers: %d\n", num_facilities, num_customers);
        printf("Facility locations and costs:\n");
        for (int i = 0; i < num_facilities; i++) {
            printf("  %s - Cost: %.2f\n", facilities[i].name, facility_costs[i]);
        }
        printf("Customer locations:\n");
        for (int j = 0; j < num_customers; j++) {
            printf("  %s\n", customers[j].name);
        }
    }
    
    // Initialize
    best_cost = DBL_MAX;
    nodes_explored = 0;
    nodes_pruned = 0;
    
    // Get initial upper bound using greedy heuristic
    FacilitySolution greedy_solution = greedy_heuristic();
    if (greedy_solution.total_cost != DBL_MAX) {
        best_solution = greedy_solution;
        best_cost = greedy_solution.total_cost;
        
        if (verbose) {
            printf("Greedy upper bound: %.2f\n", best_cost);
        }
    }
    
    // Initialize root node
    FacilityNode root;
    root.level = 0;
    root.fixed_cost = 0.0;
    root.service_cost = 0.0;
    root.total_cost = 0.0;
    memset(root.facility_open, 0, sizeof(root.facility_open));
    root.bound = calculate_bound(&root);
    
    if (verbose) {
        printf("Root node bound: %.2f\n", root.bound);
    }
    
    // Initialize priority queue
    PriorityQueue pq;
    pq_init(&pq);
    pq_push(&pq, root);
    
    while (!pq_empty(&pq)) {
        FacilityNode current = pq_pop(&pq);
        nodes_explored++;
        
        if (verbose && nodes_explored <= 20) {
            printf("Exploring node at level %d, cost: %.2f, bound: %.2f\n",
                   current.level, current.total_cost, current.bound);
            printf("  Open facilities: ");
            print_open_facilities(current.facility_open);
        }
        
        // Pruning check
        if (current.bound >= best_cost) {
            nodes_pruned++;
            if (verbose && nodes_explored <= 20) {
                printf("  Pruned: bound %.2f >= best %.2f\n", current.bound, best_cost);
            }
            continue;
        }
        
        // Check if we have a complete solution
        if (current.level == num_facilities) {
            FacilitySolution solution = construct_solution(&current);
            if (solution.total_cost != DBL_MAX && solution.total_cost < best_cost) {
                best_solution = solution;
                best_cost = solution.total_cost;
                
                if (verbose) {
                    printf("*** New best solution found! ***\n");
                    printf("Total cost: %.2f\n", best_cost);
                    printf("Fixed cost: %.2f\n", solution.fixed_cost);
                    printf("Service cost: %.2f\n", solution.service_cost);
                    printf("Open facilities: ");
                    print_open_facilities(solution.facility_open);
                }
            }
            continue;
        }
        
        // Branch 1: Open current facility
        FacilityNode open_node = current;
        open_node.facility_open[current.level] = 1;
        open_node.level = current.level + 1;
        open_node.fixed_cost = current.fixed_cost + facility_costs[current.level];
        open_node.total_cost = open_node.fixed_cost + calculate_service_cost(&open_node);
        open_node.bound = calculate_bound(&open_node);
        
        if (open_node.bound < best_cost) {
            pq_push(&pq, open_node);
        } else {
            nodes_pruned++;
        }
        
        // Branch 2: Don't open current facility
        FacilityNode close_node = current;
        close_node.level = current.level + 1;
        close_node.total_cost = close_node.fixed_cost + calculate_service_cost(&close_node);
        close_node.bound = calculate_bound(&close_node);
        
        if (close_node.bound < best_cost) {
            pq_push(&pq, close_node);
        } else {
            nodes_pruned++;
        }
    }
    
    clock_t end = clock();
    result.execution_time = ((double)(end - start)) / CLOCKS_PER_SEC * 1000.0;
    result.best_solution = best_solution;
    result.nodes_explored = nodes_explored;
    result.nodes_pruned = nodes_pruned;
    
    if (verbose) {
        printf("=== Final Results ===\n");
        if (best_cost != DBL_MAX) {
            printf("Optimal total cost: %.2f\n", best_solution.total_cost);
            printf("Fixed costs: %.2f\n", best_solution.fixed_cost);
            printf("Service costs: %.2f\n", best_solution.service_cost);
            printf("Open facilities: ");
            print_open_facilities(best_solution.facility_open);
            printf("Customer assignments:\n");
            for (int j = 0; j < num_customers; j++) {
                int facility = best_solution.customer_assignment[j];
                printf("  %s -> %s (cost: %.2f)\n", 
                       customers[j].name, facilities[facility].name, 
                       service_costs[facility][j]);
            }
        }
        printf("Nodes explored: %d\n", nodes_explored);
        printf("Nodes pruned: %d\n", nodes_pruned);
        printf("Execution time: %.2f ms\n", result.execution_time);
    }
    
    return result;
}

void print_open_facilities(int* facility_open) {
    int first = 1;
    for (int i = 0; i < num_facilities; i++) {
        if (facility_open[i]) {
            if (!first) printf(", ");
            printf("%s", facilities[i].name);
            first = 0;
        }
    }
    if (first) printf("None");
    printf("\n");
}

int main() {
    printf("=== Facility Location Problem - Branch & Bound ===\n\n");
    
    // Test case 1: Small warehouse location problem
    num_facilities = 3;
    num_customers = 4;
    
    facilities[0] = (Location){0, 1.0, 1.0, "Warehouse_A"};
    facilities[1] = (Location){1, 4.0, 1.0, "Warehouse_B"};
    facilities[2] = (Location){2, 2.0, 4.0, "Warehouse_C"};
    
    customers[0] = (Location){0, 2.0, 2.0, "Customer_1"};
    customers[1] = (Location){1, 3.0, 1.0, "Customer_2"};
    customers[2] = (Location){2, 1.0, 3.0, "Customer_3"};
    customers[3] = (Location){3, 4.0, 3.0, "Customer_4"};
    
    facility_costs[0] = 50.0;
    facility_costs[1] = 60.0;
    facility_costs[2] = 55.0;
    
    // Calculate service costs (distance-based)
    for (int i = 0; i < num_facilities; i++) {
        for (int j = 0; j < num_customers; j++) {
            service_costs[i][j] = distance(&facilities[i], &customers[j]);
        }
    }
    
    printf("Test Case 1: 3 Warehouses, 4 Customers\n");
    
    verbose = 1;
    LocationResult result1 = solve_facility_location();
    
    // Test case 2: Larger facility location problem
    printf("\n============================================================\n");
    printf("Test Case 2: 5 Facilities, 6 Customers\n");
    
    num_facilities = 5;
    num_customers = 6;
    
    facilities[0] = (Location){0, 0.0, 0.0, "Facility_A"};
    facilities[1] = (Location){1, 2.0, 0.0, "Facility_B"};
    facilities[2] = (Location){2, 4.0, 0.0, "Facility_C"};
    facilities[3] = (Location){3, 1.0, 3.0, "Facility_D"};
    facilities[4] = (Location){4, 3.0, 3.0, "Facility_E"};
    
    customers[0] = (Location){0, 0.5, 1.0, "Cust_1"};
    customers[1] = (Location){1, 1.5, 1.0, "Cust_2"};
    customers[2] = (Location){2, 2.5, 1.0, "Cust_3"};
    customers[3] = (Location){3, 3.5, 1.0, "Cust_4"};
    customers[4] = (Location){4, 1.0, 2.0, "Cust_5"};
    customers[5] = (Location){5, 3.0, 2.0, "Cust_6"};
    
    facility_costs[0] = 40.0;
    facility_costs[1] = 45.0;
    facility_costs[2] = 50.0;
    facility_costs[3] = 35.0;
    facility_costs[4] = 42.0;
    
    // Calculate service costs
    for (int i = 0; i < num_facilities; i++) {
        for (int j = 0; j < num_customers; j++) {
            service_costs[i][j] = distance(&facilities[i], &customers[j]);
        }
    }
    
    verbose = 0;
    LocationResult result2 = solve_facility_location();
    
    printf("Optimal solution found:\n");
    printf("Total cost: %.2f\n", result2.best_solution.total_cost);
    printf("Fixed costs: %.2f\n", result2.best_solution.fixed_cost);
    printf("Service costs: %.2f\n", result2.best_solution.service_cost);
    printf("Open facilities: ");
    print_open_facilities(result2.best_solution.facility_open);
    
    printf("\nCustomer assignments:\n");
    for (int j = 0; j < num_customers; j++) {
        int facility = result2.best_solution.customer_assignment[j];
        printf("  %s -> %s (distance: %.2f)\n", 
               customers[j].name, facilities[facility].name, 
               distance(&facilities[facility], &customers[j]));
    }
    
    printf("\nPerformance metrics:\n");
    printf("Nodes explored: %d\n", result2.nodes_explored);
    printf("Nodes pruned: %d\n", result2.nodes_pruned);
    printf("Execution time: %.2f ms\n", result2.execution_time);
    
    // Performance analysis
    printf("\n=== Performance Analysis ===\n");
    printf("Branch & Bound for Facility Location:\n");
    printf("- Guarantees optimal solution\n");
    printf("- Effective bounds reduce search space dramatically\n");
    printf("- Greedy heuristics provide good initial bounds\n");
    printf("- Scales well for medium-sized problems\n");
    
    printf("\nBounding Techniques:\n");
    printf("- Lagrangian relaxation for tight lower bounds\n");
    printf("- Service cost minimization per customer\n");
    printf("- Amortized facility costs across customers\n");
    
    printf("\nReal-world Considerations:\n");
    printf("- Capacity constraints (capacitated version)\n");
    printf("- Multiple service types and priorities\n");
    printf("- Dynamic demand patterns\n");
    printf("- Geographic and regulatory constraints\n");
    
    // Scaling analysis
    printf("\n=== Scaling Analysis ===\n");
    
    srand(42);
    int problem_sizes[][2] = {{3, 4}, {4, 5}, {5, 6}};
    int num_sizes = sizeof(problem_sizes) / sizeof(problem_sizes[0]);
    
    printf("Facilities | Customers | Nodes Explored | Nodes Pruned | Time (ms) | Optimal Cost\n");
    printf("---------------------------------------------------------------------------------\n");
    
    for (int s = 0; s < num_sizes; s++) {
        num_facilities = problem_sizes[s][0];
        num_customers = problem_sizes[s][1];
        
        // Generate random problem instance
        for (int i = 0; i < num_facilities; i++) {
            facilities[i].id = i;
            facilities[i].x = (double)rand() / RAND_MAX * 10.0;
            facilities[i].y = (double)rand() / RAND_MAX * 10.0;
            sprintf(facilities[i].name, "F%d", i);
            facility_costs[i] = 30.0 + (double)rand() / RAND_MAX * 40.0;
        }
        
        for (int j = 0; j < num_customers; j++) {
            customers[j].id = j;
            customers[j].x = (double)rand() / RAND_MAX * 10.0;
            customers[j].y = (double)rand() / RAND_MAX * 10.0;
            sprintf(customers[j].name, "C%d", j);
        }
        
        // Calculate service costs
        for (int i = 0; i < num_facilities; i++) {
            for (int j = 0; j < num_customers; j++) {
                service_costs[i][j] = distance(&facilities[i], &customers[j]);
            }
        }
        
        verbose = 0;
        LocationResult result = solve_facility_location();
        
        printf("%9d | %9d | %14d | %12d | %8.2f | %11.2f\n",
               num_facilities, num_customers, result.nodes_explored, result.nodes_pruned,
               result.execution_time, result.best_solution.total_cost != DBL_MAX ? 
               result.best_solution.total_cost : 0.0);
    }
    
    printf("\nKey Observations:\n");
    printf("- Exponential growth in search space (2^n)\n");
    printf("- Effective pruning keeps actual nodes explored reasonable\n");
    printf("- Good bounds are crucial for performance\n");
    printf("- Greedy heuristics provide strong initial solutions\n");
    
    return 0;
}