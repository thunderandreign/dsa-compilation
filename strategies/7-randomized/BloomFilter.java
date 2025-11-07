/**
 * Bloom Filter - Advanced Probabilistic Set Membership Implementation
 * 
 * This implementation demonstrates the Bloom Filter, a space-efficient probabilistic 
 * data structure used to test whether an element is a member of a set. It can have 
 * false positives but never false negatives.
 * 
 * Key Concepts:
 * - Space Efficiency: Uses much less space than storing actual elements
 * - Probabilistic Nature: False positives possible, false negatives impossible
 * - Hash Functions: Multiple independent hash functions for element mapping
 * - Trade-offs: Space vs accuracy, configurable false positive rates
 * 
 * Time Complexity:
 * - Insert: O(k) where k is the number of hash functions
 * - Query: O(k) where k is the number of hash functions
 * - Space: O(m) where m is the size of the bit array
 * 
 * Applications:
 * - Web caching and CDNs (avoid expensive disk lookups)
 * - Database query optimization (bloom joins)
 * - Distributed systems (reduce network calls)
 * - Spell checkers and blacklist filtering
 * - Cryptocurrency and blockchain systems
 */

import java.util.*;
import java.nio.charset.StandardCharsets;
import java.security.MessageDigest;
import java.security.NoSuchAlgorithmException;

public class BloomFilter {
    
    static class BloomFilterConfig {
        int expectedElements;
        double falsePositiveRate;
        int optimalBitArraySize;
        int optimalHashFunctions;
        
        BloomFilterConfig(int expectedElements, double falsePositiveRate) {
            this.expectedElements = expectedElements;
            this.falsePositiveRate = falsePositiveRate;
            
            // Calculate optimal parameters
            // m = -(n * ln(p)) / (ln(2)^2) where n=elements, p=false positive rate
            this.optimalBitArraySize = (int) Math.ceil(
                -(expectedElements * Math.log(falsePositiveRate)) / Math.pow(Math.log(2), 2));
            
            // k = (m/n) * ln(2) where m=bit array size, n=elements
            this.optimalHashFunctions = (int) Math.ceil(
                ((double) optimalBitArraySize / expectedElements) * Math.log(2));
        }
        
        @Override
        public String toString() {
            return String.format("Expected: %d elements, FP Rate: %.4f, Bits: %d, Hash Functions: %d",
                               expectedElements, falsePositiveRate, optimalBitArraySize, optimalHashFunctions);
        }
    }
    
    static class BloomFilterStats {
        int totalInserts;
        int totalQueries;
        int truePositives;
        int falsePositives;
        int trueNegatives;
        double actualFalsePositiveRate;
        double fillRatio;
        
        void updateStats(int inserts, int queries, int tp, int fp, int tn, double fillRatio) {
            this.totalInserts = inserts;
            this.totalQueries = queries;
            this.truePositives = tp;
            this.falsePositives = fp;
            this.trueNegatives = tn;
            this.actualFalsePositiveRate = queries > 0 ? (double) fp / (fp + tn) : 0.0;
            this.fillRatio = fillRatio;
        }
        
        @Override
        public String toString() {
            return String.format("Inserts: %d, Queries: %d, TP: %d, FP: %d, TN: %d, " +
                               "Actual FP Rate: %.4f, Fill Ratio: %.3f",
                               totalInserts, totalQueries, truePositives, falsePositives, 
                               trueNegatives, actualFalsePositiveRate, fillRatio);
        }
    }
    
    static class HashFunction {
        private int seed;
        private MessageDigest md5;
        
        HashFunction(int seed) {
            this.seed = seed;
            try {
                this.md5 = MessageDigest.getInstance("MD5");
            } catch (NoSuchAlgorithmException e) {
                throw new RuntimeException("MD5 not available", e);
            }
        }
        
        int hash(String element, int arraySize) {
            try {
                md5.reset();
                md5.update((element + seed).getBytes(StandardCharsets.UTF_8));
                byte[] digest = md5.digest();
                
                // Convert first 4 bytes to int
                int hash = 0;
                for (int i = 0; i < 4; i++) {
                    hash = (hash << 8) | (digest[i] & 0xFF);
                }
                
                return Math.abs(hash) % arraySize;
            } catch (Exception e) {
                // Fallback to simple hash
                return Math.abs((element + seed).hashCode()) % arraySize;
            }
        }
    }
    
    private BitSet bitArray;
    private int bitArraySize;
    private int numHashFunctions;
    private List<HashFunction> hashFunctions;
    private int insertCount;
    private boolean verbose;
    private List<String> operations;
    private BloomFilterConfig config;
    
    public BloomFilter(int expectedElements, double falsePositiveRate, boolean verbose) {
        this.config = new BloomFilterConfig(expectedElements, falsePositiveRate);
        this.bitArraySize = config.optimalBitArraySize;
        this.numHashFunctions = config.optimalHashFunctions;
        this.bitArray = new BitSet(bitArraySize);
        this.insertCount = 0;
        this.verbose = verbose;
        this.operations = new ArrayList<>();
        
        // Initialize hash functions with different seeds
        this.hashFunctions = new ArrayList<>();
        for (int i = 0; i < numHashFunctions; i++) {
            hashFunctions.add(new HashFunction(i * 31 + 17));
        }
        
        if (verbose) {
            operations.add("=== Bloom Filter Initialized ===");
            operations.add(config.toString());
        }
    }
    
    public BloomFilter(int bitArraySize, int numHashFunctions, boolean verbose) {
        this.bitArraySize = bitArraySize;
        this.numHashFunctions = numHashFunctions;
        this.bitArray = new BitSet(bitArraySize);
        this.insertCount = 0;
        this.verbose = verbose;
        this.operations = new ArrayList<>();
        
        // Initialize hash functions
        this.hashFunctions = new ArrayList<>();
        for (int i = 0; i < numHashFunctions; i++) {
            hashFunctions.add(new HashFunction(i * 31 + 17));
        }
        
        if (verbose) {
            operations.add("=== Bloom Filter Initialized (Custom) ===");
            operations.add(String.format("Bit Array Size: %d, Hash Functions: %d", 
                          bitArraySize, numHashFunctions));
        }
    }
    
    /**
     * Add an element to the bloom filter
     */
    public void add(String element) {
        if (verbose) {
            operations.add(String.format("=== Adding element: %s ===", element));
        }
        
        for (int i = 0; i < numHashFunctions; i++) {
            int hash = hashFunctions.get(i).hash(element, bitArraySize);
            bitArray.set(hash);
            
            if (verbose) {
                operations.add(String.format("Hash function %d: %s -> bit %d", i, element, hash));
            }
        }
        
        insertCount++;
        
        if (verbose) {
            operations.add(String.format("Element added. Total insertions: %d", insertCount));
        }
    }
    
    /**
     * Test if an element might be in the set
     */
    public boolean mightContain(String element) {
        if (verbose) {
            operations.add(String.format("=== Checking element: %s ===", element));
        }
        
        for (int i = 0; i < numHashFunctions; i++) {
            int hash = hashFunctions.get(i).hash(element, bitArraySize);
            
            if (!bitArray.get(hash)) {
                if (verbose) {
                    operations.add(String.format("Hash function %d: %s -> bit %d (NOT SET)", 
                                  i, element, hash));
                    operations.add("Result: DEFINITELY NOT in set");
                }
                return false; // Definitely not in the set
            }
            
            if (verbose) {
                operations.add(String.format("Hash function %d: %s -> bit %d (SET)", 
                              i, element, hash));
            }
        }
        
        if (verbose) {
            operations.add("Result: MIGHT be in set (all bits set)");
        }
        
        return true; // Might be in the set
    }
    
    /**
     * Get the current fill ratio of the bit array
     */
    public double getFillRatio() {
        return (double) bitArray.cardinality() / bitArraySize;
    }
    
    /**
     * Calculate the expected false positive rate based on current state
     */
    public double getExpectedFalsePositiveRate() {
        double fillRatio = getFillRatio();
        return Math.pow(fillRatio, numHashFunctions);
    }
    
    /**
     * Clear all bits in the bloom filter
     */
    public void clear() {
        bitArray.clear();
        insertCount = 0;
        
        if (verbose) {
            operations.add("=== Bloom Filter Cleared ===");
        }
    }
    
    /**
     * Create a union of this bloom filter with another
     */
    public BloomFilter union(BloomFilter other) {
        if (this.bitArraySize != other.bitArraySize || 
            this.numHashFunctions != other.numHashFunctions) {
            throw new IllegalArgumentException("Bloom filters must have same parameters for union");
        }
        
        BloomFilter result = new BloomFilter(this.bitArraySize, this.numHashFunctions, verbose);
        result.bitArray.or(this.bitArray);
        result.bitArray.or(other.bitArray);
        result.insertCount = this.insertCount + other.insertCount; // Approximate
        
        if (verbose) {
            operations.add("=== Bloom Filter Union Created ===");
        }
        
        return result;
    }
    
    /**
     * Create an intersection of this bloom filter with another
     */
    public BloomFilter intersection(BloomFilter other) {
        if (this.bitArraySize != other.bitArraySize || 
            this.numHashFunctions != other.numHashFunctions) {
            throw new IllegalArgumentException("Bloom filters must have same parameters for intersection");
        }
        
        BloomFilter result = new BloomFilter(this.bitArraySize, this.numHashFunctions, verbose);
        result.bitArray.or(this.bitArray);
        result.bitArray.and(other.bitArray);
        result.insertCount = Math.min(this.insertCount, other.insertCount); // Conservative estimate
        
        if (verbose) {
            operations.add("=== Bloom Filter Intersection Created ===");
        }
        
        return result;
    }
    
    /**
     * Estimate the number of elements in the bloom filter
     */
    public int estimateElementCount() {
        double fillRatio = getFillRatio();
        if (fillRatio == 0) return 0;
        if (fillRatio >= 1) return Integer.MAX_VALUE; // Overflow case
        
        // n ≈ -(m/k) * ln(1 - X) where X is the fraction of set bits
        double estimate = -(bitArraySize / (double) numHashFunctions) * Math.log(1 - fillRatio);
        return (int) Math.round(estimate);
    }
    
    /**
     * Serialize bloom filter to byte array
     */
    public byte[] serialize() {
        return bitArray.toByteArray();
    }
    
    /**
     * Deserialize bloom filter from byte array
     */
    public static BloomFilter deserialize(byte[] data, int bitArraySize, int numHashFunctions, boolean verbose) {
        BloomFilter filter = new BloomFilter(bitArraySize, numHashFunctions, verbose);
        filter.bitArray = BitSet.valueOf(data);
        return filter;
    }
    
    /**
     * Get detailed statistics about the bloom filter
     */
    public BloomFilterStats getStatistics() {
        BloomFilterStats stats = new BloomFilterStats();
        stats.totalInserts = insertCount;
        stats.fillRatio = getFillRatio();
        // Other stats need to be computed externally with test data
        return stats;
    }
    
    /**
     * Display bloom filter structure (for small filters)
     */
    public void display() {
        if (bitArraySize > 100) {
            System.out.printf("Bloom Filter: %d bits, %d hash functions, %d/%d bits set (%.3f%% full)\n",
                             bitArraySize, numHashFunctions, bitArray.cardinality(), 
                             bitArraySize, getFillRatio() * 100);
            return;
        }
        
        System.out.println("=== Bloom Filter Bit Array ===");
        System.out.printf("Size: %d bits, Hash Functions: %d, Fill Ratio: %.3f\n", 
                         bitArraySize, numHashFunctions, getFillRatio());
        
        // Display bit array in groups of 10
        for (int i = 0; i < bitArraySize; i++) {
            if (i % 10 == 0) {
                System.out.printf("\nBits %3d-%3d: ", i, Math.min(i + 9, bitArraySize - 1));
            }
            System.out.print(bitArray.get(i) ? "1" : "0");
        }
        System.out.println("\n");
    }
    
    public int size() {
        return insertCount;
    }
    
    public int getBitArraySize() {
        return bitArraySize;
    }
    
    public int getNumHashFunctions() {
        return numHashFunctions;
    }
    
    public List<String> getOperations() {
        return new ArrayList<>(operations);
    }
    
    public void clearOperations() {
        operations.clear();
    }
    
    public static void main(String[] args) {
        System.out.println("=== Bloom Filter - Comprehensive Analysis ===\n");
        
        // Test case 1: Basic operations with step-by-step demonstration
        System.out.println("Test Case 1: Basic Operations");
        BloomFilter basicFilter = new BloomFilter(100, 0.01, true);
        
        // Add some elements
        String[] elements = {"apple", "banana", "cherry", "date", "elderberry"};
        
        System.out.println("Adding elements to Bloom Filter:");
        for (String element : elements) {
            basicFilter.add(element);
            System.out.println("\nOperations for add(\"" + element + "\"):");
            for (String op : basicFilter.getOperations()) {
                System.out.println("  " + op);
            }
            basicFilter.clearOperations();
        }
        
        basicFilter.display();
        
        // Test membership
        String[] testElements = {"apple", "grape", "cherry", "kiwi", "date"};
        
        System.out.println("Testing membership:");
        for (String element : testElements) {
            boolean result = basicFilter.mightContain(element);
            System.out.printf("mightContain(\"%s\"): %s\n", element, result);
            
            if (element.equals("apple")) {
                System.out.println("Detailed operations:");
                for (String op : basicFilter.getOperations()) {
                    System.out.println("  " + op);
                }
                basicFilter.clearOperations();
            }
        }
        
        // Test case 2: False positive rate analysis
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 2: False Positive Rate Analysis");
        
        double[] targetRates = {0.1, 0.01, 0.001};
        int testElements = 1000;
        
        for (double targetRate : targetRates) {
            System.out.printf("\nTesting with target false positive rate: %.3f\n", targetRate);
            
            BloomFilter fpFilter = new BloomFilter(testElements, targetRate, false);
            Set<String> actualElements = new HashSet<>();
            
            // Add elements to both bloom filter and actual set
            Random random = new Random(42);
            for (int i = 0; i < testElements; i++) {
                String element = "element_" + i;
                fpFilter.add(element);
                actualElements.add(element);
            }
            
            // Test with elements not in the set
            int falsePositives = 0;
            int trueNegatives = 0;
            int testCount = 10000;
            
            for (int i = 0; i < testCount; i++) {
                String testElement = "test_" + random.nextInt(50000);
                
                if (!actualElements.contains(testElement)) {
                    if (fpFilter.mightContain(testElement)) {
                        falsePositives++;
                    } else {
                        trueNegatives++;
                    }
                }
            }
            
            double actualFPRate = (double) falsePositives / (falsePositives + trueNegatives);
            double expectedFPRate = fpFilter.getExpectedFalsePositiveRate();
            
            System.out.printf("Elements added: %d\n", testElements);
            System.out.printf("Fill ratio: %.3f\n", fpFilter.getFillRatio());
            System.out.printf("Target FP rate: %.4f\n", targetRate);
            System.out.printf("Expected FP rate: %.4f\n", expectedFPRate);
            System.out.printf("Actual FP rate: %.4f (%d/%d)\n", 
                             actualFPRate, falsePositives, falsePositives + trueNegatives);
            System.out.printf("Accuracy: %.2f%%\n", 
                             (1 - Math.abs(actualFPRate - targetRate) / targetRate) * 100);
        }
        
        // Test case 3: Memory efficiency comparison
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 3: Memory Efficiency Analysis");
        
        int[] elementCounts = {1000, 10000, 100000};
        
        System.out.printf("%-10s | %-15s | %-15s | %-15s | %-10s\n", 
                         "Elements", "Bloom Filter", "HashSet", "Ratio", "Space Saved");
        System.out.println("-".repeat(75));
        
        for (int elementCount : elementCounts) {
            // Bloom filter memory usage
            BloomFilter memFilter = new BloomFilter(elementCount, 0.01, false);
            int bloomBits = memFilter.getBitArraySize();
            int bloomBytes = bloomBits / 8 + (bloomBits % 8 == 0 ? 0 : 1);
            
            // Estimate HashSet memory usage (rough approximation)
            // HashSet overhead + string storage + hash table overhead
            int avgStringLength = 10; // Assume average string length
            int hashSetBytes = elementCount * (avgStringLength + 48); // 48 bytes overhead per entry
            
            double ratio = (double) hashSetBytes / bloomBytes;
            double spaceSaved = (1.0 - (double) bloomBytes / hashSetBytes) * 100;
            
            System.out.printf("%-10d | %-15s | %-15s | %-15.1fx | %-10.1f%%\n",
                             elementCount,
                             formatBytes(bloomBytes),
                             formatBytes(hashSetBytes),
                             ratio,
                             spaceSaved);
        }
        
        // Test case 4: Set operations (union/intersection)
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 4: Set Operations");
        
        BloomFilter set1 = new BloomFilter(50, 0.1, true);
        BloomFilter set2 = new BloomFilter(50, 0.1, false);
        
        // Add elements to first set
        String[] set1Elements = {"a", "b", "c", "d", "e"};
        for (String element : set1Elements) {
            set1.add(element);
        }
        
        // Add elements to second set (some overlap)
        String[] set2Elements = {"d", "e", "f", "g", "h"};
        for (String element : set2Elements) {
            set2.add(element);
        }
        
        System.out.println("Set 1 contains: " + Arrays.toString(set1Elements));
        System.out.println("Set 2 contains: " + Arrays.toString(set2Elements));
        
        set1.display();
        set2.display();
        
        // Union operation
        BloomFilter unionSet = set1.union(set2);
        System.out.println("Union of Set 1 and Set 2:");
        unionSet.display();
        
        // Test union membership
        String[] unionTestElements = {"a", "c", "f", "h", "x", "z"};
        System.out.println("Testing union membership:");
        for (String element : unionTestElements) {
            boolean inUnion = unionSet.mightContain(element);
            boolean inSet1 = set1.mightContain(element);
            boolean inSet2 = set2.mightContain(element);
            System.out.printf("\"%s\": Union=%s, Set1=%s, Set2=%s\n", 
                             element, inUnion, inSet1, inSet2);
        }
        
        // Intersection operation
        BloomFilter intersectionSet = set1.intersection(set2);
        System.out.println("\nIntersection of Set 1 and Set 2:");
        intersectionSet.display();
        
        // Test case 5: Performance benchmarking
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 5: Performance Benchmarking");
        
        performanceBenchmark();
        
        // Test case 6: Real-world applications
        System.out.println("\n" + "=".repeat(60));
        System.out.println("Test Case 6: Real-World Applications");
        
        demonstrateRealWorldApplications();
        
        System.out.println("\n=== Bloom Filter Analysis Summary ===");
        System.out.println("Key Properties:");
        System.out.println("- Space-efficient probabilistic set membership testing");
        System.out.println("- False positives possible, false negatives impossible");
        System.out.println("- Optimal parameters depend on expected elements and desired FP rate");
        System.out.println("- Memory usage is independent of element size");
        
        System.out.println("\nTrade-offs:");
        System.out.println("- Space efficiency vs accuracy");
        System.out.println("- Cannot delete elements (variants like Counting Bloom Filters exist)");
        System.out.println("- Hash function quality affects performance");
        System.out.println("- Fill ratio affects false positive rate exponentially");
    }
    
    private static String formatBytes(int bytes) {
        if (bytes < 1024) return bytes + " B";
        if (bytes < 1024 * 1024) return String.format("%.1f KB", bytes / 1024.0);
        return String.format("%.1f MB", bytes / (1024.0 * 1024.0));
    }
    
    private static void performanceBenchmark() {
        System.out.println("Comparing Bloom Filter vs HashSet performance:");
        
        int[] sizes = {10000, 100000, 1000000};
        
        System.out.printf("%-10s | %-15s | %-15s | %-15s | %-15s\n", 
                         "Size", "Bloom Add (ms)", "HashSet Add", "Bloom Query", "HashSet Query");
        System.out.println("-".repeat(80));
        
        for (int size : sizes) {
            // Bloom Filter benchmark
            BloomFilter bloomFilter = new BloomFilter(size, 0.01, false);
            long bloomAddTime = System.nanoTime();
            
            for (int i = 0; i < size; i++) {
                bloomFilter.add("element_" + i);
            }
            
            bloomAddTime = System.nanoTime() - bloomAddTime;
            
            // Query benchmark
            long bloomQueryTime = System.nanoTime();
            for (int i = 0; i < 10000; i++) {
                bloomFilter.mightContain("query_" + i);
            }
            bloomQueryTime = System.nanoTime() - bloomQueryTime;
            
            // HashSet benchmark
            Set<String> hashSet = new HashSet<>();
            long hashAddTime = System.nanoTime();
            
            for (int i = 0; i < size; i++) {
                hashSet.add("element_" + i);
            }
            
            hashAddTime = System.nanoTime() - hashAddTime;
            
            // HashSet query benchmark
            long hashQueryTime = System.nanoTime();
            for (int i = 0; i < 10000; i++) {
                hashSet.contains("query_" + i);
            }
            hashQueryTime = System.nanoTime() - hashQueryTime;
            
            System.out.printf("%-10d | %-15.1f | %-15.1f | %-15.1f | %-15.1f\n",
                             size,
                             bloomAddTime / 1_000_000.0,
                             hashAddTime / 1_000_000.0,
                             bloomQueryTime / 1_000_000.0,
                             hashQueryTime / 1_000_000.0);
        }
    }
    
    private static void demonstrateRealWorldApplications() {
        // Application 1: Web crawler URL deduplication
        System.out.println("\n1. Web Crawler URL Deduplication");
        BloomFilter urlFilter = new BloomFilter(1000000, 0.001, false);
        
        // Simulate crawling URLs
        String[] domains = {"example.com", "test.org", "demo.net", "sample.io"};
        Random random = new Random(42);
        
        System.out.println("Simulating web crawler with 100,000 URLs...");
        Set<String> actualUrls = new HashSet<>(); // For verification
        
        for (int i = 0; i < 100000; i++) {
            String domain = domains[random.nextInt(domains.length)];
            String path = "/page" + random.nextInt(10000);
            String url = "https://" + domain + path;
            
            urlFilter.add(url);
            actualUrls.add(url);
        }
        
        // Test duplicate detection
        int duplicateTests = 10000;
        int correctSkips = 0;
        int falsePositives = 0;
        
        for (int i = 0; i < duplicateTests; i++) {
            String domain = domains[random.nextInt(domains.length)];
            String path = "/newpage" + i;
            String testUrl = "https://" + domain + path;
            
            boolean bloomSays = urlFilter.mightContain(testUrl);
            boolean actuallyExists = actualUrls.contains(testUrl);
            
            if (bloomSays && !actuallyExists) {
                falsePositives++;
            } else if (!bloomSays && !actuallyExists) {
                correctSkips++;
            }
        }
        
        System.out.printf("URLs processed: %d\n", actualUrls.size());
        System.out.printf("Bloom filter size: %s\n", formatBytes(urlFilter.getBitArraySize() / 8));
        System.out.printf("Fill ratio: %.3f\n", urlFilter.getFillRatio());
        System.out.printf("Correct skips: %d/%d (%.2f%%)\n", 
                         correctSkips, duplicateTests, (double) correctSkips / duplicateTests * 100);
        System.out.printf("False positives: %d (%.3f%%)\n", 
                         falsePositives, (double) falsePositives / duplicateTests * 100);
        
        // Application 2: Database query optimization
        System.out.println("\n2. Database Query Optimization (Bloom Joins)");
        
        // Simulate large table join optimization
        BloomFilter tableFilter = new BloomFilter(50000, 0.01, false);
        
        // "Small" table keys (would be used to build bloom filter)
        Set<Integer> smallTableKeys = new HashSet<>();
        for (int i = 0; i < 5000; i++) {
            int key = random.nextInt(100000);
            smallTableKeys.add(key);
            tableFilter.add("key_" + key);
        }
        
        // Simulate large table scan
        int largeTableRows = 100000;
        int potentialJoins = 0;
        int actualJoins = 0;
        int savedScans = 0;
        
        for (int i = 0; i < largeTableRows; i++) {
            int largeTableKey = random.nextInt(150000);
            String keyStr = "key_" + largeTableKey;
            
            // Bloom filter check (fast)
            if (tableFilter.mightContain(keyStr)) {
                potentialJoins++;
                // Would perform expensive join operation here
                if (smallTableKeys.contains(largeTableKey)) {
                    actualJoins++;
                }
            } else {
                savedScans++;
                // Skip expensive join - bloom filter says definitely not present
            }
        }
        
        System.out.printf("Large table rows: %d\n", largeTableRows);
        System.out.printf("Small table keys: %d\n", smallTableKeys.size());
        System.out.printf("Potential joins (bloom filter positive): %d\n", potentialJoins);
        System.out.printf("Actual joins: %d\n", actualJoins);
        System.out.printf("Scans saved by bloom filter: %d (%.2f%%)\n", 
                         savedScans, (double) savedScans / largeTableRows * 100);
        System.out.printf("False positive joins: %d\n", potentialJoins - actualJoins);
        
        // Application 3: Distributed cache optimization
        System.out.println("\n3. Distributed Cache Optimization");
        
        // Simulate multiple cache nodes with bloom filters
        BloomFilter[] cacheBloomFilters = new BloomFilter[3];
        Set<String>[] actualCaches = new Set[3];
        
        for (int i = 0; i < 3; i++) {
            cacheBloomFilters[i] = new BloomFilter(10000, 0.01, false);
            actualCaches[i] = new HashSet<>();
        }
        
        // Populate caches with different data
        for (int cache = 0; cache < 3; cache++) {
            for (int i = 0; i < 3000; i++) {
                String key = "cache" + cache + "_key" + random.nextInt(5000);
                cacheBloomFilters[cache].add(key);
                actualCaches[cache].add(key);
            }
        }
        
        // Simulate cache queries
        int queries = 10000;
        int networkCallsSaved = 0;
        int unnecessaryNetworkCalls = 0;
        
        for (int q = 0; q < queries; q++) {
            String queryKey = "query_key" + random.nextInt(20000);
            
            for (int cache = 0; cache < 3; cache++) {
                boolean bloomSays = cacheBloomFilters[cache].mightContain(queryKey);
                boolean actuallyInCache = actualCaches[cache].contains(queryKey);
                
                if (!bloomSays) {
                    // Bloom filter says definitely not in cache - skip network call
                    networkCallsSaved++;
                } else if (bloomSays && !actuallyInCache) {
                    // False positive - unnecessary network call
                    unnecessaryNetworkCalls++;
                }
                // If bloomSays && actuallyInCache - necessary network call (correct positive)
            }
        }
        
        System.out.printf("Cache queries: %d × 3 nodes = %d total checks\n", queries, queries * 3);
        System.out.printf("Network calls saved: %d\n", networkCallsSaved);
        System.out.printf("Unnecessary network calls (false positives): %d\n", unnecessaryNetworkCalls);
        System.out.printf("Network efficiency: %.2f%% calls avoided\n", 
                         (double) networkCallsSaved / (queries * 3) * 100);
        
        System.out.println("\nBloom Filter Applications Summary:");
        System.out.println("- Web crawling and URL deduplication");
        System.out.println("- Database join optimization (bloom joins)");
        System.out.println("- Distributed caching systems");
        System.out.println("- Spell checkers and content filtering");
        System.out.println("- Bitcoin and blockchain transaction validation");
        System.out.println("- CDN cache optimization");
        System.out.println("- Malware detection and security filtering");
    }
}