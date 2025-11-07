#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 10

// Define a node for the linked list
typedef struct Node {
    int key;
    char value[50];
    struct Node* next;
} Node;

// Define the hash table
Node* hashTable[TABLE_SIZE];

// Hash function
int hashFunction(int key) {
    return key % TABLE_SIZE;
}

// Insert a key-value pair into the hash table
void insert(int key, const char* value) {
    int index = hashFunction(key);
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    strcpy(newNode->value, value);
    newNode->next = hashTable[index];
    hashTable[index] = newNode;
}

// Search for a value by key
char* search(int key) {
    int index = hashFunction(key);
    Node* temp = hashTable[index];
    while (temp != NULL) {
        if (temp->key == key) {
            return temp->value;
        }
        temp = temp->next;
    }
    return NULL;
}

// Delete a key-value pair
void delete(int key) {
    int index = hashFunction(key);
    Node* temp = hashTable[index];
    Node* prev = NULL;

    while (temp != NULL && temp->key != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Key %d not found.\n", key);
        return;
    }

    if (prev == NULL) {
        hashTable[index] = temp->next;
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Key %d deleted.\n", key);
}

// Display the hash table
void display() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        printf("Index %d: ", i);
        Node* temp = hashTable[i];
        while (temp != NULL) {
            printf("-> [Key: %d, Value: %s] ", temp->key, temp->value);
            temp = temp->next;
        }
        printf("-> NULL\n");
    }
}

int main() {
    // Initialize the hash table
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = NULL;
    }

    // Insert key-value pairs
    insert(1, "Apple");
    insert(2, "Banana");
    insert(12, "Cherry");
    insert(22, "Date");

    // Display the hash table
    printf("Initial Hash Table:\n");
    display();

    // Search for a value
    int key = 2;
    char* value = search(key);
    if (value) {
        printf("Value for key %d: %s\n", key, value);
    } else {
        printf("Key %d not found.\n", key);
    }

    // Delete a key-value pair
    delete(12);
    printf("Hash Table after deletion:\n");
    display();

    return 0;
}