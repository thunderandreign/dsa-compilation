#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>


typedef struct Node {
    char data;
    struct Node* next;
} Node;

typedef struct Stack {
    struct Node* head;
    int size;
} Stack;

Stack* createStack() {
    Stack* newStack = (Stack*)malloc(sizeof(Stack));
    if (!newStack) {
        printf("Memory allocation failed!!\n");
        exit(1);
    }
    newStack->head = NULL;
    newStack->size = 0;
    return newStack;
}

void push(Stack* s, char value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!!\n");
        exit(1);
    }
    newNode->data = value;
    newNode->next = s->head;
    s->head = newNode;
    s->size++;
}

char pop(Stack* s) {
    if (s->size == 0) return '\0';
    char popped = s->head->data;
    Node* temp = s->head;
    s->head = s->head->next;
    free(temp);
    s->size--;
    return popped;
}

char peek(Stack* s) {
    if (s->size == 0) return '\0';
    return s->head->data;
}

bool isEmpty(Stack* s) {
    return s->size == 0;
}

int precedence(char c) {
    switch (c) {
        case '+':
        case '-': return 1;
        case '*':
        case '/': return 2;
        case '^': return 3;
        default: return -1;
    }
}

bool isOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

bool isOperand(char c) {
    return isalnum(c);
}
char* infixToPostfix(char* infix) {
    Stack* stack = createStack();
    int len = strlen(infix);
    char* result = (char*)malloc(len + 1);
    int k = 0;

    for (int i = 0; i < len; i++) {
        char c = infix[i];

        if (isOperand(c)) {
            result[k++] = c;
        }
        else if (c == '(') {
            push(stack, c);
        }
        else if (c == ')') {
            while (!isEmpty(stack) && peek(stack) != '(') {
                result[k++] = pop(stack);
            }
            pop(stack); // remove '('
        }
        else if (isOperator(c)) {
            while (!isEmpty(stack) && precedence(peek(stack)) >= precedence(c)) {
                result[k++] = pop(stack);
            }
            push(stack, c);
        }
    }

    while (!isEmpty(stack)) {
        result[k++] = pop(stack);
    }

    result[k] = '\0';
    return result;
}

int main() {
    char infix[] = "(A+B)*C-(D-E)*(F+G)";
    printf("Infix:   %s\n", infix);
    
    char* postfix = infixToPostfix(infix);
    printf("Postfix: %s\n", postfix);


    free(postfix);

    return 0;
}
