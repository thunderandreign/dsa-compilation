/**
 * Facility Location Problem - Branch & Bound Implementation
 * 
 * This implementation demonstrates the Branch & Bound strategy for solving
 * the uncapacitated facility location problem, where we need to select
 * optimal facility locations to minimize total cost (facility + service costs).
 * 
 * Key Concepts:
 * - Branch & Bound: Systematic enumeration with cost-based pruning
 * - Dual Bounds: Lagrangian relaxation for tight lower bounds
 * - Greedy Heuristics: Upper bound generation for effective pruning
 * - Subproblem Structure: Binary decisions for facility opening
 * 
 * Time Complexity: O(2^n) worst case, but effective pruning reduces this
 * Space Complexity: O(n×m) for distance matrix + O(n) for recursion
 * 
 * Real-world Applications:
 * - Warehouse location optimization
 * - Hospital and emergency service placement
 * - Retail store location planning
 * - Data center placement
 * - Supply chain network design
 */

import java.util.*;

public class FacilityLocation {
    
    static class Location {
        int id;
        double x, y;
        String name;
        
        Location(int id, double x, double y, String name) {
            this.id = id;
            this.x = x;
            this.y = y;
            this.name = name;
        }
        
        double distanceTo(Location other) {
            return Math.sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
        }
        
        @Override
        public String toString() {
            return name + "(" + id + ")";
        }
    }
    
    static class FacilityNode {
        int level;                    // Current facility being considered
        boolean[] facilityOpen;       // Which facilities are open
        double fixedCost;            // Total facility opening costs
        double serviceCost;          // Total service costs
        double totalCost;            // Total cost so far
        double bound;                // Lower bound estimate
        
        FacilityNode(int numFacilities) {
            level = 0;
            facilityOpen = new boolean[numFacilities];
            fixedCost = 0.0;
            serviceCost = 0.0;
            totalCost = 0.0;
            bound = 0.0;
        }
        
        FacilityNode(FacilityNode other) {
            level = other.level;
            facilityOpen = other.facilityOpen.clone();
            fixedCost = other.fixedCost;
            serviceCost = other.serviceCost;
            totalCost = other.totalCost;
            bound = other.bound;
        }
    }
    
    static class FacilitySolution {
        boolean[] facilityOpen;
        int[] customerAssignment;     // Which facility serves each customer
        double fixedCost;
        double serviceCost;
        double totalCost;
        
        FacilitySolution(int numFacilities, int numCustomers) {
            facilityOpen = new boolean[numFacilities];
            customerAssignment = new int[numCustomers];
            Arrays.fill(customerAssignment, -1);
            fixedCost = 0.0;
            serviceCost = 0.0;
            totalCost = 0.0;
        }
        
        FacilitySolution(FacilitySolution other) {
            facilityOpen = other.facilityOpen.clone();
            customerAssignment = other.customerAssignment.clone();
            fixedCost = other.fixedCost;
            serviceCost = other.serviceCost;
            totalCost = other.totalCost;
        }
    }
    
    static class LocationResult {
        FacilitySolution bestSolution;
        int nodesExplored;
        int nodesPruned;
        double executionTime;
        List<String> steps;
        
        LocationResult(FacilitySolution solution, int explored, int pruned,
                      double time, List<String> steps) {
            this.bestSolution = solution;
            this.nodesExplored = explored;
            this.nodesPruned = pruned;
            this.executionTime = time;
            this.steps = new ArrayList<>(steps);
        }
    }
    
    private Location[] facilities;
    private Location[] customers;
    private double[] facilityCosts;      // Fixed cost to open each facility
    private double[][] serviceCosts;     // Cost to serve customer j from facility i
    private int numFacilities;
    private int numCustomers;
    
    private FacilitySolution bestSolution;
    private double bestCost;
    private int nodesExplored;
    private int nodesPruned;
    private List<String> steps;
    private boolean verbose;
    
    public FacilityLocation(Location[] facilities, Location[] customers, 
                           double[] facilityCosts, boolean verbose) {
        this.facilities = facilities;
        this.customers = customers;
        this.facilityCosts = facilityCosts;
        this.numFacilities = facilities.length;
        this.numCustomers = customers.length;
        this.verbose = verbose;
        this.steps = new ArrayList<>();
        
        // Calculate service costs (distance-based)
        this.serviceCosts = new double[numFacilities][numCustomers];
        for (int i = 0; i < numFacilities; i++) {
            for (int j = 0; j < numCustomers; j++) {
                this.serviceCosts[i][j] = facilities[i].distanceTo(customers[j]);
            }
        }
        
        this.bestCost = Double.MAX_VALUE;
        this.bestSolution = null;
        this.nodesExplored = 0;
        this.nodesPruned = 0;
    }
    
    /**
     * Solve facility location using Branch & Bound
     */
    public LocationResult solveFacilityLocation() {
        long startTime = System.nanoTime();
        
        if (verbose) {
            steps.add("=== Starting Facility Location Branch & Bound Solution ===");
            steps.add("Facilities: " + numFacilities + ", Customers: " + numCustomers);
            steps.add("Facility locations and costs:");
            for (int i = 0; i < numFacilities; i++) {
                steps.add("  " + facilities[i] + " - Cost: " + facilityCosts[i]);
            }
            steps.add("Customer locations:");
            for (int j = 0; j < numCustomers; j++) {
                steps.add("  " + customers[j]);
            }
        }
        
        // Get initial upper bound using greedy heuristic
        FacilitySolution greedySolution = greedyHeuristic();
        if (greedySolution != null) {
            bestSolution = greedySolution;
            bestCost = greedySolution.totalCost;
            
            if (verbose) {
                steps.add("Greedy upper bound: " + String.format("%.2f", bestCost));
            }
        }
        
        // Initialize root node
        FacilityNode root = new FacilityNode(numFacilities);
        root.bound = calculateBound(root);
        
        if (verbose) {
            steps.add("Root node bound: " + String.format("%.2f", root.bound));
        }
        
        // Use priority queue for best-first search
        PriorityQueue<FacilityNode> pq = new PriorityQueue<>((a, b) -> 
            Double.compare(a.bound, b.bound));
        pq.offer(root);
        
        while (!pq.isEmpty()) {
            FacilityNode current = pq.poll();
            nodesExplored++;
            
            if (verbose && nodesExplored <= 20) {
                steps.add("Exploring node at level " + current.level + 
                         ", cost: " + String.format("%.2f", current.totalCost) + 
                         ", bound: " + String.format("%.2f", current.bound));
                steps.add("  Open facilities: " + getOpenFacilitiesString(current.facilityOpen));
            }
            
            // Pruning check
            if (current.bound >= bestCost) {
                nodesPruned++;
                if (verbose && nodesExplored <= 20) {
                    steps.add("  Pruned: bound " + String.format("%.2f", current.bound) + 
                             " >= best " + String.format("%.2f", bestCost));
                }
                continue;
            }
            
            // Check if we have a complete solution
            if (current.level == numFacilities) {
                FacilitySolution solution = constructSolution(current);
                if (solution != null && solution.totalCost < bestCost) {
                    bestSolution = solution;
                    bestCost = solution.totalCost;
                    
                    if (verbose) {
                        steps.add("*** New best solution found! ***");
                        steps.add("Total cost: " + String.format("%.2f", bestCost));
                        steps.add("Fixed cost: " + String.format("%.2f", solution.fixedCost));
                        steps.add("Service cost: " + String.format("%.2f", solution.serviceCost));
                        steps.add("Open facilities: " + getOpenFacilitiesString(solution.facilityOpen));
                    }
                }
                continue;
            }
            
            // Branch 1: Open current facility
            FacilityNode openNode = new FacilityNode(current);
            openNode.facilityOpen[current.level] = true;
            openNode.level = current.level + 1;
            openNode.fixedCost = current.fixedCost + facilityCosts[current.level];
            openNode.totalCost = openNode.fixedCost + calculateServiceCost(openNode);
            openNode.bound = calculateBound(openNode);
            
            if (openNode.bound < bestCost) {
                pq.offer(openNode);
            } else {
                nodesPruned++;
            }
            
            // Branch 2: Don't open current facility
            FacilityNode closeNode = new FacilityNode(current);
            closeNode.level = current.level + 1;
            closeNode.totalCost = closeNode.fixedCost + calculateServiceCost(closeNode);
            closeNode.bound = calculateBound(closeNode);
            
            if (closeNode.bound < bestCost) {
                pq.offer(closeNode);
            } else {
                nodesPruned++;
            }
        }
        
        long endTime = System.nanoTime();
        double executionTime = (endTime - startTime) / 1_000_000.0;
        
        if (verbose) {
            steps.add("=== Final Results ===");
            if (bestSolution != null) {
                steps.add("Optimal total cost: " + String.format("%.2f", bestSolution.totalCost));
                steps.add("Fixed costs: " + String.format("%.2f", bestSolution.fixedCost));
                steps.add("Service costs: " + String.format("%.2f", bestSolution.serviceCost));
                steps.add("Open facilities: " + getOpenFacilitiesString(bestSolution.facilityOpen));
                steps.add("Customer assignments:");
                for (int j = 0; j < numCustomers; j++) {
                    int facility = bestSolution.customerAssignment[j];
                    steps.add("  " + customers[j] + " -> " + facilities[facility] + 
                             " (cost: " + String.format("%.2f", serviceCosts[facility][j]) + ")");
                }
            }
            steps.add("Nodes explored: " + nodesExplored);
            steps.add("Nodes pruned: " + nodesPruned);
            steps.add("Execution time: " + String.format("%.2f", executionTime) + " ms");
        }
        
        return new LocationResult(bestSolution, nodesExplored, nodesPruned, 
                                executionTime, steps);
    }
    
    /**
     * Calculate lower bound using Lagrangian relaxation approach
     */
    private double calculateBound(FacilityNode node) {
        double bound = node.fixedCost;
        
        // For each customer, add minimum service cost from available facilities
        for (int j = 0; j < numCustomers; j++) {
            double minServiceCost = Double.MAX_VALUE;
            boolean hasOpenFacility = false;
            
            // Check already opened facilities
            for (int i = 0; i < node.level; i++) {
                if (node.facilityOpen[i]) {
                    hasOpenFacility = true;
                    minServiceCost = Math.min(minServiceCost, serviceCosts[i][j]);
                }
            }
            
            // If no facility is open yet, consider minimum from remaining facilities
            if (!hasOpenFacility || node.level < numFacilities) {
                for (int i = node.level; i < numFacilities; i++) {
                    // Consider cost of opening facility i plus service cost
                    double costWithFacility = serviceCosts[i][j];
                    if (!hasOpenFacility) {
                        // If no facility is open, we must open at least one
                        costWithFacility += facilityCosts[i] / numCustomers; // Amortized cost
                    }
                    minServiceCost = Math.min(minServiceCost, costWithFacility);
                }
            }
            
            if (minServiceCost != Double.MAX_VALUE) {
                bound += minServiceCost;
            }
        }
        
        return bound;
    }
    
    /**
     * Calculate service cost for current facility configuration
     */
    private double calculateServiceCost(FacilityNode node) {
        double serviceCost = 0.0;
        
        for (int j = 0; j < numCustomers; j++) {
            double minCost = Double.MAX_VALUE;
            
            // Find cheapest open facility to serve customer j
            for (int i = 0; i < numFacilities; i++) {
                if (node.facilityOpen[i]) {
                    minCost = Math.min(minCost, serviceCosts[i][j]);
                }
            }
            
            if (minCost != Double.MAX_VALUE) {
                serviceCost += minCost;
            } else {
                // No facility is open - return high cost to indicate infeasibility
                return Double.MAX_VALUE;
            }
        }
        
        return serviceCost;
    }
    
    /**
     * Construct complete solution from facility node
     */
    private FacilitySolution constructSolution(FacilityNode node) {
        // Check if at least one facility is open
        boolean hasFacility = false;
        for (boolean open : node.facilityOpen) {
            if (open) {
                hasFacility = true;
                break;
            }
        }
        
        if (!hasFacility) return null; // Infeasible solution
        
        FacilitySolution solution = new FacilitySolution(numFacilities, numCustomers);
        solution.facilityOpen = node.facilityOpen.clone();
        solution.fixedCost = node.fixedCost;
        
        // Assign each customer to cheapest open facility
        for (int j = 0; j < numCustomers; j++) {
            double minCost = Double.MAX_VALUE;
            int bestFacility = -1;
            
            for (int i = 0; i < numFacilities; i++) {
                if (solution.facilityOpen[i] && serviceCosts[i][j] < minCost) {
                    minCost = serviceCosts[i][j];
                    bestFacility = i;
                }
            }
            
            solution.customerAssignment[j] = bestFacility;
            solution.serviceCost += minCost;
        }
        
        solution.totalCost = solution.fixedCost + solution.serviceCost;
        return solution;
    }
    
    /**
     * Greedy heuristic for initial upper bound
     */
    private FacilitySolution greedyHeuristic() {
        boolean[] facilityOpen = new boolean[numFacilities];
        double totalCost = 0.0;
        
        // Greedy: repeatedly open facility that gives best cost improvement
        boolean[] customerServed = new boolean[numCustomers];
        
        while (true) {
            double bestImprovement = 0.0;
            int bestFacility = -1;
            
            for (int i = 0; i < numFacilities; i++) {
                if (!facilityOpen[i]) {
                    double improvement = calculateImprovement(i, facilityOpen, customerServed);
                    if (improvement > bestImprovement) {
                        bestImprovement = improvement;
                        bestFacility = i;
                    }
                }
            }
            
            if (bestFacility == -1 || bestImprovement <= 0) break;
            
            facilityOpen[bestFacility] = true;
            totalCost += facilityCosts[bestFacility];
            
            // Update customer service status
            for (int j = 0; j < numCustomers; j++) {
                if (!customerServed[j]) {
                    customerServed[j] = true;
                    totalCost += serviceCosts[bestFacility][j];
                } else {
                    // Check if new facility is cheaper
                    double currentCost = getCurrentServiceCost(j, facilityOpen);
                    if (serviceCosts[bestFacility][j] < currentCost) {
                        totalCost = totalCost - currentCost + serviceCosts[bestFacility][j];
                    }
                }
            }
        }
        
        // Ensure all customers are served
        for (int j = 0; j < numCustomers; j++) {
            if (!customerServed[j]) {
                // Open cheapest facility for unserved customers
                double minCost = Double.MAX_VALUE;
                int cheapestFacility = -1;
                
                for (int i = 0; i < numFacilities; i++) {
                    double cost = facilityCosts[i] + serviceCosts[i][j];
                    if (cost < minCost) {
                        minCost = cost;
                        cheapestFacility = i;
                    }
                }
                
                if (cheapestFacility != -1 && !facilityOpen[cheapestFacility]) {
                    facilityOpen[cheapestFacility] = true;
                    totalCost += facilityCosts[cheapestFacility];
                }
            }
        }
        
        // Construct solution
        FacilityNode greedyNode = new FacilityNode(numFacilities);
        greedyNode.facilityOpen = facilityOpen;
        greedyNode.level = numFacilities;
        
        for (int i = 0; i < numFacilities; i++) {
            if (facilityOpen[i]) {
                greedyNode.fixedCost += facilityCosts[i];
            }
        }
        
        return constructSolution(greedyNode);
    }
    
    private double calculateImprovement(int facility, boolean[] facilityOpen, boolean[] customerServed) {
        double improvement = -facilityCosts[facility]; // Cost of opening facility
        
        for (int j = 0; j < numCustomers; j++) {
            if (!customerServed[j]) {
                improvement += Double.MAX_VALUE / numCustomers; // High value for serving new customer
            } else {
                double currentCost = getCurrentServiceCost(j, facilityOpen);
                double newCost = serviceCosts[facility][j];
                if (newCost < currentCost) {
                    improvement += currentCost - newCost;
                }
            }
        }
        
        return improvement;
    }
    
    private double getCurrentServiceCost(int customer, boolean[] facilityOpen) {
        double minCost = Double.MAX_VALUE;
        
        for (int i = 0; i < numFacilities; i++) {
            if (facilityOpen[i]) {
                minCost = Math.min(minCost, serviceCosts[i][customer]);
            }
        }
        
        return minCost;
    }
    
    private String getOpenFacilitiesString(boolean[] facilityOpen) {
        StringBuilder sb = new StringBuilder();
        boolean first = true;
        
        for (int i = 0; i < numFacilities; i++) {
            if (facilityOpen[i]) {
                if (!first) sb.append(", ");
                sb.append(facilities[i].name);
                first = false;
            }
        }
        
        return sb.length() > 0 ? sb.toString() : "None";
    }
    
    public static void main(String[] args) {
        System.out.println("=== Facility Location Problem - Branch & Bound ===\n");
        
        // Test case 1: Small warehouse location problem
        Location[] facilities1 = {
            new Location(0, 1, 1, "Warehouse_A"),
            new Location(1, 4, 1, "Warehouse_B"),
            new Location(2, 2, 4, "Warehouse_C")
        };
        
        Location[] customers1 = {
            new Location(0, 2, 2, "Customer_1"),
            new Location(1, 3, 1, "Customer_2"),
            new Location(2, 1, 3, "Customer_3"),
            new Location(3, 4, 3, "Customer_4")
        };
        
        double[] facilityCosts1 = {50.0, 60.0, 55.0}; // Fixed costs to open facilities
        
        System.out.println("Test Case 1: 3 Warehouses, 4 Customers");
        
        FacilityLocation fl1 = new FacilityLocation(facilities1, customers1, facilityCosts1, true);
        LocationResult result1 = fl1.solveFacilityLocation();
        
        System.out.println("Branch & Bound Solution:");
        for (String step : result1.steps) {
            System.out.println(step);
        }
        
        // Test case 2: Larger facility location problem
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: 5 Facilities, 6 Customers");
        
        Location[] facilities2 = {
            new Location(0, 0, 0, "Facility_A"),
            new Location(1, 2, 0, "Facility_B"),
            new Location(2, 4, 0, "Facility_C"),
            new Location(3, 1, 3, "Facility_D"),
            new Location(4, 3, 3, "Facility_E")
        };
        
        Location[] customers2 = {
            new Location(0, 0.5, 1, "Cust_1"),
            new Location(1, 1.5, 1, "Cust_2"),
            new Location(2, 2.5, 1, "Cust_3"),
            new Location(3, 3.5, 1, "Cust_4"),
            new Location(4, 1, 2, "Cust_5"),
            new Location(5, 3, 2, "Cust_6")
        };
        
        double[] facilityCosts2 = {40.0, 45.0, 50.0, 35.0, 42.0};
        
        FacilityLocation fl2 = new FacilityLocation(facilities2, customers2, facilityCosts2, false);
        LocationResult result2 = fl2.solveFacilityLocation();
        
        System.out.println("Optimal solution found:");
        System.out.println("Total cost: " + String.format("%.2f", result2.bestSolution.totalCost));
        System.out.println("Fixed costs: " + String.format("%.2f", result2.bestSolution.fixedCost));
        System.out.println("Service costs: " + String.format("%.2f", result2.bestSolution.serviceCost));
        System.out.println("Open facilities: " + fl2.getOpenFacilitiesString(result2.bestSolution.facilityOpen));
        
        System.out.println("\nCustomer assignments:");
        for (int j = 0; j < customers2.length; j++) {
            int facility = result2.bestSolution.customerAssignment[j];
            System.out.println("  " + customers2[j] + " -> " + facilities2[facility] + 
                             " (distance: " + String.format("%.2f", 
                             facilities2[facility].distanceTo(customers2[j])) + ")");
        }
        
        System.out.println("\nPerformance metrics:");
        System.out.println("Nodes explored: " + result2.nodesExplored);
        System.out.println("Nodes pruned: " + result2.nodesPruned);
        System.out.println("Execution time: " + String.format("%.2f", result2.executionTime) + " ms");
        
        // Performance analysis
        System.out.println("\n=== Performance Analysis ===");
        System.out.println("Branch & Bound for Facility Location:");
        System.out.println("- Guarantees optimal solution");
        System.out.println("- Effective bounds reduce search space dramatically");
        System.out.println("- Greedy heuristics provide good initial bounds");
        System.out.println("- Scales well for medium-sized problems");
        
        System.out.println("\nBounding Techniques:");
        System.out.println("- Lagrangian relaxation for tight lower bounds");
        System.out.println("- Service cost minimization per customer");
        System.out.println("- Amortized facility costs across customers");
        
        System.out.println("\nReal-world Considerations:");
        System.out.println("- Capacity constraints (capacitated version)");
        System.out.println("- Multiple service types and priorities");
        System.out.println("- Dynamic demand patterns");
        System.out.println("- Geographic and regulatory constraints");
        
        demonstrateScaling();
    }
    
    private static void demonstrateScaling() {
        System.out.println("\n=== Scaling Analysis ===");
        
        Random rand = new Random(42);
        
        // Test different problem sizes
        int[][] problemSizes = {{3, 4}, {4, 5}, {5, 6}};
        
        System.out.println("Facilities | Customers | Nodes Explored | Nodes Pruned | Time (ms) | Optimal Cost");
        System.out.println("-".repeat(85));
        
        for (int[] size : problemSizes) {
            int numFacilities = size[0];
            int numCustomers = size[1];
            
            // Generate random problem instance
            Location[] facilities = new Location[numFacilities];
            Location[] customers = new Location[numCustomers];
            double[] facilityCosts = new double[numFacilities];
            
            for (int i = 0; i < numFacilities; i++) {
                facilities[i] = new Location(i, rand.nextDouble() * 10, 
                                           rand.nextDouble() * 10, "F" + i);
                facilityCosts[i] = 30 + rand.nextDouble() * 40;
            }
            
            for (int j = 0; j < numCustomers; j++) {
                customers[j] = new Location(j, rand.nextDouble() * 10, 
                                          rand.nextDouble() * 10, "C" + j);
            }
            
            FacilityLocation fl = new FacilityLocation(facilities, customers, facilityCosts, false);
            LocationResult result = fl.solveFacilityLocation();
            
            System.out.printf("%9d | %9d | %14d | %12d | %8.2f | %11.2f\n",
                numFacilities, numCustomers, result.nodesExplored, result.nodesPruned,
                result.executionTime, result.bestSolution != null ? result.bestSolution.totalCost : 0.0);
        }
        
        System.out.println("\nKey Observations:");
        System.out.println("- Exponential growth in search space (2^n)");
        System.out.println("- Effective pruning keeps actual nodes explored reasonable");
        System.out.println("- Good bounds are crucial for performance");
        System.out.println("- Greedy heuristics provide strong initial solutions");
    }
}