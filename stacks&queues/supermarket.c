#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_COUNTERS 3
#define NAME_LEN 50

typedef struct Node {
    char data[NAME_LEN];
    struct Node* next;
} Node;

typedef struct Queue {
    struct Node* head;
    struct Node* tail;
    int size;
} Queue;

// Create a new queue
Queue* createQueue() {
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    if (!newQueue) {
        printf("Memory Allocation Failed\n");
        exit(1);
    }
    newQueue->head = newQueue->tail = NULL;
    newQueue->size = 0;
    return newQueue;
}

// Create a new node
Node* createNode(const char* value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory Allocation Failed\n");
        exit(1);
    }
    strncpy(newNode->data, value, NAME_LEN - 1);
    newNode->data[NAME_LEN - 1] = '\0';
    newNode->next = NULL;
    return newNode;
}

// Enqueue
void enqueue(Queue* q, const char* value) {
    Node* node = createNode(value);
    if (q->size == 0) {
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->size++;
}

// Dequeue
char* dequeue(Queue* q) {
    if (q->size == 0) {
        return NULL;
    }
    Node* temp = q->head;
    q->head = temp->next;
    if (q->head == NULL) q->tail = NULL;
    q->size--;

    char* name = strdup(temp->data); // copy string to return
    free(temp);
    return name;
}

// Peek
char* peek(Queue* q) {
    return (q->size == 0) ? NULL : q->head->data;
}

// IsEmpty
bool isEmpty(Queue* q) {
    return q->size == 0;
}

// Size
int size(Queue* q) {
    return q->size;
}

// Print queue
void printQueue(Queue* q) {
    Node* currentNode = q->head;
    while (currentNode) {
        printf("%s -> ", currentNode->data);
        currentNode = currentNode->next;
    }
    printf("null\n");
}

Queue* counters[MAX_COUNTERS];

// Find shortest queue
int findShortestQueue() {
    int minIndex = 0;
    for (int i = 1; i < MAX_COUNTERS; i++) {
        if (counters[i]->size < counters[minIndex]->size) {
            minIndex = i;
        }
    }
    return minIndex;
}

// Customer arrival
void CustomerArrival(const char* name) {
    int idx = findShortestQueue();
    enqueue(counters[idx], name);
    printf("%s joined Counter %d\n", name, idx + 1);
}

// Process next customer
void ProcessNextCustomer(int counter) {
    if (counter < 1 || counter > MAX_COUNTERS) return;
    char* name = dequeue(counters[counter - 1]);
    if (name == NULL) {
        printf("Counter %d has no customers.\n", counter);
    } else {
        printf("%s is processed at Counter %d\n", name, counter);
        free(name);
    }
}

// Display queue
void DisplayQueue(int counter) {
    if (counter < 1 || counter > MAX_COUNTERS) return;
    printf("Counter %d queue: ", counter);
    printQueue(counters[counter - 1]);
}

int main() {
    // Initialize counters
    for (int i = 0; i < MAX_COUNTERS; i++) {
        counters[i] = createQueue();
    }

    // Scenario
    CustomerArrival("Alice");
    CustomerArrival("Bob");
    CustomerArrival("Charlie");
    CustomerArrival("David");
    CustomerArrival("Eve");

    DisplayQueue(1);
    DisplayQueue(2);
    DisplayQueue(3);

    ProcessNextCustomer(1);
    ProcessNextCustomer(2);

    DisplayQueue(1);
    DisplayQueue(2);
    DisplayQueue(3);

    return 0;
}