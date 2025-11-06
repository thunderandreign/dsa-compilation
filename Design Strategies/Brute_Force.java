import java.util.*;

public class Brute_Force {
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

    // Brute Force solution for 0/1 Knapsack Problem
    public static int knapsack(Item[] items, int capacity, List<Item> selected) {
        return knapsackHelper(items, capacity, 0, selected, new ArrayList<>());
    }

    private static int knapsackHelper(Item[] items, int capacity, int index, List<Item> selected, List<Item> current) {
        if (index == items.length || capacity == 0) {
            int value = current.stream().mapToInt(i -> i.value).sum();
            if (value > selected.stream().mapToInt(i -> i.value).sum()) {
                selected.clear();
                selected.addAll(current);
            }
            return value;
        }

        // Exclude current item
        int exclude = knapsackHelper(items, capacity, index + 1, selected, current);

        // Include current item if it fits
        int include = 0;
        if (items[index].size <= capacity) {
            current.add(items[index]);
            include = knapsackHelper(items, capacity - items[index].size, index + 1, selected, current);
            current.remove(current.size() - 1);
        }

        return Math.max(include, exclude);
    }

    public static void main(String[] args) {
        Item[] items = {
                new Item("A", 10, 60),
                new Item("B", 20, 100),
                new Item("C", 30, 120),
                new Item("D", 5, 50)
        };
        int capacity = 20;
        List<Item> selected = new ArrayList<>();
        int maxValue = knapsack(items, capacity, selected);

        System.out.println("Selected items:");
        for (Item item : selected) {
            System.out.println(item.name + " (size: " + item.size + ", value: " + item.value + ")");
        }
        System.out.println("Total value: " + maxValue);
    }
}
