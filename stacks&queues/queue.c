#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;


typedef struct Queue{
    struct Node* head;
    struct Node* tail;
    int size;
}Queue;

Queue* createQueue(){
    Queue* newQueue = (Queue*)malloc(sizeof(Queue));

    if(!newQueue){
        printf("Memory Allocation Failed/n");
    }

    newQueue->head = NULL;
    newQueue->tail = NULL;
    newQueue->size = 0;

    return newQueue;
}

Node* createNode(int value){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(!newNode){
        printf("Memory Allocation Failed/n");
    }

    newNode->data = value;
    newNode->next = NULL;

    return newNode;
}


void enqueue(Queue * q , int value){
    Node* node = createNode(value);

    if(q->size == 0){
        q->tail = q->head = node;
        q->size += 1;
    }else{
        q->tail->next = node;
        q->tail = node;
        q->size += 1;
    }
}

int dequeue(Queue * q){
    if(q->size == 0){
        return NULL;
    }

    Node* temp = q->head;
    q->head = temp->next;
    q->size -= 1;
    return temp->data;
}

bool isEmpty(Queue * q){
    if(q->size == 0){
        return true;
    }else{
        return false;
    }
}
int size(Queue * q){
    return q->size;
}
int peek(Queue * q){
    return q->head->data;
}
void printQueue(Queue * q){
    Node* currentNode = q->head;
    while(currentNode){
        printf("%d -> ",currentNode->data);
        currentNode = currentNode->next;
    }
    printf("null\n");
}

int main(){

    Queue* q1 = createQueue();

    enqueue(q1,6);
    enqueue(q1,1);
    enqueue(q1,9);
    enqueue(q1,2);
    enqueue(q1,5);

    printQueue(q1);
    printf("Size: %d\n",size(q1));
    printf("Dequeue: %d\n",dequeue(q1));
    printf("isEmpty: %d\n",isEmpty(q1));

    printQueue(q1);
    printf("Size: %d\n",size(q1));
    printf("Peek: %d\n",peek(q1));

}
