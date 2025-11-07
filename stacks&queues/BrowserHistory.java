package stacks;

public class BrowserHistory {
    static Node currentNode;
    static Stack back = new Stack();
    static Stack forward = new Stack();

    
    static class Stack {
        Node head;
        int size;

        void push(Node node) {
            if (node == null) return;
            node.next = head;
            head = node;
            size++;
        }

        Node pop() {
            if (head == null) return null;
            Node temp = head;
            head = head.next;
            temp.next = null; // detach
            size--;
            return temp;
        }

        void clear() {
            head = null;
            size = 0;
        }

        boolean isEmpty() {
            return size == 0;
        }

        @Override
        public String toString() {
            StringBuilder sb = new StringBuilder("[");
            Node curr = head;
            while (curr != null) {
                sb.append(curr.page);
                if (curr.next != null) sb.append(", ");
                curr = curr.next;
            }
            sb.append("]");
            return sb.toString();
        }
    }

    static class Node {
        String page;
        Node next;

        Node(String page) {
            this.page = page;
            this.next = null;
        }
    }

    public static void visit(String page) {
        if (currentNode != null) {
            back.push(new Node(currentNode.page));
        }
        currentNode = new Node(page);
        forward.clear(); // clear forward history
    }

    public static void back() {
        if (back.isEmpty()) {
            System.out.println("No previous page.");
            return;
        }
        forward.push(new Node(currentNode.page));
        currentNode = back.pop();
    }

    public static void forward() {
        if (forward.isEmpty()) {
            System.out.println("No forward page.");
            return;
        }
        back.push(new Node(currentNode.page));
        currentNode = forward.pop();
    }

    public static void printHistory() {
        System.out.println("Current page: " + (currentNode != null ? currentNode.page : "None"));
        System.out.println("Back stack: " + back);
        System.out.println("Forward stack: " + forward);
        System.out.println("-----------------------------");
    }

    public static void main(String[] args) {
        currentNode = new Node("Home");
        printHistory();

        visit("Page1");
        printHistory();

        visit("Page2");
        printHistory();

        back();
        printHistory();

        forward();
        printHistory();

        visit("Page3");
        printHistory();

        back();
        printHistory();

        visit("Page4"); // clears forward
        printHistory();
    }
}