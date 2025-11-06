import java.util.*;

class Item {
    String name;
    int weight, value;
    double ratio;

    Item(String name, int weight, int value) {
        this.name = name;
        this.weight = weight;
        this.value = value;
        this.ratio = (double) value / weight;
    }
}

class Node {
    int level, profit, weight;
    double bound;
    List<String> itemsTaken;

    Node() {
        itemsTaken = new ArrayList<>();
    }
}

public class Branch_Bound {
    static int W = 7; // Knapsack capacity

    static Item[] items = {
            new Item("A", 1, 15),
            new Item("B", 3, 20),
            new Item("C", 4, 30),
            new Item("D", 5, 40)
    };

    static double bound(Node u, int n) {
        if (u.weight >= W)
            return 0;
        double profit_bound = u.profit;
        int j = u.level + 1;
        int totweight = u.weight;

        while (j < n && totweight + items[j].weight <= W) {
            totweight += items[j].weight;
            profit_bound += items[j].value;
            j++;
        }
        if (j < n)
            profit_bound += (W - totweight) * items[j].ratio;
        return profit_bound;
    }

    public static void main(String[] args) {
        int n = items.length;
        Arrays.sort(items, (a, b) -> Double.compare(b.ratio, a.ratio));

        Queue<Node> Q = new LinkedList<>();
        Node u = new Node(), v = new Node();
        u.level = -1;
        u.profit = 0;
        u.weight = 0;
        u.bound = bound(u, n);

        int maxProfit = 0;
        List<String> bestItems = new ArrayList<>();
        Q.offer(u);

        while (!Q.isEmpty()) {
            u = Q.poll();
            if (u.level == n - 1)
                continue;

            v = new Node();
            v.level = u.level + 1;
            v.weight = u.weight + items[v.level].weight;
            v.profit = u.profit + items[v.level].value;
            v.itemsTaken = new ArrayList<>(u.itemsTaken);
            v.itemsTaken.add(items[v.level].name);

            if (v.weight <= W && v.profit > maxProfit) {
                maxProfit = v.profit;
                bestItems = v.itemsTaken;
            }

            v.bound = bound(v, n);
            if (v.bound > maxProfit)
                Q.offer(v);

            v = new Node();
            v.level = u.level + 1;
            v.weight = u.weight;
            v.profit = u.profit;
            v.itemsTaken = new ArrayList<>(u.itemsTaken);

            v.bound = bound(v, n);
            if (v.bound > maxProfit)
                Q.offer(v);
        }

        System.out.println("Maximum value: " + maxProfit);
        System.out.println("Items taken: " + bestItems);
    }
}