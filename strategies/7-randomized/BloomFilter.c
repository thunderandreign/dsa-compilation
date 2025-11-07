/**
 * Bloom Filter - Advanced Probabilistic Set Membership C Implementation
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define MAX_HASH_FUNCTIONS 20
#define MAX_OPERATIONS 1000

typedef struct {
    int expected_elements;
    double false_positive_rate;
    int optimal_bit_array_size;
    int optimal_hash_functions;
} BloomFilterConfig;

typedef struct {
    int total_inserts;
    int total_queries;
    int true_positives;
    int false_positives;
    int true_negatives;
    double actual_false_positive_rate;
    double fill_ratio;
} BloomFilterStats;

typedef struct {
    unsigned char* bit_array;
    int bit_array_size;
    int num_hash_functions;
    int insert_count;
    bool verbose;
    char operations[MAX_OPERATIONS][200];
    int operation_count;
    BloomFilterConfig config;
} BloomFilter;

// Hash function seeds for multiple independent hash functions
static const int HASH_SEEDS[MAX_HASH_FUNCTIONS] = {
    17, 31, 53, 97, 193, 389, 769, 1543, 3079, 6151,
    12289, 24593, 49157, 98317, 196613, 393241, 786433, 1572869, 3145739, 6291469
};

// Utility functions
void add_operation(BloomFilter* filter, const char* operation) {
    if (filter->verbose && filter->operation_count < MAX_OPERATIONS) {
        strncpy(filter->operations[filter->operation_count], operation, 199);
        filter->operations[filter->operation_count][199] = '\0';
        filter->operation_count++;
    }
}

void print_operations(const BloomFilter* filter) {
    for (int i = 0; i < filter->operation_count; i++) {
        printf("  %s\n", filter->operations[i]);
    }
}

void clear_operations(BloomFilter* filter) {
    filter->operation_count = 0;
}

// Simple hash function using djb2 algorithm with different seeds
unsigned int hash_function(const char* str, int seed, int array_size) {
    unsigned int hash = 5381 + seed;
    int c;
    
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    
    return hash % array_size;
}

// Calculate optimal bloom filter parameters
BloomFilterConfig calculate_optimal_parameters(int expected_elements, double false_positive_rate) {
    BloomFilterConfig config;
    config.expected_elements = expected_elements;
    config.false_positive_rate = false_positive_rate;
    
    // m = -(n * ln(p)) / (ln(2)^2) where n=elements, p=false positive rate
    config.optimal_bit_array_size = (int) ceil(
        -(expected_elements * log(false_positive_rate)) / pow(log(2), 2));
    
    // k = (m/n) * ln(2) where m=bit array size, n=elements
    config.optimal_hash_functions = (int) ceil(
        ((double) config.optimal_bit_array_size / expected_elements) * log(2));
    
    // Clamp to reasonable limits
    if (config.optimal_hash_functions > MAX_HASH_FUNCTIONS) {
        config.optimal_hash_functions = MAX_HASH_FUNCTIONS;
    }
    if (config.optimal_hash_functions < 1) {
        config.optimal_hash_functions = 1;
    }
    
    return config;
}

// Create a new bloom filter with optimal parameters
BloomFilter* create_bloom_filter(int expected_elements, double false_positive_rate, bool verbose) {
    BloomFilter* filter = malloc(sizeof(BloomFilter));
    
    filter->config = calculate_optimal_parameters(expected_elements, false_positive_rate);
    filter->bit_array_size = filter->config.optimal_bit_array_size;
    filter->num_hash_functions = filter->config.optimal_hash_functions;
    
    // Allocate bit array (using bytes, each byte stores 8 bits)
    int byte_array_size = (filter->bit_array_size + 7) / 8; // Round up division
    filter->bit_array = calloc(byte_array_size, sizeof(unsigned char));
    
    filter->insert_count = 0;
    filter->verbose = verbose;
    filter->operation_count = 0;
    
    if (verbose) {
        add_operation(filter, "=== Bloom Filter Initialized ===");
        char msg[200];
        sprintf(msg, "Expected: %d elements, FP Rate: %.4f, Bits: %d, Hash Functions: %d",
                expected_elements, false_positive_rate, 
                filter->bit_array_size, filter->num_hash_functions);
        add_operation(filter, msg);
    }
    
    return filter;
}

// Create a bloom filter with custom parameters
BloomFilter* create_bloom_filter_custom(int bit_array_size, int num_hash_functions, bool verbose) {
    BloomFilter* filter = malloc(sizeof(BloomFilter));
    
    filter->bit_array_size = bit_array_size;
    filter->num_hash_functions = num_hash_functions;
    
    if (num_hash_functions > MAX_HASH_FUNCTIONS) {
        num_hash_functions = MAX_HASH_FUNCTIONS;
        filter->num_hash_functions = MAX_HASH_FUNCTIONS;
    }
    
    // Allocate bit array
    int byte_array_size = (bit_array_size + 7) / 8;
    filter->bit_array = calloc(byte_array_size, sizeof(unsigned char));
    
    filter->insert_count = 0;
    filter->verbose = verbose;
    filter->operation_count = 0;
    
    if (verbose) {
        add_operation(filter, "=== Bloom Filter Initialized (Custom) ===");
        char msg[200];
        sprintf(msg, "Bit Array Size: %d, Hash Functions: %d", bit_array_size, num_hash_functions);
        add_operation(filter, msg);
    }
    
    return filter;
}

// Set a bit in the bit array
void set_bit(BloomFilter* filter, int bit_index) {
    int byte_index = bit_index / 8;
    int bit_offset = bit_index % 8;
    filter->bit_array[byte_index] |= (1 << bit_offset);
}

// Get a bit from the bit array
bool get_bit(const BloomFilter* filter, int bit_index) {
    int byte_index = bit_index / 8;
    int bit_offset = bit_index % 8;
    return (filter->bit_array[byte_index] & (1 << bit_offset)) != 0;
}

// Add an element to the bloom filter
void bloom_add(BloomFilter* filter, const char* element) {
    if (filter->verbose) {
        char msg[200];
        sprintf(msg, "=== Adding element: %s ===", element);
        add_operation(filter, msg);
    }
    
    for (int i = 0; i < filter->num_hash_functions; i++) {
        unsigned int hash = hash_function(element, HASH_SEEDS[i], filter->bit_array_size);
        set_bit(filter, hash);
        
        if (filter->verbose) {
            char msg[200];
            sprintf(msg, "Hash function %d: %s -> bit %u", i, element, hash);
            add_operation(filter, msg);
        }
    }
    
    filter->insert_count++;
    
    if (filter->verbose) {
        char msg[200];
        sprintf(msg, "Element added. Total insertions: %d", filter->insert_count);
        add_operation(filter, msg);
    }
}

// Test if an element might be in the set
bool bloom_might_contain(BloomFilter* filter, const char* element) {
    if (filter->verbose) {
        char msg[200];
        sprintf(msg, "=== Checking element: %s ===", element);
        add_operation(filter, msg);
    }
    
    for (int i = 0; i < filter->num_hash_functions; i++) {
        unsigned int hash = hash_function(element, HASH_SEEDS[i], filter->bit_array_size);
        
        if (!get_bit(filter, hash)) {
            if (filter->verbose) {
                char msg[200];
                sprintf(msg, "Hash function %d: %s -> bit %u (NOT SET)", i, element, hash);
                add_operation(filter, msg);
                add_operation(filter, "Result: DEFINITELY NOT in set");
            }
            return false; // Definitely not in the set
        }
        
        if (filter->verbose) {
            char msg[200];
            sprintf(msg, "Hash function %d: %s -> bit %u (SET)", i, element, hash);
            add_operation(filter, msg);
        }
    }
    
    if (filter->verbose) {
        add_operation(filter, "Result: MIGHT be in set (all bits set)");
    }
    
    return true; // Might be in the set
}

// Get the current fill ratio of the bit array
double get_fill_ratio(const BloomFilter* filter) {
    int set_bits = 0;
    
    for (int i = 0; i < filter->bit_array_size; i++) {
        if (get_bit(filter, i)) {
            set_bits++;
        }
    }
    
    return (double) set_bits / filter->bit_array_size;
}

// Calculate the expected false positive rate based on current state
double get_expected_false_positive_rate(const BloomFilter* filter) {
    double fill_ratio = get_fill_ratio(filter);
    return pow(fill_ratio, filter->num_hash_functions);
}

// Clear all bits in the bloom filter
void bloom_clear(BloomFilter* filter) {
    int byte_array_size = (filter->bit_array_size + 7) / 8;
    memset(filter->bit_array, 0, byte_array_size);
    filter->insert_count = 0;
    
    if (filter->verbose) {
        add_operation(filter, "=== Bloom Filter Cleared ===");
    }
}

// Create a union of two bloom filters
BloomFilter* bloom_union(const BloomFilter* filter1, const BloomFilter* filter2, bool verbose) {
    if (filter1->bit_array_size != filter2->bit_array_size || 
        filter1->num_hash_functions != filter2->num_hash_functions) {
        printf("Error: Bloom filters must have same parameters for union\n");
        return NULL;
    }
    
    BloomFilter* result = create_bloom_filter_custom(filter1->bit_array_size, 
                                                    filter1->num_hash_functions, verbose);
    
    int byte_array_size = (filter1->bit_array_size + 7) / 8;
    
    // Perform bitwise OR operation
    for (int i = 0; i < byte_array_size; i++) {
        result->bit_array[i] = filter1->bit_array[i] | filter2->bit_array[i];
    }
    
    result->insert_count = filter1->insert_count + filter2->insert_count; // Approximate
    
    if (verbose) {
        add_operation(result, "=== Bloom Filter Union Created ===");
    }
    
    return result;
}

// Create an intersection of two bloom filters
BloomFilter* bloom_intersection(const BloomFilter* filter1, const BloomFilter* filter2, bool verbose) {
    if (filter1->bit_array_size != filter2->bit_array_size || 
        filter1->num_hash_functions != filter2->num_hash_functions) {
        printf("Error: Bloom filters must have same parameters for intersection\n");
        return NULL;
    }
    
    BloomFilter* result = create_bloom_filter_custom(filter1->bit_array_size, 
                                                    filter1->num_hash_functions, verbose);
    
    int byte_array_size = (filter1->bit_array_size + 7) / 8;
    
    // Perform bitwise AND operation
    for (int i = 0; i < byte_array_size; i++) {
        result->bit_array[i] = filter1->bit_array[i] & filter2->bit_array[i];
    }
    
    result->insert_count = (filter1->insert_count < filter2->insert_count) ? 
                          filter1->insert_count : filter2->insert_count; // Conservative estimate
    
    if (verbose) {
        add_operation(result, "=== Bloom Filter Intersection Created ===");
    }
    
    return result;
}

// Estimate the number of elements in the bloom filter
int estimate_element_count(const BloomFilter* filter) {
    double fill_ratio = get_fill_ratio(filter);
    if (fill_ratio == 0.0) return 0;
    if (fill_ratio >= 1.0) return INT_MAX; // Overflow case
    
    // n ≈ -(m/k) * ln(1 - X) where X is the fraction of set bits
    double estimate = -((double) filter->bit_array_size / filter->num_hash_functions) * log(1 - fill_ratio);
    return (int) round(estimate);
}

// Display bloom filter structure (for small filters)
void bloom_display(const BloomFilter* filter) {
    if (filter->bit_array_size > 100) {
        int set_bits = 0;
        for (int i = 0; i < filter->bit_array_size; i++) {
            if (get_bit(filter, i)) set_bits++;
        }
        
        printf("Bloom Filter: %d bits, %d hash functions, %d/%d bits set (%.3f%% full)\n",
               filter->bit_array_size, filter->num_hash_functions, set_bits, 
               filter->bit_array_size, get_fill_ratio(filter) * 100);
        return;
    }
    
    printf("=== Bloom Filter Bit Array ===\n");
    printf("Size: %d bits, Hash Functions: %d, Fill Ratio: %.3f\n", 
           filter->bit_array_size, filter->num_hash_functions, get_fill_ratio(filter));
    
    // Display bit array in groups of 10
    for (int i = 0; i < filter->bit_array_size; i++) {
        if (i % 10 == 0) {
            printf("\nBits %3d-%3d: ", i, (i + 9 < filter->bit_array_size) ? i + 9 : filter->bit_array_size - 1);
        }
        printf("%c", get_bit(filter, i) ? '1' : '0');
    }
    printf("\n\n");
}

// Free bloom filter memory
void free_bloom_filter(BloomFilter* filter) {
    if (filter) {
        free(filter->bit_array);
        free(filter);
    }
}

// Format bytes for display
void format_bytes(int bytes, char* buffer) {
    if (bytes < 1024) {
        sprintf(buffer, "%d B", bytes);
    } else if (bytes < 1024 * 1024) {
        sprintf(buffer, "%.1f KB", bytes / 1024.0);
    } else {
        sprintf(buffer, "%.1f MB", bytes / (1024.0 * 1024.0));
    }
}

void performance_benchmark() {
    printf("Comparing Bloom Filter vs hash table performance:\n");
    
    int sizes[] = {10000, 100000, 500000};
    int num_sizes = sizeof(sizes) / sizeof(sizes[0]);
    
    printf("%-10s | %-15s | %-15s\n", "Size", "Bloom Add (ms)", "Bloom Query (ms)");
    printf("----------------------------------------\n");
    
    for (int s = 0; s < num_sizes; s++) {
        int size = sizes[s];
        
        // Bloom Filter benchmark
        BloomFilter* bloom_filter = create_bloom_filter(size, 0.01, false);
        
        clock_t bloom_add_start = clock();
        for (int i = 0; i < size; i++) {
            char element[50];
            sprintf(element, "element_%d", i);
            bloom_add(bloom_filter, element);
        }
        clock_t bloom_add_end = clock();
        
        // Query benchmark
        clock_t bloom_query_start = clock();
        for (int i = 0; i < 10000; i++) {
            char query[50];
            sprintf(query, "query_%d", i);
            bloom_might_contain(bloom_filter, query);
        }
        clock_t bloom_query_end = clock();
        
        double bloom_add_time = ((double)(bloom_add_end - bloom_add_start)) / CLOCKS_PER_SEC * 1000;
        double bloom_query_time = ((double)(bloom_query_end - bloom_query_start)) / CLOCKS_PER_SEC * 1000;
        
        printf("%-10d | %-15.1f | %-15.1f\n", size, bloom_add_time, bloom_query_time);
        
        free_bloom_filter(bloom_filter);
    }
}

void demonstrate_real_world_applications() {
    printf("\n=== Real-World Applications ===\n");
    
    // Application 1: Web crawler URL deduplication
    printf("\n1. Web Crawler URL Deduplication\n");
    BloomFilter* url_filter = create_bloom_filter(100000, 0.001, false);
    
    // Simulate crawling URLs
    const char* domains[] = {"example.com", "test.org", "demo.net", "sample.io"};
    int num_domains = sizeof(domains) / sizeof(domains[0]);
    
    printf("Simulating web crawler with 50,000 URLs...\n");
    
    // Add URLs to filter
    srand(42); // Fixed seed for reproducible results
    for (int i = 0; i < 50000; i++) {
        char url[200];
        const char* domain = domains[rand() % num_domains];
        int page = rand() % 10000;
        sprintf(url, "https://%s/page%d", domain, page);
        bloom_add(url_filter, url);
    }
    
    // Test duplicate detection
    int duplicate_tests = 10000;
    int bloom_positives = 0;
    
    for (int i = 0; i < duplicate_tests; i++) {
        char test_url[200];
        const char* domain = domains[rand() % num_domains];
        sprintf(test_url, "https://%s/newpage%d", domain, i);
        
        if (bloom_might_contain(url_filter, test_url)) {
            bloom_positives++;
        }
    }
    
    char size_str[50];
    int byte_size = (url_filter->bit_array_size + 7) / 8;
    format_bytes(byte_size, size_str);
    
    printf("URLs processed: 50,000\n");
    printf("Bloom filter size: %s\n", size_str);
    printf("Fill ratio: %.3f\n", get_fill_ratio(url_filter));
    printf("New URLs flagged as potential duplicates: %d (%.3f%%)\n", 
           bloom_positives, (double) bloom_positives / duplicate_tests * 100);
    
    free_bloom_filter(url_filter);
    
    // Application 2: Database query optimization
    printf("\n2. Database Query Optimization (Bloom Joins)\n");
    
    // Simulate large table join optimization
    BloomFilter* table_filter = create_bloom_filter(5000, 0.01, false);
    
    // "Small" table keys (build bloom filter)
    int small_table_keys[5000];
    for (int i = 0; i < 5000; i++) {
        small_table_keys[i] = rand() % 100000;
        char key_str[50];
        sprintf(key_str, "key_%d", small_table_keys[i]);
        bloom_add(table_filter, key_str);
    }
    
    // Simulate large table scan
    int large_table_rows = 100000;
    int potential_joins = 0;
    int saved_scans = 0;
    
    for (int i = 0; i < large_table_rows; i++) {
        int large_table_key = rand() % 150000;
        char key_str[50];
        sprintf(key_str, "key_%d", large_table_key);
        
        // Bloom filter check (fast)
        if (bloom_might_contain(table_filter, key_str)) {
            potential_joins++;
            // Would perform expensive join operation here
        } else {
            saved_scans++;
            // Skip expensive join - bloom filter says definitely not present
        }
    }
    
    printf("Large table rows: %d\n", large_table_rows);
    printf("Small table keys: 5,000\n");
    printf("Potential joins (bloom filter positive): %d\n", potential_joins);
    printf("Scans saved by bloom filter: %d (%.2f%%)\n", 
           saved_scans, (double) saved_scans / large_table_rows * 100);
    
    free_bloom_filter(table_filter);
    
    // Application 3: Spell checker
    printf("\n3. Spell Checker Application\n");
    
    BloomFilter* dictionary = create_bloom_filter(50000, 0.001, false);
    
    // Add common English words to dictionary
    const char* common_words[] = {
        "the", "be", "to", "of", "and", "a", "in", "that", "have", "i",
        "it", "for", "not", "on", "with", "he", "as", "you", "do", "at",
        "this", "but", "his", "by", "from", "they", "we", "say", "her", "she",
        "or", "an", "will", "my", "one", "all", "would", "there", "their",
        "what", "so", "up", "out", "if", "about", "who", "get", "which", "go", "me"
    };
    
    int num_common_words = sizeof(common_words) / sizeof(common_words[0]);
    
    // Add words to dictionary bloom filter
    for (int i = 0; i < num_common_words; i++) {
        bloom_add(dictionary, common_words[i]);
    }
    
    // Test spell checking
    const char* test_words[] = {"the", "hello", "wrold", "computer", "xyz", "and", "teh"};
    int num_test_words = sizeof(test_words) / sizeof(test_words[0]);
    
    printf("Dictionary contains %d common words\n", num_common_words);
    printf("Spell checking results:\n");
    
    for (int i = 0; i < num_test_words; i++) {
        bool might_be_correct = bloom_might_contain(dictionary, test_words[i]);
        printf("  '%s': %s\n", test_words[i], 
               might_be_correct ? "Might be correct" : "Definitely misspelled");
    }
    
    free_bloom_filter(dictionary);
    
    printf("\nBloom Filter Applications Summary:\n");
    printf("- Web crawling and URL deduplication\n");
    printf("- Database join optimization (bloom joins)\n");
    printf("- Distributed caching systems\n");
    printf("- Spell checkers and content filtering\n");
    printf("- Bitcoin and blockchain transaction validation\n");
    printf("- CDN cache optimization\n");
    printf("- Malware detection and security filtering\n");
}

int main() {
    printf("=== Bloom Filter - Comprehensive Analysis ===\n\n");
    
    srand(42); // Fixed seed for reproducible results
    
    // Test case 1: Basic operations with step-by-step demonstration
    printf("Test Case 1: Basic Operations\n");
    BloomFilter* basic_filter = create_bloom_filter(100, 0.01, true);
    
    // Add some elements
    const char* elements[] = {"apple", "banana", "cherry", "date", "elderberry"};
    int num_elements = sizeof(elements) / sizeof(elements[0]);
    
    printf("Adding elements to Bloom Filter:\n");
    for (int i = 0; i < num_elements; i++) {
        bloom_add(basic_filter, elements[i]);
        printf("\nOperations for add(\"%s\"):\n", elements[i]);
        print_operations(basic_filter);
        clear_operations(basic_filter);
    }
    
    bloom_display(basic_filter);
    
    // Test membership
    const char* test_elements[] = {"apple", "grape", "cherry", "kiwi", "date"};
    int num_test_elements = sizeof(test_elements) / sizeof(test_elements[0]);
    
    printf("Testing membership:\n");
    for (int i = 0; i < num_test_elements; i++) {
        bool result = bloom_might_contain(basic_filter, test_elements[i]);
        printf("mightContain(\"%s\"): %s\n", test_elements[i], result ? "true" : "false");
        
        if (strcmp(test_elements[i], "apple") == 0) {
            printf("Detailed operations:\n");
            print_operations(basic_filter);
            clear_operations(basic_filter);
        }
    }
    
    // Test case 2: False positive rate analysis
    printf("\n%s\n", "============================================================");
    printf("Test Case 2: False Positive Rate Analysis\n");
    
    double target_rates[] = {0.1, 0.01, 0.001};
    int num_rates = sizeof(target_rates) / sizeof(target_rates[0]);
    int test_elements_count = 1000;
    
    for (int r = 0; r < num_rates; r++) {
        double target_rate = target_rates[r];
        printf("\nTesting with target false positive rate: %.3f\n", target_rate);
        
        BloomFilter* fp_filter = create_bloom_filter(test_elements_count, target_rate, false);
        
        // Add elements to bloom filter
        for (int i = 0; i < test_elements_count; i++) {
            char element[50];
            sprintf(element, "element_%d", i);
            bloom_add(fp_filter, element);
        }
        
        // Test with elements not in the set
        int false_positives = 0;
        int true_negatives = 0;
        int test_count = 10000;
        
        for (int i = 0; i < test_count; i++) {
            char test_element[50];
            sprintf(test_element, "test_%d", rand() % 50000);
            
            if (bloom_might_contain(fp_filter, test_element)) {
                false_positives++;
            } else {
                true_negatives++;
            }
        }
        
        double actual_fp_rate = (double) false_positives / (false_positives + true_negatives);
        double expected_fp_rate = get_expected_false_positive_rate(fp_filter);
        
        printf("Elements added: %d\n", test_elements_count);
        printf("Fill ratio: %.3f\n", get_fill_ratio(fp_filter));
        printf("Target FP rate: %.4f\n", target_rate);
        printf("Expected FP rate: %.4f\n", expected_fp_rate);
        printf("Actual FP rate: %.4f (%d/%d)\n", 
               actual_fp_rate, false_positives, false_positives + true_negatives);
        printf("Accuracy: %.2f%%\n", 
               (1 - fabs(actual_fp_rate - target_rate) / target_rate) * 100);
        
        free_bloom_filter(fp_filter);
    }
    
    // Test case 3: Memory efficiency comparison
    printf("\n%s\n", "============================================================");
    printf("Test Case 3: Memory Efficiency Analysis\n");
    
    int element_counts[] = {1000, 10000, 100000};
    int num_counts = sizeof(element_counts) / sizeof(element_counts[0]);
    
    printf("%-10s | %-15s | %-15s | %-15s | %-10s\n", 
           "Elements", "Bloom Filter", "Hash Table", "Ratio", "Space Saved");
    printf("---------------------------------------------------------------------------\n");
    
    for (int i = 0; i < num_counts; i++) {
        int element_count = element_counts[i];
        
        // Bloom filter memory usage
        BloomFilter* mem_filter = create_bloom_filter(element_count, 0.01, false);
        int bloom_bytes = (mem_filter->bit_array_size + 7) / 8; // Convert bits to bytes
        
        // Estimate hash table memory usage (rough approximation)
        int avg_string_length = 10; // Assume average string length
        int hash_table_bytes = element_count * (avg_string_length + 8); // 8 bytes pointer overhead
        
        double ratio = (double) hash_table_bytes / bloom_bytes;
        double space_saved = (1.0 - (double) bloom_bytes / hash_table_bytes) * 100;
        
        char bloom_str[50], hash_str[50];
        format_bytes(bloom_bytes, bloom_str);
        format_bytes(hash_table_bytes, hash_str);
        
        printf("%-10d | %-15s | %-15s | %-15.1fx | %-10.1f%%\n",
               element_count, bloom_str, hash_str, ratio, space_saved);
        
        free_bloom_filter(mem_filter);
    }
    
    // Test case 4: Set operations (union/intersection)
    printf("\n%s\n", "============================================================");
    printf("Test Case 4: Set Operations\n");
    
    BloomFilter* set1 = create_bloom_filter(50, 0.1, true);
    BloomFilter* set2 = create_bloom_filter_custom(set1->bit_array_size, set1->num_hash_functions, false);
    
    // Add elements to first set
    const char* set1_elements[] = {"a", "b", "c", "d", "e"};
    int set1_size = sizeof(set1_elements) / sizeof(set1_elements[0]);
    
    for (int i = 0; i < set1_size; i++) {
        bloom_add(set1, set1_elements[i]);
    }
    
    // Add elements to second set (some overlap)
    const char* set2_elements[] = {"d", "e", "f", "g", "h"};
    int set2_size = sizeof(set2_elements) / sizeof(set2_elements[0]);
    
    for (int i = 0; i < set2_size; i++) {
        bloom_add(set2, set2_elements[i]);
    }
    
    printf("Set 1 contains: ");
    for (int i = 0; i < set1_size; i++) {
        printf("%s ", set1_elements[i]);
    }
    printf("\n");
    
    printf("Set 2 contains: ");
    for (int i = 0; i < set2_size; i++) {
        printf("%s ", set2_elements[i]);
    }
    printf("\n\n");
    
    bloom_display(set1);
    bloom_display(set2);
    
    // Union operation
    BloomFilter* union_set = bloom_union(set1, set2, false);
    printf("Union of Set 1 and Set 2:\n");
    bloom_display(union_set);
    
    // Test union membership
    const char* union_test_elements[] = {"a", "c", "f", "h", "x", "z"};
    int union_test_size = sizeof(union_test_elements) / sizeof(union_test_elements[0]);
    
    printf("Testing union membership:\n");
    for (int i = 0; i < union_test_size; i++) {
        bool in_union = bloom_might_contain(union_set, union_test_elements[i]);
        bool in_set1 = bloom_might_contain(set1, union_test_elements[i]);
        bool in_set2 = bloom_might_contain(set2, union_test_elements[i]);
        printf("\"%s\": Union=%s, Set1=%s, Set2=%s\n", 
               union_test_elements[i], 
               in_union ? "true" : "false",
               in_set1 ? "true" : "false", 
               in_set2 ? "true" : "false");
    }
    
    // Intersection operation
    BloomFilter* intersection_set = bloom_intersection(set1, set2, false);
    printf("\nIntersection of Set 1 and Set 2:\n");
    bloom_display(intersection_set);
    
    // Test case 5: Performance benchmarking
    printf("\n%s\n", "============================================================");
    printf("Test Case 5: Performance Benchmarking\n");
    
    performance_benchmark();
    
    // Clean up
    free_bloom_filter(basic_filter);
    free_bloom_filter(set1);
    free_bloom_filter(set2);
    free_bloom_filter(union_set);
    free_bloom_filter(intersection_set);
    
    printf("\n=== Bloom Filter Analysis Summary ===\n");
    printf("Key Properties:\n");
    printf("- Space-efficient probabilistic set membership testing\n");
    printf("- False positives possible, false negatives impossible\n");
    printf("- Optimal parameters depend on expected elements and desired FP rate\n");
    printf("- Memory usage is independent of element size\n");
    
    printf("\nTrade-offs:\n");
    printf("- Space efficiency vs accuracy\n");
    printf("- Cannot delete elements (variants like Counting Bloom Filters exist)\n");
    printf("- Hash function quality affects performance\n");
    printf("- Fill ratio affects false positive rate exponentially\n");
    
    demonstrate_real_world_applications();
    
    return 0;
}