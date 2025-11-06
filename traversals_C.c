C IMPLEMENTATION
#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100

// TreeNode structure for binary tree
typedef struct TreeNode {
    int data;
    struct TreeNode* left;
    struct TreeNode* right;
} TreeNode;

// Queue structure for level-order traversal
typedef struct Queue {
    TreeNode** items;
    int front;
    int rear;
    int capacity;
} Queue;

// Stack structure for iterative traversals
typedef struct Stack {
    TreeNode** items;
    int top;
    int capacity;
} Stack;

// ==================== UTILITY FUNCTIONS ====================

/**
 * Create a new tree node with given data
 */
TreeNode* createNode(int data) {
    TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

/**
 * Create and initialize a queue
 */
Queue* createQueue(int capacity) {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->items = (TreeNode**)malloc(sizeof(TreeNode*) * capacity);
    queue->front = 0;
    queue->rear = 0;
    queue->capacity = capacity;
    return queue;
}

/**
 * Add node to queue (enqueue operation)
 */
void enqueue(Queue* queue, TreeNode* item) {
    if (queue->rear < queue->capacity) {
        queue->items[queue->rear++] = item;
    }
}

/**
 * Remove node from queue (dequeue operation)
 */
TreeNode* dequeue(Queue* queue) {
    if (queue->front < queue->rear) {
        return queue->items[queue->front++];
    }
    return NULL;
}

/**
 * Check if queue is empty
 */
int isQueueEmpty(Queue* queue) {
    return queue->front == queue->rear;
}

/**
 * Get current queue size
 */
int queueSize(Queue* queue) {
    return queue->rear - queue->front;
}

/**
 * Create and initialize a stack
 */
Stack* createStack(int capacity) {
    Stack* stack = (Stack*)malloc(sizeof(Stack));
    stack->items = (TreeNode**)malloc(sizeof(TreeNode*) * capacity);
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}

/**
 * Push node to stack
 */
void push(Stack* stack, TreeNode* item) {
    if (stack->top < stack->capacity - 1) {
        stack->items[++stack->top] = item;
    }
}

/**
 * Pop node from stack
 */
TreeNode* pop(Stack* stack) {
    if (stack->top >= 0) {
        return stack->items[stack->top--];
    }
    return NULL;
}

/**
 * Check if stack is empty
 */
int isStackEmpty(Stack* stack) {
    return stack->top == -1;
}

/**
 * Free tree memory (post-order traversal)
 */
void freeTree(TreeNode* root) {
    if (root == NULL) return;
    
    freeTree(root->left);   // Free left subtree
    freeTree(root->right);  // Free right subtree
    free(root);            // Free current node
}

// ==================== DEPTH-FIRST TRAVERSALS ====================

/**
 * PRE-ORDER TRAVERSAL (Depth-First)
 * Order: Root -> Left -> Right
 */
void preOrder(TreeNode* root) {
    // Base case: if tree is empty, return
    if (root == NULL) return;
    
    printf("%d ", root->data);  // 1. Process ROOT node first
    preOrder(root->left);       // 2. Traverse entire LEFT subtree
    preOrder(root->right);      // 3. Traverse entire RIGHT subtree
}

/**
 * IN-ORDER TRAVERSAL (Depth-First)
 * Order: Left -> Root -> Right
 */
void inOrder(TreeNode* root) {
    // Base case: if tree is empty, return
    if (root == NULL) return;
    
    inOrder(root->left);        // 1. Traverse entire LEFT subtree first
    printf("%d ", root->data);  // 2. Process ROOT node
    inOrder(root->right);       // 3. Traverse entire RIGHT subtree
}

/**
 * POST-ORDER TRAVERSAL (Depth-First)
 * Order: Left -> Right -> Root
 */
void postOrder(TreeNode* root) {
    // Base case: if tree is empty, return
    if (root == NULL) return;
    
    postOrder(root->left);      // 1. Traverse entire LEFT subtree first
    postOrder(root->right);     // 2. Traverse entire RIGHT subtree
    printf("%d ", root->data);  // 3. Process ROOT node last
}

// ==================== BREADTH-FIRST TRAVERSAL ====================

/**
 * LEVEL-ORDER TRAVERSAL (Breadth-First)
 * Order: Process all nodes level by level from left to right
 */
void levelOrder(TreeNode* root) {
    // If tree is empty, nothing to traverse
    if (root == NULL) return;
    
    // Create queue for BFS traversal
    Queue* queue = createQueue(MAX_SIZE);
    enqueue(queue, root);  // Start with root node
    
    printf("Level Order: ");
    
    // Process nodes until queue is empty
    while (!isQueueEmpty(queue)) {
        TreeNode* current = dequeue(queue);  // Get front node
        printf("%d ", current->data);        // Process current node
        
        // Add children to queue for later processing
        if (current->left != NULL) enqueue(queue, current->left);
        if (current->right != NULL) enqueue(queue, current->right);
    }
    printf("\n");
    
    // Free queue memory
    free(queue->items);
    free(queue);
}

/**
 * LEVEL-ORDER TRAVERSAL with Level Separation
 * Shows nodes at each level separately
 */
void levelOrderWithLevels(TreeNode* root) {
    if (root == NULL) return;
    
    Queue* queue = createQueue(MAX_SIZE);
    enqueue(queue, root);
    int level = 0;
    
    while (!isQueueEmpty(queue)) {
        int levelSize = queueSize(queue);  // Nodes at current level
        printf("Level %d: ", level);
        
        // Process all nodes at current level
        for (int i = 0; i < levelSize; i++) {
            TreeNode* current = dequeue(queue);
            printf("%d ", current->data);
            
            // Add children to queue
            if (current->left != NULL) enqueue(queue, current->left);
            if (current->right != NULL) enqueue(queue, current->right);
        }
        printf("\n");
        level++;
    }
    
    free(queue->items);
    free(queue);
}

// ==================== ITERATIVE TRAVERSALS ====================

/**
 * ITERATIVE PRE-ORDER TRAVERSAL
 * Uses explicit stack instead of recursion
 */
void iterativePreOrder(TreeNode* root) {
    if (root == NULL) return;
    
    Stack* stack = createStack(MAX_SIZE);
    push(stack, root);  // Start with root
    
    printf("Iterative Pre-Order: ");
    
    while (!isStackEmpty(stack)) {
        TreeNode* current = pop(stack);
        printf("%d ", current->data);
        
        // Push right child first (so left is processed first - LIFO)
        if (current->right != NULL) push(stack, current->right);
        if (current->left != NULL) push(stack, current->left);
    }
    printf("\n");
    
    free(stack->items);
    free(stack);
}

/**
 * ITERATIVE IN-ORDER TRAVERSAL
 * Uses stack to simulate recursion
 */
void iterativeInOrder(TreeNode* root) {
    if (root == NULL) return;
    
    Stack* stack = createStack(MAX_SIZE);
    TreeNode* current = root;
    
    printf("Iterative In-Order: ");
    
    while (current != NULL || !isStackEmpty(stack)) {
        // Reach the leftmost node of current node
        while (current != NULL) {
            push(stack, current);
            current = current->left;
        }
        
        // Current must be NULL at this point
        current = pop(stack);
        printf("%d ", current->data);
        
        // Visit the right subtree
        current = current->right;
    }
    printf("\n");
    
    free(stack->items);
    free(stack);
}

// ==================== MAIN FUNCTION ====================

int main() {
    // Create the same sample binary tree as Java version:
    //       1
    //      / \
    //     2   3
    //    / \   \
    //   4   5   6
    //  /
    // 7
    
    TreeNode* root = createNode(1);
    root->left = createNode(2);
    root->right = createNode(3);
    root->left->left = createNode(4);
    root->left->right = createNode(5);
    root->right->right = createNode(6);
    root->left->left->left = createNode(7);
    
    printf("Binary Tree Structure:\n");
    printf("       1\n");
    printf("      / \\\n");
    printf("     2   3\n");
    printf("    / \\   \\\n");
    printf("   4   5   6\n");
    printf("  /\n");
    printf(" 7\n");
    printf("\n");
    
    // Depth-First Traversals
    printf("=== DEPTH-FIRST TRAVERSALS ===\n");
    printf("Pre-Order (Root->Left->Right): ");
    preOrder(root);
    printf("\n");
    
    printf("In-Order (Left->Root->Right): ");
    inOrder(root);
    printf("\n");
    
    printf("Post-Order (Left->Right->Root): ");
    postOrder(root);
    printf("\n");
    
    // Breadth-First Traversals
    printf("\n=== BREADTH-FIRST TRAVERSALS ===\n");
    levelOrder(root);
    
    printf("\nLevel-by-Level Traversal:\n");
    levelOrderWithLevels(root);
    
    // Iterative Traversals
    printf("\n=== ITERATIVE TRAVERSALS ===\n");
    iterativePreOrder(root);
    iterativeInOrder(root);
    
    // Free allocated memory
    freeTree(root);
    
    return 0;
}
