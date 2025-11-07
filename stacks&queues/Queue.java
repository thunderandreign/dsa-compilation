package linked_lists;

public class Queue {
	Node head;
	Node tail;
	int size;
	
	
	// Nodes	
	class Node{
		int data;
		Node next;
		
		Node(int data){
			this.data = data;
			this.next = null;
		}
	}
	
	public void enqueue(int value) {
		Node newNode = new Node(value);
		
		if(isEmpty()) {
			this.head = this.tail = newNode;
			this.size++;
		}else {
			this.tail.next = newNode;
			this.tail = newNode;
			this.size++;
		}
	}
	
	public int dequeue() {
		if(isEmpty()) {
			return 0;
		}
		
		Node head = this.head;
		this.head = head.next;
		this.size--;
		
		return head.data;
	}
	
	public boolean isEmpty() {
		if(this.size == 0) {
			return true;
		}else {
			return false;
		}
	}
	
	public int peek() {
		return this.head.data;
	}
	
	public int size() {
		return this.size;
	}
	
	public void printQueue() {
		Node currentNode = this.head;
		
		while(currentNode != null) {
			System.out.print(currentNode.data+" -> ");
			currentNode = currentNode.next;
		}
		System.out.println("null");
	}
	
	public static void main(String[] args) {
		
		Queue q1 = new Queue();
		
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
