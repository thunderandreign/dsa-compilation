/**
 * Brute Force Strategy: Password Cracking
 * Core Idea: Try every possible combination of characters until password is found
 * Time Complexity: O(c^n) where c is character set size, n is password length
 * Space Complexity: O(n) for recursion stack and current attempt
 */
public class PasswordCracking {
    
    // Character sets for different password types
    private static final String LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
    private static final String UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    private static final String DIGITS = "0123456789";
    private static final String SYMBOLS = "!@#$%^&*";
    
    /**
     * Attempt to crack a password using brute force approach
     * @param targetPassword The password to crack
     * @param charset Character set to use for attempts
     * @param maxLength Maximum length to try
     * @return The cracked password or null if not found
     */
    public static String crackPassword(String targetPassword, String charset, int maxLength) {
        // Try all possible lengths from 1 to maxLength
        for (int length = 1; length <= maxLength; length++) {
            String result = tryAllCombinations(targetPassword, charset, length, "", 0);
            if (result != null) {
                return result;
            }
        }
        return null; // Password not found
    }
    
    /**
     * Recursively try all combinations of given length
     * @param target Target password
     * @param charset Available characters
     * @param length Target length
     * @param current Current password being built
     * @param attempts Counter for number of attempts (passed by reference concept)
     * @return Found password or null
     */
    private static String tryAllCombinations(String target, String charset, int length, 
                                           String current, int attempts) {
        // Base case: reached target length
        if (current.length() == length) {
            attempts++;
            if (attempts % 10000 == 0) {
                System.out.println("Attempts: " + attempts + ", Current: " + current);
            }
            
            if (current.equals(target)) {
                System.out.println("Password cracked! Attempts: " + attempts);
                return current;
            }
            return null;
        }
        
        // Try each character in the charset at current position
        for (int i = 0; i < charset.length(); i++) {
            String newCurrent = current + charset.charAt(i);
            String result = tryAllCombinations(target, charset, length, newCurrent, attempts);
            if (result != null) {
                return result;
            }
        }
        
        return null;
    }
    
    /**
     * Crack password with attempt counting
     * @param targetPassword Password to crack
     * @param charset Character set to use
     * @param maxLength Maximum length to try
     * @return Result object containing password and attempt count
     */
    public static CrackResult crackPasswordWithCount(String targetPassword, String charset, int maxLength) {
        int totalAttempts = 0;
        
        for (int length = 1; length <= maxLength; length++) {
            CrackResult result = tryAllCombinationsWithCount(targetPassword, charset, length, "", totalAttempts);
            if (result.found) {
                return result;
            }
            totalAttempts = result.attempts;
        }
        
        return new CrackResult(null, false, totalAttempts);
    }
    
    /**
     * Helper method that tracks attempt count
     */
    private static CrackResult tryAllCombinationsWithCount(String target, String charset, int length, 
                                                         String current, int startAttempts) {
        if (current.length() == length) {
            startAttempts++;
            if (current.equals(target)) {
                return new CrackResult(current, true, startAttempts);
            }
            return new CrackResult(null, false, startAttempts);
        }
        
        for (int i = 0; i < charset.length(); i++) {
            String newCurrent = current + charset.charAt(i);
            CrackResult result = tryAllCombinationsWithCount(target, charset, length, newCurrent, startAttempts);
            if (result.found) {
                return result;
            }
            startAttempts = result.attempts;
        }
        
        return new CrackResult(null, false, startAttempts);
    }
    
    /**
     * Calculate theoretical maximum attempts needed
     * @param charset Character set size
     * @param maxLength Maximum password length
     * @return Maximum possible attempts
     */
    public static long calculateMaxAttempts(String charset, int maxLength) {
        long total = 0;
        long charsetSize = charset.length();
        
        for (int length = 1; length <= maxLength; length++) {
            total += Math.pow(charsetSize, length);
        }
        
        return total;
    }
    
    /**
     * Result class to hold cracking results
     */
    static class CrackResult {
        String password;
        boolean found;
        int attempts;
        
        CrackResult(String password, boolean found, int attempts) {
            this.password = password;
            this.found = found;
            this.attempts = attempts;
        }
    }
    
    public static void main(String[] args) {
        System.out.println("=== Password Cracking - Brute Force ===");
        
        // Test Case 1: Simple numeric password
        System.out.println("Test Case 1: Numeric password");
        String target1 = "123";
        String charset1 = DIGITS;
        System.out.println("Target password: \"" + target1 + "\"");
        System.out.println("Character set: " + charset1);
        System.out.println("Max theoretical attempts: " + calculateMaxAttempts(charset1, 3));
        
        CrackResult result1 = crackPasswordWithCount(target1, charset1, 4);
        if (result1.found) {
            System.out.println("Success! Password: \"" + result1.password + "\"");
            System.out.println("Actual attempts: " + result1.attempts);
        } else {
            System.out.println("Failed to crack password");
        }
        System.out.println();
        
        // Test Case 2: Short lowercase password
        System.out.println("Test Case 2: Lowercase password");
        String target2 = "cat";
        String charset2 = LOWERCASE;
        System.out.println("Target password: \"" + target2 + "\"");
        System.out.println("Character set: " + charset2.substring(0, 10) + "..."); // Show first 10 chars
        
        CrackResult result2 = crackPasswordWithCount(target2, charset2, 3);
        if (result2.found) {
            System.out.println("Success! Password: \"" + result2.password + "\"");
            System.out.println("Actual attempts: " + result2.attempts);
        } else {
            System.out.println("Failed to crack password");
        }
        System.out.println();
        
        // Test Case 3: Single character password
        System.out.println("Test Case 3: Single character");
        String target3 = "X";
        String charset3 = UPPERCASE;
        System.out.println("Target password: \"" + target3 + "\"");
        
        CrackResult result3 = crackPasswordWithCount(target3, charset3, 2);
        if (result3.found) {
            System.out.println("Success! Password: \"" + result3.password + "\"");
            System.out.println("Actual attempts: " + result3.attempts);
        } else {
            System.out.println("Failed to crack password");
        }
        System.out.println();
        
        // Test Case 4: Password with mixed case (demonstrate limitation)
        System.out.println("Test Case 4: Mixed character set");
        String target4 = "A1";
        String charset4 = UPPERCASE + DIGITS;
        System.out.println("Target password: \"" + target4 + "\"");
        System.out.println("Character set size: " + charset4.length());
        
        CrackResult result4 = crackPasswordWithCount(target4, charset4, 2);
        if (result4.found) {
            System.out.println("Success! Password: \"" + result4.password + "\"");
            System.out.println("Actual attempts: " + result4.attempts);
        } else {
            System.out.println("Failed to crack password");
        }
        System.out.println();
        
        // Test Case 5: Demonstrate exponential growth
        System.out.println("Test Case 5: Complexity demonstration");
        System.out.println("Theoretical attempts for different lengths (digits only):");
        for (int len = 1; len <= 5; len++) {
            long attempts = (long) Math.pow(10, len);
            System.out.println("Length " + len + ": up to " + attempts + " attempts");
        }
    }
}