typedef struct {
    char name[10];
    int size;
    int value;
} Item;

int compare(const void* a, const void* b) {
    Item* itemA = (Item*)a;
    Item* itemB = (Item*)b;
    double ratioA = (double)itemA->value / itemA->size;
    double ratioB = (double)itemB->value / itemB->size;
    if (ratioA < ratioB) return 1;
    else if (ratioA > ratioB) return -1;
    else return 0;
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

    // Sort items by value/size ratio in descending order
    qsort(items, n, sizeof(Item), compare);

    double totalValue = 0;
    int remainingCapacity = capacity;

    printf("Selected items:\n");
    for (int i = 0; i < n; i++) {
        if (items[i].size <= remainingCapacity) {
            printf("%s (size: %d, value: %d)\n", items[i].name, items[i].size, items[i].value);
            totalValue += items[i].value;
            remainingCapacity -= items[i].size;
        }
    }
    printf("Total value: %.0f\n", totalValue);

    return 0;
}