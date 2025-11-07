/**
 * Monte Carlo Integration - Advanced Implementation
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

import java.util.*;
import java.util.function.*;

public class MonteCarloIntegration {
    
    static class IntegrationResult {
        double estimate;
        double standardError;
        double confidenceInterval;
        int sampleCount;
        long computationTime;
        
        IntegrationResult(double estimate, double standardError, double confidenceInterval, 
                         int sampleCount, long computationTime) {
            this.estimate = estimate;
            this.standardError = standardError;
            this.confidenceInterval = confidenceInterval;
            this.sampleCount = sampleCount;
            this.computationTime = computationTime;
        }
        
        @Override
        public String toString() {
            return String.format("Estimate: %.6f ± %.6f (95%% CI), Samples: %d, Time: %dms", 
                               estimate, confidenceInterval, sampleCount, computationTime);
        }
    }
    
    static class ConvergenceAnalysis {
        List<Integer> sampleSizes;
        List<Double> estimates;
        List<Double> errors;
        List<Double> standardErrors;
        
        ConvergenceAnalysis() {
            sampleSizes = new ArrayList<>();
            estimates = new ArrayList<>();
            errors = new ArrayList<>();
            standardErrors = new ArrayList<>();
        }
        
        void addPoint(int samples, double estimate, double actualValue, double standardError) {
            sampleSizes.add(samples);
            estimates.add(estimate);
            errors.add(Math.abs(estimate - actualValue));
            standardErrors.add(standardError);
        }
    }
    
    private Random random;
    private boolean verbose;
    private List<String> steps;
    
    public MonteCarloIntegration(boolean verbose) {
        this.random = new Random();
        this.verbose = verbose;
        this.steps = new ArrayList<>();
    }
    
    public MonteCarloIntegration(long seed, boolean verbose) {
        this.random = new Random(seed);
        this.verbose = verbose;
        this.steps = new ArrayList<>();
    }
    
    /**
     * Basic Monte Carlo integration for single-variable functions
     */
    public IntegrationResult integrateFunction(Function<Double, Double> function, 
                                             double lowerBound, double upperBound, 
                                             int numSamples) {
        if (verbose) {
            steps.add("=== Basic Monte Carlo Integration ===");
            steps.add(String.format("Integrating over [%.3f, %.3f] with %d samples", 
                                   lowerBound, upperBound, numSamples));
        }
        
        long startTime = System.nanoTime();
        
        double sum = 0.0;
        double sumSquares = 0.0;
        double width = upperBound - lowerBound;
        
        for (int i = 0; i < numSamples; i++) {
            double x = lowerBound + random.nextDouble() * width;
            double y = function.apply(x);
            
            sum += y;
            sumSquares += y * y;
            
            if (verbose && i < 10) {
                steps.add(String.format("Sample %d: x=%.4f, f(x)=%.4f", i + 1, x, y));
            }
        }
        
        double mean = sum / numSamples;
        double variance = (sumSquares / numSamples) - (mean * mean);
        double standardError = Math.sqrt(variance / numSamples);
        
        double estimate = mean * width;
        double estimateError = standardError * width;
        double confidenceInterval = 1.96 * estimateError; // 95% confidence interval
        
        long endTime = System.nanoTime();
        long computationTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add(String.format("Mean function value: %.6f", mean));
            steps.add(String.format("Standard error: %.6f", standardError));
            steps.add(String.format("Final estimate: %.6f ± %.6f", estimate, confidenceInterval));
        }
        
        return new IntegrationResult(estimate, estimateError, confidenceInterval, 
                                   numSamples, computationTime);
    }
    
    /**
     * Monte Carlo integration with hit-or-miss method (useful for irregular regions)
     */
    public IntegrationResult integrateHitOrMiss(Function<Double, Double> function,
                                              double lowerBound, double upperBound,
                                              double maxValue, int numSamples) {
        if (verbose) {
            steps.add("=== Hit-or-Miss Monte Carlo Integration ===");
            steps.add(String.format("Region: [%.3f, %.3f] × [0, %.3f]", 
                                   lowerBound, upperBound, maxValue));
        }
        
        long startTime = System.nanoTime();
        
        int hits = 0;
        double width = upperBound - lowerBound;
        
        for (int i = 0; i < numSamples; i++) {
            double x = lowerBound + random.nextDouble() * width;
            double y = random.nextDouble() * maxValue;
            
            if (y <= function.apply(x)) {
                hits++;
            }
            
            if (verbose && i < 10) {
                boolean hit = y <= function.apply(x);
                steps.add(String.format("Sample %d: (%.4f, %.4f) -> %s", 
                                       i + 1, x, y, hit ? "HIT" : "MISS"));
            }
        }
        
        double hitRatio = (double) hits / numSamples;
        double estimate = hitRatio * width * maxValue;
        
        // Binomial standard error
        double variance = hitRatio * (1 - hitRatio) / numSamples;
        double standardError = Math.sqrt(variance) * width * maxValue;
        double confidenceInterval = 1.96 * standardError;
        
        long endTime = System.nanoTime();
        long computationTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add(String.format("Hits: %d / %d (%.4f%%)", hits, numSamples, hitRatio * 100));
            steps.add(String.format("Estimate: %.6f ± %.6f", estimate, confidenceInterval));
        }
        
        return new IntegrationResult(estimate, standardError, confidenceInterval, 
                                   numSamples, computationTime);
    }
    
    /**
     * Multidimensional Monte Carlo integration
     */
    public IntegrationResult integrateMultidimensional(Function<double[], Double> function,
                                                     double[] lowerBounds, double[] upperBounds,
                                                     int numSamples) {
        int dimensions = lowerBounds.length;
        
        if (verbose) {
            steps.add("=== Multidimensional Monte Carlo Integration ===");
            steps.add(String.format("Dimensions: %d, Samples: %d", dimensions, numSamples));
        }
        
        long startTime = System.nanoTime();
        
        double sum = 0.0;
        double sumSquares = 0.0;
        double volume = 1.0;
        
        // Calculate volume of integration region
        for (int i = 0; i < dimensions; i++) {
            volume *= (upperBounds[i] - lowerBounds[i]);
        }
        
        for (int i = 0; i < numSamples; i++) {
            double[] point = new double[dimensions];
            
            // Generate random point in the integration region
            for (int d = 0; d < dimensions; d++) {
                point[d] = lowerBounds[d] + random.nextDouble() * (upperBounds[d] - lowerBounds[d]);
            }
            
            double value = function.apply(point);
            sum += value;
            sumSquares += value * value;
            
            if (verbose && i < 5) {
                steps.add(String.format("Sample %d: %s -> %.4f", 
                                       i + 1, Arrays.toString(point), value));
            }
        }
        
        double mean = sum / numSamples;
        double variance = (sumSquares / numSamples) - (mean * mean);
        double standardError = Math.sqrt(variance / numSamples);
        
        double estimate = mean * volume;
        double estimateError = standardError * volume;
        double confidenceInterval = 1.96 * estimateError;
        
        long endTime = System.nanoTime();
        long computationTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add(String.format("Integration volume: %.6f", volume));
            steps.add(String.format("Mean function value: %.6f", mean));
            steps.add(String.format("Final estimate: %.6f ± %.6f", estimate, confidenceInterval));
        }
        
        return new IntegrationResult(estimate, estimateError, confidenceInterval, 
                                   numSamples, computationTime);
    }
    
    /**
     * Importance sampling Monte Carlo integration
     */
    public IntegrationResult integrateImportanceSampling(Function<Double, Double> function,
                                                       Function<Double, Double> importanceFunction,
                                                       Function<Random, Double> importanceSampler,
                                                       int numSamples) {
        if (verbose) {
            steps.add("=== Importance Sampling Monte Carlo Integration ===");
            steps.add(String.format("Using importance sampling with %d samples", numSamples));
        }
        
        long startTime = System.nanoTime();
        
        double sum = 0.0;
        double sumSquares = 0.0;
        
        for (int i = 0; i < numSamples; i++) {
            double x = importanceSampler.apply(random);
            double functionValue = function.apply(x);
            double importanceValue = importanceFunction.apply(x);
            
            double weight = functionValue / importanceValue;
            sum += weight;
            sumSquares += weight * weight;
            
            if (verbose && i < 10) {
                steps.add(String.format("Sample %d: x=%.4f, f(x)=%.4f, g(x)=%.4f, w=%.4f", 
                                       i + 1, x, functionValue, importanceValue, weight));
            }
        }
        
        double mean = sum / numSamples;
        double variance = (sumSquares / numSamples) - (mean * mean);
        double standardError = Math.sqrt(variance / numSamples);
        double confidenceInterval = 1.96 * standardError;
        
        long endTime = System.nanoTime();
        long computationTime = (endTime - startTime) / 1_000_000;
        
        if (verbose) {
            steps.add(String.format("Weighted average: %.6f", mean));
            steps.add(String.format("Standard error: %.6f", standardError));
            steps.add(String.format("Final estimate: %.6f ± %.6f", mean, confidenceInterval));
        }
        
        return new IntegrationResult(mean, standardError, confidenceInterval, 
                                   numSamples, computationTime);
    }
    
    /**
     * Stratified sampling Monte Carlo integration
     */
    public IntegrationResult integrateStratified(Function<Double, Double> function,
                                               double lowerBound, double upperBound,
                                               int numStrata, int samplesPerStratum) {
        if (verbose) {
            steps.add("=== Stratified Sampling Monte Carlo Integration ===");
            steps.add(String.format("Strata: %d, Samples per stratum: %d", 
                                   numStrata, samplesPerStratum));
        }
        
        long startTime = System.nanoTime();
        
        double totalSum = 0.0;
        double totalSumSquares = 0.0;
        double width = upperBound - lowerBound;
        double stratumWidth = width / numStrata;
        
        for (int s = 0; s < numStrata; s++) {
            double stratumLower = lowerBound + s * stratumWidth;
            double stratumUpper = stratumLower + stratumWidth;
            
            double stratumSum = 0.0;
            for (int i = 0; i < samplesPerStratum; i++) {
                double x = stratumLower + random.nextDouble() * stratumWidth;
                double y = function.apply(x);
                stratumSum += y;
                
                if (verbose && s < 3 && i < 3) {
                    steps.add(String.format("Stratum %d, Sample %d: x=%.4f, f(x)=%.4f", 
                                           s + 1, i + 1, x, y));
                }
            }
            
            double stratumMean = stratumSum / samplesPerStratum;
            totalSum += stratumMean;
            totalSumSquares += stratumMean * stratumMean;
        }
        
        double overallMean = totalSum / numStrata;
        double variance = (totalSumSquares / numStrata) - (overallMean * overallMean);
        double standardError = Math.sqrt(variance / numStrata);
        
        double estimate = overallMean * width;
        double estimateError = standardError * width;
        double confidenceInterval = 1.96 * estimateError;
        
        long endTime = System.nanoTime();
        long computationTime = (endTime - startTime) / 1_000_000;
        
        int totalSamples = numStrata * samplesPerStratum;
        
        if (verbose) {
            steps.add(String.format("Overall mean: %.6f", overallMean));
            steps.add(String.format("Final estimate: %.6f ± %.6f", estimate, confidenceInterval));
        }
        
        return new IntegrationResult(estimate, estimateError, confidenceInterval, 
                                   totalSamples, computationTime);
    }
    
    /**
     * Convergence analysis - shows how accuracy improves with sample size
     */
    public ConvergenceAnalysis analyzeConvergence(Function<Double, Double> function,
                                                double lowerBound, double upperBound,
                                                double actualValue, int[] sampleSizes) {
        ConvergenceAnalysis analysis = new ConvergenceAnalysis();
        
        if (verbose) {
            steps.add("=== Convergence Analysis ===");
            steps.add(String.format("Actual integral value: %.6f", actualValue));
        }
        
        for (int numSamples : sampleSizes) {
            IntegrationResult result = integrateFunction(function, lowerBound, upperBound, numSamples);
            analysis.addPoint(numSamples, result.estimate, actualValue, result.standardError);
            
            if (verbose) {
                double error = Math.abs(result.estimate - actualValue);
                double errorPercent = (error / Math.abs(actualValue)) * 100;
                steps.add(String.format("Samples: %d, Estimate: %.6f, Error: %.6f (%.2f%%)", 
                                       numSamples, result.estimate, error, errorPercent));
            }
        }
        
        return analysis;
    }
    
    public List<String> getSteps() {
        return new ArrayList<>(steps);
    }
    
    public void clearSteps() {
        steps.clear();
    }
    
    public static void main(String[] args) {
        System.out.println("=== Monte Carlo Integration - Comprehensive Analysis ===\n");
        
        MonteCarloIntegration integrator = new MonteCarloIntegration(42, true);
        
        // Test case 1: Simple polynomial integration
        System.out.println("Test Case 1: Polynomial Integration");
        System.out.println("Integrating f(x) = x² from 0 to 2");
        System.out.println("Analytical result: 8/3 ≈ 2.666667");
        
        Function<Double, Double> polynomial = x -> x * x;
        IntegrationResult polyResult = integrator.integrateFunction(polynomial, 0.0, 2.0, 100000);
        
        System.out.println("\nStep-by-step execution:");
        for (String step : integrator.getSteps()) {
            System.out.println(step);
        }
        
        double actualPoly = 8.0 / 3.0;
        double errorPoly = Math.abs(polyResult.estimate - actualPoly);
        System.out.printf("Result: %s\n", polyResult);
        System.out.printf("Actual: %.6f, Error: %.6f (%.3f%%)\n\n", 
                         actualPoly, errorPoly, (errorPoly/actualPoly)*100);
        
        integrator.clearSteps();
        
        // Test case 2: Trigonometric function with hit-or-miss
        System.out.println("=".repeat(60));
        System.out.println("Test Case 2: Hit-or-Miss Method");
        System.out.println("Integrating f(x) = sin(x) from 0 to π");
        System.out.println("Analytical result: 2.0");
        
        Function<Double, Double> sine = Math::sin;
        IntegrationResult sineResult = integrator.integrateHitOrMiss(sine, 0.0, Math.PI, 1.0, 50000);
        
        System.out.println("\nHit-or-miss execution:");
        for (String step : integrator.getSteps()) {
            System.out.println(step);
        }
        
        double actualSine = 2.0;
        double errorSine = Math.abs(sineResult.estimate - actualSine);
        System.out.printf("Result: %s\n", sineResult);
        System.out.printf("Actual: %.6f, Error: %.6f (%.3f%%)\n\n", 
                         actualSine, errorSine, (errorSine/actualSine)*100);
        
        integrator.clearSteps();
        
        // Test case 3: Multidimensional integration
        System.out.println("=".repeat(60));
        System.out.println("Test Case 3: Multidimensional Integration");
        System.out.println("Integrating f(x,y) = x²y over [0,1] × [0,1]");
        System.out.println("Analytical result: 1/6 ≈ 0.166667");
        
        Function<double[], Double> multiFunc = point -> point[0] * point[0] * point[1];
        double[] lowerBounds = {0.0, 0.0};
        double[] upperBounds = {1.0, 1.0};
        
        IntegrationResult multiResult = integrator.integrateMultidimensional(
            multiFunc, lowerBounds, upperBounds, 100000);
        
        System.out.println("\nMultidimensional execution:");
        for (String step : integrator.getSteps()) {
            System.out.println(step);
        }
        
        double actualMulti = 1.0 / 6.0;
        double errorMulti = Math.abs(multiResult.estimate - actualMulti);
        System.out.printf("Result: %s\n", multiResult);
        System.out.printf("Actual: %.6f, Error: %.6f (%.3f%%)\n\n", 
                         actualMulti, errorMulti, (errorMulti/actualMulti)*100);
        
        integrator.clearSteps();
        
        // Test case 4: Importance sampling
        System.out.println("=".repeat(60));
        System.out.println("Test Case 4: Importance Sampling");
        System.out.println("Integrating f(x) = e^(-x²) (Gaussian) from 0 to ∞");
        System.out.println("Using exponential importance function");
        
        Function<Double, Double> gaussian = x -> Math.exp(-x * x);
        Function<Double, Double> exponential = x -> Math.exp(-x); // Importance function
        Function<Random, Double> expSampler = rnd -> -Math.log(1 - rnd.nextDouble()); // Exponential sampler
        
        IntegrationResult importanceResult = integrator.integrateImportanceSampling(
            gaussian, exponential, expSampler, 50000);
        
        System.out.println("\nImportance sampling execution:");
        for (String step : integrator.getSteps()) {
            System.out.println(step);
        }
        
        double actualGaussian = Math.sqrt(Math.PI) / 2; // ≈ 0.886227
        double errorImportance = Math.abs(importanceResult.estimate - actualGaussian);
        System.out.printf("Result: %s\n", importanceResult);
        System.out.printf("Actual: %.6f, Error: %.6f (%.3f%%)\n\n", 
                         actualGaussian, errorImportance, (errorImportance/actualGaussian)*100);
        
        integrator.clearSteps();
        
        // Test case 5: Stratified sampling comparison
        System.out.println("=".repeat(60));
        System.out.println("Test Case 5: Stratified vs Basic Sampling");
        System.out.println("Integrating f(x) = √x from 0 to 1");
        System.out.println("Analytical result: 2/3 ≈ 0.666667");
        
        Function<Double, Double> sqrt = Math::sqrt;
        
        // Basic sampling
        integrator = new MonteCarloIntegration(42, false);
        IntegrationResult basicResult = integrator.integrateFunction(sqrt, 0.0, 1.0, 10000);
        
        // Stratified sampling
        IntegrationResult stratifiedResult = integrator.integrateStratified(sqrt, 0.0, 1.0, 100, 100);
        
        double actualSqrt = 2.0 / 3.0;
        double errorBasic = Math.abs(basicResult.estimate - actualSqrt);
        double errorStratified = Math.abs(stratifiedResult.estimate - actualSqrt);
        
        System.out.printf("Basic Sampling: %.6f ± %.6f (Error: %.6f)\n", 
                         basicResult.estimate, basicResult.confidenceInterval, errorBasic);
        System.out.printf("Stratified Sampling: %.6f ± %.6f (Error: %.6f)\n", 
                         stratifiedResult.estimate, stratifiedResult.confidenceInterval, errorStratified);
        System.out.printf("Stratified improvement: %.2fx more accurate\n\n", errorBasic / errorStratified);
        
        // Test case 6: Convergence analysis
        System.out.println("=".repeat(60));
        System.out.println("Test Case 6: Convergence Analysis");
        
        integrator = new MonteCarloIntegration(42, false);
        int[] sampleSizes = {100, 500, 1000, 5000, 10000, 50000, 100000};
        ConvergenceAnalysis convergence = integrator.analyzeConvergence(
            polynomial, 0.0, 2.0, 8.0/3.0, sampleSizes);
        
        System.out.printf("%-10s | %-12s | %-12s | %-12s | %-12s\n", 
                         "Samples", "Estimate", "Error", "Std Error", "Error/√n");
        System.out.println("-".repeat(70));
        
        for (int i = 0; i < sampleSizes.length; i++) {
            double errorRate = convergence.errors.get(i) * Math.sqrt(sampleSizes[i]);
            System.out.printf("%-10d | %-12.6f | %-12.6f | %-12.6f | %-12.6f\n",
                sampleSizes[i], convergence.estimates.get(i), convergence.errors.get(i),
                convergence.standardErrors.get(i), errorRate);
        }
        
        System.out.println("\n=== Monte Carlo Integration Analysis ===");
        System.out.println("Key Insights:");
        System.out.println("- Error decreases as O(1/√n) with sample size");
        System.out.println("- Multidimensional problems don't suffer from curse of dimensionality");
        System.out.println("- Variance reduction techniques (stratification, importance sampling) improve accuracy");
        System.out.println("- Hit-or-miss method useful for irregular integration regions");
        System.out.println("- Method is embarrassingly parallel - easy to distribute computation");
        
        demonstratePracticalApplications();
    }
    
    private static void demonstratePracticalApplications() {
        System.out.println("\n=== Practical Applications ===");
        
        MonteCarloIntegration integrator = new MonteCarloIntegration(42, false);
        
        // Application 1: Option pricing (Black-Scholes approximation)
        System.out.println("\n1. Financial Application: European Call Option Pricing");
        System.out.println("Parameters: S₀=100, K=105, T=0.25, r=0.05, σ=0.2");
        
        double S0 = 100, K = 105, T = 0.25, r = 0.05, sigma = 0.2;
        int numPaths = 100000;
        
        Random optionRandom = new Random(42);
        double payoffSum = 0.0;
        
        for (int i = 0; i < numPaths; i++) {
            double Z = optionRandom.nextGaussian();
            double ST = S0 * Math.exp((r - 0.5 * sigma * sigma) * T + sigma * Math.sqrt(T) * Z);
            double payoff = Math.max(ST - K, 0);
            payoffSum += payoff;
        }
        
        double optionPrice = Math.exp(-r * T) * payoffSum / numPaths;
        
        // Analytical Black-Scholes price for comparison
        double d1 = (Math.log(S0/K) + (r + 0.5*sigma*sigma)*T) / (sigma*Math.sqrt(T));
        double d2 = d1 - sigma*Math.sqrt(T);
        double analyticalPrice = S0 * normalCDF(d1) - K * Math.exp(-r*T) * normalCDF(d2);
        
        System.out.printf("Monte Carlo Price: $%.4f\n", optionPrice);
        System.out.printf("Analytical Price: $%.4f\n", analyticalPrice);
        System.out.printf("Error: $%.4f (%.2f%%)\n", 
                         Math.abs(optionPrice - analyticalPrice), 
                         Math.abs(optionPrice - analyticalPrice) / analyticalPrice * 100);
        
        // Application 2: π estimation
        System.out.println("\n2. Geometric Application: π Estimation");
        System.out.println("Using unit circle inscribed in square");
        
        Random piRandom = new Random(42);
        int insideCircle = 0;
        int totalPoints = 1000000;
        
        for (int i = 0; i < totalPoints; i++) {
            double x = piRandom.nextDouble() * 2 - 1; // [-1, 1]
            double y = piRandom.nextDouble() * 2 - 1; // [-1, 1]
            
            if (x*x + y*y <= 1) {
                insideCircle++;
            }
        }
        
        double piEstimate = 4.0 * insideCircle / totalPoints;
        double piError = Math.abs(piEstimate - Math.PI);
        
        System.out.printf("Estimated π: %.6f\n", piEstimate);
        System.out.printf("Actual π: %.6f\n", Math.PI);
        System.out.printf("Error: %.6f (%.4f%%)\n", piError, piError / Math.PI * 100);
        
        // Application 3: Volume of irregular 3D shape
        System.out.println("\n3. Geometric Application: Volume of Irregular 3D Shape");
        System.out.println("f(x,y,z) = x² + y² + z² ≤ 1 (unit sphere)");
        
        Function<double[], Double> sphereIndicator = point -> {
            double sum = 0;
            for (double coord : point) {
                sum += coord * coord;
            }
            return sum <= 1.0 ? 1.0 : 0.0;
        };
        
        double[] cubeMin = {-1, -1, -1};
        double[] cubeMax = {1, 1, 1};
        
        IntegrationResult volumeResult = integrator.integrateMultidimensional(
            sphereIndicator, cubeMin, cubeMax, 500000);
        
        double actualSphereVolume = 4.0 * Math.PI / 3.0; // ≈ 4.188790
        double volumeError = Math.abs(volumeResult.estimate - actualSphereVolume);
        
        System.out.printf("Estimated Volume: %.6f ± %.6f\n", 
                         volumeResult.estimate, volumeResult.confidenceInterval);
        System.out.printf("Actual Volume: %.6f\n", actualSphereVolume);
        System.out.printf("Error: %.6f (%.3f%%)\n", volumeError, volumeError/actualSphereVolume*100);
        
        System.out.println("\nAdvantages of Monte Carlo Integration:");
        System.out.println("- Dimension-independent convergence rate O(1/√n)");
        System.out.println("- Handles irregular integration domains naturally");
        System.out.println("- Easy to parallelize for large-scale computation");
        System.out.println("- Provides statistical confidence bounds");
        System.out.println("- Works well for high-dimensional problems");
    }
    
    // Cumulative normal distribution function approximation
    private static double normalCDF(double x) {
        return 0.5 * (1 + erf(x / Math.sqrt(2)));
    }
    
    // Error function approximation
    private static double erf(double x) {
        double a1 =  0.254829592;
        double a2 = -0.284496736;
        double a3 =  1.421413741;
        double a4 = -1.453152027;
        double a5 =  1.061405429;
        double p  =  0.3275911;
        
        int sign = x >= 0 ? 1 : -1;
        x = Math.abs(x);
        
        double t = 1.0 / (1.0 + p * x);
        double y = 1.0 - (((((a5 * t + a4) * t) + a3) * t + a2) * t + a1) * t * Math.exp(-x * x);
        
        return sign * y;
    }
}