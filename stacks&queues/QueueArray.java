package stacks;

public class QueueArray {
    private int[] arr;     
    private int front;     
    private int rear;      
    private int size;      
    private int capacity;  

    // constructor
    public QueueArray(int capacity) {
        this.capacity = capacity;
        arr = new int[capacity];
        front = 0;
        rear = -1;
        size = 0;
    }

    // add element to queue
    public void enqueue(int value) {
        if (size == capacity) {
            System.out.println("Queue overflow!");
            return;
        }
        rear = (rear + 1) % capacity; // circular increment
        arr[rear] = value;
        size++;
    }

    // remove element from queue
    public int dequeue() {
        if (isEmpty()) {
            System.out.println("Queue underflow!");
            return -1;
        }
        int value = arr[front];
        front = (front + 1) % capacity; // circular increment
        size--;
        return value;
    }

    // check if queue is empty
    public boolean isEmpty() {
        return size == 0;
    }

    // peek at front element
    public int peek() {
        if (isEmpty()) {
            System.out.println("Queue is empty!");
            return -1;
        }
        return arr[front];
    }

    // return size of queue
    public int size() {
        return size;
    }

    // print queue contents
    public void printQueue() {
        if (isEmpty()) {
            System.out.println("Queue is empty!");
            return;
        }
        int count = size;
        int i = front;
        while (count > 0) {
            System.out.print(arr[i] + " -> ");
            i = (i + 1) % capacity;
            count--;
        }
        System.out.println("null");
    }

    // test
    public static void main(String[] args) {
        QueueArray q1 = new QueueArray(5);

        q1.enqueue(7);
        q1.enqueue(2);
        q1.enqueue(0);
        q1.enqueue(4);
        q1.enqueue(6);

        q1.printQueue();
        System.out.println(q1.size());
        System.out.println(q1.dequeue());
        System.out.println(q1.isEmpty());
        q1.printQueue();
        System.out.println(q1.size());
        System.out.println(q1.peek());
    }
}