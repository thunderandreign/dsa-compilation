#include <stdio.h>
#include <stdlib.h>

/**
 * Greedy Strategy: Activity Selection Problem
 * Core Idea: Always choose the activity that finishes earliest (and doesn't conflict)
 * Time Complexity: O(n log n) due to sorting by finish time
 * Space Complexity: O(1) excluding input
 */

/**
 * Activity structure
 */
typedef struct {
    int start;
    int finish;
    int index; // Original index for tracking
    char name[10]; // Activity name for display
} Activity;

/**
 * Compare function for sorting activities by finish time
 */
int compareByFinishTime(const void* a, const void* b) {
    Activity* actA = (Activity*)a;
    Activity* actB = (Activity*)b;
    return actA->finish - actB->finish;
}

/**
 * Solve activity selection using greedy approach
 * @param activities Array of activities
 * @param n Number of activities
 * @param selected Array to store selection status
 * @return Number of selected activities
 */
int activitySelection(Activity activities[], int n, int selected[]) {
    if (n == 0) return 0;
    
    // Step 1: Sort activities by finish time (greedy choice preparation)
    qsort(activities, n, sizeof(Activity), compareByFinishTime);
    
    // Initialize selection array
    for (int i = 0; i < n; i++) {
        selected[activities[i].index] = 0;
    }
    
    // Step 2: Always select the first activity (earliest finish time)
    selected[activities[0].index] = 1;
    int count = 1;
    int lastSelectedFinishTime = activities[0].finish;
    
    // Step 3: Greedily select non-conflicting activities
    for (int i = 1; i < n; i++) {
        // Greedy choice: if activity starts after last selected activity finishes
        if (activities[i].start >= lastSelectedFinishTime) {
            selected[activities[i].index] = 1;
            lastSelectedFinishTime = activities[i].finish;
            count++;
        }
    }
    
    return count;
}

/**
 * Check if two activities conflict
 */
int hasConflict(Activity a, Activity b) {
    return !(a.finish <= b.start || b.finish <= a.start);
}

/**
 * Print activities information
 */
void printActivities(Activity originalActivities[], int n) {
    printf("Activities (Name, Start, Finish):\n");
    for (int i = 0; i < n; i++) {
        printf("  %s: [%d, %d]\n", 
               originalActivities[i].name, 
               originalActivities[i].start, 
               originalActivities[i].finish);
    }
}

/**
 * Print selection result
 */
void printSelection(Activity originalActivities[], int n, int selected[], int count) {
    printf("Selected Activities (%d total):\n", count);
    
    int selectionOrder = 1;
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            printf("  %d. %s: [%d, %d]\n", 
                   selectionOrder++, originalActivities[i].name,
                   originalActivities[i].start, originalActivities[i].finish);
        }
    }
}

/**
 * Demonstrate greedy algorithm step by step
 */
void demonstrateGreedySteps(Activity activities[], int n) {
    printf("Greedy Algorithm Steps:\n");
    
    // Create copy for sorting
    Activity* sortedActivities = (Activity*)malloc(n * sizeof(Activity));
    for (int i = 0; i < n; i++) {
        sortedActivities[i] = activities[i];
    }
    
    qsort(sortedActivities, n, sizeof(Activity), compareByFinishTime);
    
    printf("1. Sort by finish time:\n");
    for (int i = 0; i < n; i++) {
        printf("   %s: [%d, %d]\n", 
               sortedActivities[i].name, 
               sortedActivities[i].start, 
               sortedActivities[i].finish);
    }
    
    printf("2. Greedy selection process:\n");
    printf("   - Select %s (first activity, finishes at %d)\n", 
           sortedActivities[0].name, sortedActivities[0].finish);
    
    int lastFinish = sortedActivities[0].finish;
    int count = 1;
    
    for (int i = 1; i < n; i++) {
        if (sortedActivities[i].start >= lastFinish) {
            printf("   - Select %s (starts at %d >= %d, finishes at %d)\n", 
                   sortedActivities[i].name, sortedActivities[i].start, 
                   lastFinish, sortedActivities[i].finish);
            lastFinish = sortedActivities[i].finish;
            count++;
        } else {
            printf("   - Skip %s (starts at %d < %d, conflicts with previous)\n", 
                   sortedActivities[i].name, sortedActivities[i].start, lastFinish);
        }
    }
    
    printf("3. Total selected: %d activities\n", count);
    free(sortedActivities);
}

/**
 * Create timeline visualization
 */
void printTimeline(Activity originalActivities[], int n, int selected[]) {
    printf("Timeline Visualization:\n");
    
    // Find time range
    int minTime = originalActivities[0].start;
    int maxTime = originalActivities[0].finish;
    
    for (int i = 1; i < n; i++) {
        if (originalActivities[i].start < minTime) minTime = originalActivities[i].start;
        if (originalActivities[i].finish > maxTime) maxTime = originalActivities[i].finish;
    }
    
    // Print time scale
    printf("Time: ");
    for (int t = minTime; t <= maxTime; t++) {
        printf("%2d ", t);
    }
    printf("\n");
    
    // Print each activity
    for (int i = 0; i < n; i++) {
        printf("%-4s: ", originalActivities[i].name);
        
        for (int t = minTime; t <= maxTime; t++) {
            if (t >= originalActivities[i].start && t < originalActivities[i].finish) {
                printf(selected[i] ? " * " : " - ");
            } else {
                printf("   ");
            }
        }
        
        printf(" %s\n", selected[i] ? "(Selected)" : "(Skipped)");
    }
    
    printf("Legend: * = Selected activity, - = Unselected activity\n");
}

/**
 * Compare with alternative selection strategy
 */
void compareStrategies(Activity activities[], int n) {
    printf("Strategy Comparison:\n");
    
    // Greedy by finish time
    Activity* greedyActivities = (Activity*)malloc(n * sizeof(Activity));
    for (int i = 0; i < n; i++) {
        greedyActivities[i] = activities[i];
    }
    
    int* greedySelected = (int*)calloc(n, sizeof(int));
    int greedyCount = activitySelection(greedyActivities, n, greedySelected);
    
    printf("1. Greedy by finish time: %d activities\n", greedyCount);
    
    // Alternative: Greedy by duration (shortest first)
    printf("2. Alternative - Greedy by shortest duration:\n");
    
    // Find shortest duration activities
    int shortestCount = 0;
    int* shortestSelected = (int*)calloc(n, sizeof(int));
    
    // Simple greedy by shortest duration (not optimal, just for comparison)
    int used[n];
    for (int i = 0; i < n; i++) used[i] = 0;
    
    while (1) {
        int shortestIdx = -1;
        int shortestDuration = 1000;
        
        // Find shortest unused activity that doesn't conflict
        for (int i = 0; i < n; i++) {
            if (used[i]) continue;
            
            int duration = activities[i].finish - activities[i].start;
            if (duration < shortestDuration) {
                // Check conflicts with already selected
                int hasConflictWithSelected = 0;
                for (int j = 0; j < n; j++) {
                    if (shortestSelected[j] && hasConflict(activities[i], activities[j])) {
                        hasConflictWithSelected = 1;
                        break;
                    }
                }
                
                if (!hasConflictWithSelected) {
                    shortestIdx = i;
                    shortestDuration = duration;
                }
            }
        }
        
        if (shortestIdx == -1) break; // No more activities can be added
        
        shortestSelected[shortestIdx] = 1;
        used[shortestIdx] = 1;
        shortestCount++;
    }
    
    printf("   Selected %d activities (may not be optimal)\n", shortestCount);
    printf("   Greedy by finish time is provably optimal!\n");
    
    free(greedyActivities);
    free(greedySelected);
    free(shortestSelected);
}

int main() {
    printf("=== Activity Selection - Greedy Algorithm ===\n");
    
    // Test Case 1: Classic example
    printf("Test Case 1: Classic activity selection\n");
    Activity activities1[] = {
        {1, 4, 0, "A1"},
        {3, 5, 1, "A2"},
        {0, 6, 2, "A3"},
        {5, 7, 3, "A4"},
        {3, 9, 4, "A5"},
        {5, 9, 5, "A6"},
        {6, 10, 6, "A7"},
        {8, 11, 7, "A8"},
        {8, 12, 8, "A9"},
        {2, 14, 9, "A10"},
        {12, 16, 10, "A11"}
    };
    int n1 = 11;
    int selected1[11];
    
    printActivities(activities1, n1);
    printf("\n");
    
    int count1 = activitySelection(activities1, n1, selected1);
    printSelection(activities1, n1, selected1, count1);
    printf("\n");
    
    demonstrateGreedySteps(activities1, n1);
    printf("\n");
    
    printTimeline(activities1, n1, selected1);
    printf("\n");
    
    // Test Case 2: Simple non-overlapping case
    printf("Test Case 2: Simple non-overlapping activities\n");
    Activity activities2[] = {
        {1, 3, 0, "B1"},
        {4, 6, 1, "B2"},
        {7, 9, 2, "B3"}
    };
    int n2 = 3;
    int selected2[3];
    
    printActivities(activities2, n2);
    printf("\n");
    
    int count2 = activitySelection(activities2, n2, selected2);
    printSelection(activities2, n2, selected2, count2);
    printf("Note: All activities selected since none overlap\n\n");
    
    // Test Case 3: All activities overlap
    printf("Test Case 3: All activities overlap\n");
    Activity activities3[] = {
        {1, 5, 0, "C1"},
        {2, 6, 1, "C2"},
        {3, 7, 2, "C3"},
        {2, 8, 3, "C4"}
    };
    int n3 = 4;
    int selected3[4];
    
    printActivities(activities3, n3);
    printf("\n");
    
    int count3 = activitySelection(activities3, n3, selected3);
    printSelection(activities3, n3, selected3, count3);
    printf("Note: Only one activity selected since all overlap\n");
    printf("Greedy chooses the one that finishes earliest (C1)\n\n");
    
    // Test Case 4: Edge case - single activity
    printf("Test Case 4: Single activity\n");
    Activity activities4[] = {
        {2, 5, 0, "D1"}
    };
    int n4 = 1;
    int selected4[1];
    
    printActivities(activities4, n4);
    printf("\n");
    
    int count4 = activitySelection(activities4, n4, selected4);
    printSelection(activities4, n4, selected4, count4);
    printf("\n");
    
    // Test Case 5: Strategy comparison
    printf("Test Case 5: Comparing selection strategies\n");
    Activity activities5[] = {
        {0, 2, 0, "E1"},  // Duration: 2
        {1, 100, 1, "E2"}, // Duration: 99 (long, blocks many)
        {3, 4, 2, "E3"},  // Duration: 1
        {5, 6, 3, "E4"},  // Duration: 1
        {7, 8, 4, "E5"}   // Duration: 1
    };
    int n5 = 5;
    int selected5[5];
    
    printActivities(activities5, n5);
    printf("\n");
    
    int count5 = activitySelection(activities5, n5, selected5);
    printSelection(activities5, n5, selected5, count5);
    printf("\n");
    
    compareStrategies(activities5, n5);
    
    return 0;
}