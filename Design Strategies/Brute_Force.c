#include <stdio.h>
#include <string.h>

typedef struct {
    char name[20];
    int size;
    int value;
} Item;

int maxValue = 0;
int bestSelected[100];
int tempSelected[100];

int knapsack(Item items[], int n, int capacity, int index, int currValue, int currSize) {
    if (index == n || capacity == 0) {
        if (currValue > maxValue) {
            maxValue = currValue;
            memcpy(bestSelected, tempSelected, sizeof(int) * n);
        }
        return currValue;
    }

    // Exclude current item
    tempSelected[index] = 0;
    int exclude = knapsack(items, n, capacity, index + 1, currValue, currSize);

    // Include current item if it fits
    int include = 0;
    if (items[index].size <= capacity) {
        tempSelected[index] = 1;
        include = knapsack(items, n, capacity - items[index].size, index + 1, currValue + items[index].value, currSize + items[index].size);
    }

    return (include > exclude) ? include : exclude;
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

    memset(bestSelected, 0, sizeof(bestSelected));
    memset(tempSelected, 0, sizeof(tempSelected));
    int result = knapsack(items, n, capacity, 0, 0, 0);

    printf("Selected items:\n");
    for (int i = 0; i < n; i++) {
        if (bestSelected[i]) {
            printf("%s (size: %d, value: %d)\n", items[i].name, items[i].size, items[i].value);
        }
    }
    printf("Total value: %d\n", maxValue);

    return 0;
}