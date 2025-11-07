import java.util.*;

/**
 * Greedy Strategy: Activity Selection Problem
 * Core Idea: Always choose the activity that finishes earliest and doesn't conflict
 * Time Complexity: O(n log n) due to sorting by finish time
 * Space Complexity: O(n) for storing activities and results
 */
public class ActivitySelection {
    
    /**
     * Activity class to represent activities with start and finish times
     */
    static class Activity {
        int id;
        int startTime;
        int finishTime;
        String name;
        
        Activity(int id, String name, int startTime, int finishTime) {
            this.id = id;
            this.name = name;
            this.startTime = startTime;
            this.finishTime = finishTime;
        }
        
        @Override
        public String toString() {
            return String.format("A%d:%s [%d-%d]", id, name, startTime, finishTime);
        }
    }
    
    /**
     * Select maximum number of non-overlapping activities using greedy approach
     * @param activities Array of activities
     * @return List of selected activities
     */
    public static List<Activity> selectActivities(Activity[] activities) {
        if (activities.length == 0) {
            return new ArrayList<>();
        }
        
        // Step 1: Sort activities by finish time (greedy choice preparation)
        Arrays.sort(activities, (a, b) -> Integer.compare(a.finishTime, b.finishTime));
        
        List<Activity> selected = new ArrayList<>();
        
        // Step 2: Always select the first activity (earliest finish time)
        selected.add(activities[0]);
        int lastFinishTime = activities[0].finishTime;
        
        // Step 3: For remaining activities, select if it starts after last selected finishes
        for (int i = 1; i < activities.length; i++) {
            // Greedy choice: if current activity starts after last selected finishes
            if (activities[i].startTime >= lastFinishTime) {
                selected.add(activities[i]);
                lastFinishTime = activities[i].finishTime;
            }
        }
        
        return selected;
    }
    
    /**
     * Alternative implementation: recursive approach
     */
    public static List<Activity> selectActivitiesRecursive(Activity[] activities) {
        if (activities.length == 0) {
            return new ArrayList<>();
        }
        
        // Sort by finish time
        Arrays.sort(activities, (a, b) -> Integer.compare(a.finishTime, b.finishTime));
        
        List<Activity> result = new ArrayList<>();
        selectActivitiesRecursiveHelper(activities, 0, -1, result);
        return result;
    }
    
    /**
     * Recursive helper for activity selection
     * @param activities Sorted array of activities
     * @param index Current index being considered
     * @param lastSelected Index of last selected activity (-1 if none)
     * @param result List to store selected activities
     */
    private static void selectActivitiesRecursiveHelper(Activity[] activities, int index, 
                                                       int lastSelected, List<Activity> result) {
        // Base case: no more activities to consider
        if (index >= activities.length) {
            return;
        }
        
        // If this activity doesn't overlap with last selected, choose it
        if (lastSelected == -1 || activities[index].startTime >= activities[lastSelected].finishTime) {
            result.add(activities[index]);
            selectActivitiesRecursiveHelper(activities, index + 1, index, result);
        } else {
            // Skip current activity and move to next
            selectActivitiesRecursiveHelper(activities, index + 1, lastSelected, result);
        }
    }
    
    /**
     * Calculate total time covered by selected activities
     */
    public static int calculateTotalTime(List<Activity> activities) {
        int totalTime = 0;
        for (Activity activity : activities) {
            totalTime += (activity.finishTime - activity.startTime);
        }
        return totalTime;
    }
    
    /**
     * Print activities in a formatted table
     */
    public static void printActivities(Activity[] activities, String title) {
        System.out.println(title);
        System.out.println("ID | Name      | Start | Finish | Duration");
        System.out.println("---|-----------|-------|--------|----------");
        
        for (Activity activity : activities) {
            int duration = activity.finishTime - activity.startTime;
            System.out.printf("%-2d | %-9s | %-5d | %-6d | %-8d\n", 
                            activity.id, activity.name, activity.startTime, 
                            activity.finishTime, duration);
        }
    }
    
    /**
     * Print selected activities with timeline
     */
    public static void printSelectedActivities(List<Activity> selected) {
        System.out.println("Selected activities (in execution order):");
        for (int i = 0; i < selected.size(); i++) {
            Activity activity = selected.get(i);
            System.out.printf("%d. %s\n", i + 1, activity);
        }
        
        System.out.println("\nTimeline visualization:");
        if (!selected.isEmpty()) {
            int maxTime = selected.get(selected.size() - 1).finishTime;
            System.out.print("Time: ");
            for (int t = 0; t <= maxTime; t += 2) {
                System.out.printf("%-3d", t);
            }
            System.out.println();
            
            for (Activity activity : selected) {
                System.out.printf("A%-2d: ", activity.id);
                for (int t = 0; t <= maxTime; t += 2) {
                    if (t >= activity.startTime && t < activity.finishTime) {
                        System.out.print("███");
                    } else {
                        System.out.print("   ");
                    }
                }
                System.out.println(" " + activity.name);
            }
        }
    }
    
    /**
     * Show why greedy choice works by comparing with other strategies
     */
    public static void demonstrateGreedyChoice(Activity[] activities) {
        System.out.println("Demonstrating why greedy choice (earliest finish time) works:");
        
        // Sort by finish time
        Activity[] byFinish = Arrays.copyOf(activities, activities.length);
        Arrays.sort(byFinish, (a, b) -> Integer.compare(a.finishTime, b.finishTime));
        
        // Sort by start time
        Activity[] byStart = Arrays.copyOf(activities, activities.length);
        Arrays.sort(byStart, (a, b) -> Integer.compare(a.startTime, b.startTime));
        
        // Sort by duration (shortest first)
        Activity[] byDuration = Arrays.copyOf(activities, activities.length);
        Arrays.sort(byDuration, (a, b) -> Integer.compare(
            a.finishTime - a.startTime, b.finishTime - b.startTime));
        
        System.out.println("1. Greedy by finish time: " + selectActivities(byFinish).size() + " activities");
        System.out.println("2. Greedy by start time: " + selectActivitiesByStart(byStart).size() + " activities");
        System.out.println("3. Greedy by duration: " + selectActivitiesByStart(byDuration).size() + " activities");
        System.out.println("Note: Finish time strategy typically gives optimal result");
    }
    
    /**
     * Helper method: select activities by start time (for comparison)
     */
    private static List<Activity> selectActivitiesByStart(Activity[] activities) {
        if (activities.length == 0) return new ArrayList<>();
        
        List<Activity> selected = new ArrayList<>();
        selected.add(activities[0]);
        int lastFinishTime = activities[0].finishTime;
        
        for (int i = 1; i < activities.length; i++) {
            if (activities[i].startTime >= lastFinishTime) {
                selected.add(activities[i]);
                lastFinishTime = activities[i].finishTime;
            }
        }
        
        return selected;
    }
    
    public static void main(String[] args) {
        System.out.println("=== Activity Selection - Greedy Algorithm ===");
        
        // Test Case 1: Classic activity selection problem
        System.out.println("Test Case 1: Classic activity selection");
        Activity[] activities1 = {
            new Activity(1, "Meeting", 1, 4),
            new Activity(2, "Workshop", 3, 5),
            new Activity(3, "Lunch", 0, 6),
            new Activity(4, "Training", 5, 7),
            new Activity(5, "Review", 3, 9),
            new Activity(6, "Planning", 5, 9),
            new Activity(7, "Demo", 6, 10),
            new Activity(8, "Testing", 8, 11),
            new Activity(9, "Deploy", 8, 12),
            new Activity(10, "Cleanup", 2, 14),
            new Activity(11, "Report", 12, 16)
        };
        
        printActivities(activities1, "Available activities:");
        System.out.println();
        
        List<Activity> selected1 = selectActivities(activities1);
        System.out.println("Maximum activities selected: " + selected1.size());
        printSelectedActivities(selected1);
        System.out.println("Total duration: " + calculateTotalTime(selected1) + " units");
        System.out.println();
        
        demonstrateGreedyChoice(activities1);
        System.out.println();
        
        // Test Case 2: All activities overlap
        System.out.println("Test Case 2: All activities overlap");
        Activity[] activities2 = {
            new Activity(1, "Task A", 0, 10),
            new Activity(2, "Task B", 2, 8),
            new Activity(3, "Task C", 4, 6),
            new Activity(4, "Task D", 1, 9)
        };
        
        printActivities(activities2, "Overlapping activities:");
        System.out.println();
        
        List<Activity> selected2 = selectActivities(activities2);
        System.out.println("Maximum activities selected: " + selected2.size());
        printSelectedActivities(selected2);
        System.out.println();
        
        // Test Case 3: No overlaps (all activities can be selected)
        System.out.println("Test Case 3: No overlapping activities");
        Activity[] activities3 = {
            new Activity(1, "Morning", 0, 2),
            new Activity(2, "Noon", 2, 4),
            new Activity(3, "Afternoon", 4, 6),
            new Activity(4, "Evening", 6, 8)
        };
        
        printActivities(activities3, "Non-overlapping activities:");
        System.out.println();
        
        List<Activity> selected3 = selectActivities(activities3);
        System.out.println("Maximum activities selected: " + selected3.size());
        printSelectedActivities(selected3);
        System.out.println();
        
        // Test Case 4: Single activity
        System.out.println("Test Case 4: Single activity");
        Activity[] activities4 = {
            new Activity(1, "Solo", 5, 10)
        };
        
        List<Activity> selected4 = selectActivities(activities4);
        System.out.println("Activities selected: " + selected4.size());
        System.out.println("Selected: " + selected4.get(0));
        System.out.println();
        
        // Test Case 5: Compare iterative vs recursive
        System.out.println("Test Case 5: Iterative vs Recursive comparison");
        List<Activity> iterativeResult = selectActivities(activities1);
        List<Activity> recursiveResult = selectActivitiesRecursive(activities1);
        
        System.out.println("Iterative approach: " + iterativeResult.size() + " activities");
        System.out.println("Recursive approach: " + recursiveResult.size() + " activities");
        System.out.println("Results match: " + (iterativeResult.size() == recursiveResult.size()));
    }
}