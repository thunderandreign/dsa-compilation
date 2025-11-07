import java.util.Hashtable;

public class HashTableExample {
    public static void main(String[] args) {
        // Create a Hashtable
        Hashtable<Integer, String> hashtable = new Hashtable<>();

        // Insert key-value pairs into the Hashtable
        hashtable.put(1, "Apple");
        hashtable.put(2, "Banana");
        hashtable.put(3, "Cherry");
        hashtable.put(4, "Date");

        // Display the Hashtable
        System.out.println("Initial Hashtable: " + hashtable);

        // Access a value using a key
        int key = 2;
        System.out.println("Value for key " + key + ": " + hashtable.get(key));

        // Remove a key-value pair
        hashtable.remove(3);
        System.out.println("Hashtable after removing key 3: " + hashtable);

        // Check if a key exists
        System.out.println("Does key 4 exist? " + hashtable.containsKey(4));

        // Check if a value exists
        System.out.println("Does value 'Banana' exist? " + hashtable.contains("Banana"));

        // Iterate through the Hashtable
        System.out.println("Iterating through the Hashtable:");
        hashtable.forEach((k, v) -> System.out.println("Key: " + k + ", Value: " + v));
    }
}