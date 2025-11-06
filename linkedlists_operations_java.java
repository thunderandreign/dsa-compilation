 Java Implementation

Import java.io.*;

class ListNode {
    int val;
    ListNode next;

    ListNode(int x) {
        val = x;
        next = null;
    }
}

public class LinkedList {
    ListNode head;

    // Insert a node at the end
    public void insertEnd(int val) {
        ListNode newNode = new ListNode(val);
        if (head == null) {
            head = newNode;
            return;
        }
        ListNode temp = head;
        while (temp.next != null) {
            temp = temp.next;
        }
        temp.next = newNode;
    }

    // Delete a node by value
    public void deleteNode(int val) {
        ListNode temp = head;
        ListNode prev = null;

        // Handle the case for the head
        if (temp != null && temp.val == val) {
            head = temp.next;
            return;
        }

        // Search for the node to delete
        while (temp != null && temp.val != val) {
            prev = temp;
            temp = temp.next;
        }

        if (temp == null) return; // Value not found

        prev.next = temp.next; // Unlink the node
    }

    // Print the linked list
    public void printList() {
        ListNode current = head;
        while (current != null) {
            System.out.print(current.val + " -> ");
            current = current.next;
        }
        System.out.println("NULL");
    }

    // Search for a value
    public boolean search(int val) {
        ListNode current = head;
        while (current != null) {
            if (current.val == val) return true; // Found
            current = current.next;
        }
        return false; // Not found
    }

    public static void main(String[] args) {
        LinkedList list = new LinkedList();

        // Insert elements
        list.insertEnd(1);
        list.insertEnd(2);
        list.insertEnd(3);

        // Print the list
        list.printList(); // Output: 1 -> 2 -> 3 -> NULL

        // Search for a value
        System.out.println("Searching for 2: " + (list.search(2) ? "Found" : "Not Found"));

        // Delete a node
        list.deleteNode(2);
        list.printList(); // Output: 1 -> 3 -> NULL
    }
}
