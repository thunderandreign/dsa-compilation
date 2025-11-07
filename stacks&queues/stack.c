#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct Node{
    int data;
    struct Node* next;
}Node;


typedef struct Stack{
    struct Node* head;
    int size;
}Stack;


Stack* createStack(){
    Stack* newStack = (Stack*)malloc(sizeof(Stack));

    if(!newStack){
        printf("Memory allocation failed!!\n");
    }

    newStack->head = NULL;
    newStack->size = 0;

    return newStack;
}

void push(Stack *s,int value){
    Node* newNode = (Node*)malloc(sizeof(Node));

    if(!newNode){
        printf("Memory allocation failed!!\n");
    }


    newNode->data = value;

    if(s->head != NULL){
        newNode->next = s->head;
    }

    s->head = newNode;
    s->size += 1;

}

int pop(Stack* s){

    if(s->size == 0){
        return NULL;
    }

    int popped = s->head->data;
    s->head = s->head->next;
    s->size -= 1;
    return popped;
}

bool isEmpty(Stack* s){
    if(s->size == 0){
        return true;
    }else{
        return false;
    }
}

int size(Stack* s){
    return s->size;
}

int peek(Stack* s){
    if(s->size == 0){
        return NULL;
    }
    return s->head->data;
}

int main(){
    Stack* stack1 = createStack();

    push(stack1, 3);
    push(stack1, 9);
    push(stack1, 7);
    push(stack1, 1);

    printf("%d\n",size(stack1));
    printf("%d\n",pop(stack1));
    printf("%d\n",isEmpty(stack1));
    printf("%d\n",size(stack1));
    printf("%d\n",peek(stack1));

    //Free memories


    return 0;
}
