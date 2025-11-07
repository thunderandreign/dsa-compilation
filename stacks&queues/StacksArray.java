package stacks;

public class StacksArray {
    private int[] arr;   
    private int top;     
    private int capacity;

    // constructor
    public StacksArray(int capacity) {
        this.capacity = capacity;
        arr = new int[capacity];
        top = -1;
    }

    // push element onto stack
    public void push(int value) {
        if (top == capacity - 1) {
            System.out.println("Stack overflow!");
            return;
        }
        arr[++top] = value;
    }

    // pop element from stack
    public int pop() {
        if (isEmpty()) {
            System.out.println("Stack underflow!");
            return -1;
        }
        return arr[top--];
    }

    // check if stack is empty
    public boolean isEmpty() {
        return top == -1;
    }

    // return size of stack
    public int size() {
        return top + 1;
    }

    // peek at top element
    public int peek() {
        if (isEmpty()) {
            System.out.println("Stack is empty!");
            return -1;
        }
        return arr[top];
    }

    // print stack contents
    public void printStack() {
        if (isEmpty()) {
            System.out.println("Stack is empty!");
            return;
        }
        for (int i = top; i >= 0; i--) {
            System.out.print(arr[i] + " -> ");
        }
        System.out.println("end");
    }

    // test
    public static void main(String[] args) {
        StacksArray stack1 = new StacksArray(10);

        stack1.push(4);
        stack1.push(9);
        stack1.push(6);
        stack1.push(1);

        stack1.printStack();

        System.out.println(stack1.size());
        System.out.println(stack1.pop());
        System.out.println(stack1.isEmpty());
        System.out.println(stack1.size());
        System.out.println(stack1.peek());

        stack1.printStack();
    }
}