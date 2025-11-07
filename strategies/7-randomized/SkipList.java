/**
 * Skip List - Advanced Probabilistic Data Structure Implementation
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

import java.util.*;

public class SkipList {
    
    static class SkipListNode {
        int key;
        String value;
        SkipListNode[] forward;
        int level;
        
        SkipListNode(int key, String value, int level) {
            this.key = key;
            this.value = value;
            this.level = level;
            this.forward = new SkipListNode[level + 1];
        }
        
        @Override
        public String toString() {
            return String.format("Node[key=%d, value=%s, level=%d]", key, value, level);
        }
    }
    
    static class SearchResult {
        boolean found;
        String value;
        int comparisons;
        List<Integer> searchPath;
        
        SearchResult(boolean found, String value, int comparisons, List<Integer> searchPath) {
            this.found = found;
            this.value = value;
            this.comparisons = comparisons;
            this.searchPath = new ArrayList<>(searchPath);
        }
        
        @Override
        public String toString() {
            return String.format("Found: %s, Value: %s, Comparisons: %d, Path: %s", 
                               found, value, comparisons, searchPath);
        }
    }
    
    static class PerformanceMetrics {
        int totalNodes;
        int maxLevel;
        double averageLevel;
        int totalForwardPointers;
        double spaceOverhead;
        Map<Integer, Integer> levelDistribution;
        
        PerformanceMetrics() {
            levelDistribution = new HashMap<>();
        }
        
        @Override
        public String toString() {
            return String.format("Nodes: %d, Max Level: %d, Avg Level: %.2f, " +
                               "Total Pointers: %d, Space Overhead: %.2fx",
                               totalNodes, maxLevel, averageLevel, 
                               totalForwardPointers, spaceOverhead);
        }
    }
    
    private static final int MAX_LEVEL = 16;
    private static final double PROBABILITY = 0.5;
    
    private SkipListNode header;
    private int currentLevel;
    private int size;
    private Random random;
    private boolean verbose;
    private List<String> operations;
    
    public SkipList(boolean verbose) {
        this.header = new SkipListNode(Integer.MIN_VALUE, null, MAX_LEVEL);
        this.currentLevel = 0;
        this.size = 0;
        this.random = new Random();
        this.verbose = verbose;
        this.operations = new ArrayList<>();
        
        if (verbose) {
            operations.add("=== Skip List Initialized ===");
            operations.add(String.format("Max Level: %d, Probability: %.2f", MAX_LEVEL, PROBABILITY));
        }
    }
    
    public SkipList(long seed, boolean verbose) {
        this.header = new SkipListNode(Integer.MIN_VALUE, null, MAX_LEVEL);
        this.currentLevel = 0;
        this.size = 0;
        this.random = new Random(seed);
        this.verbose = verbose;
        this.operations = new ArrayList<>();
        
        if (verbose) {
            operations.add("=== Skip List Initialized with Seed ===");
            operations.add(String.format("Seed: %d, Max Level: %d, Probability: %.2f", 
                          seed, MAX_LEVEL, PROBABILITY));
        }
    }
    
    /**
     * Generate random level using geometric distribution
     */
    private int randomLevel() {
        int level = 0;
        while (random.nextDouble() < PROBABILITY && level < MAX_LEVEL) {
            level++;
        }
        return level;
    }
    
    /**
     * Search for a key in the skip list
     */
    public SearchResult search(int key) {
        List<Integer> searchPath = new ArrayList<>();
        int comparisons = 0;
        
        if (verbose) {
            operations.add(String.format("=== Searching for key %d ===", key));
        }
        
        SkipListNode current = header;
        
        // Start from the highest level and work downward
        for (int level = currentLevel; level >= 0; level--) {
            while (current.forward[level] != null && current.forward[level].key < key) {
                current = current.forward[level];
                searchPath.add(current.key);
                comparisons++;
                
                if (verbose) {
                    operations.add(String.format("Level %d: Moving to node %d", level, current.key));
                }
            }
            comparisons++; // Count the comparison that failed the while condition
            
            if (verbose && level > 0) {
                operations.add(String.format("Level %d: Dropping down", level));
            }
        }
        
        // Move to the next node at level 0
        current = current.forward[0];
        if (current != null) {
            comparisons++;
        }
        
        boolean found = current != null && current.key == key;
        String value = found ? current.value : null;
        
        if (verbose) {
            operations.add(String.format("Search result: %s, Comparisons: %d", 
                          found ? "FOUND" : "NOT FOUND", comparisons));
        }
        
        return new SearchResult(found, value, comparisons, searchPath);
    }
    
    /**
     * Insert a key-value pair into the skip list
     */
    public boolean insert(int key, String value) {
        SkipListNode[] update = new SkipListNode[MAX_LEVEL + 1];
        SkipListNode current = header;
        
        if (verbose) {
            operations.add(String.format("=== Inserting key %d, value %s ===", key, value));
        }
        
        // Find the insertion point and record update pointers
        for (int level = currentLevel; level >= 0; level--) {
            while (current.forward[level] != null && current.forward[level].key < key) {
                current = current.forward[level];
            }
            update[level] = current;
            
            if (verbose) {
                operations.add(String.format("Level %d: Update pointer at node %d", 
                              level, current.key));
            }
        }
        
        current = current.forward[0];
        
        // If key already exists, update the value
        if (current != null && current.key == key) {
            current.value = value;
            if (verbose) {
                operations.add("Key already exists - updated value");
            }
            return false; // Indicate that no new node was inserted
        }
        
        // Generate random level for new node
        int newLevel = randomLevel();
        
        if (verbose) {
            operations.add(String.format("Generated random level: %d", newLevel));
        }
        
        // If new level is higher than current level, update header pointers
        if (newLevel > currentLevel) {
            for (int level = currentLevel + 1; level <= newLevel; level++) {
                update[level] = header;
            }
            currentLevel = newLevel;
            
            if (verbose) {
                operations.add(String.format("Increased current level to: %d", currentLevel));
            }
        }
        
        // Create new node and update pointers
        SkipListNode newNode = new SkipListNode(key, value, newLevel);
        
        for (int level = 0; level <= newLevel; level++) {
            newNode.forward[level] = update[level].forward[level];
            update[level].forward[level] = newNode;
            
            if (verbose) {
                operations.add(String.format("Level %d: Linked new node", level));
            }
        }
        
        size++;
        
        if (verbose) {
            operations.add(String.format("Insertion completed. Size: %d", size));
        }
        
        return true;
    }
    
    /**
     * Delete a key from the skip list
     */
    public boolean delete(int key) {
        SkipListNode[] update = new SkipListNode[MAX_LEVEL + 1];
        SkipListNode current = header;
        
        if (verbose) {
            operations.add(String.format("=== Deleting key %d ===", key));
        }
        
        // Find the node to delete and record update pointers
        for (int level = currentLevel; level >= 0; level--) {
            while (current.forward[level] != null && current.forward[level].key < key) {
                current = current.forward[level];
            }
            update[level] = current;
        }
        
        current = current.forward[0];
        
        // If key doesn't exist, return false
        if (current == null || current.key != key) {
            if (verbose) {
                operations.add("Key not found - deletion failed");
            }
            return false;
        }
        
        if (verbose) {
            operations.add(String.format("Found node to delete: %s", current));
        }
        
        // Update pointers to skip the deleted node
        for (int level = 0; level <= current.level; level++) {
            update[level].forward[level] = current.forward[level];
            
            if (verbose) {
                operations.add(String.format("Level %d: Updated pointer to skip deleted node", level));
            }
        }
        
        // Reduce current level if necessary
        while (currentLevel > 0 && header.forward[currentLevel] == null) {
            currentLevel--;
            if (verbose) {
                operations.add(String.format("Reduced current level to: %d", currentLevel));
            }
        }
        
        size--;
        
        if (verbose) {
            operations.add(String.format("Deletion completed. Size: %d", size));
        }
        
        return true;
    }
    
    /**
     * Display the skip list structure
     */
    public void display() {
        System.out.println("=== Skip List Structure ===");
        System.out.printf("Size: %d, Current Level: %d\n", size, currentLevel);
        
        for (int level = currentLevel; level >= 0; level--) {
            System.out.printf("Level %2d: ", level);
            SkipListNode current = header.forward[level];
            
            while (current != null) {
                System.out.printf("[%d:%s] -> ", current.key, current.value);
                current = current.forward[level];
            }
            System.out.println("NULL");
        }
        System.out.println();
    }
    
    /**
     * Get all key-value pairs in sorted order
     */
    public List<Map.Entry<Integer, String>> getAllEntries() {
        List<Map.Entry<Integer, String>> entries = new ArrayList<>();
        SkipListNode current = header.forward[0];
        
        while (current != null) {
            entries.add(new AbstractMap.SimpleEntry<>(current.key, current.value));
            current = current.forward[0];
        }
        
        return entries;
    }
    
    /**
     * Range query: find all keys in the given range [minKey, maxKey]
     */
    public List<Map.Entry<Integer, String>> rangeQuery(int minKey, int maxKey) {
        List<Map.Entry<Integer, String>> result = new ArrayList<>();
        
        if (verbose) {
            operations.add(String.format("=== Range Query [%d, %d] ===", minKey, maxKey));
        }
        
        // Find the first node >= minKey
        SkipListNode current = header;
        for (int level = currentLevel; level >= 0; level--) {
            while (current.forward[level] != null && current.forward[level].key < minKey) {
                current = current.forward[level];
            }
        }
        
        current = current.forward[0];
        
        // Collect all nodes in the range
        while (current != null && current.key <= maxKey) {
            result.add(new AbstractMap.SimpleEntry<>(current.key, current.value));
            current = current.forward[0];
        }
        
        if (verbose) {
            operations.add(String.format("Range query found %d entries", result.size()));
        }
        
        return result;
    }
    
    /**
     * Calculate performance metrics
     */
    public PerformanceMetrics calculateMetrics() {
        PerformanceMetrics metrics = new PerformanceMetrics();
        
        metrics.totalNodes = size;
        metrics.maxLevel = currentLevel;
        
        // Calculate level distribution and average level
        int totalLevels = 0;
        int totalPointers = 0;
        
        SkipListNode current = header.forward[0];
        while (current != null) {
            int nodeLevel = current.level;
            totalLevels += nodeLevel + 1; // +1 because level is 0-indexed
            totalPointers += nodeLevel + 1;
            
            metrics.levelDistribution.merge(nodeLevel, 1, Integer::sum);
            current = current.forward[0];
        }
        
        if (size > 0) {
            metrics.averageLevel = (double) totalLevels / size;
            metrics.spaceOverhead = (double) totalPointers / size;
        }
        
        metrics.totalForwardPointers = totalPointers;
        
        return metrics;
    }
    
    /**
     * Verify skip list invariants
     */
    public boolean verifyIntegrity() {
        // Check if keys are in sorted order at level 0
        SkipListNode current = header.forward[0];
        int prevKey = Integer.MIN_VALUE;
        
        while (current != null) {
            if (current.key <= prevKey) {
                System.err.printf("Integrity violation: Key %d <= previous key %d\n", 
                                current.key, prevKey);
                return false;
            }
            prevKey = current.key;
            current = current.forward[0];
        }
        
        // Check if higher levels are subsets of lower levels
        for (int level = 1; level <= currentLevel; level++) {
            Set<Integer> currentLevelKeys = new HashSet<>();
            Set<Integer> lowerLevelKeys = new HashSet<>();
            
            // Collect keys at current level
            current = header.forward[level];
            while (current != null) {
                currentLevelKeys.add(current.key);
                current = current.forward[level];
            }
            
            // Collect keys at level below
            current = header.forward[level - 1];
            while (current != null) {
                lowerLevelKeys.add(current.key);
                current = current.forward[level - 1];
            }
            
            // Current level should be a subset of lower level
            if (!lowerLevelKeys.containsAll(currentLevelKeys)) {
                System.err.printf("Integrity violation: Level %d is not a subset of level %d\n", 
                                level, level - 1);
                return false;
            }
        }
        
        return true;
    }
    
    public int size() {
        return size;
    }
    
    public boolean isEmpty() {
        return size == 0;
    }
    
    public List<String> getOperations() {
        return new ArrayList<>(operations);
    }
    
    public void clearOperations() {
        operations.clear();
    }
    
    public static void main(String[] args) {
        System.out.println("=== Skip List - Comprehensive Analysis ===\n");
        
        // Test case 1: Basic operations with step-by-step demonstration
        System.out.println("Test Case 1: Basic Operations");
        SkipList skipList = new SkipList(42, true);
        
        // Insert some key-value pairs
        int[] keys = {3, 6, 7, 9, 12, 19, 17, 26, 21, 25};
        String[] values = {"three", "six", "seven", "nine", "twelve", 
                          "nineteen", "seventeen", "twenty-six", "twenty-one", "twenty-five"};
        
        System.out.println("Inserting key-value pairs:");
        for (int i = 0; i < keys.length; i++) {
            skipList.insert(keys[i], values[i]);
            if (i < 5) { // Show details for first few insertions
                System.out.println("\nOperations for insert(" + keys[i] + ", " + values[i] + "):");
                for (String op : skipList.getOperations()) {
                    System.out.println("  " + op);
                }
                skipList.clearOperations();
            }
        }
        
        skipList.display();
        
        // Search operations
        System.out.println("Search Operations:");
        int[] searchKeys = {7, 15, 21, 30};
        
        for (int key : searchKeys) {
            SearchResult result = skipList.search(key);
            System.out.printf("Search(%d): %s\n", key, result);
            
            if (skipList.verbose && key == 7) {
                System.out.println("Detailed search path:");
                for (String op : skipList.getOperations()) {
                    System.out.println("  " + op);
                }
                skipList.clearOperations();
            }
        }
        
        // Test case 2: Performance comparison with different data sizes
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: Performance Analysis");
        
        int[] dataSizes = {100, 1000, 10000};
        
        for (int dataSize : dataSizes) {
            System.out.printf("\nAnalyzing Skip List with %d elements:\n", dataSize);
            
            SkipList perfList = new SkipList(42, false);
            Random perfRandom = new Random(42);
            
            // Insert random data
            long insertTime = System.nanoTime();
            Set<Integer> insertedKeys = new HashSet<>();
            
            for (int i = 0; i < dataSize; i++) {
                int key = perfRandom.nextInt(dataSize * 2);
                insertedKeys.add(key);
                perfList.insert(key, "value" + key);
            }
            
            insertTime = System.nanoTime() - insertTime;
            
            // Perform random searches
            List<Integer> keyList = new ArrayList<>(insertedKeys);
            int numSearches = Math.min(1000, keyList.size());
            int totalComparisons = 0;
            
            long searchTime = System.nanoTime();
            for (int i = 0; i < numSearches; i++) {
                int randomKey = keyList.get(perfRandom.nextInt(keyList.size()));
                SearchResult result = perfList.search(randomKey);
                totalComparisons += result.comparisons;
            }
            searchTime = System.nanoTime() - searchTime;
            
            // Calculate metrics
            PerformanceMetrics metrics = perfList.calculateMetrics();
            double avgComparisons = (double) totalComparisons / numSearches;
            double theoreticalComparisons = Math.log(perfList.size()) / Math.log(2);
            
            System.out.printf("Size: %d\n", perfList.size());
            System.out.printf("Insert Time: %.2f ms\n", insertTime / 1_000_000.0);
            System.out.printf("Search Time: %.2f ms (%d searches)\n", 
                             searchTime / 1_000_000.0, numSearches);
            System.out.printf("Average Comparisons: %.2f (theoretical: %.2f)\n", 
                             avgComparisons, theoreticalComparisons);
            System.out.printf("Performance Metrics: %s\n", metrics);
            
            // Verify integrity
            boolean isValid = perfList.verifyIntegrity();
            System.out.printf("Integrity Check: %s\n", isValid ? "PASSED" : "FAILED");
        }
        
        // Test case 3: Level distribution analysis
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 3: Level Distribution Analysis");
        
        SkipList levelAnalysis = new SkipList(42, false);
        Random levelRandom = new Random(42);
        
        // Insert 10000 elements and analyze level distribution
        for (int i = 0; i < 10000; i++) {
            levelAnalysis.insert(levelRandom.nextInt(50000), "value" + i);
        }
        
        PerformanceMetrics levelMetrics = levelAnalysis.calculateMetrics();
        
        System.out.println("Level Distribution:");
        System.out.printf("%-8s | %-8s | %-12s | %-12s\n", "Level", "Count", "Percentage", "Expected %");
        System.out.println("-".repeat(50));
        
        for (int level = 0; level <= levelMetrics.maxLevel; level++) {
            int count = levelMetrics.levelDistribution.getOrDefault(level, 0);
            double percentage = (count * 100.0) / levelMetrics.totalNodes;
            double expected = 50.0 * Math.pow(0.5, level); // Expected percentage
            
            System.out.printf("%-8d | %-8d | %-12.2f | %-12.2f\n", 
                             level, count, percentage, expected);
        }
        
        System.out.printf("\nTheoretical vs Actual Analysis:\n");
        System.out.printf("Expected Average Level: %.2f\n", 1.0 / PROBABILITY);
        System.out.printf("Actual Average Level: %.2f\n", levelMetrics.averageLevel);
        System.out.printf("Expected Space Overhead: %.2fx\n", 1.0 / (1.0 - PROBABILITY));
        System.out.printf("Actual Space Overhead: %.2fx\n", levelMetrics.spaceOverhead);
        
        // Test case 4: Range query demonstration
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 4: Range Query Operations");
        
        SkipList rangeList = new SkipList(42, true);
        
        // Insert ordered data
        for (int i = 1; i <= 20; i += 2) {
            rangeList.insert(i, "value" + i);
        }
        
        rangeList.display();
        
        // Perform range queries
        int[][] ranges = {{3, 9}, {10, 15}, {5, 25}};
        
        for (int[] range : ranges) {
            List<Map.Entry<Integer, String>> results = rangeList.rangeQuery(range[0], range[1]);
            System.out.printf("Range [%d, %d]: ", range[0], range[1]);
            for (Map.Entry<Integer, String> entry : results) {
                System.out.printf("(%d:%s) ", entry.getKey(), entry.getValue());
            }
            System.out.println();
        }
        
        // Test case 5: Probabilistic analysis
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 5: Probabilistic Behavior Analysis");
        
        // Run multiple trials with different seeds
        int numTrials = 100;
        int elementsPerTrial = 1000;
        
        List<Double> avgComparisons = new ArrayList<>();
        List<Integer> maxLevels = new ArrayList<>();
        List<Double> spaceOverheads = new ArrayList<>();
        
        for (int trial = 0; trial < numTrials; trial++) {
            SkipList trialList = new SkipList(trial, false);
            Random trialRandom = new Random(trial);
            
            // Insert elements
            Set<Integer> keys = new HashSet<>();
            for (int i = 0; i < elementsPerTrial; i++) {
                int key = trialRandom.nextInt(elementsPerTrial * 2);
                keys.add(key);
                trialList.insert(key, "value" + key);
            }
            
            // Measure search performance
            List<Integer> keyList = new ArrayList<>(keys);
            int searches = Math.min(100, keyList.size());
            int totalComparisons = 0;
            
            for (int i = 0; i < searches; i++) {
                int randomKey = keyList.get(trialRandom.nextInt(keyList.size()));
                SearchResult result = trialList.search(randomKey);
                totalComparisons += result.comparisons;
            }
            
            PerformanceMetrics trialMetrics = trialList.calculateMetrics();
            
            avgComparisons.add((double) totalComparisons / searches);
            maxLevels.add(trialMetrics.maxLevel);
            spaceOverheads.add(trialMetrics.spaceOverhead);
        }
        
        // Calculate statistics
        double meanComparisons = avgComparisons.stream().mapToDouble(Double::doubleValue).average().orElse(0);
        double stdComparisons = Math.sqrt(avgComparisons.stream()
            .mapToDouble(d -> Math.pow(d - meanComparisons, 2)).average().orElse(0));
        
        double meanMaxLevel = maxLevels.stream().mapToInt(Integer::intValue).average().orElse(0);
        double meanSpaceOverhead = spaceOverheads.stream().mapToDouble(Double::doubleValue).average().orElse(0);
        
        double theoreticalComparisons = Math.log(elementsPerTrial) / Math.log(2);
        
        System.out.printf("Probabilistic Analysis (%d trials, %d elements each):\n", 
                         numTrials, elementsPerTrial);
        System.out.printf("Average Comparisons: %.2f ± %.2f (theoretical: %.2f)\n", 
                         meanComparisons, stdComparisons, theoreticalComparisons);
        System.out.printf("Average Max Level: %.2f (theoretical: %.2f)\n", 
                         meanMaxLevel, Math.log(elementsPerTrial) / Math.log(2));
        System.out.printf("Average Space Overhead: %.2fx (theoretical: %.2fx)\n", 
                         meanSpaceOverhead, 1.0 / (1.0 - PROBABILITY));
        
        System.out.println("\n=== Skip List Analysis Summary ===");
        System.out.println("Key Advantages:");
        System.out.println("- Simple probabilistic balancing (no complex rotations)");
        System.out.println("- Expected O(log n) performance for all operations");
        System.out.println("- Excellent for concurrent implementations");
        System.out.println("- Natural range query support");
        System.out.println("- Predictable memory access patterns");
        
        System.out.println("\nComparison with Balanced Trees:");
        System.out.println("- Simpler implementation than AVL/Red-Black trees");
        System.out.println("- Better constant factors in practice");
        System.out.println("- More space overhead but better cache performance");
        System.out.println("- Probabilistic guarantees vs deterministic guarantees");
        
        demonstrateRealWorldApplications();
    }
    
    private static void demonstrateRealWorldApplications() {
        System.out.println("\n=== Real-World Applications ===");
        
        // Application 1: In-memory database index
        System.out.println("\n1. In-Memory Database Index Simulation");
        SkipList dbIndex = new SkipList(42, false);
        
        // Simulate database records
        String[] records = {
            "emp_001:John Doe:Software Engineer",
            "emp_003:Alice Smith:Product Manager", 
            "emp_007:Bob Johnson:Data Scientist",
            "emp_012:Carol Brown:UX Designer",
            "emp_019:David Wilson:DevOps Engineer"
        };
        
        // Index by employee ID
        for (String record : records) {
            String[] parts = record.split(":");
            int empId = Integer.parseInt(parts[0].substring(4)); // Extract ID from emp_XXX
            dbIndex.insert(empId, record);
        }
        
        System.out.println("Database Index Contents:");
        for (Map.Entry<Integer, String> entry : dbIndex.getAllEntries()) {
            System.out.printf("ID %03d: %s\n", entry.getKey(), entry.getValue());
        }
        
        // Demonstrate range queries (e.g., find employees with IDs 5-15)
        System.out.println("\nEmployees with IDs 5-15:");
        for (Map.Entry<Integer, String> entry : dbIndex.rangeQuery(5, 15)) {
            System.out.printf("  %s\n", entry.getValue());
        }
        
        // Application 2: LRU Cache with Skip List
        System.out.println("\n2. Time-based Cache Simulation");
        SkipList timeCache = new SkipList(42, false);
        
        // Simulate cache entries with timestamps
        long baseTime = System.currentTimeMillis();
        String[] cacheData = {"page_A", "page_B", "page_C", "page_D", "page_E"};
        
        for (int i = 0; i < cacheData.length; i++) {
            int timestamp = (int) (baseTime + i * 1000); // 1 second intervals
            timeCache.insert(timestamp, cacheData[i]);
        }
        
        System.out.println("Cache contents (by timestamp):");
        for (Map.Entry<Integer, String> entry : timeCache.getAllEntries()) {
            System.out.printf("Time %d: %s\n", entry.getKey(), entry.getValue());
        }
        
        // Find entries in last 3 seconds
        int cutoffTime = (int) (baseTime + 2000);
        System.out.printf("\nRecent entries (after timestamp %d):\n", cutoffTime);
        for (Map.Entry<Integer, String> entry : timeCache.rangeQuery(cutoffTime, Integer.MAX_VALUE)) {
            System.out.printf("  %s\n", entry.getValue());
        }
        
        // Application 3: Priority queue simulation
        System.out.println("\n3. Task Scheduling Priority Queue");
        SkipList taskQueue = new SkipList(42, false);
        
        // Simulate tasks with priorities (lower number = higher priority)
        String[] tasks = {
            "Critical Security Patch",    // Priority 1
            "Database Backup",           // Priority 3  
            "Code Review",               // Priority 5
            "Update Documentation",      // Priority 7
            "System Monitoring"          // Priority 2
        };
        int[] priorities = {1, 3, 5, 7, 2};
        
        for (int i = 0; i < tasks.length; i++) {
            taskQueue.insert(priorities[i], tasks[i]);
        }
        
        System.out.println("Task Queue (by priority):");
        for (Map.Entry<Integer, String> entry : taskQueue.getAllEntries()) {
            System.out.printf("Priority %d: %s\n", entry.getKey(), entry.getValue());
        }
        
        // Process high-priority tasks (priority <= 3)
        System.out.println("\nHigh-priority tasks to process first:");
        for (Map.Entry<Integer, String> entry : taskQueue.rangeQuery(1, 3)) {
            System.out.printf("  Processing: %s (Priority %d)\n", entry.getValue(), entry.getKey());
        }
        
        System.out.println("\nSkip List Applications Summary:");
        System.out.println("- Redis Sorted Sets (ZSET commands)");
        System.out.println("- Concurrent data structures in Java");
        System.out.println("- Database indexing systems");
        System.out.println("- Priority queues and schedulers");
        System.out.println("- Time-series data storage");
        System.out.println("- Distributed systems coordination");
    }
}