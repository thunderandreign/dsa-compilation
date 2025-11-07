package queues;

public class Supermarket {
    static class Queue {
        Node head;
        Node tail;
        int size;

        // Node class
        class Node {
            String data;
            Node next;

            Node(String data) {
                this.data = data;
                this.next = null;
            }
        }

        // Enqueue
        public void enqueue(String value) {
            Node newNode = new Node(value);
            if (isEmpty()) {
                this.head = this.tail = newNode;
            } else {
                this.tail.next = newNode;
                this.tail = newNode;
            }
            this.size++;
        }

        // Dequeue
        public String dequeue() {
            if (isEmpty()) {
                return null;
            }
            Node temp = this.head;
            this.head = head.next;
            if (this.head == null) this.tail = null;
            this.size--;
            return temp.data;
        }

        // Peek
        public String peek() {
            return isEmpty() ? null : this.head.data;
        }

        // IsEmpty
        public boolean isEmpty() {
            return this.size == 0;
        }

        // Size
        public int size() {
            return this.size;
        }

        // Print queue
        public void printQueue() {
            Node currentNode = this.head;
            while (currentNode != null) {
                System.out.print(currentNode.data + " -> ");
                currentNode = currentNode.next;
            }
            System.out.println("null");
        }
    }

    
    static final int MAX_COUNTERS = 3;
    static Queue[] counters = new Queue[MAX_COUNTERS];

   
    static {
        for (int i = 0; i < MAX_COUNTERS; i++) {
            counters[i] = new Queue();
        }
    }

    private static int findShortestQueue() {
        int minIndex = 0;
        for (int i = 1; i < MAX_COUNTERS; i++) {
            if (counters[i].size() < counters[minIndex].size()) {
                minIndex = i;
            }
        }
        return minIndex;
    }

    // Customer arrival
    public static void CustomerArrival(String name) {
        int idx = findShortestQueue();
        counters[idx].enqueue(name);
        System.out.println(name + " joined Counter " + (idx + 1));
    }

    // Process next customer
    public static void ProcessNextCustomer(int counter) {
        if (counter < 1 || counter > MAX_COUNTERS) return;
        String name = counters[counter - 1].dequeue();
        if (name == null) {
            System.out.println("Counter " + counter + " has no customers.");
        } else {
            System.out.println(name + " is processed at Counter " + counter);
        }
    }

    // Display queue
    public static void DisplayQueue(int counter) {
        if (counter < 1 || counter > MAX_COUNTERS) return;
        System.out.print("Counter " + counter + " queue: ");
        counters[counter - 1].printQueue();
    }

    // -------- Demo --------
    public static void main(String[] args) {
        CustomerArrival("Alice");
        CustomerArrival("Bob");
        CustomerArrival("Charlie");
        CustomerArrival("David");
        CustomerArrival("Eve");

        DisplayQueue(1);
        DisplayQueue(2);
        DisplayQueue(3);

        ProcessNextCustomer(1);
        ProcessNextCustomer(2);

        DisplayQueue(1);
        DisplayQueue(2);
        DisplayQueue(3);
    }
}