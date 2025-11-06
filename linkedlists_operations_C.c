C Implementation

#include <stdio.h>
#include <stdlib.h>

// Define the ListNode structure
typedef struct ListNode {
    int val;
    struct ListNode *next;
} ListNode;

// Function to create a new node
ListNode* createNode(int val) {
    ListNode* newNode = (ListNode*)malloc(sizeof(ListNode));
    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

// Function to insert a node at the end
void insertEnd(ListNode** head, int val) {
    ListNode* newNode = createNode(val);
    if (*head == NULL) {
        *head = newNode;
        return;
    }
    ListNode* temp = *head;
    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
}

// Function to delete a node by value
void deleteNode(ListNode** head, int val) {
    ListNode* temp = *head;
    ListNode* prev = NULL;

    // Handle the case for the head
    if (temp != NULL && temp->val == val) {
        *head = temp->next;
        free(temp);
        return;
    }

    // Search for the node to delete
    while (temp != NULL && temp->val != val) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return; // Value not found

    prev->next = temp->next;
    free(temp);
}

// Function to print the linked list
void printList(ListNode* head) {
    while (head) {
        printf("%d -> ", head->val);
        head = head->next;
    }
    printf("NULL\n");
}

// Function to search for a value
int search(ListNode* head, int val) {
    ListNode* current = head;
    while (current) {
        if (current->val == val) return 1; // Found
        current = current->next;
    }
    return 0; // Not found
}

// Main function to demonstrate linked list operations
int main() {
    ListNode* head = NULL;

    // Insert elements
    insertEnd(&head, 1);
    insertEnd(&head, 2);
    insertEnd(&head, 3);

    // Print the list
    printList(head); // Output: 1 -> 2 -> 3 -> NULL

    // Search for a value
    printf("Searching for 2: %s\n", search(head, 2) ? "Found" : "Not Found");

    // Delete a node
    deleteNode(&head, 2);
    printList(head); // Output: 1 -> 3 -> NULL

    // Cleanup
    deleteNode(&head, 1);
    deleteNode(&head, 3);

    return 0;
}
