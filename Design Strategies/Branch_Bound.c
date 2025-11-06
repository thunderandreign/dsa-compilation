#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ITEMS 10
#define MAX_NAME 10

typedef struct {
    char name[MAX_NAME];
    int weight, value;
    double ratio;
} Item;

typedef struct Node {
    int level, profit, weight;
    double bound;
    char itemsTaken[MAX_ITEMS][MAX_NAME];
    int itemsCount;
    struct Node* next;
} Node;

int W = 7; // Knapsack capacity

Item items[] = {
    {"A", 1, 15, 0},
    {"B", 3, 20, 0},
    {"C", 4, 30, 0},
    {"D", 5, 40, 0}
};

int n = sizeof(items) / sizeof(items[0]);

int cmp(const void* a, const void* b) {
    double r1 = ((Item*)a)->ratio;
    double r2 = ((Item*)b)->ratio;
    if (r1 < r2) return 1;
    else if (r1 > r2) return -1;
    else return 0;
}

double bound(Node* u) {
    if (u->weight >= W)
        return 0;
    double profit_bound = u->profit;
    int j = u->level + 1;
    int totweight = u->weight;

    while (j < n && totweight + items[j].weight <= W) {
        totweight += items[j].weight;
        profit_bound += items[j].value;
        j++;
    }
    if (j < n)
        profit_bound += (W - totweight) * items[j].ratio;
    return profit_bound;
}

// Simple queue for BFS
typedef struct Queue {
    Node* front;
    Node* rear;
} Queue;

void enqueue(Queue* q, Node* node) {
    node->next = NULL;
    if (q->rear == NULL) {
        q->front = q->rear = node;
    } else {
        q->rear->next = node;
        q->rear = node;
    }
}

Node* dequeue(Queue* q) {
    if (q->front == NULL) return NULL;
    Node* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) q->rear = NULL;
    return temp;
}

int main() {
    // Calculate ratios
    for (int i = 0; i < n; i++)
        items[i].ratio = (double)items[i].value / items[i].weight;
    qsort(items, n, sizeof(Item), cmp);

    Queue q = {NULL, NULL};
    Node* u = (Node*)calloc(1, sizeof(Node));
    Node* v = NULL;
    u->level = -1;
    u->profit = 0;
    u->weight = 0;
    u->itemsCount = 0;
    u->bound = bound(u);

    int maxProfit = 0;
    char bestItems[MAX_ITEMS][MAX_NAME];
    int bestCount = 0;

    enqueue(&q, u);

    while ((u = dequeue(&q)) != NULL) {
        if (u->level == n - 1) {
            free(u);
            continue;
        }

        // Take the next item
        v = (Node*)calloc(1, sizeof(Node));
        v->level = u->level + 1;
        v->weight = u->weight + items[v->level].weight;
        v->profit = u->profit + items[v->level].value;
        v->itemsCount = u->itemsCount;
        for (int i = 0; i < u->itemsCount; i++)
            strcpy(v->itemsTaken[i], u->itemsTaken[i]);
        strcpy(v->itemsTaken[v->itemsCount++], items[v->level].name);

        if (v->weight <= W && v->profit > maxProfit) {
            maxProfit = v->profit;
            bestCount = v->itemsCount;
            for (int i = 0; i < bestCount; i++)
                strcpy(bestItems[i], v->itemsTaken[i]);
        }

        v->bound = bound(v);
        if (v->bound > maxProfit)
            enqueue(&q, v);
        else
            free(v);

        // Do not take the next item
        v = (Node*)calloc(1, sizeof(Node));
        v->level = u->level + 1;
        v->weight = u->weight;
        v->profit = u->profit;
        v->itemsCount = u->itemsCount;
        for (int i = 0; i < u->itemsCount; i++)
            strcpy(v->itemsTaken[i], u->itemsTaken[i]);

        v->bound = bound(v);
        if (v->bound > maxProfit)
            enqueue(&q, v);
        else
            free(v);

        free(u);
    }

    printf("Maximum value: %d\n", maxProfit);
    printf("Items taken: ");
    for (int i = 0; i < bestCount; i++)
        printf("%s ", bestItems[i]);
    printf("\n");

    return 0;
}