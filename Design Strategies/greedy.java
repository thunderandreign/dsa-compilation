import java.io.*;
import java.util.*;

public class greedy {
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

    public static void main(String[] args) {
        Item[] items = {
                new Item("A", 10, 60),
                new Item("B", 20, 100),
                new Item("C", 30, 120),
                new Item("D", 5, 50)
        };
        int capacity = 20;

        // Sort items by value/size ratio in descending order
        Arrays.sort(items, (a, b) -> Double.compare(
                (double) b.value / b.size, (double) a.value / a.size));

        double totalValue = 0;
        int remainingCapacity = capacity;

        System.out.println("Selected items:");
        for (Item item : items) {
            if (item.size <= remainingCapacity) {
                System.out.println(item.name + " (size: " + item.size + ", value: " + item.value + ")");
                totalValue += item.value;
                remainingCapacity -= item.size;
            }
        }
        System.out.println("Total value: " + totalValue);
    }
}
