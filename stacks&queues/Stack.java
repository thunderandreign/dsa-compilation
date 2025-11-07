package linked_lists;

public class Stack {
	Node head;
	int size;
	
		
	public void push(int value) {
		Node node = new Node(value);
		
		if(this.head != null) {
			node.next = this.head;	
		}
		
		this.head = node;
		this.size++;
	}
	
	public int pop() {
				
		int popped = this.head.data;
		this.head = this.head.next;
		this.size--;
		return popped;
	}
	
	public boolean isEmpty() {
		if(this.size == 0) {
			return true;
		}else {
			return false;
		}
	}
	
	public int size() {
		if(isEmpty()) {
			return 0;
		}else {
			return this.size;
		}
		
	}
	
	public int peek() {
		if(isEmpty()) {
			return 0;
		}else {
			return this.head.data;
		}
	}
	
	public void printStack() {
		Node currentNode = this.head;
		
		while(currentNode != null) {
			System.out.print(currentNode.data+" -> ");
			currentNode = currentNode.next;
		}
		
		System.out.println("end");
	}
	
	class Node{
		int data;
		Node next;
		
		Node(int data){
			this.data = data;
			this.next = null;
		}
	}
	
	public static void main(String[] args) {
		
		Stack stack1 = new Stack();
		
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
