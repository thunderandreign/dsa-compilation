package stacks;

public class Postfix {
    Node head;
    int size;

    class Node {
        char data;
        Node next;
        Node(char data) {
            this.data = data;
            this.next = null;
        }
    }

    public void push(char value) {
        Node node = new Node(value);
        if (this.head != null) node.next = this.head;
        this.head = node;
        this.size++;
    }

    public char pop() {
        if (isEmpty()) return '\0';
        char popped = this.head.data;
        this.head = this.head.next;
        this.size--;
        return popped;
    }

    public boolean isEmpty() {
        return this.size == 0;
    }

    public char peek() {
        if (isEmpty()) return '\0';
        return this.head.data;
    }
    
    private static int precedence(char c) {
        switch (c) {
            case '+':
            case '-':
                return 1;
            case '*':
            case '/':
                return 2;
            case '^':
                return 3;
        }
        return -1;
    }

    private static boolean isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }

    private static boolean isOperand(char c) {
        return Character.isLetterOrDigit(c);
    }
    
    public static String infixToPostfix(String expression) {
        Postfix stack = new Postfix();
        StringBuilder result = new StringBuilder();

        for (int i = 0; i < expression.length(); i++) {
            char c = expression.charAt(i);

            if (isOperand(c)) {
                result.append(c);
            } 
            else if (c == '(') {
                stack.push(c);
            } 
            else if (c == ')') {
                while (!stack.isEmpty() && stack.peek() != '(') {
                    result.append(stack.pop());
                }
                stack.pop(); // remove '('
            } 
            else if (isOperator(c)) {
                while (!stack.isEmpty() && precedence(c) <= precedence(stack.peek())) {
                    result.append(stack.pop());
                }
                stack.push(c);
            }
        }

        while (!stack.isEmpty()) {
            result.append(stack.pop());
        }

        return result.toString();
    }
    
    
    public static void main(String[] args) {
        String infix = "(A+B)*C-(D-E)*(F+G)";
        System.out.println("Infix:   " + infix);
        System.out.println("Postfix: " + infixToPostfix(infix));
    }
}
