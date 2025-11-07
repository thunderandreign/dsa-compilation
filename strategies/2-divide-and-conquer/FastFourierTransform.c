#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

/**
 * Divide and Conquer Strategy: Fast Fourier Transform (FFT) - Simplified
 * Core Idea: Divide polynomial into even and odd coefficients, compute recursively
 * Time Complexity: O(n log n) where n is the number of points
 * Space Complexity: O(n log n) for recursion and temporary arrays
 * 
 * Note: This is a simplified educational implementation for demonstration purposes
 */

#define PI 3.14159265358979323846

/**
 * Complex number structure for FFT calculations
 */
typedef struct {
    double real;
    double imag;
} Complex;

/**
 * Create a complex number
 */
Complex makeComplex(double real, double imag) {
    Complex c;
    c.real = real;
    c.imag = imag;
    return c;
}

/**
 * Add two complex numbers
 */
Complex addComplex(Complex a, Complex b) {
    return makeComplex(a.real + b.real, a.imag + b.imag);
}

/**
 * Subtract two complex numbers
 */
Complex subtractComplex(Complex a, Complex b) {
    return makeComplex(a.real - b.real, a.imag - b.imag);
}

/**
 * Multiply two complex numbers
 */
Complex multiplyComplex(Complex a, Complex b) {
    double newReal = a.real * b.real - a.imag * b.imag;
    double newImag = a.real * b.imag + a.imag * b.real;
    return makeComplex(newReal, newImag);
}

/**
 * Calculate magnitude of complex number
 */
double magnitude(Complex c) {
    return sqrt(c.real * c.real + c.imag * c.imag);
}

/**
 * Print complex number
 */
void printComplex(Complex c) {
    if (c.imag >= 0) {
        printf("%.3f + %.3fi", c.real, c.imag);
    } else {
        printf("%.3f - %.3fi", c.real, -c.imag);
    }
}

/**
 * Fast Fourier Transform implementation using divide and conquer
 * @param x Input complex array (length must be power of 2)
 * @param n Length of array
 * @param result Output array (must be pre-allocated)
 */
void fft(Complex x[], int n, Complex result[]) {
    // Base case
    if (n == 1) {
        result[0] = x[0];
        return;
    }
    
    // Divide: separate even and odd indices
    Complex* even = (Complex*)malloc((n / 2) * sizeof(Complex));
    Complex* odd = (Complex*)malloc((n / 2) * sizeof(Complex));
    Complex* evenFFT = (Complex*)malloc((n / 2) * sizeof(Complex));
    Complex* oddFFT = (Complex*)malloc((n / 2) * sizeof(Complex));
    
    for (int i = 0; i < n / 2; i++) {
        even[i] = x[2 * i];
        odd[i] = x[2 * i + 1];
    }
    
    // Conquer: recursively compute FFT of even and odd parts
    fft(even, n / 2, evenFFT);
    fft(odd, n / 2, oddFFT);
    
    // Combine: merge the results
    for (int k = 0; k < n / 2; k++) {
        // Calculate twiddle factor: e^(-2πik/n)
        double angle = -2 * PI * k / n;
        Complex twiddle = makeComplex(cos(angle), sin(angle));
        
        Complex oddPart = multiplyComplex(twiddle, oddFFT[k]);
        
        result[k] = addComplex(evenFFT[k], oddPart);
        result[k + n / 2] = subtractComplex(evenFFT[k], oddPart);
    }
    
    // Clean up
    free(even);
    free(odd);
    free(evenFFT);
    free(oddFFT);
}

/**
 * Inverse Fast Fourier Transform
 * @param x Input complex array from FFT
 * @param n Length of array
 * @param result Output array (original signal)
 */
void ifft(Complex x[], int n, Complex result[]) {
    // Conjugate the input
    Complex* conjugated = (Complex*)malloc(n * sizeof(Complex));
    for (int i = 0; i < n; i++) {
        conjugated[i] = makeComplex(x[i].real, -x[i].imag);
    }
    
    // Apply FFT
    fft(conjugated, n, result);
    
    // Conjugate the result and scale by n
    for (int i = 0; i < n; i++) {
        result[i] = makeComplex(result[i].real / n, -result[i].imag / n);
    }
    
    free(conjugated);
}

/**
 * Discrete Fourier Transform (naive approach for comparison)
 * @param x Input complex array
 * @param n Length of array
 * @param result Output array
 */
void dft(Complex x[], int n, Complex result[]) {
    for (int k = 0; k < n; k++) {
        result[k] = makeComplex(0, 0);
        
        for (int j = 0; j < n; j++) {
            double angle = -2 * PI * k * j / n;
            Complex twiddle = makeComplex(cos(angle), sin(angle));
            Complex term = multiplyComplex(x[j], twiddle);
            result[k] = addComplex(result[k], term);
        }
    }
}

/**
 * Convert real array to complex array
 */
void realToComplex(double real[], int n, Complex complex[]) {
    for (int i = 0; i < n; i++) {
        complex[i] = makeComplex(real[i], 0);
    }
}

/**
 * Find next power of 2 greater than or equal to n
 */
int nextPowerOfTwo(int n) {
    int power = 1;
    while (power < n) {
        power *= 2;
    }
    return power;
}

/**
 * Check if n is power of 2
 */
int isPowerOfTwo(int n) {
    return (n > 0) && ((n & (n - 1)) == 0);
}

int main() {
    printf("=== Fast Fourier Transform - Divide and Conquer ===\n");
    
    // Test Case 1: Simple 4-point signal
    printf("Test Case 1: Simple 4-point signal\n");
    double signal1[] = {1, 2, 3, 4};
    int n1 = 4;
    Complex complexSignal1[4];
    Complex fftResult1[4];
    Complex ifftResult1[4];
    
    realToComplex(signal1, n1, complexSignal1);
    
    printf("Input signal: ");
    for (int i = 0; i < n1; i++) {
        printf("%.1f ", complexSignal1[i].real);
    }
    printf("\n");
    
    fft(complexSignal1, n1, fftResult1);
    printf("FFT result:\n");
    for (int i = 0; i < n1; i++) {
        printf("  X[%d] = ", i);
        printComplex(fftResult1[i]);
        printf("\n");
    }
    
    ifft(fftResult1, n1, ifftResult1);
    printf("IFFT result (should match input): ");
    for (int i = 0; i < n1; i++) {
        printf("%.1f ", ifftResult1[i].real);
    }
    printf("\n\n");
    
    // Test Case 2: Sine wave
    printf("Test Case 2: Sine wave (8 points)\n");
    int n2 = 8;
    double signal2[8];
    Complex complexSignal2[8];
    Complex fftResult2[8];
    
    for (int i = 0; i < n2; i++) {
        signal2[i] = sin(2 * PI * i / n2); // One complete cycle
    }
    
    realToComplex(signal2, n2, complexSignal2);
    printf("Input sine wave: ");
    for (int i = 0; i < n2; i++) {
        printf("%.3f ", complexSignal2[i].real);
    }
    printf("\n");
    
    fft(complexSignal2, n2, fftResult2);
    printf("FFT magnitudes:\n");
    for (int i = 0; i < n2; i++) {
        printf("  |X[%d]| = %.3f\n", i, magnitude(fftResult2[i]));
    }
    printf("\n");
    
    // Test Case 3: Performance comparison (FFT vs DFT)
    printf("Test Case 3: Performance comparison\n");
    double signal3[] = {1, 0, 1, 0, 1, 0, 1, 0}; // Square wave
    int n3 = 8;
    Complex complexSignal3[8];
    Complex fftResult3[8];
    Complex dftResult3[8];
    
    realToComplex(signal3, n3, complexSignal3);
    printf("Input square wave: ");
    for (int i = 0; i < n3; i++) {
        printf("%.0f ", complexSignal3[i].real);
    }
    printf("\n");
    
    // Time FFT
    clock_t startFFT = clock();
    fft(complexSignal3, n3, fftResult3);
    clock_t endFFT = clock();
    
    // Time DFT
    clock_t startDFT = clock();
    dft(complexSignal3, n3, dftResult3);
    clock_t endDFT = clock();
    
    long fftTime = (endFFT - startFFT) * 1000000 / CLOCKS_PER_SEC;
    long dftTime = (endDFT - startDFT) * 1000000 / CLOCKS_PER_SEC;
    
    printf("FFT time: %ld microseconds\n", fftTime);
    printf("DFT time: %ld microseconds\n", dftTime);
    if (fftTime > 0) {
        printf("Speedup: %.2fx\n", (double)dftTime / fftTime);
    }
    
    // Verify results match
    int resultsMatch = 1;
    for (int i = 0; i < n3; i++) {
        double diff = fabs(fftResult3[i].real - dftResult3[i].real) + 
                     fabs(fftResult3[i].imag - dftResult3[i].imag);
        if (diff > 1e-10) {
            resultsMatch = 0;
            break;
        }
    }
    printf("Results match: %s\n", resultsMatch ? "Yes" : "No");
    printf("\n");
    
    // Test Case 4: Power of 2 check
    printf("Test Case 4: Power of 2 demonstration\n");
    int testSizes[] = {1, 2, 4, 8, 16, 32, 15, 17};
    int numTests = sizeof(testSizes) / sizeof(testSizes[0]);
    
    printf("Testing which sizes are powers of 2:\n");
    for (int i = 0; i < numTests; i++) {
        printf("  %d: %s\n", testSizes[i], 
               isPowerOfTwo(testSizes[i]) ? "Yes" : "No");
    }
    printf("\n");
    
    // Test Case 5: Simple magnitude spectrum
    printf("Test Case 5: Magnitude spectrum\n");
    double signal5[] = {1, 1, 0, 0}; // Simple step function
    int n5 = 4;
    Complex complexSignal5[4];
    Complex fftResult5[4];
    
    realToComplex(signal5, n5, complexSignal5);
    printf("Input step function: ");
    for (int i = 0; i < n5; i++) {
        printf("%.0f ", complexSignal5[i].real);
    }
    printf("\n");
    
    fft(complexSignal5, n5, fftResult5);
    printf("Frequency domain (magnitude and phase):\n");
    for (int i = 0; i < n5; i++) {
        double mag = magnitude(fftResult5[i]);
        double phase = atan2(fftResult5[i].imag, fftResult5[i].real);
        printf("  Bin %d: Magnitude = %.3f, Phase = %.3f rad\n", i, mag, phase);
    }
    
    return 0;
}