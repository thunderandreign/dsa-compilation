#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Brute Force Strategy: Password Cracking
 * Core Idea: Try every possible combination of characters until password is found
 * Time Complexity: O(c^n) where c is character set size, n is password length
 * Space Complexity: O(n) for recursion stack and current attempt
 */

// Character sets for different password types
const char* LOWERCASE = "abcdefghijklmnopqrstuvwxyz";
const char* UPPERCASE = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char* DIGITS = "0123456789";
const char* SYMBOLS = "!@#$%^&*";

typedef struct {
    char password[100];
    bool found;
    int attempts;
} CrackResult;

/**
 * Calculate power function for integers
 */
long long power(int base, int exp) {
    long long result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

/**
 * Recursively try all combinations of given length
 * @param target Target password
 * @param charset Available characters
 * @param maxLength Target length
 * @param current Current password being built
 * @param currentLength Current length of password being built
 * @param attempts Pointer to attempt counter
 * @return true if password found, false otherwise
 */
bool tryAllCombinations(const char* target, const char* charset, int maxLength,
                       char* current, int currentLength, int* attempts) {
    // Base case: reached target length
    if (currentLength == maxLength) {
        current[currentLength] = '\0'; // Null terminate
        (*attempts)++;
        
        if (*attempts % 10000 == 0) {
            printf("Attempts: %d, Current: %s\n", *attempts, current);
        }
        
        if (strcmp(current, target) == 0) {
            printf("Password cracked! Attempts: %d\n", *attempts);
            return true;
        }
        return false;
    }
    
    // Try each character in the charset at current position
    int charsetLen = strlen(charset);
    for (int i = 0; i < charsetLen; i++) {
        current[currentLength] = charset[i];
        if (tryAllCombinations(target, charset, maxLength, current, currentLength + 1, attempts)) {
            return true;
        }
    }
    
    return false;
}

/**
 * Attempt to crack a password using brute force approach
 * @param targetPassword The password to crack
 * @param charset Character set to use for attempts
 * @param maxLength Maximum length to try
 * @param result Pointer to result structure
 */
void crackPassword(const char* targetPassword, const char* charset, int maxLength, CrackResult* result) {
    result->found = false;
    result->attempts = 0;
    strcpy(result->password, "");
    
    char current[100];
    int targetLen = strlen(targetPassword);
    
    // Try all possible lengths from 1 to maxLength
    for (int length = 1; length <= maxLength; length++) {
        if (tryAllCombinations(targetPassword, charset, length, current, 0, &result->attempts)) {
            strcpy(result->password, targetPassword);
            result->found = true;
            return;
        }
        
        // If we know the target length, we can stop early
        if (length == targetLen && result->found) {
            break;
        }
    }
}

/**
 * Calculate theoretical maximum attempts needed
 * @param charsetSize Size of character set
 * @param maxLength Maximum password length
 * @return Maximum possible attempts
 */
long long calculateMaxAttempts(int charsetSize, int maxLength) {
    long long total = 0;
    
    for (int length = 1; length <= maxLength; length++) {
        total += power(charsetSize, length);
    }
    
    return total;
}

/**
 * Helper function to print character set info
 */
void printCharsetInfo(const char* charset) {
    int len = strlen(charset);
    if (len <= 20) {
        printf("Character set: %s\n", charset);
    } else {
        printf("Character set: %.10s... (length: %d)\n", charset, len);
    }
}

int main() {
    printf("=== Password Cracking - Brute Force ===\n");
    
    // Test Case 1: Simple numeric password
    printf("Test Case 1: Numeric password\n");
    const char* target1 = "123";
    const char* charset1 = DIGITS;
    printf("Target password: \"%s\"\n", target1);
    printCharsetInfo(charset1);
    printf("Max theoretical attempts: %lld\n", calculateMaxAttempts(strlen(charset1), 3));
    
    CrackResult result1;
    crackPassword(target1, charset1, 4, &result1);
    if (result1.found) {
        printf("Success! Password: \"%s\"\n", result1.password);
        printf("Actual attempts: %d\n", result1.attempts);
    } else {
        printf("Failed to crack password\n");
    }
    printf("\n");
    
    // Test Case 2: Short lowercase password
    printf("Test Case 2: Lowercase password\n");
    const char* target2 = "cat";
    const char* charset2 = LOWERCASE;
    printf("Target password: \"%s\"\n", target2);
    printCharsetInfo(charset2);
    
    CrackResult result2;
    crackPassword(target2, charset2, 3, &result2);
    if (result2.found) {
        printf("Success! Password: \"%s\"\n", result2.password);
        printf("Actual attempts: %d\n", result2.attempts);
    } else {
        printf("Failed to crack password\n");
    }
    printf("\n");
    
    // Test Case 3: Single character password
    printf("Test Case 3: Single character\n");
    const char* target3 = "X";
    const char* charset3 = UPPERCASE;
    printf("Target password: \"%s\"\n", target3);
    printCharsetInfo(charset3);
    
    CrackResult result3;
    crackPassword(target3, charset3, 2, &result3);
    if (result3.found) {
        printf("Success! Password: \"%s\"\n", result3.password);
        printf("Actual attempts: %d\n", result3.attempts);
    } else {
        printf("Failed to crack password\n");
    }
    printf("\n");
    
    // Test Case 4: Two character numeric
    printf("Test Case 4: Two digit number\n");
    const char* target4 = "42";
    const char* charset4 = DIGITS;
    printf("Target password: \"%s\"\n", target4);
    printCharsetInfo(charset4);
    
    CrackResult result4;
    crackPassword(target4, charset4, 2, &result4);
    if (result4.found) {
        printf("Success! Password: \"%s\"\n", result4.password);
        printf("Actual attempts: %d\n", result4.attempts);
    } else {
        printf("Failed to crack password\n");
    }
    printf("\n");
    
    // Test Case 5: Demonstrate exponential growth
    printf("Test Case 5: Complexity demonstration\n");
    printf("Theoretical attempts for different lengths (digits only):\n");
    int digitLen = strlen(DIGITS);
    for (int len = 1; len <= 5; len++) {
        long long attempts = power(digitLen, len);
        printf("Length %d: up to %lld attempts\n", len, attempts);
    }
    
    return 0;
}