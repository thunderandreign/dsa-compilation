/**
 * Divide and Conquer Strategy: Fast Fourier Transform (FFT) - Simplified
 * Core Idea: Divide polynomial into even and odd coefficients, compute recursively
 * Time Complexity: O(n log n) where n is the number of points
 * Space Complexity: O(n log n) for recursion and temporary arrays
 * 
 * Note: This is a simplified educational implementation for demonstration purposes
 */
public class FastFourierTransform {
    
    /**
     * Complex number class for FFT calculations
     */
    static class Complex {
        double real, imag;
        
        Complex(double real, double imag) {
            this.real = real;
            this.imag = imag;
        }
        
        Complex(double real) {
            this(real, 0);
        }
        
        // Addition
        Complex add(Complex other) {
            return new Complex(this.real + other.real, this.imag + other.imag);
        }
        
        // Subtraction
        Complex subtract(Complex other) {
            return new Complex(this.real - other.real, this.imag - other.imag);
        }
        
        // Multiplication
        Complex multiply(Complex other) {
            double newReal = this.real * other.real - this.imag * other.imag;
            double newImag = this.real * other.imag + this.imag * other.real;
            return new Complex(newReal, newImag);
        }
        
        // Magnitude
        double magnitude() {
            return Math.sqrt(real * real + imag * imag);
        }
        
        @Override
        public String toString() {
            if (imag >= 0) {
                return String.format("%.3f + %.3fi", real, imag);
            } else {
                return String.format("%.3f - %.3fi", real, -imag);
            }
        }
    }
    
    /**
     * Fast Fourier Transform implementation using divide and conquer
     * @param x Input complex array (length must be power of 2)
     * @return FFT of input array
     */
    public static Complex[] fft(Complex[] x) {
        int n = x.length;
        
        // Base case
        if (n == 1) {
            return new Complex[]{x[0]};
        }
        
        // Ensure n is power of 2
        if (n % 2 != 0) {
            throw new IllegalArgumentException("Array length must be power of 2");
        }
        
        // Divide: separate even and odd indices
        Complex[] even = new Complex[n / 2];
        Complex[] odd = new Complex[n / 2];
        
        for (int i = 0; i < n / 2; i++) {
            even[i] = x[2 * i];
            odd[i] = x[2 * i + 1];
        }
        
        // Conquer: recursively compute FFT of even and odd parts
        Complex[] evenFFT = fft(even);
        Complex[] oddFFT = fft(odd);
        
        // Combine: merge the results
        Complex[] result = new Complex[n];
        
        for (int k = 0; k < n / 2; k++) {
            // Calculate twiddle factor: e^(-2πik/n)
            double angle = -2 * Math.PI * k / n;
            Complex twiddle = new Complex(Math.cos(angle), Math.sin(angle));
            
            Complex oddPart = twiddle.multiply(oddFFT[k]);
            
            result[k] = evenFFT[k].add(oddPart);
            result[k + n / 2] = evenFFT[k].subtract(oddPart);
        }
        
        return result;
    }
    
    /**
     * Inverse Fast Fourier Transform
     * @param x Input complex array from FFT
     * @return Original signal (approximately)
     */
    public static Complex[] ifft(Complex[] x) {
        int n = x.length;
        
        // Conjugate the input
        Complex[] conjugated = new Complex[n];
        for (int i = 0; i < n; i++) {
            conjugated[i] = new Complex(x[i].real, -x[i].imag);
        }
        
        // Apply FFT
        Complex[] result = fft(conjugated);
        
        // Conjugate the result and scale by n
        for (int i = 0; i < n; i++) {
            result[i] = new Complex(result[i].real / n, -result[i].imag / n);
        }
        
        return result;
    }
    
    /**
     * Discrete Fourier Transform (naive approach for comparison)
     * @param x Input complex array
     * @return DFT of input array
     */
    public static Complex[] dft(Complex[] x) {
        int n = x.length;
        Complex[] result = new Complex[n];
        
        for (int k = 0; k < n; k++) {
            result[k] = new Complex(0, 0);
            
            for (int j = 0; j < n; j++) {
                double angle = -2 * Math.PI * k * j / n;
                Complex twiddle = new Complex(Math.cos(angle), Math.sin(angle));
                result[k] = result[k].add(x[j].multiply(twiddle));
            }
        }
        
        return result;
    }
    
    /**
     * Convert real array to complex array
     */
    public static Complex[] realToComplex(double[] real) {
        Complex[] complex = new Complex[real.length];
        for (int i = 0; i < real.length; i++) {
            complex[i] = new Complex(real[i], 0);
        }
        return complex;
    }
    
    /**
     * Find next power of 2 greater than or equal to n
     */
    public static int nextPowerOfTwo(int n) {
        int power = 1;
        while (power < n) {
            power *= 2;
        }
        return power;
    }
    
    /**
     * Pad array with zeros to make length a power of 2
     */
    public static Complex[] padToPowerOfTwo(Complex[] x) {
        int newSize = nextPowerOfTwo(x.length);
        Complex[] padded = new Complex[newSize];
        
        for (int i = 0; i < x.length; i++) {
            padded[i] = x[i];
        }
        
        for (int i = x.length; i < newSize; i++) {
            padded[i] = new Complex(0, 0);
        }
        
        return padded;
    }
    
    public static void main(String[] args) {
        System.out.println("=== Fast Fourier Transform - Divide and Conquer ===");
        
        // Test Case 1: Simple 4-point signal
        System.out.println("Test Case 1: Simple 4-point signal");
        double[] signal1 = {1, 2, 3, 4};
        Complex[] complexSignal1 = realToComplex(signal1);
        
        System.out.print("Input signal: ");
        for (int i = 0; i < complexSignal1.length; i++) {
            System.out.printf("%.1f ", complexSignal1[i].real);
        }
        System.out.println();
        
        Complex[] fftResult1 = fft(complexSignal1);
        System.out.println("FFT result:");
        for (int i = 0; i < fftResult1.length; i++) {
            System.out.println("  X[" + i + "] = " + fftResult1[i]);
        }
        
        Complex[] ifftResult1 = ifft(fftResult1);
        System.out.print("IFFT result (should match input): ");
        for (int i = 0; i < ifftResult1.length; i++) {
            System.out.printf("%.1f ", ifftResult1[i].real);
        }
        System.out.println("\n");
        
        // Test Case 2: Sine wave
        System.out.println("Test Case 2: Sine wave (8 points)");
        int n2 = 8;
        double[] signal2 = new double[n2];
        for (int i = 0; i < n2; i++) {
            signal2[i] = Math.sin(2 * Math.PI * i / n2); // One complete cycle
        }
        
        Complex[] complexSignal2 = realToComplex(signal2);
        System.out.print("Input sine wave: ");
        for (int i = 0; i < complexSignal2.length; i++) {
            System.out.printf("%.3f ", complexSignal2[i].real);
        }
        System.out.println();
        
        Complex[] fftResult2 = fft(complexSignal2);
        System.out.println("FFT magnitudes:");
        for (int i = 0; i < fftResult2.length; i++) {
            System.out.printf("  |X[%d]| = %.3f\n", i, fftResult2[i].magnitude());
        }
        System.out.println();
        
        // Test Case 3: Performance comparison (FFT vs DFT)
        System.out.println("Test Case 3: Performance comparison");
        double[] signal3 = {1, 0, 1, 0, 1, 0, 1, 0}; // Square wave
        Complex[] complexSignal3 = realToComplex(signal3);
        
        System.out.print("Input square wave: ");
        for (int i = 0; i < complexSignal3.length; i++) {
            System.out.printf("%.0f ", complexSignal3[i].real);
        }
        System.out.println();
        
        // Time FFT
        long startFFT = System.nanoTime();
        Complex[] fftResult3 = fft(complexSignal3);
        long endFFT = System.nanoTime();
        
        // Time DFT
        long startDFT = System.nanoTime();
        Complex[] dftResult3 = dft(complexSignal3);
        long endDFT = System.nanoTime();
        
        System.out.println("FFT time: " + (endFFT - startFFT) + " nanoseconds");
        System.out.println("DFT time: " + (endDFT - startDFT) + " nanoseconds");
        System.out.println("Speedup: " + String.format("%.2fx", (double)(endDFT - startDFT) / (endFFT - startFFT)));
        
        // Verify results match
        boolean resultsMatch = true;
        for (int i = 0; i < fftResult3.length; i++) {
            double diff = Math.abs(fftResult3[i].real - dftResult3[i].real) + 
                         Math.abs(fftResult3[i].imag - dftResult3[i].imag);
            if (diff > 1e-10) {
                resultsMatch = false;
                break;
            }
        }
        System.out.println("Results match: " + resultsMatch);
        System.out.println();
        
        // Test Case 4: Non-power-of-2 input (with padding)
        System.out.println("Test Case 4: Non-power-of-2 input (with padding)");
        double[] signal4 = {1, 2, 3, 4, 5}; // Length 5
        Complex[] complexSignal4 = realToComplex(signal4);
        
        System.out.print("Original signal (length " + complexSignal4.length + "): ");
        for (int i = 0; i < complexSignal4.length; i++) {
            System.out.printf("%.0f ", complexSignal4[i].real);
        }
        System.out.println();
        
        Complex[] paddedSignal4 = padToPowerOfTwo(complexSignal4);
        System.out.print("Padded signal (length " + paddedSignal4.length + "): ");
        for (int i = 0; i < paddedSignal4.length; i++) {
            System.out.printf("%.0f ", paddedSignal4[i].real);
        }
        System.out.println();
        
        Complex[] fftResult4 = fft(paddedSignal4);
        System.out.println("FFT magnitudes:");
        for (int i = 0; i < fftResult4.length; i++) {
            System.out.printf("  |X[%d]| = %.3f\n", i, fftResult4[i].magnitude());
        }
    }
}