#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct Queue {
    int *arr;       // array to store queue elements
    int capacity;   // maximum size of queue
    int front;      // index of front element
    int rear;       // index of rear element
    int size;       // current number of elements
} Queue;

// create a queue with given capacity
Queue* createQueue(int capacity) {
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));
    if (!newQueue) {
        printf("Memory allocation failed!\n");
        return NULL;
    }
    newQueue->capacity = capacity;
    newQueue->arr = (int*)malloc(capacity * sizeof(int));
    if (!newQueue->arr) {
        printf("Memory allocation failed!\n");
        free(newQueue);
        return NULL;
    }
    newQueue->front = 0;
    newQueue->rear = -1;
    newQueue->size = 0;
    return newQueue;
}

// add element to queue
void enqueue(Queue* q, int value) {
    if (q->size == q->capacity) {
        printf("Queue overflow!\n");
        return;
    }
    q->rear = (q->rear + 1) % q->capacity; // circular increment
    q->arr[q->rear] = value;
    q->size++;
}

// remove element from queue
int dequeue(Queue* q) {
    if (q->size == 0) {
        printf("Queue underflow!\n");
        return -1; // sentinel value
    }
    int value = q->arr[q->front];
    q->front = (q->front + 1) % q->capacity; // circular increment
    q->size--;
    return value;
}

// check if queue is empty
bool isEmpty(Queue* q) {
    return q->size == 0;
}

// return size of queue
int size(Queue* q) {
    return q->size;
}

// peek at front element
int peek(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return -1;
    }
    return q->arr[q->front];
}

// print queue contents
void printQueue(Queue* q) {
    if (isEmpty(q)) {
        printf("Queue is empty!\n");
        return;
    }
    int count = q->size;
    int i = q->front;
    while (count > 0) {
        printf("%d -> ", q->arr[i]);
        i = (i + 1) % q->capacity;
        count--;
    }
    printf("null\n");
}

// free memory
void freeQueue(Queue* q) {
    if (q) {
        free(q->arr);
        free(q);
    }
}

int main() {
    Queue* q1 = createQueue(10);

    enqueue(q1, 6);
    enqueue(q1, 1);
    enqueue(q1, 9);
    enqueue(q1, 2);
    enqueue(q1, 5);

    printQueue(q1);
    printf("Size: %d\n", size(q1));
    printf("Dequeue: %d\n", dequeue(q1));
    printf("isEmpty: %d\n", isEmpty(q1));

    printQueue(q1);
    printf("Size: %d\n", size(q1));
    printf("Peek: %d\n", peek(q1));

    freeQueue(q1);
    return 0;
}