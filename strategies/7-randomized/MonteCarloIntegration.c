/**
 * Monte Carlo Integration - Advanced C Implementation
 * 
 * This implementation demonstrates Monte Carlo methods for numerical integration,
 * showing how random sampling can solve complex mathematical problems that are
 * difficult or impossible to solve analytically.
 * 
 * Key Concepts:
 * - Random Sampling: Using random points to estimate integrals
 * - Law of Large Numbers: Convergence as sample size increases
 * - Statistical Estimation: Confidence intervals and error bounds
 * - Variance Reduction: Techniques to improve accuracy
 * 
 * Time Complexity: O(n) where n is the number of samples
 * Space Complexity: O(1) for basic integration, O(n) for importance sampling
 * 
 * Applications:
 * - Physics simulations (particle interactions, heat transfer)
 * - Financial modeling (option pricing, risk assessment)
 * - Computer graphics (global illumination, ray tracing)
 * - Machine learning (Bayesian inference, MCMC sampling)
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#define MAX_DIMENSIONS 10
#define MAX_SAMPLES 1000000
#define MAX_STEPS 1000

typedef struct {
    double estimate;
    double standard_error;
    double confidence_interval;
    int sample_count;
    long computation_time_ms;
} IntegrationResult;

typedef struct {
    int sample_sizes[20];
    double estimates[20];
    double errors[20];
    double standard_errors[20];
    int count;
} ConvergenceAnalysis;

typedef struct {
    char steps[MAX_STEPS][200];
    int step_count;
    bool verbose;
} StepTracker;

// Function pointer types
typedef double (*SingleVarFunction)(double x);
typedef double (*MultiVarFunction)(const double* point, int dimensions);
typedef double (*ImportanceSampler)(void);

// Global random state
static unsigned int random_seed = 1;

// Utility functions
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

void set_random_seed(unsigned int seed) {
    random_seed = seed;
    srand(seed);
}

// Simple linear congruential generator for consistent randomization
double uniform_random() {
    random_seed = (random_seed * 1103515245 + 12345) & 0x7fffffff;
    return (double)random_seed / 0x7fffffff;
}

// Box-Muller transform for Gaussian random numbers
double gaussian_random() {
    static bool has_spare = false;
    static double spare;
    
    if (has_spare) {
        has_spare = false;
        return spare;
    }
    
    has_spare = true;
    double u = uniform_random();
    double v = uniform_random();
    double mag = sqrt(-2.0 * log(u));
    spare = mag * cos(2.0 * M_PI * v);
    
    return mag * sin(2.0 * M_PI * v);
}

void print_result(const char* method, const IntegrationResult* result) {
    printf("%s: %.6f ± %.6f (95%% CI), Samples: %d, Time: %ldms\n",
           method, result->estimate, result->confidence_interval, 
           result->sample_count, result->computation_time_ms);
}

// Basic Monte Carlo integration for single-variable functions
IntegrationResult integrate_function(SingleVarFunction function, double lower_bound, 
                                   double upper_bound, int num_samples, StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Basic Monte Carlo Integration ===");
        char step[200];
        sprintf(step, "Integrating over [%.3f, %.3f] with %d samples", 
                lower_bound, upper_bound, num_samples);
        add_step(tracker, step);
    }
    
    clock_t start_time = clock();
    
    double sum = 0.0;
    double sum_squares = 0.0;
    double width = upper_bound - lower_bound;
    
    for (int i = 0; i < num_samples; i++) {
        double x = lower_bound + uniform_random() * width;
        double y = function(x);
        
        sum += y;
        sum_squares += y * y;
        
        if (tracker->verbose && i < 10) {
            char step[200];
            sprintf(step, "Sample %d: x=%.4f, f(x)=%.4f", i + 1, x, y);
            add_step(tracker, step);
        }
    }
    
    double mean = sum / num_samples;
    double variance = (sum_squares / num_samples) - (mean * mean);
    double standard_error = sqrt(variance / num_samples);
    
    double estimate = mean * width;
    double estimate_error = standard_error * width;
    double confidence_interval = 1.96 * estimate_error; // 95% confidence interval
    
    clock_t end_time = clock();
    long computation_time = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    if (tracker->verbose) {
        char step[200];
        sprintf(step, "Mean function value: %.6f", mean);
        add_step(tracker, step);
        sprintf(step, "Standard error: %.6f", standard_error);
        add_step(tracker, step);
        sprintf(step, "Final estimate: %.6f ± %.6f", estimate, confidence_interval);
        add_step(tracker, step);
    }
    
    IntegrationResult result = {estimate, estimate_error, confidence_interval, 
                               num_samples, computation_time};
    return result;
}

// Monte Carlo integration with hit-or-miss method
IntegrationResult integrate_hit_or_miss(SingleVarFunction function, double lower_bound,
                                       double upper_bound, double max_value, int num_samples,
                                       StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Hit-or-Miss Monte Carlo Integration ===");
        char step[200];
        sprintf(step, "Region: [%.3f, %.3f] × [0, %.3f]", 
                lower_bound, upper_bound, max_value);
        add_step(tracker, step);
    }
    
    clock_t start_time = clock();
    
    int hits = 0;
    double width = upper_bound - lower_bound;
    
    for (int i = 0; i < num_samples; i++) {
        double x = lower_bound + uniform_random() * width;
        double y = uniform_random() * max_value;
        
        if (y <= function(x)) {
            hits++;
        }
        
        if (tracker->verbose && i < 10) {
            bool hit = y <= function(x);
            char step[200];
            sprintf(step, "Sample %d: (%.4f, %.4f) -> %s", 
                    i + 1, x, y, hit ? "HIT" : "MISS");
            add_step(tracker, step);
        }
    }
    
    double hit_ratio = (double) hits / num_samples;
    double estimate = hit_ratio * width * max_value;
    
    // Binomial standard error
    double variance = hit_ratio * (1 - hit_ratio) / num_samples;
    double standard_error = sqrt(variance) * width * max_value;
    double confidence_interval = 1.96 * standard_error;
    
    clock_t end_time = clock();
    long computation_time = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    if (tracker->verbose) {
        char step[200];
        sprintf(step, "Hits: %d / %d (%.4f%%)", hits, num_samples, hit_ratio * 100);
        add_step(tracker, step);
        sprintf(step, "Estimate: %.6f ± %.6f", estimate, confidence_interval);
        add_step(tracker, step);
    }
    
    IntegrationResult result = {estimate, standard_error, confidence_interval, 
                               num_samples, computation_time};
    return result;
}

// Multidimensional Monte Carlo integration
IntegrationResult integrate_multidimensional(MultiVarFunction function, 
                                           const double* lower_bounds, 
                                           const double* upper_bounds,
                                           int dimensions, int num_samples, 
                                           StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Multidimensional Monte Carlo Integration ===");
        char step[200];
        sprintf(step, "Dimensions: %d, Samples: %d", dimensions, num_samples);
        add_step(tracker, step);
    }
    
    clock_t start_time = clock();
    
    double sum = 0.0;
    double sum_squares = 0.0;
    double volume = 1.0;
    
    // Calculate volume of integration region
    for (int i = 0; i < dimensions; i++) {
        volume *= (upper_bounds[i] - lower_bounds[i]);
    }
    
    double* point = malloc(dimensions * sizeof(double));
    
    for (int i = 0; i < num_samples; i++) {
        // Generate random point in the integration region
        for (int d = 0; d < dimensions; d++) {
            point[d] = lower_bounds[d] + uniform_random() * (upper_bounds[d] - lower_bounds[d]);
        }
        
        double value = function(point, dimensions);
        sum += value;
        sum_squares += value * value;
        
        if (tracker->verbose && i < 5) {
            char step[200];
            char point_str[100] = "[";
            for (int d = 0; d < dimensions; d++) {
                char temp[20];
                sprintf(temp, "%.3f", point[d]);
                strcat(point_str, temp);
                if (d < dimensions - 1) strcat(point_str, ", ");
            }
            strcat(point_str, "]");
            sprintf(step, "Sample %d: %s -> %.4f", i + 1, point_str, value);
            add_step(tracker, step);
        }
    }
    
    free(point);
    
    double mean = sum / num_samples;
    double variance = (sum_squares / num_samples) - (mean * mean);
    double standard_error = sqrt(variance / num_samples);
    
    double estimate = mean * volume;
    double estimate_error = standard_error * volume;
    double confidence_interval = 1.96 * estimate_error;
    
    clock_t end_time = clock();
    long computation_time = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    if (tracker->verbose) {
        char step[200];
        sprintf(step, "Integration volume: %.6f", volume);
        add_step(tracker, step);
        sprintf(step, "Mean function value: %.6f", mean);
        add_step(tracker, step);
        sprintf(step, "Final estimate: %.6f ± %.6f", estimate, confidence_interval);
        add_step(tracker, step);
    }
    
    IntegrationResult result = {estimate, estimate_error, confidence_interval, 
                               num_samples, computation_time};
    return result;
}

// Stratified sampling Monte Carlo integration
IntegrationResult integrate_stratified(SingleVarFunction function, double lower_bound,
                                     double upper_bound, int num_strata, 
                                     int samples_per_stratum, StepTracker* tracker) {
    if (tracker->verbose) {
        add_step(tracker, "=== Stratified Sampling Monte Carlo Integration ===");
        char step[200];
        sprintf(step, "Strata: %d, Samples per stratum: %d", 
                num_strata, samples_per_stratum);
        add_step(tracker, step);
    }
    
    clock_t start_time = clock();
    
    double total_sum = 0.0;
    double total_sum_squares = 0.0;
    double width = upper_bound - lower_bound;
    double stratum_width = width / num_strata;
    
    for (int s = 0; s < num_strata; s++) {
        double stratum_lower = lower_bound + s * stratum_width;
        double stratum_upper = stratum_lower + stratum_width;
        
        double stratum_sum = 0.0;
        for (int i = 0; i < samples_per_stratum; i++) {
            double x = stratum_lower + uniform_random() * stratum_width;
            double y = function(x);
            stratum_sum += y;
            
            if (tracker->verbose && s < 3 && i < 3) {
                char step[200];
                sprintf(step, "Stratum %d, Sample %d: x=%.4f, f(x)=%.4f", 
                        s + 1, i + 1, x, y);
                add_step(tracker, step);
            }
        }
        
        double stratum_mean = stratum_sum / samples_per_stratum;
        total_sum += stratum_mean;
        total_sum_squares += stratum_mean * stratum_mean;
    }
    
    double overall_mean = total_sum / num_strata;
    double variance = (total_sum_squares / num_strata) - (overall_mean * overall_mean);
    double standard_error = sqrt(variance / num_strata);
    
    double estimate = overall_mean * width;
    double estimate_error = standard_error * width;
    double confidence_interval = 1.96 * estimate_error;
    
    clock_t end_time = clock();
    long computation_time = ((end_time - start_time) * 1000) / CLOCKS_PER_SEC;
    
    int total_samples = num_strata * samples_per_stratum;
    
    if (tracker->verbose) {
        char step[200];
        sprintf(step, "Overall mean: %.6f", overall_mean);
        add_step(tracker, step);
        sprintf(step, "Final estimate: %.6f ± %.6f", estimate, confidence_interval);
        add_step(tracker, step);
    }
    
    IntegrationResult result = {estimate, estimate_error, confidence_interval, 
                               total_samples, computation_time};
    return result;
}

// Convergence analysis
ConvergenceAnalysis analyze_convergence(SingleVarFunction function, double lower_bound,
                                       double upper_bound, double actual_value,
                                       const int* sample_sizes, int num_sizes,
                                       StepTracker* tracker) {
    ConvergenceAnalysis analysis;
    analysis.count = 0;
    
    if (tracker->verbose) {
        add_step(tracker, "=== Convergence Analysis ===");
        char step[200];
        sprintf(step, "Actual integral value: %.6f", actual_value);
        add_step(tracker, step);
    }
    
    for (int i = 0; i < num_sizes && analysis.count < 20; i++) {
        int num_samples = sample_sizes[i];
        StepTracker dummy_tracker;
        init_step_tracker(&dummy_tracker, false);
        
        IntegrationResult result = integrate_function(function, lower_bound, upper_bound, 
                                                    num_samples, &dummy_tracker);
        
        analysis.sample_sizes[analysis.count] = num_samples;
        analysis.estimates[analysis.count] = result.estimate;
        analysis.errors[analysis.count] = fabs(result.estimate - actual_value);
        analysis.standard_errors[analysis.count] = result.standard_error;
        analysis.count++;
        
        if (tracker->verbose) {
            double error = fabs(result.estimate - actual_value);
            double error_percent = (error / fabs(actual_value)) * 100;
            char step[200];
            sprintf(step, "Samples: %d, Estimate: %.6f, Error: %.6f (%.2f%%)", 
                    num_samples, result.estimate, error, error_percent);
            add_step(tracker, step);
        }
    }
    
    return analysis;
}

// Test functions
double polynomial_function(double x) {
    return x * x; // x²
}

double sine_function(double x) {
    return sin(x);
}

double sqrt_function(double x) {
    return sqrt(x);
}

double gaussian_function(double x) {
    return exp(-x * x);
}

double multi_function(const double* point, int dimensions) {
    if (dimensions >= 2) {
        return point[0] * point[0] * point[1]; // x²y
    }
    return 0.0;
}

double sphere_indicator(const double* point, int dimensions) {
    double sum = 0.0;
    for (int i = 0; i < dimensions; i++) {
        sum += point[i] * point[i];
    }
    return sum <= 1.0 ? 1.0 : 0.0;
}

// Error function approximation
double erf_approx(double x) {
    double a1 =  0.254829592;
    double a2 = -0.284496736;
    double a3 =  1.421413741;
    double a4 = -1.453152027;
    double a5 =  1.061405429;
    double p  =  0.3275911;
    
    int sign = x >= 0 ? 1 : -1;
    x = fabs(x);
    
    double t = 1.0 / (1.0 + p * x);
    double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * exp(-x * x);
    
    return sign * y;
}

double normal_cdf(double x) {
    return 0.5 * (1 + erf_approx(x / sqrt(2)));
}

void demonstrate_practical_applications() {
    printf("\n=== Practical Applications ===\n");
    
    // Application 1: Option pricing (Black-Scholes approximation)
    printf("\n1. Financial Application: European Call Option Pricing\n");
    printf("Parameters: S₀=100, K=105, T=0.25, r=0.05, σ=0.2\n");
    
    double S0 = 100, K = 105, T = 0.25, r = 0.05, sigma = 0.2;
    int num_paths = 100000;
    
    set_random_seed(42);
    double payoff_sum = 0.0;
    
    for (int i = 0; i < num_paths; i++) {
        double Z = gaussian_random();
        double ST = S0 * exp((r - 0.5 * sigma * sigma) * T + sigma * sqrt(T) * Z);
        double payoff = fmax(ST - K, 0);
        payoff_sum += payoff;
    }
    
    double option_price = exp(-r * T) * payoff_sum / num_paths;
    
    // Analytical Black-Scholes price for comparison
    double d1 = (log(S0/K) + (r + 0.5*sigma*sigma)*T) / (sigma*sqrt(T));
    double d2 = d1 - sigma*sqrt(T);
    double analytical_price = S0 * normal_cdf(d1) - K * exp(-r*T) * normal_cdf(d2);
    
    printf("Monte Carlo Price: $%.4f\n", option_price);
    printf("Analytical Price: $%.4f\n", analytical_price);
    printf("Error: $%.4f (%.2f%%)\n", 
           fabs(option_price - analytical_price), 
           fabs(option_price - analytical_price) / analytical_price * 100);
    
    // Application 2: π estimation
    printf("\n2. Geometric Application: π Estimation\n");
    printf("Using unit circle inscribed in square\n");
    
    set_random_seed(42);
    int inside_circle = 0;
    int total_points = 1000000;
    
    for (int i = 0; i < total_points; i++) {
        double x = uniform_random() * 2 - 1; // [-1, 1]
        double y = uniform_random() * 2 - 1; // [-1, 1]
        
        if (x*x + y*y <= 1) {
            inside_circle++;
        }
    }
    
    double pi_estimate = 4.0 * inside_circle / total_points;
    double pi_error = fabs(pi_estimate - M_PI);
    
    printf("Estimated π: %.6f\n", pi_estimate);
    printf("Actual π: %.6f\n", M_PI);
    printf("Error: %.6f (%.4f%%)\n", pi_error, pi_error / M_PI * 100);
    
    // Application 3: Volume of irregular 3D shape
    printf("\n3. Geometric Application: Volume of Irregular 3D Shape\n");
    printf("f(x,y,z) = x² + y² + z² ≤ 1 (unit sphere)\n");
    
    double cube_min[] = {-1, -1, -1};
    double cube_max[] = {1, 1, 1};
    
    StepTracker volume_tracker;
    init_step_tracker(&volume_tracker, false);
    
    IntegrationResult volume_result = integrate_multidimensional(
        sphere_indicator, cube_min, cube_max, 3, 500000, &volume_tracker);
    
    double actual_sphere_volume = 4.0 * M_PI / 3.0; // ≈ 4.188790
    double volume_error = fabs(volume_result.estimate - actual_sphere_volume);
    
    printf("Estimated Volume: %.6f ± %.6f\n", 
           volume_result.estimate, volume_result.confidence_interval);
    printf("Actual Volume: %.6f\n", actual_sphere_volume);
    printf("Error: %.6f (%.3f%%)\n", volume_error, volume_error/actual_sphere_volume*100);
    
    printf("\nAdvantages of Monte Carlo Integration:\n");
    printf("- Dimension-independent convergence rate O(1/√n)\n");
    printf("- Handles irregular integration domains naturally\n");
    printf("- Easy to parallelize for large-scale computation\n");
    printf("- Provides statistical confidence bounds\n");
    printf("- Works well for high-dimensional problems\n");
}

int main() {
    printf("=== Monte Carlo Integration - Comprehensive Analysis ===\n\n");
    
    set_random_seed(42); // Fixed seed for reproducible results
    
    // Test case 1: Simple polynomial integration
    printf("Test Case 1: Polynomial Integration\n");
    printf("Integrating f(x) = x² from 0 to 2\n");
    printf("Analytical result: 8/3 ≈ 2.666667\n");
    
    StepTracker tracker;
    init_step_tracker(&tracker, true);
    
    IntegrationResult poly_result = integrate_function(polynomial_function, 0.0, 2.0, 100000, &tracker);
    
    printf("\nStep-by-step execution:\n");
    print_steps(&tracker);
    
    double actual_poly = 8.0 / 3.0;
    double error_poly = fabs(poly_result.estimate - actual_poly);
    print_result("Polynomial Integration", &poly_result);
    printf("Actual: %.6f, Error: %.6f (%.3f%%)\n\n", 
           actual_poly, error_poly, (error_poly/actual_poly)*100);
    
    // Test case 2: Trigonometric function with hit-or-miss
    printf("%s\n", "============================================================");
    printf("Test Case 2: Hit-or-Miss Method\n");
    printf("Integrating f(x) = sin(x) from 0 to π\n");
    printf("Analytical result: 2.0\n");
    
    init_step_tracker(&tracker, true);
    IntegrationResult sine_result = integrate_hit_or_miss(sine_function, 0.0, M_PI, 1.0, 50000, &tracker);
    
    printf("\nHit-or-miss execution:\n");
    print_steps(&tracker);
    
    double actual_sine = 2.0;
    double error_sine = fabs(sine_result.estimate - actual_sine);
    print_result("Hit-or-Miss Integration", &sine_result);
    printf("Actual: %.6f, Error: %.6f (%.3f%%)\n\n", 
           actual_sine, error_sine, (error_sine/actual_sine)*100);
    
    // Test case 3: Multidimensional integration
    printf("%s\n", "============================================================");
    printf("Test Case 3: Multidimensional Integration\n");
    printf("Integrating f(x,y) = x²y over [0,1] × [0,1]\n");
    printf("Analytical result: 1/6 ≈ 0.166667\n");
    
    double lower_bounds[] = {0.0, 0.0};
    double upper_bounds[] = {1.0, 1.0};
    
    init_step_tracker(&tracker, true);
    IntegrationResult multi_result = integrate_multidimensional(
        multi_function, lower_bounds, upper_bounds, 2, 100000, &tracker);
    
    printf("\nMultidimensional execution:\n");
    print_steps(&tracker);
    
    double actual_multi = 1.0 / 6.0;
    double error_multi = fabs(multi_result.estimate - actual_multi);
    print_result("Multidimensional Integration", &multi_result);
    printf("Actual: %.6f, Error: %.6f (%.3f%%)\n\n", 
           actual_multi, error_multi, (error_multi/actual_multi)*100);
    
    // Test case 4: Stratified sampling comparison
    printf("%s\n", "============================================================");
    printf("Test Case 4: Stratified vs Basic Sampling\n");
    printf("Integrating f(x) = √x from 0 to 1\n");
    printf("Analytical result: 2/3 ≈ 0.666667\n");
    
    // Basic sampling
    init_step_tracker(&tracker, false);
    IntegrationResult basic_result = integrate_function(sqrt_function, 0.0, 1.0, 10000, &tracker);
    
    // Stratified sampling
    IntegrationResult stratified_result = integrate_stratified(sqrt_function, 0.0, 1.0, 100, 100, &tracker);
    
    double actual_sqrt = 2.0 / 3.0;
    double error_basic = fabs(basic_result.estimate - actual_sqrt);
    double error_stratified = fabs(stratified_result.estimate - actual_sqrt);
    
    printf("Basic Sampling: %.6f ± %.6f (Error: %.6f)\n", 
           basic_result.estimate, basic_result.confidence_interval, error_basic);
    printf("Stratified Sampling: %.6f ± %.6f (Error: %.6f)\n", 
           stratified_result.estimate, stratified_result.confidence_interval, error_stratified);
    printf("Stratified improvement: %.2fx more accurate\n\n", error_basic / error_stratified);
    
    // Test case 5: Convergence analysis
    printf("%s\n", "============================================================");
    printf("Test Case 5: Convergence Analysis\n");
    
    init_step_tracker(&tracker, false);
    int sample_sizes[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
    int num_sizes = sizeof(sample_sizes) / sizeof(sample_sizes[0]);
    
    ConvergenceAnalysis convergence = analyze_convergence(
        polynomial_function, 0.0, 2.0, 8.0/3.0, sample_sizes, num_sizes, &tracker);
    
    printf("%-10s | %-12s | %-12s | %-12s | %-12s\n", 
           "Samples", "Estimate", "Error", "Std Error", "Error/√n");
    printf("----------------------------------------------------------------------\n");
    
    for (int i = 0; i < convergence.count; i++) {
        double error_rate = convergence.errors[i] * sqrt(convergence.sample_sizes[i]);
        printf("%-10d | %-12.6f | %-12.6f | %-12.6f | %-12.6f\n",
            convergence.sample_sizes[i], convergence.estimates[i], convergence.errors[i],
            convergence.standard_errors[i], error_rate);
    }
    
    printf("\n=== Monte Carlo Integration Analysis ===\n");
    printf("Key Insights:\n");
    printf("- Error decreases as O(1/√n) with sample size\n");
    printf("- Multidimensional problems don't suffer from curse of dimensionality\n");
    printf("- Variance reduction techniques (stratification, importance sampling) improve accuracy\n");
    printf("- Hit-or-miss method useful for irregular integration regions\n");
    printf("- Method is embarrassingly parallel - easy to distribute computation\n");
    
    demonstrate_practical_applications();
    
    return 0;
}