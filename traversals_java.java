JAVA IMPLEMENTATION OF TRAVERSALS
import java.util.*;

// TreeNode class represents a single node in the binary tree
class TreeNode {
    int data;
    TreeNode left;
    TreeNode right;
    
    // Constructor to create a new tree node
    public TreeNode(int data) {
        this.data = data;
        this.left = null;
        this.right = null;
    }
}

public class AllTreeTraversals {
    
    // ==================== DEPTH-FIRST TRAVERSALS ====================
    
    /**
     * PRE-ORDER TRAVERSAL (Depth-First)
     * Order: Root -> Left -> Right
     * Process the current node first, then left subtree, then right subtree
     * Used for: Creating a copy of the tree, prefix expressions
     */
    public static void preOrder(TreeNode root) {
        // Base case: if tree is empty, return
        if (root == null) return;
        
        System.out.print(root.data + " ");  // 1. Process ROOT node first
        preOrder(root.left);                // 2. Traverse entire LEFT subtree
        preOrder(root.right);               // 3. Traverse entire RIGHT subtree
    }
    
    /**
     * IN-ORDER TRAVERSAL (Depth-First)
     * Order: Left -> Root -> Right
     * Process left subtree first, then current node, then right subtree
     * Used for: Binary Search Trees (gives sorted order), infix expressions
     */
    public static void inOrder(TreeNode root) {
        // Base case: if tree is empty, return
        if (root == null) return;
        
        inOrder(root.left);                 // 1. Traverse entire LEFT subtree first
        System.out.print(root.data + " ");  // 2. Process ROOT node
        inOrder(root.right);                // 3. Traverse entire RIGHT subtree
    }
    
    /**
     * POST-ORDER TRAVERSAL (Depth-First)
     * Order: Left -> Right -> Root
     * Process left subtree first, then right subtree, then current node
     * Used for: Deleting trees, postfix expressions, computing directory sizes
     */
    public static void postOrder(TreeNode root) {
        // Base case: if tree is empty, return
        if (root == null) return;
        
        postOrder(root.left);               // 1. Traverse entire LEFT subtree first
        postOrder(root.right);              // 2. Traverse entire RIGHT subtree
        System.out.print(root.data + " ");  // 3. Process ROOT node last
    }
    
    // ==================== BREADTH-FIRST TRAVERSAL ====================
    
    /**
     * LEVEL-ORDER TRAVERSAL (Breadth-First)
     * Order: Process all nodes level by level from left to right
     * Uses Queue data structure (FIFO - First In First Out)
     * Used for: Finding shortest path, level-wise processing
     */
    public static void levelOrder(TreeNode root) {
        // If tree is empty, nothing to traverse
        if (root == null) return;
        
        // Create a queue to store nodes for processing
        Queue<TreeNode> queue = new LinkedList<>();
        queue.add(root);  // Start with the root node
        
        System.out.print("Level Order: ");
        
        // Continue until all nodes are processed
        while (!queue.isEmpty()) {
            TreeNode current = queue.remove();  // Get the front node from queue
            System.out.print(current.data + " "); // Process the current node
            
            // Add left child to queue if it exists
            if (current.left != null) {
                queue.add(current.left);
            }
            // Add right child to queue if it exists
            if (current.right != null) {
                queue.add(current.right);
            }
        }
    }
    
    /**
     * LEVEL-ORDER TRAVERSAL with Level Separation
     * Shows which nodes are at each level clearly
     * Useful for understanding tree structure
     */
    public static void levelOrderWithLevels(TreeNode root) {
        if (root == null) return;
        
        Queue<TreeNode> queue = new LinkedList<>();
        queue.add(root);
        int level = 0;
        
        while (!queue.isEmpty()) {
            int levelSize = queue.size();  // Number of nodes at current level
            System.out.print("Level " + level + ": ");
            
            // Process all nodes at the current level
            for (int i = 0; i < levelSize; i++) {
                TreeNode current = queue.remove();
                System.out.print(current.data + " ");
                
                // Add children to queue for next level processing
                if (current.left != null) queue.add(current.left);
                if (current.right != null) queue.add(current.right);
            }
            System.out.println();  // New line after each level
            level++;
        }
    }
    
    // ==================== ITERATIVE TRAVERSALS ====================
    
    /**
     * ITERATIVE PRE-ORDER TRAVERSAL
     * Uses explicit stack instead of recursion
     * Useful for large trees to avoid stack overflow
     */
    public static void iterativePreOrder(TreeNode root) {
        if (root == null) return;
        
        Stack<TreeNode> stack = new Stack<>();
        stack.push(root);  // Start with root node
        
        System.out.print("Iterative Pre-Order: ");
        
        while (!stack.isEmpty()) {
            TreeNode current = stack.pop();
            System.out.print(current.data + " ");
            
            // Push right child first so left is processed first (LIFO)
            if (current.right != null) stack.push(current.right);
            if (current.left != null) stack.push(current.left);
        }
    }
    
    /**
     * ITERATIVE IN-ORDER TRAVERSAL
     * Uses stack to simulate recursion
     */
    public static void iterativeInOrder(TreeNode root) {
        if (root == null) return;
        
        Stack<TreeNode> stack = new Stack<>();
        TreeNode current = root;
        
        System.out.print("Iterative In-Order: ");
        
        while (current != null || !stack.isEmpty()) {
            // Reach the leftmost node of the current node
            while (current != null) {
                stack.push(current);
                current = current.left;
            }
            
            // Current must be null at this point
            current = stack.pop();
            System.out.print(current.data + " ");
            
            // Now visit the right subtree
            current = current.right;
        }
    }
    
    // ==================== MAIN METHOD ====================
    
    public static void main(String[] args) {
        // Create a sample binary tree:
        //       1
        //      / \
        //     2   3
        //    / \   \
        //   4   5   6
        //  /
        // 7
        
        TreeNode root = new TreeNode(1);
        root.left = new TreeNode(2);
        root.right = new TreeNode(3);
        root.left.left = new TreeNode(4);
        root.left.right = new TreeNode(5);
        root.right.right = new TreeNode(6);
        root.left.left.left = new TreeNode(7);
        
        System.out.println("Binary Tree Structure:");
        System.out.println("       1");
        System.out.println("      / \\");
        System.out.println("     2   3");
        System.out.println("    / \\   \\");
        System.out.println("   4   5   6");
        System.out.println("  /");
        System.out.println(" 7");
        System.out.println();
        
        // Depth-First Traversals
        System.out.println("=== DEPTH-FIRST TRAVERSALS ===");
        System.out.print("Pre-Order (Root->Left->Right): ");
        preOrder(root);
        System.out.println();
        
        System.out.print("In-Order (Left->Root->Right): ");
        inOrder(root);
        System.out.println();
        
        System.out.print("Post-Order (Left->Right->Root): ");
        postOrder(root);
        System.out.println();
        
        // Breadth-First Traversals
        System.out.println("\n=== BREADTH-FIRST TRAVERSALS ===");
        levelOrder(root);
        System.out.println();
        
        System.out.println("\nLevel-by-Level Traversal:");
        levelOrderWithLevels(root);
        
        // Iterative Traversals
        System.out.println("\n=== ITERATIVE TRAVERSALS ===");
        iterativePreOrder(root);
        System.out.println();
        
        iterativeInOrder(root);
        System.out.println();
    }
}
