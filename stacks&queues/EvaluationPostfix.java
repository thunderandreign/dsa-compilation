package stacks;

public class EvaluationPostfix {

    static class CharStack {
        private Node head;
        private int size;

        private static class Node {
            char data;
            Node next;
            Node(char data) { this.data = data; }
        }

        public void push(char value) {
            Node node = new Node(value);
            node.next = head;
            head = node;
            size++;
        }

        public char pop() {
            if (isEmpty()) throw new RuntimeException("Stack Underflow");
            char val = head.data;
            head = head.next;
            size--;
            return val;
        }

        public char peek() {
            if (isEmpty()) return '\0';
            return head.data;
        }

        public boolean isEmpty() {
            return size == 0;
        }
    }

    static class IntStack {
        private Node head;
        private int size;

        private static class Node {
            int data;
            Node next;
            Node(int data) { this.data = data; }
        }

        public void push(int value) {
            Node node = new Node(value);
            node.next = head;
            head = node;
            size++;
        }

        public int pop() {
            if (isEmpty()) throw new RuntimeException("Stack Underflow");
            int val = head.data;
            head = head.next;
            size--;
            return val;
        }

        public boolean isEmpty() {
            return size == 0;
        }
    }

    static int precedence(char c) {
        switch (c) {
            case '+':
            case '-': return 1;
            case '*':
            case '/': return 2;
            case '^': return 3;
            default: return -1;
        }
    }

    static boolean isOperator(char c) {
        return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
    }

    static boolean isOperand(char c) {
        return Character.isDigit(c);
    }

    static String infixToPostfix(String infix) {
        CharStack stack = new CharStack();
        StringBuilder postfix = new StringBuilder();

        for (int i = 0; i < infix.length(); i++) {
            char c = infix.charAt(i);

            if (Character.isWhitespace(c)) continue; // ignore spaces

            if (isOperand(c)) {
                postfix.append(c);
            } 
            else if (c == '(') {
                stack.push(c);
            } 
            else if (c == ')') {
                while (!stack.isEmpty() && stack.peek() != '(') {
                    postfix.append(stack.pop());
                }
                stack.pop(); // remove '('
            } 
            else if (isOperator(c)) {
                while (!stack.isEmpty() && precedence(stack.peek()) >= precedence(c)) {
                    postfix.append(stack.pop());
                }
                stack.push(c);
            }
        }

        while (!stack.isEmpty()) {
            postfix.append(stack.pop());
        }

        return postfix.toString();
    }

    static int evaluatePostfix(String postfix) {
        IntStack stack = new IntStack();

        for (int i = 0; i < postfix.length(); i++) {
            char c = postfix.charAt(i);

            if (Character.isDigit(c)) {
                stack.push(c - '0');
            } 
            else if (isOperator(c)) {
                int val2 = stack.pop();
                int val1 = stack.pop();
                int result = 0;

                switch (c) {
                    case '+': result = val1 + val2; break;
                    case '-': result = val1 - val2; break;
                    case '*': result = val1 * val2; break;
                    case '/': result = val1 / val2; break;
                    case '^': result = (int)Math.pow(val1, val2); break;
                }

                stack.push(result);
            }
        }

        return stack.pop();
    }

    public static void main(String[] args) {
        String infix = "3+(2*4)-5";

        System.out.println("Infix:   " + infix);
        String postfix = infixToPostfix(infix);
        System.out.println("Postfix: " + postfix);

        int result = evaluatePostfix(postfix);
        System.out.println("Result:  " + result);
    }
}
