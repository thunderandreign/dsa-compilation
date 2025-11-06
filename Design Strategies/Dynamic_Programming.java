public class Dynamic_Programming {

    static class Item {
        String name;
        int size;
        int value;

        Item(String name, int size, int value) {
            this.name = name;
            this.size = size;
            this.value = value;
        }
    }

    // Dynamic Programming solution for 0/1 Knapsack Problem
    public static int knapsack(Item[] items, int capacity, boolean[] selected) {
        int n = items.length;
        int[][] dp = new int[n + 1][capacity + 1];

        // Build table dp[][] in bottom up manner
        for (int i = 1; i <= n; i++) {
            for (int w = 0; w <= capacity; w++) {
                if (items[i - 1].size <= w) {
                    dp[i][w] = Math.max(
                            items[i - 1].value + dp[i - 1][w - items[i - 1].size],
                            dp[i - 1][w]);
                } else {
                    dp[i][w] = dp[i - 1][w];
                }
            }
        }

        // Find which items are selected
        int w = capacity;
        for (int i = n; i > 0 && w > 0; i--) {
            if (dp[i][w] != dp[i - 1][w]) {
                selected[i - 1] = true;
                w -= items[i - 1].size;
            }
        }

        return dp[n][capacity];
    }

    public static void main(String[] args) {
        Item[] items = {
                new Item("A", 10, 60),
                new Item("B", 20, 100),
                new Item("C", 30, 120),
                new Item("D", 5, 50)
        };
        int capacity = 20;
        boolean[] selected = new boolean[items.length];
        int maxValue = knapsack(items, capacity, selected);

        System.out.println("Selected items:");
        for (int i = 0; i < items.length; i++) {
            if (selected[i]) {
                System.out.println(items[i].name + " (size: " + items[i].size + ", value: " + items[i].value + ")");
            }
        }
        System.out.println("Total value: " + maxValue);
    }
}
