#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int size;
    int value;
} Item;

int knapsack(Item items[], int n, int capacity, int selected[]) {
    int dp[n + 1][capacity + 1];
    memset(dp, 0, sizeof(dp));

    // Build table dp[][] in bottom up manner
    for (int i = 1; i <= n; i++) {
        for (int w = 0; w <= capacity; w++) {
            if (items[i - 1].size <= w) {
                int include = items[i - 1].value + dp[i - 1][w - items[i - 1].size];
                int exclude = dp[i - 1][w];
                dp[i][w] = (include > exclude) ? include : exclude;
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Find which items are selected
    int w = capacity;
    for (int i = n; i > 0 && w > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            selected[i - 1] = 1;
            w -= items[i - 1].size;
        } else {
            selected[i - 1] = 0;
        }
    }

    return dp[n][capacity];
}

int main() {
    Item items[] = {
        {"A", 10, 60},
        {"B", 20, 100},
        {"C", 30, 120},
        {"D", 5, 50}
    };
    int n = sizeof(items) / sizeof(items[0]);
    int capacity = 20;
    int selected[100] = {0};

    int maxValue = knapsack(items, n, capacity, selected);

    printf("Selected items:\n");
    for (int i = 0; i < n; i++) {
        if (selected[i]) {
            printf("%s (size: %d, value: %d)\n", items[i].name, items[i].size, items[i].value);
        }
    }
    printf("Total value: %d\n", maxValue);

    return 0;
}