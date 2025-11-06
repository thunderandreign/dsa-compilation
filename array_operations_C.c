// C Program implementing array operations step by step
// Each function handles one task, and all lines are explained.

#include <stdio.h>
#include <stdlib.h>

// Function to create array
int* createArray(int size) {
    int* arr = (int*)malloc(size * sizeof(int));
    printf("Enter %d elements:\n", size);
    for (int i = 0; i < size; i++) scanf("%d", &arr[i]);
    return arr;
}

// 🔹 Traversal
void traverseArray(int* arr, int size) {
    printf("Array elements: ");
    for (int i = 0; i < size; i++) printf("%d ", arr[i]);
    printf("\n");
}

// 🔹 Insertion
int* insertElement(int* arr, int* size, int index, int value) {
    *size += 1;
    arr = realloc(arr, (*size) * sizeof(int));
    for (int i = *size - 1; i > index; i--)
        arr[i] = arr[i - 1];
    arr[index] = value;
    return arr;
}

// 🔹 Deletion
int* deleteElement(int* arr, int* size, int index) {
    for (int i = index; i < *size - 1; i++)
        arr[i] = arr[i + 1];
    *size -= 1;
    arr = realloc(arr, (*size) * sizeof(int));
    return arr;
}

// 🔹 Update
void updateElement(int* arr, int index, int newValue) {
    arr[index] = newValue;
}

// 🔹 Merge
int* mergeArrays(int* a, int sizeA, int* b, int sizeB) {
    int* merged = malloc((sizeA + sizeB) * sizeof(int));
    for (int i = 0; i < sizeA; i++) merged[i] = a[i];
    for (int i = 0; i < sizeB; i++) merged[sizeA + i] = b[i];
    return merged;
}

// 🔹 Bubble Sort
void bubbleSort(int* arr, int size) {
    for (int i = 0; i < size - 1; i++)
        for (int j = 0; j < size - i - 1; j++)
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}

// 🔹 Selection Sort
void selectionSort(int* arr, int size) {
    for (int i = 0; i < size - 1; i++) {
        int min = i;
        for (int j = i + 1; j < size; j++)
            if (arr[j] < arr[min]) min = j;
        int temp = arr[i];
        arr[i] = arr[min];
        arr[min] = temp;
    }
}

// 🔹 Insertion Sort
void insertionSort(int* arr, int size) {
    for (int i = 1; i < size; i++) {
        int key = arr[i], j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

// 🔹 Shell Sort
void shellSort(int* arr, int size) {
    for (int gap = size / 2; gap > 0; gap /= 2)
        for (int i = gap; i < size; i++) {
            int temp = arr[i];
            int j = i;
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }
}

// 🔹 Quick Sort Helpers
int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j < high; j++)
        if (arr[j] <= pivot) {
            i++;
            int temp = arr[i]; arr[i] = arr[j]; arr[j] = temp;
        }
    int temp = arr[i + 1]; arr[i + 1] = arr[high]; arr[high] = temp;
    return i + 1;
}

void quickSort(int* arr, int low, int high) {
    if (low < high) {
        int pivot = partition(arr, low, high);
        quickSort(arr, low, pivot - 1);
        quickSort(arr, pivot + 1, high);
    }
}

// 🔹 Sequential Search
int sequentialSearch(int* arr, int size, int key) {
    for (int i = 0; i < size; i++)
        if (arr[i] == key) return i;
    return -1;
}

// 🔹 Binary Search
int binarySearch(int* arr, int size, int key) {
    int low = 0, high = size - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        if (arr[mid] == key) return mid;
        else if (arr[mid] < key) low = mid + 1;
        else high = mid - 1;
    }
    return -1;
}

// 🔹 Interpolation Search
int interpolationSearch(int* arr, int size, int key) {
    int low = 0, high = size - 1;
    while (low <= high && key >= arr[low] && key <= arr[high]) {
        if (low == high) return (arr[low] == key) ? low : -1;
        int pos = low + ((key - arr[low]) * (high - low)) / (arr[high] - arr[low]);
        if (arr[pos] == key) return pos;
        if (arr[pos] < key) low = pos + 1;
        else high = pos - 1;
    }
    return -1;
}

// MAIN FUNCTION
int main() {
    int size = 5;
    int* arr = createArray(size);
    traverseArray(arr, size);

    arr = insertElement(arr, &size, 2, 99);
    arr = deleteElement(arr, &size, 3);
    updateElement(arr, 1, 77);
    traverseArray(arr, size);

    int b[] = {10, 20, 30};
    int* merged = mergeArrays(arr, size, b, 3);
    traverseArray(merged, size + 3);

    bubbleSort(merged, size + 3);
    traverseArray(merged, size + 3);

    printf("Binary search for 20: index %d\n", binarySearch(merged, size + 3, 20));

    free(arr);
    free(merged);
    return 0;
}
