#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

typedef struct CharNode {
    char data;
    struct CharNode* next;
} CharNode;

typedef struct CharStack {
    struct CharNode* head;
    int size;
} CharStack;

CharStack* createCharStack() {
    CharStack* newStack = (CharStack*)malloc(sizeof(CharStack));
    newStack->head = NULL;
    newStack->size = 0;
    return newStack;
}

void pushChar(CharStack* s, char value) {
    CharNode* newNode = (CharNode*)malloc(sizeof(CharNode));
    newNode->data = value;
    newNode->next = s->head;
    s->head = newNode;
    s->size++;
}

char popChar(CharStack* s) {
    if (s->size == 0) return '\0';
    char popped = s->head->data;
    CharNode* temp = s->head;
    s->head = s->head->next;
    free(temp);
    s->size--;
    return popped;
}

char peekChar(CharStack* s) {
    if (s->size == 0) return '\0';
    return s->head->data;
}

bool isEmptyChar(CharStack* s) {
    return s->size == 0;
}

typedef struct IntNode {
    int data;
    struct IntNode* next;
} IntNode;

typedef struct IntStack {
    struct IntNode* head;
    int size;
} IntStack;

IntStack* createIntStack() {
    IntStack* s = (IntStack*)malloc(sizeof(IntStack));
    s->head = NULL;
    s->size = 0;
    return s;
}

void pushInt(IntStack* s, int value) {
    IntNode* newNode = (IntNode*)malloc(sizeof(IntNode));
    newNode->data = value;
    newNode->next = s->head;
    s->head = newNode;
    s->size++;
}

int popInt(IntStack* s) {
    if (s->size == 0) return 0;
    int popped = s->head->data;
    IntNode* temp = s->head;
    s->head = s->head->next;
    free(temp);
    s->size--;
    return popped;
}

bool isEmptyInt(IntStack* s) {
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
    return isdigit(c);
}

char* infixToPostfix(char* infix) {
    CharStack* stack = createCharStack();
    int len = strlen(infix);
    char* result = (char*)malloc(len + 1);
    int k = 0;

    for (int i = 0; i < len; i++) {
        char c = infix[i];

        if (isOperand(c)) {
            result[k++] = c;
        }
        else if (c == '(') {
            pushChar(stack, c);
        }
        else if (c == ')') {
            while (!isEmptyChar(stack) && peekChar(stack) != '(') {
                result[k++] = popChar(stack);
            }
            popChar(stack); // remove '('
        }
        else if (isOperator(c)) {
            while (!isEmptyChar(stack) && precedence(peekChar(stack)) >= precedence(c)) {
                result[k++] = popChar(stack);
            }
            pushChar(stack, c);
        }
    }

    while (!isEmptyChar(stack)) {
        result[k++] = popChar(stack);
    }

    result[k] = '\0';
    return result;
}

int evaluatePostfix(char* postfix) {
    IntStack* stack = createIntStack();
    int len = strlen(postfix);

    for (int i = 0; i < len; i++) {
        char c = postfix[i];

        if (isdigit(c)) {
            // convert char digit to int value
            pushInt(stack, c - '0');
        }
        else if (isOperator(c)) {
            int val2 = popInt(stack);
            int val1 = popInt(stack);
            int result;

            switch (c) {
                case '+': result = val1 + val2; break;
                case '-': result = val1 - val2; break;
                case '*': result = val1 * val2; break;
                case '/': result = val1 / val2; break;
                case '^': {
                    result = 1;
                    for (int j = 0; j < val2; j++) result *= val1;
                    break;
                }
                default: result = 0;
            }

            pushInt(stack, result);
        }
    }

    return popInt(stack);
}

int main() {
    char infix[] = "3+(2*4)-5";
    printf("Infix:   %s\n", infix);

    char* postfix = infixToPostfix(infix);
    printf("Postfix: %s\n", postfix);

    int result = evaluatePostfix(postfix);
    printf("Result:  %d\n", result);

    free(postfix);
    return 0;
}
